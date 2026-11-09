"""Subprocess supervision for reconstruct.py run. Python alone accepts and commits."""
import copy
import json
import os
import re
import shlex
import shutil
import subprocess
import time
from pathlib import Path

import yaml


class Rejected(RuntimeError):
    """An ordinary unsuccessful candidate, which can be archived and restored."""


def review_split(before, after, target):
    """Permit only splitting the containing assembly span around the selected C unit."""
    expected = copy.deepcopy(before)
    start, end = target["offset"], target["offset"] + target["size"]
    for index, segment in enumerate(before["segments"]):
        if not isinstance(segment, dict):
            continue
        rows = segment.get("subsegments", [])
        # The current reviewed config has an explicit subsegment after every
        # selectable assembly span, including the final data subsegment.
        for i, row in enumerate(rows[:-1]):
            if not isinstance(row, list) or row[1] != "asm":
                continue
            low, high = row[0], rows[i + 1][0]
            if not low <= start < end <= high:
                continue
            try:
                proposed = after["segments"][index]["subsegments"]
                selected = next(r for r in proposed if r[0] == start and r[1] == "c")
            except (KeyError, IndexError, TypeError, StopIteration) as error:
                raise Rejected("Selected function needs its own C subsegment") from error
            if len(selected) != 3 or not re.fullmatch(r"[A-Za-z_]\w*", str(selected[2])):
                raise Rejected("Automatic C unit names must be plain filenames")
            replacement = []
            if low < start:
                replacement.append(row)
            replacement.append(selected)
            if end < high:
                suffix = next((r for r in proposed if r[0] == end), None)
                if (not isinstance(suffix, list) or len(suffix) != 3 or suffix[1] != "asm"
                        or not re.fullmatch(r"[A-Za-z_]\w*", str(suffix[2]))):
                    raise Rejected("Retained assembly must resume at the original function end")
                replacement.append(suffix)
            expected["segments"][index]["subsegments"][i:i + 1] = replacement
            if after != expected:
                raise Rejected("Split changed options, unrelated segments or boundaries")
            return f"src/{selected[2]}.c"
    raise Rejected("Target is outside a reviewed assembly span")


def review_source(root, paths, entry, symbol):
    """Conservative syntax checks, not a proof of readability or semantic C."""
    if entry not in paths:
        raise Rejected("Selected C translation unit was not added")
    if any(Path(p).suffix == ".c" and p != entry for p in paths):
        raise Rejected("Automatic candidates may add only the selected C translation unit")
    pending = [root / p for p in paths if Path(p).suffix in (".c", ".h")]
    seen = set()
    while pending:
        path = pending.pop()
        if path in seen:
            continue
        seen.add(path)
        if path.is_symlink() or not path.is_file() or not path.resolve().is_relative_to((root / "src").resolve()):
            raise Rejected(f"Source include leaves the project source tree: {path}")
        text = path.read_text()
        clean = re.sub(r"/\*.*?\*/|//[^\n]*", " ", text.replace("\\\n", ""), flags=re.S)
        if re.search(r"\b(?:asm|__asm|__asm__|INCLUDE_ASM|GLOBAL_ASM|__attribute__)\b|\.(?:incbin|word|byte)\b|##", clean):
            raise Rejected(f"Assembly/byte embedding or section attributes in {path.name}")
        if re.search(r"\[[^\]]*\]\s*=", clean):
            raise Rejected(f"Initialized arrays require manual source review: {path.name}")
        for line in clean.splitlines():
            if not line.lstrip().startswith("#"):
                continue
            include = re.fullmatch(r'\s*#\s*include\s+"([A-Za-z_]\w*\.h)"\s*', line)
            if include:
                pending.append(path.parent / include[1])
            elif not re.fullmatch(r"\s*#\s*(?:ifndef\s+\w+|define\s+\w+|endif)\s*", line):
                raise Rejected(f"Unsupported preprocessor directive in {path.name}")
        if path == root / entry and not re.search(r"\b" + re.escape(symbol) + r"\s*\([^;{}]*\)\s*\{", clean):
            raise Rejected("Selected function definition is missing from its C unit")


