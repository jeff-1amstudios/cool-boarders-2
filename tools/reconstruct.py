#!/usr/bin/env python3
"""Bounded, headless reconstruction work queue; an agent supplies the C edits."""
import argparse
import hashlib
import json
import os
import subprocess
import sys
import time
import shutil
from contextlib import contextmanager
from pathlib import Path

from rebuild import ROOT, reference

STATE = ROOT / "reports/reconstruction/state.json"
REVIEW = ROOT / "config/reconstruction.json"
PROTECTED = ("config/usa-disc.json", "config/compiler.json", "tools/rebuild.py", "tools/c_sources.py",
             "tools/validate.py", "tools/objdiff.py", "tools/reconstruct.py",
             "tools/reconstruct_run.py", "tools/boundary_audit.py",
             "tools/ghidra_scripts/ExportBoundaryEvidence.java", "tools/ghidra_scripts/ReviewBoundaries.java",
             "tools/ghidra-docker.sh",
             "config/function-boundaries.json", "config/reconstruction.json", "config/psyq-sdk.json",
             "config/overlays.json", "tools/overlays.py", "tools/ghidra_scripts/ImportOverlays.java",
             "tools/ghidra_scripts/ExportOverlayInventory.java", "tools/ghidra_scripts/ExportAllOverlays.java",
             "tools/ghidra_scripts/AuditOverlayReferences.java", "tools/overlay_rebuild.py",
             "tools/splat_ext/bytes.py", "tools/target_queue.py", "tools/reconstruction_backlog.py", "tools/reconstruction_step.py", "tools/reconstruction_runtime.py", "Makefile") + tuple(
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
        from reconstruction_runtime import follow_log
        import threading
        done = threading.Event()
        output = threading.Thread(target=follow_log, args=(path, done, name == "agent", os.environ.get("CB2_VERBOSE") == "1"), daemon=True)
        output.start()
        try:
            process.communicate(input=input_text, timeout=timeout)
            code = process.returncode
        except BaseException as error:
            import signal
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            process.wait()
            if isinstance(error, subprocess.TimeoutExpired):
                raise CommandTimeout(f"Command timed out; inspect {path}") from error
            raise
        finally:
            done.set()
            output.join()
    if code:
        raise RuntimeError(f"Command failed ({code}); inspect {path}")


def inventory():
    from reconstruction_backlog import inventory as complete_inventory
    return complete_inventory()


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


def choose(report, session, identity=None):
    if read(REVIEW).get("matching_paused", False):
        raise NoTarget("C matching is paused by config/reconstruction.json")
    from reconstruction_backlog import key
    attempted = {key(r) for r in session["completed"] + session["deferred"]}
    for row in report["targets"]:
        if row["status"] == "needs_review" and key(row) not in attempted and (identity is None or key(row) == identity):
            return row
    raise NoTarget("No remaining targets for this batch")


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
    write(directory / "context.json", {**{k: v for k, v in row.items() if k != "body"}, "compiler_config": recipe, "compiler": read(ROOT / recipe)})
    session["active"] = {**{k: row[k] for k in ("identity", "image", "build_config", "symbol_file", "boundary_findings", "boundary_warnings", "source_directory", "ranges", "body_sha256") if k in row},
                         "address": row["address"], "name": row["name"], "size": row["size"],
                         "offset": row["offset"],
                         "context": str(directory.relative_to(ROOT))}
    save_state(session)
    return report


def validation_command(active):
    return ["make", "validate", *(["IMAGE=" + active["image"]] if "image" in active else []),
            *(["UNIT=" + active["unit"]] if "image" in active and active.get("unit") else [])]


def validate(session):
    check_session(session)
    active = session["active"]
    if not active:
        raise RuntimeError("No active target")
    directory = ROOT / active["context"]
    attempt = directory / "candidate"
    attempt.mkdir(parents=True, exist_ok=True)
    archive_edits(attempt)
    try:
        command(validation_command(active), "validate", directory=attempt,
                timeout=min(300, max(1, session["deadline"] - time.time())))
    finally:
        image = active.get("image", "SCUS_943.58")
        report_path = ROOT / 'reports/boot-rebuild.json' if image == 'SCUS_943.58' else ROOT / 'reports/overlay-rebuild' / (Path(active['build_config']).stem + '.json')
        if report_path.exists():
            shutil.copy2(report_path, directory / 'validation.json')
        if active.get('unit'):
            diff = ROOT / 'reports/objdiff' / (active['unit'] + '.json')
            if diff.exists():
                shutil.copy2(diff, directory / 'object-diff.json')
    image = active.get("image", "SCUS_943.58")
    if image == "SCUS_943.58":
        report = read(ROOT / "reports/boot-rebuild.json")
        _, original, sha = reference()
        if not report["identical"] or report["reference_sha256"] != sha or (ROOT / "build/SCUS_943.58").read_bytes() != original:
            raise RuntimeError("Full executable gate failed")
    else:
        from overlays import load
        catalog, _ = load()
        spec = catalog['overlays'][image]
        report = read(ROOT / 'reports/overlay-rebuild' / (Path(spec['build_config']).stem + '.json'))
        rebuilt = ROOT / 'build/overlays' / Path(spec['build_config']).stem / image
        if not report['identical'] or rebuilt.read_bytes() != (ROOT / 'orig/disc/files' / image).read_bytes():
            raise RuntimeError('Full overlay gate failed: ' + image)
    current = inventory()
    from reconstruction_backlog import key
    target = next((r for r in current.get("targets", current.get("functions", [])) if key(r) == key(active)), None)
    if target is None or target.get("reconstruction", target["status"]) != "c" or target.get("body_bytes", target.get("size")) != active["size"]:
        raise RuntimeError("Target was not converted to C at its original boundary")
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
        allowed = (name in ("scus_943.58.yaml", "symbol_addrs.txt")
                   or (name.startswith("config/overlays/") and path.suffix in (".yaml", ".txt"))
                   or (len(path.parts) > 1 and path.parts[0] == "src" and path.suffix in (".c", ".h"))
)
        if not allowed or path.is_absolute() or ".." in path.parts:
            raise RuntimeError(f"Not an authored reconstruction path: {name}")
        if (ROOT / path).is_symlink():
            raise RuntimeError(f"Refusing symlink: {name}")
    return sorted(paths)


def begin_batch(limit, minutes):
    if min(limit, minutes) < 1:
        raise RuntimeError("Budgets must be positive")
    if STATE.exists() and state().get("active"):
        raise RuntimeError("Inspect the recorded active attempt before starting another batch")
    if changes() or git("diff", "--cached", "--name-only"):
        raise RuntimeError("Batch requires a clean checkout and index")
    directory = STATE.parent / "sessions" / str(time.time_ns())
    command(["make", "validate"], "baseline", directory=directory)
    session = {"head": git("rev-parse", "HEAD"), "protected": fingerprints(),
               "directory": str(directory.relative_to(ROOT)),
               "deadline": time.time() + minutes * 60, "limit": limit,
               "completed": [], "deferred": [], "active": None}
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


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    batch = sub.add_parser("start", aliases=["run"], help="Match targets serially, streaming stdout")
    batch.add_argument("--limit", type=int, default=10)
    batch.add_argument("--minutes", type=int, default=120)
    batch.add_argument("--agent-seconds", type=int, default=900, help="Hard timeout per agent")
    batch.add_argument("--verbose", action="store_true", help="Also print echoed compiler/assembler commands")
    batch.add_argument("--agent-command", help="Custom launcher argv; prompt on stdin, no shell")
    sub.add_parser("status", help="Show the current or last batch")
    args = parser.parse_args()
    if args.command == "status":
        session = state()
        print(json.dumps({"directory":session['directory'], "head":session['head'],
                         "completed":len(session['completed']), "deferred":len(session['deferred']),
                         "active":session.get('active'), "stop_reason":session.get('stop_reason'),
                         "result":session.get('result')},indent=2))
    else:
        if args.verbose:
            os.environ["CB2_VERBOSE"] = "1"
        from reconstruct_run import run_batch
        run_batch(args, sys.modules[__name__])


if __name__ == "__main__":
    try:
        import signal
        def interrupted(signum, frame):
            raise KeyboardInterrupt
        signal.signal(signal.SIGTERM, interrupted)
        if len(sys.argv) > 1 and sys.argv[1] == "status":
            main()
        else:
            with exclusive_worker(STATE.parent):
                main()
    except KeyboardInterrupt:
        print("Stopped: interrupted; inspect reconstruction state and preserved work.", file=sys.stderr)
        sys.exit(130)
    except (RuntimeError, subprocess.CalledProcessError, OSError) as error:
        print(f"Stopped: {error}", file=sys.stderr)
        sys.exit(1)
