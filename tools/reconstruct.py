#!/usr/bin/env python3
"""Bounded, headless reconstruction work queue; an agent supplies the C edits."""
import argparse
import hashlib
import json
import re
import subprocess
import sys
import time
import shutil
from contextlib import contextmanager
from pathlib import Path

import yaml

from rebuild import ROOT, reference
from boundary_audit import (audit_rows, review_matches, prepare_ghidra_export,
                            check_export_complete, check_review_export)

STATE = ROOT / "reports/reconstruction/state.json"
INVENTORY = ROOT / "reports/reconstruction/inventory.json"
REVIEW = ROOT / "config/reconstruction.json"
BOUNDARIES = ROOT / "config/function-boundaries.json"
SDK = ROOT / "config/psyq-sdk.json"
FUNCTION = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1\s*$", re.M | re.S)
INSTRUCTION = re.compile(r"/\* ([0-9A-F]+) ([0-9A-F]+) ([0-9A-F]{8}) \*/\s*(.*)")
PROTECTED = ("config/usa-disc.json", "config/compiler.json", "tools/rebuild.py",
             "tools/validate.py", "tools/objdiff.py", "tools/reconstruct.py",
             "tools/reconstruct_run.py", "tools/boundary_audit.py",
             "tools/ghidra_scripts/ExportBoundaryEvidence.java", "tools/ghidra_scripts/ReviewBoundaries.java",
             "tools/ghidra-docker.sh",
             "config/function-boundaries.json", "config/reconstruction.json", "config/psyq-sdk.json",
             "config/overlays.json", "tools/overlays.py", "tools/ghidra_scripts/ImportOverlays.java",
             "tools/ghidra_scripts/ExportOverlayInventory.java", "tools/ghidra_scripts/ExportAllOverlays.java",
             "tools/ghidra_scripts/AuditOverlayReferences.java", "tools/overlay_rebuild.py",
             "tools/splat_ext/bytes.py", "tools/target_queue.py", "tools/reconstruction_backlog.py", "Makefile") + tuple(
                 p.relative_to(ROOT).as_posix() for p in sorted((ROOT / "config/overlays").glob("*")) if p.is_file())


class NoTarget(RuntimeError):
    pass


class CommandTimeout(RuntimeError):
    pass


def read(path):
    return json.loads(path.read_text())


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(".tmp")
    temporary.write_text(json.dumps(value, indent=2) + "\n")
    temporary.replace(path)


def save_state(session):
    write(STATE, session)
    if "directory" in session:
        write(ROOT / session["directory"] / "state.json", session)


@contextmanager
def exclusive_worker(directory):
    import fcntl
    directory.mkdir(parents=True, exist_ok=True)
    with (directory / "worker.lock").open("w") as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            raise RuntimeError("Another reconstruction worker command is running")
        yield


def git(*args):
    return subprocess.check_output(["git", *args], cwd=ROOT, text=True).strip()


def changes():
    # Include untracked, nonignored files; never stage ignored artifacts.
    paths = set()
    for args in (("diff", "--name-only", "-z", "HEAD"),
                 ("ls-files", "--others", "--exclude-standard", "-z")):
        output = subprocess.check_output(["git", *args], cwd=ROOT)
        paths.update(p.decode() for p in output.split(b"\0") if p)
    return paths


def fingerprints():
    return {p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest() for p in PROTECTED}


def content_hashes(paths):
    return {p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest() for p in paths}


