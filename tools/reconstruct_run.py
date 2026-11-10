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
from reconstruction_backlog import key, slug, source_dir, source_file, coverage
from c_sources import sections, function_source


class Rejected(RuntimeError):
    """An ordinary unsuccessful candidate, which can be archived and restored."""


def review_literal_split(expected, after, unit):
    """Allow one existing rodata span to supply the selected C object's literals."""
    candidates = [(i, j, row) for i, segment in enumerate(after['segments']) if isinstance(segment, dict)
                  for j, row in enumerate(segment.get('subsegments', []))
                  if isinstance(row, list) and len(row) == 3 and row[1:] == ['.rodata', unit]]
    if not candidates:
        return
    if len(candidates) != 1:
        raise Rejected('The selected C object needs one contiguous literal rodata span')
    index, position, literal = candidates[0]
    proposed = after['segments'][index]['subsegments']
    if position + 1 >= len(proposed):
        raise Rejected('Literal rodata needs an explicit end boundary')
    following = proposed[position + 1]
    end = following.get('start') if isinstance(following, dict) else following[0]
    rows = expected['segments'][index].get('subsegments', [])
    for i, row in enumerate(rows[:-1]):
        next_row = rows[i + 1]
        high = next_row.get('start') if isinstance(next_row, dict) else next_row[0]
        if not (isinstance(row, list) and row[1] == 'rodata' and row[0] <= literal[0] < end <= high):
            continue
        replacement = ([row] if row[0] < literal[0] else []) + [literal]
        if end < high:
            if (not isinstance(following, list) or len(following) != 3 or following[1] != 'rodata'
                    or not re.fullmatch(r'[A-Za-z_]\w*', str(following[2]))):
                raise Rejected('Retained rodata must resume at the literal end')
            replacement.append(following)
        rows[i:i + 1] = replacement
        return
    raise Rejected('Literal ownership must split one existing retained rodata span')


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
            following = rows[i + 1]
            low = row[0]
            high = following[0] if isinstance(following, list) else following["start"]
            if not low <= start < end <= high:
                continue
            try:
                proposed = after["segments"][index]["subsegments"]
                selected = next(r for r in proposed if isinstance(r, list) and r[0] == start and r[1] == "c")
            except (KeyError, IndexError, TypeError, StopIteration) as error:
                raise Rejected("Selected function needs its own C subsegment") from error
            if len(selected) != 3 or not re.fullmatch(r"[A-Za-z_]\w*", str(selected[2])):
                raise Rejected("Automatic C unit names must be plain filenames")
            replacement = []
            if low < start:
                replacement.append(row)
            replacement.append(selected)
            if end < high:
                suffix = next((r for r in proposed if isinstance(r, list) and r[0] == end), None)
                if (not isinstance(suffix, list) or len(suffix) != 3 or suffix[1] != "asm"
                        or not re.fullmatch(r"[A-Za-z_]\w*", str(suffix[2]))):
                    raise Rejected("Retained assembly must resume at the original function end")
                replacement.append(suffix)
            expected["segments"][index]["subsegments"][i:i + 1] = replacement
            review_literal_split(expected, after, selected[2])
            if after != expected:
                raise Rejected("Split changed options, unrelated segments or boundaries")
            return source_file(target)
    raise Rejected("Target is outside a reviewed assembly span")


