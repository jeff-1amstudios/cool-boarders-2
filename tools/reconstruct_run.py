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
from c_sources import sections, function_source, header_path


class Rejected(RuntimeError):
    """An ordinary unsuccessful candidate, which can be archived and restored."""


class AgentUnavailable(RuntimeError):
    """Infrastructure failure: stop the batch and keep the target eligible."""


def agent_failure(path):
    """Read error events only; quoted source/tool output is not a launcher error."""
    if not path.exists():
        return None
    failure = None
    with path.open(errors='replace') as stream:
        for line in stream:
            try:
                event = json.loads(line)
            except ValueError:
                # Also support non-JSON custom launchers.
                if re.search(r'usage limit|quota exceeded|insufficient_quota|rate.limit.exceeded', line, re.I):
                    failure = line.strip()[:1000]
                continue
            if not isinstance(event, dict):
                continue
            if event.get('type') == 'error':
                failure = str(event.get('message', 'Agent error'))[:1000]
            elif event.get('type') == 'turn.failed':
                failure = str(event.get('error', {}).get('message', 'Agent turn failed'))[:1000]
    return failure


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
                pending.append(header_path(path, include[1]))
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
        instructions = (self.w.ROOT / "FUNCTION_AGENTS.md").read_text()
        prompt = (
            instructions + "\n\n# Selected function\n"
            f"Implement exactly {active['name']} at {key(active)} ({active['size']} bytes).\n"
            f"Work on existing branch {active['branch']}; Python owns branch switches and commits.\n"
            f"Read {directory / 'context.json'} and {directory / 'target.s'}.\n"
            f"Start from current pseudocode in {directory / 'ghidra/target.ghidra.c'}.\n"
            f"Read cached types in {directory / 'ghidra/ghidra-types.h'} and current image header "
            f"{Path(source_file(active)).with_suffix('.h')}.\n"
            f"Append only this function to {source_file(active)}, with // FUNCTION: {key(active)}.\n"
            f"New headers belong under {source_dir(active)}. Split only this function in "
            f"{active.get('build_config', 'scus_943.58.yaml')}; append symbols only to "
            f"{active.get('symbol_file', 'symbol_addrs.txt')}.\n"
            f"Use tools/.venv/bin/python tools/reconstruction_step.py --context {directory / 'context.json'}.\n"
            f"Write evidence to {directory / 'ghidra-update.json'}.\n"
            f"You have at most {int(seconds)} seconds. Leave your best candidate and finish once "
            "the selected object matches and evidence is written.\n")
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
            # The supplied worker brief replaces repository-wide setup guidance
            # for this invocation only; retain the normal sandbox/approval policy.
            argv += ["--no-daemon", "--config", "project_doc_max_bytes=0", "--config", f"log_dir={json.dumps(str(cache / 'logs'))}",
                     "--config", f"sqlite_home={json.dumps(str(cache / 'state'))}",
                     "--ask-for-approval", "never", "exec", "--ephemeral",
                     "--sandbox", "workspace-write", "--json", "-"]
        try:
            self.w.command(argv, "agent", timeout=seconds, directory=directory, input_text=prompt, env=env)
        except self.w.CommandTimeout:
            failure = agent_failure(directory / 'agent.log')
            if failure:
                raise AgentUnavailable(failure)
            raise
        except (RuntimeError, OSError) as error:
            raise AgentUnavailable(agent_failure(directory / 'agent.log') or str(error)) from error
        failure = agent_failure(directory / 'agent.log')
        if failure:
            raise AgentUnavailable(failure)

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
            # Appending a FUNCTION marker can add separator whitespace to the
            # preceding block. Ignore only that trailing whitespace; preserve
            # exact comparisons of all declarations and function contents.
            preserved = {name: block.rstrip(' \t\r\n') for name, block in blocks.items()}
            proposed = {name: block.rstrip(' \t\r\n') for name, block in new_blocks.items()}
            if proposed != {**preserved, identity: proposed.get(identity)} or identity in blocks:
                raise Rejected('Existing overlay source or unrelated function blocks changed')
            function_source(text, identity)
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

    def reject(self, session, tracked, reason, *, retryable=False):
        active = session["active"]
        archive = self.w.ROOT / active["context"] / "candidate"
        archive.mkdir(parents=True, exist_ok=True)
        self.w.archive_edits(archive)
        active["failure"] = reason
        active["retryable"] = retryable
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
        if retryable:
            # No baseline rebuild or deferral for unavailable infrastructure.
            # Keep the active identity so the next start retries it first.
            active['phase'] = 'interrupted'
            session.update(stop_reason=reason, phase='interrupted')
            from reconstruction_runtime import attempt_result
            self.w.write(self.w.ROOT / active['context'] / 'result.json',
                         attempt_result(self.w.ROOT, active, 'interrupted', reason))
            self.w.save_state(session)
            return
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

    def resume(self, session):
        """Recover only known pre-integration states; never guess about commits."""
        if not session.get('active'):
            # A stop between recovery and selection must retain the retry identity.
            session['active'] = dict(session['interrupted'][-1])
        active = session['active']
        phase = active.get('phase')
        if phase not in (None, 'creating_branch', 'working', 'checkpointed',
                         'restoring_main', 'interrupted'):
            raise RuntimeError(f"Cannot automatically recover phase {phase}; inspect Git/analysis state"
                               + (' or run recover-ghidra' if phase == 'syncing_ghidra' else ''))
        branch = self.w.git('branch', '--show-current')
        head = self.w.git('rev-parse', 'HEAD')
        main_head = self.w.git('rev-parse', 'refs/heads/main')
        # Supervisor fixes may be committed while a batch is stopped. Accept only
        # fast-forward maintenance changes on clean main, never changed game or
        # compiler inputs, merges, or history rewrites.
        maintenance = {'tools/reconstruct.py', 'tools/reconstruct_run.py',
                       'docs/reconstruction-worker.md', 'README.md'}
        if main_head != session['integration_head']:
            ancestry = self.w.git('rev-list', '--parents',
                                  session['integration_head'] + '..' + main_head).splitlines()
            ancestor = subprocess.run(['git', 'merge-base', '--is-ancestor', session['integration_head'], main_head],
                                      cwd=self.w.ROOT).returncode == 0
            edited = set(self.w.git('diff', '--name-only', session['integration_head'], main_head).splitlines())
            if (branch != 'main' or self.w.changes() or not ancestor
                    or any(len(line.split()) != 2 for line in ancestry) or not edited <= maintenance):
                raise RuntimeError('main moved outside supervisor maintenance; refusing automatic recovery')
        if self.w.git('diff', '--cached', '--name-only'):
            raise RuntimeError('Recovery requires an unchanged empty index')
        if branch == 'main':
            if head != main_head or self.w.changes():
                raise RuntimeError('Recovery requires unchanged clean main')
            session.update(integration_head=main_head, protected=self.w.fingerprints())
        elif (branch == active.get('branch') and phase in ('working', 'creating_branch', 'checkpointed')
              and head == (active.get('checkpoint') or active.get('base_commit'))):
            session.update(branch=branch, head=head)
            self.w.save_state(session)
            if phase == 'checkpointed':
                self.invariant(session)
                if self.w.changes():
                    raise RuntimeError('Checkpoint checkout changed; refusing recovery')
                self.switch(session, 'main', session['integration_head'])
            else:
                tracked = set(self.w.git('ls-files').splitlines())
                self.reject(session, tracked, 'Recovered interrupted worker; retry selected target', retryable=True)
        else:
            raise RuntimeError('Unexpected recovery branch/commit; inspect preserved work')
        session.update(branch='main', head=session['integration_head'])
        self.invariant(session)
        from reconstruction_runtime import attempt_result
        self.w.write(self.w.ROOT / active['context'] / 'result.json',
                     attempt_result(self.w.ROOT, active, 'interrupted', active.get('failure', 'Interrupted batch')))
        session.setdefault('interrupted', []).append(dict(active))
        session.update(active=None, retry_target=key(active), stop_reason=None,
                       phase='running', deadline=time.time() + self.args.minutes * 60,
                       limit=max(self.args.limit, len(session['completed']) + len(session['deferred']) + 1))
        self.w.save_state(session)
        print(f"Recovered interrupted batch; retrying {key(active)} first (previous attempt archived)", flush=True)
        return session

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
        if self.w.read(self.w.REVIEW).get("matching_paused", False):
            raise RuntimeError("C matching is paused by the boundary-audit policy; no batch started")
        previous = self.w.state() if self.w.STATE.exists() else None
        if previous and (previous.get('active') or previous.get('retry_target')):
            session = self.resume(previous)
            session['sync_types'] = getattr(self.args, 'sync_types', False)
        else:
            if self.w.git("branch", "--show-current") != "main":
                raise RuntimeError("Automatic batches must start on the main integration branch")
            session = self.w.begin_batch(self.args.limit, self.args.minutes)
            session.update(branch="main", integration_head=session["head"],
                           sync_types=getattr(self.args, 'sync_types', False))
        sync_types_pending = session['sync_types']
        self.w.save_state(session)
        reason = "target limit reached"
        while len(session["completed"]) + len(session["deferred"]) < session["limit"]:
            if time.time() >= session["deadline"]:
                reason = "session time budget exhausted"
                break
            try:
                current_inventory = self.w.next_target(session, session.get('retry_target'))
                session.pop('retry_target', None)
                self.w.save_state(session)
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
                    export(self.w, active, sync_types=sync_types_pending)
                    sync_types_pending = False
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
                self.reject(session, tracked, "Interrupted during agent/validation; candidate archived", retryable=True)
                raise
            except AgentUnavailable as error:
                self.reject(session, tracked, str(error), retryable=True)
                # Exit before final validation and before selecting another function.
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
        # Update tracked documentation only after all candidate/checkout invariants
        # and the final all-image gate, never during acceptance of a worker edit.
        from readme_progress import update
        update()
        print(f"Batch stopped: {reason}; {len(session['completed'])} committed, "
              f"{len(session['deferred'])} deferred. Result: {session['result']}", flush=True)
        if reason == "interrupted":
            raise KeyboardInterrupt
        if reason.startswith("agent launcher failed"):
            raise RuntimeError(reason)


def run_batch(args, worker):
    Runner(worker, args).run()