def command(arguments, name, timeout=300, directory=None, input_text=None, env=None):
    path = (directory or STATE.parent) / f"{name}.log"
    path.parent.mkdir(parents=True, exist_ok=True)
    print(f"Running {' '.join(map(str, arguments))}; log: {path.relative_to(ROOT)}", flush=True)
    with path.open("w") as log:
        # Kill the whole build process group on timeout so compilers cannot
        # continue writing artifacts after this worker has stopped.
        process = subprocess.Popen(list(map(str, arguments)), cwd=ROOT, stdout=log,
                                   stderr=subprocess.STDOUT, start_new_session=True,
                                   stdin=subprocess.PIPE if input_text is not None else subprocess.DEVNULL,
                                   text=True, env=env)
        try:
            process.communicate(input=input_text, timeout=timeout)
            code = process.returncode
        except BaseException as error:
            import os
            import signal
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            process.wait()
            if isinstance(error, subprocess.TimeoutExpired):
                raise CommandTimeout(f"Command timed out; inspect {path}") from error
            raise
    if code:
        raise RuntimeError(f"Command failed ({code}); inspect {path}")


def parse_functions(path, target):
    rows = []
    text = path.read_text()
    for match in FUNCTION.finditer(text):
        instructions = INSTRUCTION.findall(match[2])
        if not instructions:
            continue
        offsets = [int(i[0], 16) for i in instructions]
        addresses = [int(i[1], 16) for i in instructions]
        contiguous = (offsets == list(range(offsets[0], offsets[0] + 4 * len(offsets), 4))
                      and addresses == list(range(addresses[0], addresses[0] + 4 * len(addresses), 4)))
        raw = bytes.fromhex("".join(i[2] for i in instructions))
        if target[offsets[0]:offsets[0] + len(raw)] != raw:
            raise RuntimeError(f"Stale or invalid assembly context: {match[1]}")
        body = match[2]
        ops = [i[3].split()[0] for i in instructions]
        calls = sorted(set(re.findall(r"\bjal\s+(\w+)", body)))
        refs = sorted(set(re.findall(r"\b(?:func_[0-9A-F]+|D_[0-9A-F]+)\b", body)))
        hardware = bool(re.search(r"\$(?:gp|k0|k1)|\b(?:[ml][ft]c[02]|[ls]wc2|syscall|break|rfe)\b", body))
        # Also exclude GTE instructions whose mnemonics are not ordinary MIPS.
        hardware |= any(int.from_bytes(bytes.fromhex(i[2]), "little") >> 26 in (16, 18)
                        for i in instructions)
        features = []
        if any(o in ops for o in ("lb", "lh")):
            features.append("signed-load")
        if any(o in ops for o in ("lbu", "lhu")):
            features.append("unsigned-load")
        if any(o in ops for o in ("div", "divu", "mult", "multu")):
            features.append("multiply-divide")
        if any(o.startswith("b") for o in ops):
            features.append("branch")
        if "jalr" in ops or re.search(r"\bjr\s+\$(?!ra\b)", body):
            features.append("indirect-control")
        rows.append({"name": match[1], "address": f"0x{addresses[0]:08X}",
                     "offset": offsets[0], "size": len(raw), "contiguous": contiguous,
                     "assembly": str(path.relative_to(ROOT)), "calls": calls,
                     "references": refs, "hardware_or_gp": hardware,
                     "features": features, "body": match[0]})
    return rows


def refresh_boundary_evidence():
    """A batch starts from a fresh saved-Ghidra review export, never a stale log."""
    directory = ROOT / "reports/boundary-audit/ghidra"
    prepare_ghidra_export(directory)
    command([ROOT / "tools/ghidra-docker.sh", "headless", "/projects", "CB2_USA",
             "-process", "SCUS_943.58", "-noanalysis", "-readOnly", "-scriptPath", "/scripts",
             "-postScript", "ExportBoundaryEvidence.java", "/reports/boundary-audit/ghidra",
             "-log", "/home/ghidra/boundary-export.log"], "ghidra-boundaries")
    _, target, _ = reference()
    check_export_complete(directory, target)
    check_review_export(read(BOUNDARIES), directory / "reviewed-units.json", required=True)