def review_source(root, paths, entry, symbol, *, checkpoint=False):
    """Conservative syntax checks, not a proof of readability or semantic C."""
    if not checkpoint and entry not in paths:
        raise Rejected("Selected overlay C source was not edited")
    if not checkpoint and any(Path(p).suffix == ".c" and p != entry for p in paths):
        raise Rejected("Automatic candidates may edit only the selected overlay C source")
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
        if re.search(r"/\*\s*[0-9A-Fa-f]+\s+8[0-9A-Fa-f]{7}\s+[0-9A-Fa-f]{8}\s*\*/", text):
            raise Rejected(f"Raw disassembly comments in {path.name}")
        clean = re.sub(r"/\*.*?\*/|//[^\n]*", " ", text.replace("\\\n", ""), flags=re.S)
        if path.suffix == '.c' and re.search(r'\b(?:struct|union)\s*(?:[A-Za-z_]\w*\s*)?\{', clean):
            raise Rejected(f"Struct and union definitions belong in shared headers: {path.name}")
        if path.suffix == '.c':
            _, blocks = sections(text)
            body = re.sub(r'/\*.*?\*/|//[^\n]*', ' ', ''.join(blocks.values()), flags=re.S)
            if re.search(r'\bextern\b', body):
                raise Rejected(f"Extern declarations belong at the top before FUNCTION annotations: {path.name}")
        if re.search(r"\b(?:asm|__asm|__asm__|INCLUDE_ASM|GLOBAL_ASM|__attribute__)\b|\.(?:incbin|word|byte)\b|##", clean):
            raise Rejected(f"Assembly/byte embedding or section attributes in {path.name}")
        if re.search(r'\[[^\]]*\]\s*=\s*(?:\{|")', clean):
            raise Rejected(f"Initialized arrays require manual source review: {path.name}")
        for line in clean.splitlines():
            if not line.lstrip().startswith("#"):
                continue
            include = re.fullmatch(r'\s*#\s*include\s+"([A-Za-z_]\w*\.h)"\s*', line)
            if include:
                pending.append(path.parent / include[1])
            elif not re.fullmatch(r"\s*#\s*(?:ifndef\s+\w+|define\s+\w+|endif)\s*", line):
                raise Rejected(f"Unsupported preprocessor directive in {path.name}")
        if not checkpoint and path == root / entry and not re.search(r"\b" + re.escape(symbol) + r"\s*\([^;{}]*\)\s*\{", clean):
            raise Rejected("Selected function definition is missing from its C unit")


def review_header_extension(before, after):
    """Allow new types before the closing guard without changing established layouts."""
    closing = re.search(r'(?m)^#endif[^\S\n]*\n?\Z', before)
    if (closing is None or not after.startswith(before[:closing.start()])
            or not after.endswith(before[closing.start():])):
        raise Rejected('Existing header declarations or guard changed; append before the final #endif')