class Runner:
    def __init__(self, worker, args):
        self.w = worker
        self.args = args
        if min(args.limit, args.minutes, args.agent_seconds) < 1:
            raise RuntimeError("Budgets must be positive")
        try:
            self.launcher = shlex.split(args.agent_command) if args.agent_command is not None else ["codex"]
        except ValueError as error:
            raise RuntimeError("Invalid quoting in --agent-command") from error
        if not self.launcher or not shutil.which(self.launcher[0]):
            raise RuntimeError("Agent launcher is unavailable; set --agent-command to an existing executable")

    def invariant(self, session):
        # Cleanup must remain possible after the time budget expires.
        if self.w.git("rev-parse", "HEAD") != session["head"]:
            raise RuntimeError("Agent changed HEAD; stopping without resetting the checkout")
        if self.w.fingerprints() != session["protected"]:
            raise RuntimeError("Protected build/reference/recipe changed; stopping without resetting the checkout")
        self.w.reference()
        if self.w.git("diff", "--cached", "--name-only"):
            raise RuntimeError("Agent changed the index; stopping without resetting the checkout")

    def paths(self, tracked):
        changed = self.w.changes()
        for name in changed:
            path = Path(name)
            allowed = (name in ("scus_943.58.yaml", "symbol_addrs.txt")
                       or (name not in tracked and len(path.parts) == 2 and path.parts[0] == "src"
                           and path.suffix in (".c", ".h")))
            candidate = self.w.ROOT / path
            if (not allowed or path.is_absolute() or ".." in path.parts or candidate.is_symlink()
                    or not candidate.is_file() or not candidate.resolve().is_relative_to(self.w.ROOT)):
                raise RuntimeError(f"Out-of-scope edit {name}; stopping without resetting the checkout")
        return sorted(changed)

    def launch(self, session):
        active = session["active"]
        directory = self.w.ROOT / active["context"]
        seconds = min(self.args.agent_seconds, session["deadline"] - time.time())
        if seconds <= 0:
            raise Rejected("Session time budget expired before agent launch")
        prompt = (
            f"Implement exactly {active['name']} at {active['address']} ({active['size']} bytes) as matching C.\n"
            f"Read AGENTS.md, {directory / 'context.json'} and {directory / 'target.s'}.\n"
            f"You have at most {int(seconds)} seconds. Inspect callers, callees and referenced data.\n"
            "Review both boundaries, split only this function into a new C unit and preserve all other configuration.\n"
            "You may add plain src/*.c and src/*.h files, edit only the containing Splat span, and append symbols.\n"
            "Do not edit existing source files, build tools, the reference, compiler recipe, docs or queue metadata.\n"
            "Use readable C. No inline assembly, embedded instruction bytes, section attributes, or preprocessor tricks.\n"
            "Only quoted local headers and simple header guards are accepted by the automated source checks.\n"
            "Initialized arrays require manual review; declare arrays uninitialized and assign elements explicitly.\n"
            "Iterate using make objdiff or make validate, and inspect the JSON instruction/relocation diagnostics.\n"
            "Do not invoke reconstruct.py: the supervisor holds its lock. Do not stage, commit, reset, clean or spawn agents.\n"
            "Leave your candidate edits in place. Python independently validates and decides whether to commit.\n"
            "If blocked, leave the best candidate and explain the blocker in your final response.\n")
        (directory / "agent-prompt.md").write_text(prompt)
        (directory / "instructions.md").write_text(prompt)
        env = os.environ.copy()
        temporary = self.w.ROOT / "tools/tmp/reconstruction-agents"
        temporary.mkdir(parents=True, exist_ok=True)
        env.update(TMPDIR=str(temporary), CB2_RECONSTRUCTION_CONTEXT=str(directory / "context.json"),
                   CB2_AGENT_REPORT=str(directory / "agent-last-message.txt"))
        argv = list(self.launcher)
        if self.args.agent_command is None:
            cache = self.w.ROOT / "tools/cache/reconstruction-agents" / Path(session["directory"]).name / active["address"]
            for name in ("logs", "state"):
                (cache / name).mkdir(parents=True, exist_ok=True)
            argv += ["--no-daemon", "--config", f"log_dir={json.dumps(str(cache / 'logs'))}",
                     "--config", f"sqlite_home={json.dumps(str(cache / 'state'))}",
                     "--ask-for-approval", "never", "exec", "--ephemeral",
                     "--sandbox", "workspace-write", "--json", "--output-last-message",
                     env["CB2_AGENT_REPORT"], "-"]
        self.w.write(directory / "launcher.json", {"argv": argv, "timeout_seconds": seconds})
        self.w.command(argv, "agent", timeout=seconds, directory=directory, input_text=prompt, env=env)

    def candidate(self, session, tracked, before, old_symbols, prior_c):
        paths = self.paths(tracked)
        if not paths:
            raise Rejected("Agent left no candidate edits")
        try:
            after = yaml.safe_load((self.w.ROOT / "scus_943.58.yaml").read_text())
            entry = review_split(before, after, session["active"])
        except (yaml.YAMLError, KeyError, TypeError, IndexError) as error:
            raise Rejected("Invalid candidate split configuration") from error
        symbols = (self.w.ROOT / "symbol_addrs.txt").read_text()
        if not symbols.startswith(old_symbols):
            raise Rejected("Existing symbol definitions were changed")
        review_source(self.w.ROOT, paths, entry, session["active"]["name"])
        expected_hashes = self.w.content_hashes(paths)
        current = self.w.validate(session)  # Agent messages/scores never substitute for this gate.
        now_c = {r["address"]: r["size"] for r in current["functions"] if r["status"] == "c"}
        if now_c != {**prior_c, session["active"]["address"]: session["active"]["size"]}:
            raise Rejected("C coverage changed outside the selected function")
        self.invariant(session)
        if paths != self.paths(tracked):
            raise RuntimeError("Checkout changed during acceptance validation")
        if self.w.content_hashes(paths) != expected_hashes:
            raise Rejected("Candidate contents changed during acceptance validation")
        self.w.check_session(session)
        return paths, expected_hashes

    def reject(self, session, tracked, reason):
        active = session["active"]
        archive = self.w.ROOT / active["context"] / "rejected"
        archive.mkdir(parents=True, exist_ok=True)
        self.w.archive_edits(archive)
        active["failure"] = reason
        self.w.save_state(session)
        # Never erase unexpected edits, index changes, or another process's commits.
        self.invariant(session)
        paths = self.paths(tracked)
        for name in paths:
            path = self.w.ROOT / name
            if name in tracked:
                path.write_bytes(subprocess.check_output(["git", "show", f"HEAD:{name}"], cwd=self.w.ROOT))
            else:
                path.unlink()
        if self.w.changes():
            raise RuntimeError("Checkout did not return to the passing baseline")
        self.w.command(["make", "validate"], "restored-baseline", directory=archive)
        session["deferred"].append({**active, "reason": reason})
        session["active"] = None
        self.w.save_state(session)
        print(f"Deferred {active['name']}: {reason}", flush=True)

    def run(self):
        session = self.w.begin_session(self.args.limit, 1, self.args.minutes)
        session["mode"] = "automatic"
        self.w.save_state(session)
        reason = "target limit reached"
        while len(session["completed"]) + len(session["deferred"]) < session["limit"]:
            if time.time() >= session["deadline"]:
                reason = "session time budget exhausted"
                break
            try:
                self.w.next_target(session)
            except self.w.NoTarget:
                reason = "eligible queue exhausted"
                break
            except RuntimeError:
                if time.time() >= session["deadline"] and session["active"] is None:
                    reason = "session time budget exhausted"
                    break
                raise
            before = yaml.safe_load((self.w.ROOT / "scus_943.58.yaml").read_text())
            old_symbols = (self.w.ROOT / "symbol_addrs.txt").read_text()
            tracked = set(self.w.git("ls-files").splitlines())
            prior_c = {r["address"]: r["size"] for r in self.w.read(self.w.INVENTORY)["functions"] if r["status"] == "c"}
            launched = False
            try:
                self.launch(session)
                launched = True
                self.invariant(session)
                paths, expected_hashes = self.candidate(session, tracked, before, old_symbols, prior_c)
            except KeyboardInterrupt:
                self.reject(session, tracked, "Interrupted during agent/validation; candidate archived")
                raise
            except (RuntimeError, subprocess.CalledProcessError, OSError, UnicodeError) as error:
                self.reject(session, tracked, str(error))
                if not launched and not isinstance(error, self.w.CommandTimeout):
                    # A broken/auth-failing launcher should not burn the entire queue.
                    reason = "agent launcher failed; inspect the archived log"
                    break
                continue
            # Commit errors leave the candidate/index available for inspection.
            # They must never trigger rollback after Git may have committed.
            self.w.commit_changes(session, paths, f"Match {session['active']['name']} from bounded agent run", expected_hashes)
        session["stop_reason"] = reason
        self.w.save_state(session)
        print(f"Batch stopped: {reason}; {len(session['completed'])} committed, "
              f"{len(session['deferred'])} deferred. State: {self.w.STATE}", flush=True)
        if reason.startswith("agent launcher failed"):
            raise RuntimeError(reason)


def run_batch(args, worker):
    Runner(worker, args).run()