def legacy_inventory():
    _, target, sha = reference()
    sdk = read(SDK)
    if sdk["reference_sha256"] != sha:
        raise RuntimeError("PsyQ identifications do not match the pinned reference")
    # Fresh independent split and byte-checked all-assembly reference.
    command([sys.executable, "tools/objdiff.py"], "inventory")
    review = read(REVIEW)
    config = yaml.safe_load((ROOT / "scus_943.58.yaml").read_text())
    c_units = set()
    for segment in config["segments"]:
        if isinstance(segment, dict):
            c_units.update(s[2] for s in segment.get("subsegments", [])
                           if isinstance(s, list) and s[1] == "c")
    rows = []
    for path in sorted((ROOT / "build/objdiff/target").glob("*.s")):
        # Ignore obsolete generated files left after a segment was renamed.
        active = {s[2] for seg in config["segments"] if isinstance(seg, dict)
                  for s in seg.get("subsegments", []) if isinstance(s, list) and s[1] in ("c", "asm")}
        if path.stem not in active:
            continue
        for row in parse_functions(path, target):
            item = review.get("functions", {}).get(row["address"], {})
            row.update(status="c" if path.stem in c_units else "assembly",
                       unit=path.stem, classification=item.get("classification", "unknown"),
                       boundary_reviewed=item.get("boundary_reviewed", False),
                       hold=item.get("hold", False),
                       note=item.get("note", ""))
            rows.append(row)
    if len({r["address"] for r in rows}) != len(rows):
        raise RuntimeError("Overlapping function inventory")
    for row in rows:
        row["callers"] = [r["name"] for r in rows if row["name"] in r["calls"]]
    rows.sort(key=lambda r: int(r["address"], 16))
    boundaries = read(BOUNDARIES)
    if boundaries.get("reference_sha256") != sha:
        raise RuntimeError("Boundary reviews do not match the pinned reference")
    exported = ROOT / "reports/boundary-audit/ghidra"
    check_export_complete(exported, target)
    check_review_export(boundaries, exported / "reviewed-units.json", required=True)
    diagnostics = {r["address"]: r for r in audit_rows(rows, target)}
    for row in rows:
        diagnosis = diagnostics[row["address"]]
        reviewed = boundaries.get("regions", {}).get(row["address"], {})
        row.update(boundary_reviewed=review_matches(diagnosis, reviewed),
                   boundary_kind=reviewed.get("kind", "unreviewed"),
                   library_candidate=reviewed.get("library_candidate", False),
                   sdk_name_lead=reviewed.get("sdk_name_lead"),
                   boundary_findings=diagnosis["findings"],
                   instruction_sha256=diagnosis["instruction_sha256"],
                   address_reference_seen=bool(diagnosis["possible_pointer_words_to_entry"]),
                   end_exclusive=diagnosis["end_exclusive"])
    apply_sdk_identifications(rows, sdk)
    report = {"reference_sha256": sha, "scope": "Splat boot EXE code units, including multi-entry regions; unreviewed boundaries are provisional, overlays excluded. Legacy function-count keys count generated spans, not original source functions.",
              "summary": {"detected_functions": len(rows),
                          "detected_code_units": len(rows),
                          "c_functions": sum(r["status"] == "c" for r in rows),
                          "c_bytes": sum(r["size"] for r in rows if r["status"] == "c"),
                          "retained_assembly_functions": sum(r["status"] == "assembly" for r in rows),
                          "sdk_identified_units": sum(r["classification"] == "sdk" for r in rows),
                          "sdk_identified_c_units": sum(r["classification"] == "sdk" and r["status"] == "c" for r in rows),
                          "reviewed_standalone_assembly_units": sum(r["status"] == "assembly" and r["boundary_reviewed"] for r in rows)},
              "functions": rows}
    write(INVENTORY, report)
    print(json.dumps(report["summary"], indent=2))
    return report


def inventory():
    from reconstruction_backlog import inventory as complete_inventory
    return complete_inventory()