def review_shared_extension(before, after):
    """Permit new top-level externs/includes while protecting established declarations."""
    if not after.startswith(before):
        raise Rejected('Existing shared declarations changed')
    added = re.sub(r'/\*.*?\*/|//[^\n]*', ' ', after[len(before):], flags=re.S)
    added = re.sub(r'\bextern\s+[^;{}=]+;|#[ \t]*include[ \t]+"[A-Za-z_]\w*\.h"', '', added)
    if added.strip():
        raise Rejected('Only extern declarations and local includes may be added before FUNCTION annotations')


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
        if self.w.git("branch", "--show-current") != session["branch"]:
            raise RuntimeError("Agent changed branch; stopping without resetting the checkout")
        if self.w.git("rev-parse", "refs/heads/main") != session["integration_head"]:
            raise RuntimeError("main moved unexpectedly; stopping without updating it")
        if not self.w.protected_unchanged(session):
            raise RuntimeError("Protected build/reference/recipe changed; stopping without resetting the checkout")
        self.w.reference()
        if "identity" in (session.get("active") or {}):
            from overlays import load
            load()
        if self.w.git("diff", "--cached", "--name-only"):
            raise RuntimeError("Agent changed the index; stopping without resetting the checkout")

    def switch(self, session, branch, head):
        self.w.git("switch", branch)
        if self.w.git("rev-parse", "HEAD") != head:
            raise RuntimeError(f"Unexpected tip for {branch}; inspect the recorded branch state")
        session.update(branch=branch, head=head)
        self.w.save_state(session)

    def start_branch(self, session):
        self.invariant(session)
        if session["branch"] != "main" or self.w.changes():
            raise RuntimeError("Function branches must start from clean main")
        base = "codex/match-" + (slug(session["active"]) if "identity" in session["active"] else session["active"]["address"][2:])
        existing = set(self.w.git("for-each-ref", "--format=%(refname:short)", "refs/heads").splitlines())
        if "codex" in existing:
            raise RuntimeError("Branch codex blocks the codex/ function branch namespace")
        # Failed attempts are disposable: reuse the name, starting from main.
        for old in sorted(existing):
            if old == base or re.fullmatch(re.escape(base) + r"-\d+", old):
                self.w.git("branch", "-D", old)
        branch = base
        session["active"].update(branch=branch, base_commit=session["integration_head"], phase="creating_branch")
        self.w.save_state(session)
        self.w.git("switch", "-c", branch, "main")
        session["branch"] = branch
        session["active"]["phase"] = "working"
        self.w.save_state(session)
        self.invariant(session)
        print(f"Function branch: {branch} (base {session['integration_head'][:12]})", flush=True)

    def paths(self, tracked, active):
        directory = Path(source_dir(active))
        changed = self.w.changes()
        for name in changed:
            path = Path(name)
            allowed = (name in (source_file(active), str(Path(source_file(active)).with_suffix('.h')),
                               active.get("build_config", "scus_943.58.yaml"), active.get("symbol_file", "symbol_addrs.txt"))
                       or (name not in tracked and path.parent == directory
                           and path.suffix == ".h"))
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
            f"Implement exactly {active['name']} at {key(active)} ({active['size']} bytes) as matching C.\n"
            f"Work on the existing branch {active['branch']}. Python owns all branch switches and commits.\n"
            f"Read AGENTS.md, {directory / 'context.json'} and {directory / 'target.s'}.\n"
            f"Start from the CURRENT Ghidra pseudocode in {directory / 'ghidra/target.ghidra.c'}, with named SDK calls and typed variables.\n"
            f"Read {directory / 'ghidra/ghidra-types.h'} for relevant layouts/prototypes and context.json ghidra metadata for parameter storage and callees.\n"
            "Adapt that pseudocode to compilable matching C. Use assembly to resolve uncertainty and refine compiler output; do not reconstruct the function from assembly alone.\n"
            "Preserve Ghidra string literals as C literals; do not replace them with extern address symbols to sidestep rodata ownership. You may split one existing retained rodata span into retained rodata / .rodata for this C unit / retained rodata, keeping all original offsets and unrelated boundaries. The .rodata name must equal your C subsegment name; include an explicit following boundary.\n"
            f"You have at most {int(seconds)} seconds. Inspect callers, callees and referenced data.\n"
            "Review both boundaries, give only this function a C subsegment and preserve all other configuration.\n"
            "Use the context name as the build symbol; ghidra_name is an analysis label and can differ.\n"
            "Start from current main. Do not restore candidates from earlier failed attempts.\n"
            f"Append the function to {source_file(active)}, starting with // FUNCTION: {key(active)}.\n"
            "Keep all extern declarations together at the top, before the first FUNCTION annotation. Reuse existing declarations; append new externs to that shared block. Each annotation owns only the function text until the next annotation.\n"
            f"Define structs and unions once in headers. Reuse existing types in {Path(source_file(active)).with_suffix('.h')}; add new types before its final #endif or create a guarded local header. Do not duplicate layouts in C.\n"
            f"Add headers only under {source_dir(active)}; edit only {active.get('build_config', 'scus_943.58.yaml')} and append to {active.get('symbol_file', 'symbol_addrs.txt')}.\n"
            "Explain boundary/dependency conclusions and any concrete blocker in your final response. No review forms are required.\n"
            "boundary_warnings are heuristic review hints, not proof that C is impossible. Curated holds and factual split blockers remain excluded.\n"
            "Preserve existing annotated blocks, shared declarations and header definitions exactly. Add only externs/local includes at the end of the shared top block, and append new types before the image header's final #endif. Do not edit other existing source files, build tools, the reference, compiler recipe, docs or queue metadata.\n"
            "Use readable C. No inline assembly, embedded instruction bytes, section attributes, or preprocessor tricks.\n"
            "Only quoted local headers and simple header guards are accepted by the automated source checks.\n"
            "Initialized arrays require manual review; declare arrays uninitialized and assign elements explicitly.\n"
            "Keep reads focused on this context, selected span and relevant callers/callees. Do not dump whole configs, all sources or full build logs.\n"
            f"Write {directory / 'ghidra-update.json'} with exactly {{\"function_evidence\": \"observed argument/return evidence and remaining uncertainty\", \"globals\": []}}.\n"
            "For each newly recovered global type, add a globals entry with image, address (hex string), name and evidence. It must have an authored extern declaration and matching symbol address in the selected image's symbols. Use the resident image for resident globals. Headers and the selected prototype are imported automatically after acceptance. Do not copy unrelated or speculative types.\n"
            "Use tools/.venv/bin/python tools/reconstruction_step.py for every compile/edit iteration. It compiles only your unit and prints a short diff.\n"
            "Use the hard time allowance sensibly. Restore your best candidate before finishing; if blocked, explain why and stop.\n"
            "Your scope is the selected function: once its object matches and the feedback proposal is written, finish. Python independently validates the entire containing EXE/BIN and updates Ghidra. Do not run whole-file validation yourself.\n"
            "Do not repeatedly run whole-project objdiff or all-image validation. Read object-diff.json when the compact diff is insufficient.\n"
            "Do not invoke reconstruct.py: the supervisor holds its lock. Do not stage, commit, switch branches, reset, clean or spawn agents.\n"
            "Leave your candidate edits in place. Python independently validates and decides whether to commit.\n"
            "If blocked, leave the best candidate and explain the blocker in your final response.\n")
        (directory / "agent-prompt.md").write_text(prompt)
        env = os.environ.copy()
        temporary = self.w.ROOT / "tools/tmp/reconstruction-agents"
        temporary.mkdir(parents=True, exist_ok=True)
        env.update(TMPDIR=str(temporary), CB2_RECONSTRUCTION_CONTEXT=str(directory / "context.json"),
                   CB2_AGENT_REPORT=str(directory / "agent-last-message.txt"))
        argv = list(self.launcher)
        if self.args.agent_command is None:
            cache = self.w.ROOT / "tools/cache/reconstruction-agents" / Path(session["directory"]).name / slug(active)
            for name in ("logs", "state"):
                (cache / name).mkdir(parents=True, exist_ok=True)
            argv += ["--no-daemon", "--config", f"log_dir={json.dumps(str(cache / 'logs'))}",
                     "--config", f"sqlite_home={json.dumps(str(cache / 'state'))}",
                     "--ask-for-approval", "never", "exec", "--ephemeral",
                     "--sandbox", "workspace-write", "--json", "-"]
        self.w.command(argv, "agent", timeout=seconds, directory=directory, input_text=prompt, env=env)

    def candidate(self, session, tracked, before, old_symbols, prior_c):
        active = session["active"]
        paths = self.paths(tracked, session["active"])
        if not paths:
            raise Rejected("Agent left no candidate edits")
        try:
            after = yaml.safe_load((self.w.ROOT / session["active"].get("build_config", "scus_943.58.yaml")).read_text())
            entry = review_split(before, after, session["active"])
        except (yaml.YAMLError, KeyError, TypeError, IndexError) as error:
            raise Rejected("Invalid candidate split configuration") from error
        symbols = (self.w.ROOT / session["active"].get("symbol_file", "symbol_addrs.txt")).read_text()
        if not symbols.startswith(old_symbols):
            raise Rejected("Existing symbol definitions were changed")
        review_source(self.w.ROOT, paths, entry, session["active"]["name"])
        for name in paths:
            if name in tracked and Path(name).suffix == '.h':
                previous_header = subprocess.check_output(['git', 'show', f'HEAD:{name}'], cwd=self.w.ROOT).decode()
                review_header_extension(previous_header, (self.w.ROOT / name).read_text())
        text = (self.w.ROOT / entry).read_text()
        try:
            previous = ''
            # Read bytes directly: git() strips whitespace, which is significant here.
            if entry in tracked:
                previous = subprocess.check_output(['git', 'show', f'HEAD:{entry}'], cwd=self.w.ROOT).decode()
            shared, blocks = sections(previous)
            new_shared, new_blocks = sections(text)
            review_shared_extension(shared, new_shared)
            identity = key(active)
            if new_blocks != {**blocks, identity: new_blocks.get(identity)} or identity in blocks:
                raise Rejected('Existing overlay source or unrelated function blocks changed')
            function_source(text, identity)
            from reconstruction_ghidra import literal_tokens
            if set(active.get('ghidra_literals', [])) - literal_tokens(new_blocks[identity]):
                raise Rejected('Preserve Ghidra string literals in the selected C function; do not substitute extern address symbols')
        except ValueError as error:
            raise Rejected(str(error)) from error
        subprocess.run(["git", "diff", "--check"], cwd=self.w.ROOT, check=True)
        if "image" in active:
            active["unit"] = next('/'.join(p for p in (s.get('dir', ''), r[2]) if p)
                                  for s in after['segments'] if isinstance(s, dict)
                                  for r in s.get('subsegments', []) if isinstance(r, list)
                                  and r[1] == 'c' and s['vram'] + r[0] - s['start'] == int(active['address'], 16))
        expected_hashes = self.w.content_hashes(paths)
        current = self.w.validate(session)  # Agent messages/scores never substitute for this gate.
        now_c = coverage(current)
        if now_c != {**prior_c, key(session["active"]): session["active"]["size"]}:
            raise Rejected("C coverage changed outside the selected function")
        self.invariant(session)
        if paths != self.paths(tracked, session["active"]):
            raise RuntimeError("Checkout changed during acceptance validation")
        if self.w.content_hashes(paths) != expected_hashes:
            raise Rejected("Candidate contents changed during acceptance validation")
        self.w.check_session(session)
        from reconstruction_ghidra import prepare_update
        prepare_update(self.w, active)
        self.invariant(session)
        if self.w.content_hashes(paths) != expected_hashes:
            raise Rejected("Candidate changed during Ghidra feedback planning")
        return paths, expected_hashes

    def reject(self, session, tracked, reason):
        active = session["active"]
        archive = self.w.ROOT / active["context"] / "candidate"
        archive.mkdir(parents=True, exist_ok=True)
        self.w.archive_edits(archive)
        active["failure"] = reason
        self.w.save_state(session)
        # Never erase unexpected edits, index changes, or another process's commits.
        self.invariant(session)
        paths = self.paths(tracked, session["active"])
        try:
            # A WIP may be incomplete/nonmatching, but it must still obey the
            # repository boundary: never put assembly dumps or binaries in Git.
            review_source(self.w.ROOT, paths, "", active["name"], checkpoint=True)
        except (Rejected, UnicodeError) as error:
            active["checkpoint_excluded"] = str(error)
            for name in paths:
                path = self.w.ROOT / name
                if name in tracked:
                    path.write_bytes(subprocess.check_output(["git", "show", f"HEAD:{name}"], cwd=self.w.ROOT))
                else:
                    path.unlink()
            paths = []
        active["phase"] = "checkpointing"
        self.w.save_state(session)
        message = (f"WIP: {active['name']} (not validated for main)\n\n"
                   f"Target: {key(active)}\nBaseline: {active['base_commit']}\n"
                   f"Blocker: {reason.replace(str(self.w.ROOT), '.')[:2000]}")
        if active.get("checkpoint_excluded"):
            message += f"\nCandidate kept only in local archive: {active['checkpoint_excluded']}"
        checkpoint = self.w.commit_files(paths, message, self.w.content_hashes(paths), checkpoint=True)
        active.update(checkpoint=checkpoint, phase="checkpointed")
        session["head"] = checkpoint
        self.w.save_state(session)
        self.invariant(session)
        self.switch(session, "main", session["integration_head"])
        active["phase"] = "restoring_main"
        self.w.save_state(session)
        self.invariant(session)
        if self.w.changes():
            raise RuntimeError("main checkout is not clean after switching from the function branch")
        self.w.command(self.w.validation_command({k:v for k,v in active.items() if k != "unit"}), "restored-baseline", directory=archive)
        active["phase"] = "deferred"
        from reconstruction_runtime import attempt_result
        result = attempt_result(self.w.ROOT, active, "deferred", reason)
        self.w.write(self.w.ROOT / active["context"] / "result.json", result)
        session["deferred"].append({**active, "reason": reason})
        self.w.remember_deferred(session["deferred"][-1])
        session["active"] = None
        self.w.save_state(session)
        print(f"Deferred {active['name']} on {active['branch']} ({checkpoint[:12]}): {reason}", flush=True)

    def accept(self, session, paths, expected_hashes):
        # Git/merge errors deliberately preserve the phase, branch, candidate and
        # index for inspection. Never turn a partially completed commit into WIP.
        active = session["active"]
        self.w.check_session(session)
        self.invariant(session)
        from reconstruction_ghidra import record, apply
        payload = self.w.read(self.w.ROOT / active['context'] / 'ghidra-feedback/request.json')
        feedback_path = record(self.w, active, payload)
        paths = [*paths, feedback_path]
        expected_hashes = {**expected_hashes, **self.w.content_hashes([feedback_path])}
        active['ghidra_record'] = feedback_path
        active["phase"] = "committing_match"
        self.w.save_state(session)
        commit = self.w.commit_files(paths, f"Match {active['name']} from bounded agent run", expected_hashes)
        session["head"] = commit
        active.update(commit=commit, phase="validated_commit")
        self.w.save_state(session)
        self.invariant(session)
        self.switch(session, "main", session["integration_head"])
        active["phase"] = "integrating"
        self.w.save_state(session)
        self.invariant(session)
        self.w.git("merge", "--ff-only", commit)
        if self.w.git("rev-parse", "HEAD") != commit or self.w.git("branch", "--show-current") != "main":
            raise RuntimeError("Unexpected integration result; inspect main and the validated function branch")
        session.update(head=commit, integration_head=commit, protected=self.w.fingerprints())
        self.invariant(session)
        if self.w.changes():
            raise RuntimeError("Integration left unexpected checkout changes")
        active['phase'] = 'syncing_ghidra'
        self.w.save_state(session)
        # A failure here stops the batch with the accepted commit and pending
        # record intact. Never launch another worker using stale analysis.
        apply(self.w, active, payload)
        from reconstruction_runtime import attempt_result
        self.w.write(self.w.ROOT / active["context"] / "result.json", attempt_result(self.w.ROOT, active, "matched"))
        session["completed"].append({**active, "phase": "integrated"})
        session["active"] = None
        self.w.save_state(session)
        print(f"Integrated {active['branch']} into main at {commit[:12]}", flush=True)

    def run(self):
        if self.w.git("branch", "--show-current") != "main":
            raise RuntimeError("Automatic batches must start on the main integration branch")
        if self.w.read(self.w.REVIEW).get("matching_paused", False):
            raise RuntimeError("C matching is paused by the boundary-audit policy; no batch started")
        session = self.w.begin_batch(self.args.limit, self.args.minutes)
        session.update(branch="main", integration_head=session["head"])
        self.w.save_state(session)
        reason = "target limit reached"
        while len(session["completed"]) + len(session["deferred"]) < session["limit"]:
            if time.time() >= session["deadline"]:
                reason = "session time budget exhausted"
                break
            try:
                current_inventory = self.w.next_target(session)
            except self.w.NoTarget:
                reason = "eligible queue exhausted"
                break
            except RuntimeError:
                if time.time() >= session["deadline"] and session["active"] is None:
                    reason = "session time budget exhausted"
                    break
                raise
            active = session["active"]
            number = len(session["completed"]) + len(session["deferred"]) + 1
            print(f"\nTarget {number}/{session['limit']}: {key(active)} ({active['size']} bytes)", flush=True)
            if not active.get('boundary_findings'):
                from reconstruction_ghidra import export
                try:
                    export(self.w, active)
                except BaseException:
                    # An unavailable/locked Ghidra project is infrastructure
                    # failure, not evidence against reconstructing this target.
                    session.update(active=None, stop_reason='Fresh Ghidra export failed before agent launch')
                    self.w.save_state(session)
                    raise
            self.start_branch(session)
            before = yaml.safe_load((self.w.ROOT / session["active"].get("build_config", "scus_943.58.yaml")).read_text())
            old_symbols = (self.w.ROOT / session["active"].get("symbol_file", "symbol_addrs.txt")).read_text()
            tracked = set(self.w.git("ls-files").splitlines())
            prior_c = coverage(current_inventory)
            launched = False
            try:
                if session["active"].get("boundary_findings"):
                    launched = True
                    raise Rejected("Selected-target split blocker: " + json.dumps(session["active"]["boundary_findings"]))
                self.launch(session)
                launched = True
                self.invariant(session)
                paths, expected_hashes = self.candidate(session, tracked, before, old_symbols, prior_c)
            except KeyboardInterrupt:
                self.reject(session, tracked, "Interrupted during agent/validation; candidate archived")
                reason = "interrupted"
                break
            except (RuntimeError, subprocess.CalledProcessError, OSError, UnicodeError) as error:
                self.reject(session, tracked, str(error))
                if not launched and not isinstance(error, self.w.CommandTimeout):
                    # A broken/auth-failing launcher should not burn the entire queue.
                    reason = "agent launcher failed; inspect the archived log"
                    break
                continue
            # Commit errors leave the candidate/index available for inspection.
            # They must never trigger rollback after Git may have committed.
            self.accept(session, paths, expected_hashes)
        self.invariant(session)
        session.update(phase="batch_final_validation", stop_reason=reason, full_validation_passed=False)
        self.w.save_state(session)
        try:
            self.w.command(["make", "validate"], "batch-final", directory=self.w.ROOT / session["directory"])
            session.update(full_validation_passed=True, phase="finished")
        except (RuntimeError, subprocess.CalledProcessError, OSError) as error:
            session.update(stop_reason="Final all-image validation failed: " + str(error), phase="validation_failed")
            raise
        finally:
            from reconstruction_runtime import summary
            self.w.save_state(session)
            result = summary(self.w.ROOT, session)
            session["result"] = session["directory"] + "/result.json"
            self.w.write(self.w.ROOT / session["result"], result)
            self.w.save_state(session)
        print(f"Batch stopped: {reason}; {len(session['completed'])} committed, "
              f"{len(session['deferred'])} deferred. Result: {session['result']}", flush=True)
        if reason == "interrupted":
            raise KeyboardInterrupt
        if reason.startswith("agent launcher failed"):
            raise RuntimeError(reason)


def run_batch(args, worker):
    Runner(worker, args).run()