def apply_sdk_identifications(rows, sdk):
    """Trust accepted plugin names; also exclude spans containing an SDK entry."""
    entries = [(int(address, 16), name) for address, name in sdk["functions"].items()]
    for row in rows:
        start = int(row["address"], 16)
        names = [name for address, name in entries if start <= address < start + row["size"]]
        if names:
            row.update(classification="sdk", library_candidate=True,
                       sdk_names=names, sdk_version=sdk["version"],
                       sdk_identification="accepted_psx_plugin",
                       note=(row.get("note", "") + " Accepted PSX plugin SDK identification: "
                             + ", ".join(names) + "; excluded from C reconstruction.").strip())


def state():
    if not STATE.exists():
        raise RuntimeError("Start a session with begin")
    return read(STATE)


def protected_unchanged(session):
    active = session.get("active") or {}
    allowed = {active.get("build_config"), active.get("symbol_file")}
    return {k: v for k, v in fingerprints().items() if k not in allowed} == {k: v for k, v in session["protected"].items() if k not in allowed}


def check_session(session):
    if git("rev-parse", "HEAD") != session["head"]:
        raise RuntimeError("HEAD changed outside the worker; begin a new session")
    if session.get("branch") and git("branch", "--show-current") != session["branch"]:
        raise RuntimeError("Current branch changed outside the worker")
    if not protected_unchanged(session):
        raise RuntimeError("Reference/build gate/compiler changed; review outside the function worker")
    if time.time() > session["deadline"]:
        raise RuntimeError("Session time budget exhausted; preserve/defer the attempt or begin again")


def choose(report, session, address=None):
    policy = read(REVIEW)
    if policy.get("matching_paused", False):
        raise NoTarget("C matching is paused by the boundary-audit policy; review config/reconstruction.json")
    if "targets" in report:
        from reconstruction_backlog import key
        excluded = {key(r) for r in session["completed"] + session["deferred"]}
        candidates = [r for r in report["targets"] if r["status"] == "needs_review" and key(r) not in excluded]
        if address:
            candidates = [r for r in candidates if key(r) == address]
        if not candidates:
            raise NoTarget("Complete backlog exhausted for this session")
        return candidates[0]
    excluded = {r["address"] for r in session["completed"] + session["deferred"]}
    candidates = [r for r in report["functions"] if r["status"] == "assembly"
                  and r["contiguous"] and not r["hardware_or_gp"]
                  and not r.get("hold", False)
                  and (policy.get("include_library_candidates", False) or not r.get("library_candidate", False))
                  and r.get("boundary_reviewed") is True
                  and r.get("boundary_kind") == "standalone"
                  and not r.get("boundary_findings", {})
                  and r["classification"] not in ("sdk", "handwritten")
                  and r["address"] not in excluded and r["size"] <= 160
                  and "indirect-control" not in r["features"]]
    if address:
        candidates = [r for r in candidates if r["address"] == f"0x{int(address, 16):08X}"]
    order = policy.get("pilot", [])
    candidates.sort(key=lambda r: (order.index(r["address"]) if r["address"] in order else len(order),
                                   not bool(r.get("callers") or r.get("address_reference_seen")),
                                   not r["boundary_reviewed"], bool(r["calls"]), r["size"], r["address"]))
    if not candidates:
        raise NoTarget("No eligible target; review the inventory or expand the policy")
    return candidates[0]


def next_target(session, address=None):
    check_session(session)
    if session["active"]:
        raise RuntimeError("Finish or defer the active target first")
    if len(session["completed"]) + len(session["deferred"]) >= session["limit"]:
        raise RuntimeError("Session target limit reached")
    if changes():
        raise RuntimeError("Next target requires a clean checkout; preserve unfinished edits first")
    report = inventory()
    check_session(session)
    row = choose(report, session, address)
    from reconstruction_backlog import prepare, slug
    if "targets" in report:
        row = prepare(ROOT, row, report)
    directory = ROOT / session.get("directory", "reports/reconstruction") / slug(row)
    directory.mkdir(parents=True, exist_ok=True)
    (directory / "target.s").write_text(row["body"] + "\n")
    recipe = "config/compiler.json"
    if row.get("image", "SCUS_943.58") != "SCUS_943.58":
        recipe = read(ROOT / "config/overlays.json")["overlays"][row["image"]].get("compiler_config", recipe)
    write(directory / "context.json", {**row, "compiler_config": recipe, "compiler": read(ROOT / recipe)})
    (directory / "instructions.md").write_text(
        f"Reconstruct {row['name']} ({row['size']} bytes).\n\n"
        "Review callers, data types and both split boundaries before integration.\n"
        "Use readable C; do not embed assembly or target bytes. Keep the reference and recipe fixed.\n"
        "Run tools/.venv/bin/python tools/reconstruct.py check for each integrated attempt.\n"
        "Inspect reports/objdiff.json, reports/objdiff/<unit>.json, reports/boot-rebuild.json and reports/overlay-rebuild.json.\n"
        "Use defer --reason when blocked; archive edits and restore the passing baseline before next.\n"
        "Use finish --paths <explicit authored files> --message <commit message> after review.\n")
    write(directory / "boundary-review.json", {k: row.get(k) for k in
          ("identity", "reference_sha256", "body_sha256", "ranges", "boundary_review", "boundary_findings", "incoming", "control_references", "containing_span")})
    session["active"] = {**{k: row[k] for k in ("identity", "image", "build_config", "symbol_file", "boundary_findings", "source_directory", "ranges", "body_sha256") if k in row},
                         "address": row["address"], "name": row["name"], "size": row["size"],
                         "offset": row["offset"], "attempts": 0,
                         "context": str(directory.relative_to(ROOT))}
    save_state(session)
    print(json.dumps(session["active"], indent=2))


def validate(session):
    check_session(session)
    active = session["active"]
    if not active:
        raise RuntimeError("No active target")
    if active["attempts"] >= session["attempt_limit"]:
        raise RuntimeError("Attempt limit reached; defer this target")
    active["attempts"] += 1
    save_state(session)
    attempt = ROOT / active["context"] / f"attempt-{active['attempts']}"
    attempt.mkdir(parents=True, exist_ok=True)
    archive_edits(attempt)
    try:
        command(["make", "validate"], "validate", directory=attempt,
                timeout=min(300, max(1, session["deadline"] - time.time())))
    finally:
        for name in ("objdiff.json", "boot-rebuild.json", "overlay-rebuild.json"):
            path = ROOT / "reports" / name
            if path.exists():
                shutil.copy2(path, attempt / name)
        details = ROOT / "reports/objdiff"
        if details.exists() and (attempt / "objdiff.json").exists():
            shutil.copytree(details, attempt / "objdiff", dirs_exist_ok=True)
        overlays = ROOT / "reports/overlay-rebuild"
        if overlays.exists():
            shutil.copytree(overlays, attempt / "overlay-rebuild", dirs_exist_ok=True)
    report = read(ROOT / "reports/boot-rebuild.json")
    _, original, sha = reference()
    if not report["identical"] or report["reference_sha256"] != sha or (ROOT / "build/SCUS_943.58").read_bytes() != original:
        raise RuntimeError("Full executable gate failed")
    current = inventory()
    from reconstruction_backlog import key
    target = next((r for r in current.get("targets", current.get("functions", [])) if key(r) == key(active)), None)
    if target is None or target.get("reconstruction", target["status"]) != "c" or target.get("body_bytes", target.get("size")) != active["size"]:
        raise RuntimeError("Target was not converted to C at its original boundary")
    if "identity" in active:
        from overlays import load
        catalog, _ = load()
        for image, spec in catalog['overlays'].items():
            slug = Path(spec['build_config']).stem
            rebuilt = ROOT / 'build/overlays' / slug / image
            if rebuilt.read_bytes() != (ROOT / 'orig/disc/files' / image).read_bytes():
                raise RuntimeError('Full overlay gate failed: ' + image)
    print(f"Exact full-file match; {active['name']} is integrated as C.")
    return current


def archive_edits(directory):
    (directory / "attempt.patch").write_bytes(subprocess.check_output(["git", "diff", "HEAD"], cwd=ROOT))
    sources = {}
    for name in changes():
        path = ROOT / name
        if name.startswith("src/"):
            if path.is_symlink():
                sources[name] = {"symlink": str(path.readlink())}
            elif path.is_file():
                try:
                    sources[name] = path.read_text()
                except UnicodeDecodeError:
                    archived = directory / "files" / name
                    archived.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copyfile(path, archived)
                    sources[name] = {"binary_file": str(archived.relative_to(directory))}
    write(directory / "sources.json", sources)


def audit_paths(requested, changed):
    paths = set(requested)
    if not paths or paths != changed:
        raise RuntimeError(f"Explicit paths must equal all checkout changes: {sorted(changed)}")
    for name in paths:
        path = Path(name)
        allowed = (name in ("scus_943.58.yaml", "symbol_addrs.txt", "config/reconstruction.json")
                   or (name.startswith("config/overlays/") and path.suffix in (".yaml", ".txt"))
                   or (len(path.parts) > 1 and path.parts[0] == "src" and path.suffix in (".c", ".h"))
                   or (len(path.parts) > 1 and path.parts[0] == "docs" and path.suffix == ".md"))
        if not allowed or path.is_absolute() or ".." in path.parts:
            raise RuntimeError(f"Not an authored reconstruction path: {name}")
        if (ROOT / path).is_symlink():
            raise RuntimeError(f"Refusing symlink: {name}")
    return sorted(paths)


def begin_session(limit, attempts, minutes):
    if min(limit, attempts, minutes) < 1:
        raise RuntimeError("Budgets must be positive")
    if STATE.exists() and state().get("active"):
        raise RuntimeError("Defer or finish the active target before replacing the session")
    if changes() or git("diff", "--cached", "--name-only"):
        raise RuntimeError("Begin requires a clean checkout and index")
    directory = STATE.parent / "sessions" / str(time.time_ns())
    command(["make", "validate"], "baseline", directory=directory)
    if STATE.exists() and "directory" not in state():
        write(STATE.parent / "sessions" / f"legacy-{time.time_ns()}" / "state.json", state())
    session = {"head": git("rev-parse", "HEAD"), "protected": fingerprints(),
               "directory": str(directory.relative_to(ROOT)),
               "deadline": time.time() + minutes * 60, "limit": limit,
               "attempt_limit": attempts, "completed": [], "deferred": [], "active": None}
    save_state(session)
    return session


def commit_files(paths, message, expected_hashes=None, *, checkpoint=False):
    """Stage audited files; checkpoint commits are explicitly unvalidated WIP."""
    if checkpoint and not git("branch", "--show-current").startswith("codex/match-"):
        raise RuntimeError("WIP checkpoints require a function branch; main must remain validated")
    if git("diff", "--cached", "--name-only"):
        raise RuntimeError("Commit requires an empty index")
    if paths:
        audit_paths(paths, changes())
    elif not checkpoint or changes():
        raise RuntimeError("Only a clean WIP checkpoint may have no changed files")
    if expected_hashes is not None and content_hashes(paths) != expected_hashes:
        raise RuntimeError("Candidate contents changed after validation")
    if not checkpoint:
        subprocess.run(["git", "diff", "--check"], cwd=ROOT, check=True)
    if paths:
        subprocess.run(["git", "add", "--", *paths], cwd=ROOT, check=True)
    staged = set(git("diff", "--cached", "--name-only").splitlines())
    if staged != set(paths):
        raise RuntimeError("Staged file audit failed")
    if expected_hashes is not None:
        for name in paths:
            data = subprocess.check_output(["git", "show", f":{name}"], cwd=ROOT)
            if hashlib.sha256(data).hexdigest() != expected_hashes[name]:
                raise RuntimeError("Staged contents differ from the validated candidate")
    print("Audited staged paths:", ", ".join(sorted(staged)), flush=True)
    parent = git("rev-parse", "HEAD")
    subprocess.run(["git", "commit", *(["--allow-empty"] if checkpoint else []), "-m", message], cwd=ROOT, check=True)
    commit = git("rev-parse", "HEAD")
    if git("rev-list", "--parents", "-n", "1", commit).split() != [commit, parent]:
        raise RuntimeError("Commit history changed unexpectedly; inspect the function branch")
    if set(git("diff", "--name-only", parent, commit).splitlines()) != set(paths):
        raise RuntimeError("Committed file list differs from the audited candidate")
    if expected_hashes is not None:
        for name in paths:
            data = subprocess.check_output(["git", "show", f"{commit}:{name}"], cwd=ROOT)
            if hashlib.sha256(data).hexdigest() != expected_hashes[name]:
                raise RuntimeError("Committed contents differ from the audited candidate")
    return commit


def commit_changes(session, paths, message, expected_hashes=None):
    """Manual workflow: commit after fresh validation and source review."""
    check_session(session)
    commit = commit_files(paths, message, expected_hashes)
    session["completed"].append({**session["active"], "commit": commit})
    session["head"] = commit
    session["active"] = None
    save_state(session)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("inventory")
    begin = sub.add_parser("begin")
    begin.add_argument("--limit", type=int, default=10)
    begin.add_argument("--attempts", type=int, default=6)
    begin.add_argument("--minutes", type=int, default=120)
    batch = sub.add_parser("run", help="Launch one fresh agent per function; validate and commit from Python")
    batch.add_argument("--limit", type=int, default=10)
    batch.add_argument("--minutes", type=int, default=120)
    batch.add_argument("--agent-seconds", type=int, default=900)
    batch.add_argument("--agent-command", help="Full launcher argv, shell-quoted; prompt on stdin, no shell")
    next_parser = sub.add_parser("next")
    next_parser.add_argument("--address")
    sub.add_parser("status")
    sub.add_parser("check")
    finish = sub.add_parser("finish")
    finish.add_argument("--paths", nargs="+", required=True)
    finish.add_argument("--message", required=True)
    defer = sub.add_parser("defer")
    defer.add_argument("--reason", required=True)
    args = parser.parse_args()
    if args.command == "inventory":
        legacy_inventory()
    elif args.command == "run":
        from reconstruct_run import run_batch
        run_batch(args, sys.modules[__name__])
    elif args.command == "begin":
        begin_session(args.limit, args.attempts, args.minutes)
        print("Session ready; run next to prepare one target.")
    elif args.command == "status":
        print(json.dumps(state(), indent=2))
    else:
        session = state()
        if args.command == "next":
            next_target(session, args.address)
        elif args.command == "check":
            validate(session)
        elif args.command == "defer":
            active = session["active"]
            if not active:
                raise RuntimeError("No active target")
            directory = ROOT / active["context"]
            # Preserve tracked edits and untracked authored sources without resetting anything.
            archive_edits(directory)
            session["deferred"].append({**active, "reason": args.reason})
            session["active"] = None
            save_state(session)
            print("Attempt preserved. Restore only this attempt's edits before next; no files were reset.")
        elif args.command == "finish":
            if git("diff", "--cached", "--name-only"):
                raise RuntimeError("Finish requires an empty index")
            paths = audit_paths(args.paths, changes())
            expected_hashes = content_hashes(paths)
            validate(session)  # Always rerun: earlier reports cannot authorize a commit.
            commit_changes(session, paths, args.message, expected_hashes)


if __name__ == "__main__":
    try:
        import signal
        def interrupted(signum, frame):
            raise KeyboardInterrupt
        signal.signal(signal.SIGTERM, interrupted)
        with exclusive_worker(STATE.parent):
            main()
    except KeyboardInterrupt:
        print("Stopped: interrupted; inspect reconstruction state and archived edits.", file=sys.stderr)
        sys.exit(130)
    except (RuntimeError, subprocess.CalledProcessError, OSError) as error:
        print(f"Stopped: {error}", file=sys.stderr)
        sys.exit(1)
