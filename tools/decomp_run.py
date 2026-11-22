"""Subprocess supervision for decomp.py run. Python alone accepts and commits."""
import copy
import difflib
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

import yaml
from decomp_backlog import key, slug, source_dir, source_file, coverage
from c_sources import sections, function_source, header_path
from decomp_runtime import DEFAULT_AGENT_TOKENS, usage


class Rejected(RuntimeError):
    """An ordinary unsuccessful candidate, which can be archived and restored."""


class AgentUnavailable(RuntimeError):
    """Infrastructure failure: stop the batch and keep the target eligible."""


class AgentUsageLimited(AgentUnavailable):
    """The worker can be retried after waiting for Codex usage to reset."""


USAGE_LIMIT = re.compile(
    r'usage[ _-]limit|quota exceeded|insufficient_quota|rate[ _-]limit[ _-]exceeded', re.I
)
USAGE_RETRY_SECONDS = 30 * 60


def resume_candidate(root, active):
    """Find the latest saved function, including deferrals from earlier batches.

    Export only the selected block: the worker must reconcile shared declarations
    and splits with current main rather than overwrite whole archived files.
    """
    attempts = root / 'reports/decomp'
    archives = sorted(attempts.rglob('candidate/sources.json'),
                      key=lambda path: path.stat().st_mtime_ns, reverse=True)
    for snapshot in archives:
        directory = snapshot.parent.parent
        if directory == root / active['context']:
            continue
        sources = json.loads(snapshot.read_text())
        source = sources.get(source_file(active))
        if not isinstance(source, str):
            continue
        try:
            _, blocks = sections(source)
        except ValueError:
            continue
        if key(active) not in blocks:
            continue
        context_path = directory / 'context.json'
        context = json.loads(context_path.read_text()) if context_path.exists() else {}
        # A source snapshot can contain many accepted blocks. Only resume the
        # function this attempt was actually working on.
        if context.get('identity') != key(active):
            continue
        destination = root / active['context']
        (destination / 'resume-candidate.c').write_text(blocks[key(active)])
        details = {
            'identity': key(active), 'previous_context': str(directory.relative_to(root)),
            'sources': str(snapshot.relative_to(root)),
            'function': str((destination / 'resume-candidate.c').relative_to(root)),
            'previous_body_sha256': context.get('body_sha256'),
            'current_body_sha256': active.get('body_sha256'),
            'evidence': [str(path.relative_to(root)) for name in
                         ('knowledge.md', 'result.json', 'object-diff.json',
                          'agent-last-message.txt', 'candidate/attempt.patch', 'ghidra-update.json')
                         if (path := directory / name).is_file()],
        }
        (destination / 'resume.json').write_text(json.dumps(details, indent=2) + '\n')
        return details
    return None


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
                if USAGE_LIMIT.search(line):
                    failure = line.strip()[:1000]
                continue
            if not isinstance(event, dict):
                continue
            if event.get('type') == 'error':
                failure = str(event.get('message', 'Agent error'))[:1000]
            elif event.get('type') == 'turn.failed':
                failure = str(event.get('error', {}).get('message', 'Agent turn failed'))[:1000]
            elif event.get('type') == 'turn.completed':
                failure = None
    if failure:
        error_type = AgentUsageLimited if USAGE_LIMIT.search(failure) else AgentUnavailable
        return error_type(failure)
    return None


def review_symbol_additions(before, after):
    """Allow inserted definitions while preserving existing lines and names."""
    original, proposed = before.splitlines(), after.splitlines()
    added = []
    for operation, low, high, start, end in difflib.SequenceMatcher(
            None, original, proposed, autojunk=False).get_opcodes():
        if operation == 'equal':
            continue
        if operation != 'insert':
            raise Rejected("Existing symbol definitions were changed or removed")
        added.extend(proposed[start:end])
    name_pattern = re.compile(r'^\s*([A-Za-z_]\w*)\s*=')
    names = {match[1] for line in original if (match := name_pattern.match(line))}
    for line in added:
        if not line.strip() or line.lstrip().startswith('//'):
            continue
        match = re.fullmatch(r'\s*([A-Za-z_]\w*)\s*=\s*0x[0-9a-fA-F]+;\s*(?://.*)?', line)
        if not match:
            raise Rejected("Unsupported symbol addition")
        name = match[1]
        if name in names:
            raise Rejected(f"Duplicate symbol definition: {name}")
        names.add(name)
    return added


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


def without_gte_asm(source):
    """Remove reviewed literal GTE asm statements before the general asm ban."""
    commands = {
        'rtps', 'nclip', 'op', 'dpcs', 'intpl', 'mvmva', 'ncds', 'cdp',
        'ncdt', 'nccs', 'cc', 'ncs', 'nct', 'sqr', 'dcpl', 'dpct',
        'avsz3', 'avsz4', 'rtpt', 'gpf', 'gpl', 'ncct',
    }
    # Tokenize strings first so parentheses inside templates/constraints do not
    # affect the statement boundary. Nonliteral templates remain prohibited.
    token = re.compile(r'"(?:\\.|[^"\\])*"|[()]|[^"()]+', re.S)
    start = re.compile(r'\b(?:asm|__asm|__asm__)\s*(?:(?:volatile|__volatile|__volatile__)\s*)?\(')
    while match := start.search(source):
        depth, end = 1, match.end()
        for part in token.finditer(source, end):
            if part.start() != end:
                raise Rejected('Malformed GTE inline assembly')
            end = part.end()
            if part[0] == '(':
                depth += 1
            elif part[0] == ')':
                depth -= 1
                if depth == 0:
                    break
        if depth:
            raise Rejected('Unterminated GTE inline assembly')
        contents = source[match.end():end - 1]
        template = re.match(r'\s*((?:"(?:\\.|[^"\\])*"\s*)+)(?=:|$)', contents, re.S)
        if template is None:
            raise Rejected('GTE inline assembly requires a literal template')
        instructions = ''
        for literal in re.findall(r'"((?:\\.|[^"\\])*)"', template[1]):
            if re.search(r'\\(?![ntr])', literal):
                raise Rejected('Unsupported escape in GTE instruction template')
            instructions += literal.replace(r'\n', '\n').replace(r'\t', '\t').replace(r'\r', '\r')
        count = 0
        for instruction in re.split(r'[;\n\r]', instructions):
            if not instruction.strip():
                continue
            count += 1
            fields = instruction.strip().split(None, 1)
            mnemonic, operands = fields[0].lower(), fields[1] if len(fields) > 1 else ''
            allowed = mnemonic in commands | {'mfc2', 'mtc2', 'cfc2', 'ctc2', 'lwc2', 'swc2'}
            if mnemonic == 'nop':
                allowed = not operands
            elif mnemonic == 'cop2':
                allowed = bool(re.fullmatch(r'(?:0[xX][0-9a-fA-F]+|[0-9]+)', operands))
                if allowed:
                    value = int(operands, 16 if operands.lower().startswith('0x') else 10)
                    allowed = 0 <= value <= 0x1ffffff and (value & 63) in {
                        1, 6, 12, 16, 17, 18, 19, 20, 22, 27, 28, 30, 32,
                        40, 41, 42, 45, 46, 48, 61, 62, 63,
                    }
            if not allowed or not re.fullmatch(r'[\w$%,()\[\] +\-]*', operands):
                raise Rejected('Only PSX GTE instructions and pipeline nops are allowed in inline assembly')
        if not count:
            raise Rejected('Empty GTE instruction template')
        # Keep constraints and C operand expressions visible to other guards.
        source = source[:match.start()] + ' ' + contents[template.end():] + source[end:]
    return source


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
        clean = without_gte_asm(clean)
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
    previous, proposed = before.splitlines(keepends=True), after.splitlines(keepends=True)
    for operation, old_start, _, start, end in difflib.SequenceMatcher(
            None, previous, proposed, autojunk=False).get_opcodes():
        if operation == 'equal':
            continue
        if operation != 'insert':
            raise Rejected('Existing shared declarations changed')
        prefix = re.sub(r'/\*.*?\*/|//[^\n]*', ' ', ''.join(previous[:old_start]), flags=re.S)
        prefix = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', ' ', prefix)
        if prefix.count('{') != prefix.count('}'):
            raise Rejected('New shared declarations must be outside existing helper bodies')
        # Check each insertion separately so a new comment cannot enclose
        # established declarations. Existing lines must remain byte-for-byte.
        added = re.sub(r'/\*.*?\*/|//[^\n]*', ' ', ''.join(proposed[start:end]), flags=re.S)
        added = re.sub(r'\bextern\s+[^;{}=]+;|#[ \t]*include[ \t]+"[A-Za-z_]\w*\.h"', '', added)
        if added.strip():
            raise Rejected('Only extern declarations and local includes may be added before FUNCTION annotations')


class Runner:
    def __init__(self, worker, args):
        self.w = worker
        self.args = args
        self.model = getattr(args, "model", None)
        self.effort = getattr(args, "effort", None)
        if self.effort and args.agent_command is not None:
            raise RuntimeError("--effort requires the default Codex launcher")
        if self.model and args.agent_command is not None:
            raise RuntimeError("--model requires the default Codex launcher")
        if min(args.limit, args.agent_seconds) < 1:
            raise RuntimeError("Target limit and worker timeout must be positive")
        self.agent_tokens = getattr(args, "agent_tokens", None)
        if self.agent_tokens is None:
            self.agent_tokens = DEFAULT_AGENT_TOKENS if args.agent_command is None else 0
        if self.agent_tokens < 0:
            raise RuntimeError("Worker token budget must be nonnegative")
        if self.agent_tokens and args.agent_command is not None:
            raise RuntimeError("A positive --agent-tokens budget requires the default Codex launcher")
        try:
            self.launcher = shlex.split(args.agent_command) if args.agent_command is not None else ["codex"]
        except ValueError as error:
            raise RuntimeError("Invalid quoting in --agent-command") from error
        if not self.launcher or not shutil.which(self.launcher[0]):
            raise RuntimeError("Agent launcher is unavailable; set --agent-command to an existing executable")

    def invariant(self, session):
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
        branch = base
        session["active"].update(branch=branch, base_commit=session["integration_head"], phase="creating_branch")
        self.w.save_state(session)
        if branch in existing:
            self.w.git("switch", branch)
            session.update(branch=branch, head=self.w.git("rev-parse", "HEAD"))
            session["active"]["phase"] = "updating_branch"
            self.w.save_state(session)
            # Preserve the candidate and its history. Conflicts stop here with
            # the branch/index intact rather than silently replacing old work.
            self.w.git("merge", "--no-edit", session["integration_head"])
        else:
            self.w.git("switch", "-c", branch, "main")
        session["branch"] = branch
        session["head"] = self.w.git("rev-parse", "HEAD")
        session["active"]["working_head"] = session["head"]
        session["active"]["phase"] = "working"
        self.w.save_state(session)
        self.invariant(session)
        print(f"Function branch: {branch} (base {session['integration_head'][:12]})", flush=True)

    def paths(self, tracked, active, base="HEAD"):
        directory = Path(source_dir(active))
        changed = self.w.changes(base)
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
        # Ghidra/export time must not reduce the worker's own allowance.
        seconds = self.args.agent_seconds
        budget_note = (f"The supervisor also stops you at {self.agent_tokens:,} uncached input plus output "
                       "tokens. Save useful edits and evidence as you go.\n" if self.agent_tokens else "")
        saved = resume_candidate(self.w.ROOT, active)
        current_source = self.w.ROOT / source_file(active)
        _, current_blocks = sections(current_source.read_text() if current_source.exists() else '')
        if key(active) in current_blocks:
            starting_point = (
                f"Resume the existing function directly in {source_file(active)} on this branch.\n"
                "Its source, header, symbols and split configuration are the working candidate; "
                "edit them in place. Use fresh Ghidra output as reference.\n"
            )
            if saved:
                starting_point += f"Read {directory / 'resume.json'} for prior diagnostics and notes; the branch source takes precedence over archived C.\n"
        elif saved:
            starting_point = (
                f"Resume the saved candidate in {directory / 'resume-candidate.c'}; "
                f"read {directory / 'resume.json'} for its prior sources and evidence.\n"
                "Carry forward this function and its useful declarations/header additions, "
                "reconciling them with current main, current types, symbols and split boundaries. "
                "Read the previous diagnostics and notes before changing the candidate. "
                "Do not overwrite current image sources, headers or configs with archived files. "
                "Use fresh Ghidra output as reference; do not replace the saved candidate with it. "
                "If changed reference bytes/boundaries or prohibited source forms require discarding "
                "any prior work, record the concrete reason in knowledge.md.\n"
            )
            print(f"Resuming saved candidate from {saved['previous_context']}", flush=True)
        else:
            starting_point = f"No saved candidate found; start from {directory / 'ghidra/target.ghidra.c'}.\n"
        instructions = (
            f"Your only repository/workspace root is {self.w.ROOT.resolve()}.\n\n"
            + (self.w.ROOT / "FUNCTION_AGENTS.md").read_text()
        )
        prompt = (
            "# Selected function\n"
            f"Implement exactly {active['name']} at {key(active)} ({active['size']} bytes).\n"
            f"Work on existing branch {active['branch']}; Python owns branch switches and commits.\n"
            f"Read {directory / 'context.json'} and {directory / 'target.s'}.\n"
            + starting_point
            + f"Fresh reference pseudocode: {directory / 'ghidra/target.ghidra.c'}.\n"
            f"Consult relevant cached types in {directory / 'ghidra/ghidra-types.h'} and read current image header "
            f"{Path(source_file(active)).with_suffix('.h')}.\n"
            f"Append only this function to {source_file(active)}, with // FUNCTION: {key(active)}.\n"
            f"New headers belong under {source_dir(active)}. Split only this function in "
            f"{active.get('build_config', 'scus_943.58.yaml')}; add new symbols only to "
            f"{active.get('symbol_file', 'symbol_addrs.txt')}, preserving existing definitions.\n"
            f"Use tools/.venv/bin/python tools/decomp_step.py --context {directory / 'context.json'}.\n"
            f"Write evidence to {directory / 'ghidra-update.json'}.\n"
            "Before compiling, consult docs/decomp-knowledge/README.md and relevant topic entries.\n"
            f"Record reusable findings or useful failed experiments in {directory / 'knowledge.md'} "
            "using the index's proposal format; write 'No new reusable insight' when appropriate.\n"
            f"{budget_note}"
            f"You have at most {int(seconds)} seconds. Leave your best candidate and finish once "
            "the selected object matches and evidence is written.\n")
        (directory / "agent-prompt.md").write_text(instructions + "\n\n" + prompt)
        env = os.environ.copy()
        temporary = self.w.ROOT / "tools/tmp/decomp-agents"
        temporary.mkdir(parents=True, exist_ok=True)
        env.update(TMPDIR=str(temporary), CB2_DECOMP_CONTEXT=str(directory / "context.json"),
                   CB2_AGENT_REPORT=str(directory / "agent-last-message.txt"))
        argv = list(self.launcher)
        if self.args.agent_command is None:
            cache = self.w.ROOT / "tools/cache/decomp-agents" / Path(session["directory"]).name / slug(active)
            for name in ("logs", "state"):
                (cache / name).mkdir(parents=True, exist_ok=True)
            # Disable automatic project docs AND explicitly prohibit manual
            # discovery in developer instructions. The byte limit alone does
            # not prevent a worker from searching for AGENTS.md with shell tools.
            config = ["--config", "project_doc_max_bytes=0",
                     "--config", f"developer_instructions={json.dumps(instructions)}",
                     "--config", f"log_dir={json.dumps(str(cache / 'logs'))}",
                     "--config", f"sqlite_home={json.dumps(str(cache / 'state'))}"]
            if self.model:
                config += ["--config", f"model={json.dumps(self.model)}"]
            if self.effort:
                config += ["--config", f"model_reasoning_effort={json.dumps(self.effort)}"]
            argv = [sys.executable, str(self.w.ROOT / "tools/decomp_agent.py"),
                    str(self.agent_tokens), *argv, "app-server", *config]
        try:
            # Default launcher already receives the brief as developer instructions.
            # Custom launchers still need the complete brief on stdin.
            task_input = instructions + "\n\n" + prompt if self.args.agent_command is not None else prompt
            self.w.command(argv, "agent", timeout=seconds, directory=directory, input_text=task_input, env=env)
        except self.w.CommandTimeout:
            failure = agent_failure(directory / 'agent.log')
            if failure:
                raise failure
            raise
        except (RuntimeError, OSError) as error:
            raise (agent_failure(directory / 'agent.log') or AgentUnavailable(str(error))) from error
        failure = agent_failure(directory / 'agent.log')
        if failure:
            raise failure
        if self.agent_tokens:
            return usage(directory / 'agent.log').get('budget_stop')

    def candidate(self, session, tracked, before, old_symbols, prior_c):
        active = session["active"]
        baseline = session["integration_head"]
        paths = self.paths(tracked, session["active"], baseline)
        if not paths:
            raise Rejected("Agent left no candidate edits")
        try:
            after = yaml.safe_load((self.w.ROOT / session["active"].get("build_config", "scus_943.58.yaml")).read_text())
            entry = review_split(before, after, session["active"])
        except (yaml.YAMLError, KeyError, TypeError, IndexError) as error:
            raise Rejected("Invalid candidate split configuration") from error
        symbols = (self.w.ROOT / session["active"].get("symbol_file", "symbol_addrs.txt")).read_text()
        review_symbol_additions(old_symbols, symbols)
        review_source(self.w.ROOT, paths, entry, session["active"]["name"])
        for name in paths:
            if name in tracked and Path(name).suffix == '.h':
                previous_header = subprocess.check_output(['git', 'show', f'{baseline}:{name}'], cwd=self.w.ROOT).decode()
                review_header_extension(previous_header, (self.w.ROOT / name).read_text())
        text = (self.w.ROOT / entry).read_text()
        try:
            previous = ''
            # Read bytes directly: git() strips whitespace, which is significant here.
            if entry in tracked:
                previous = subprocess.check_output(['git', 'show', f'{baseline}:{entry}'], cwd=self.w.ROOT).decode()
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
        # Normalize cosmetic EOF separators only after scope/content review,
        # before hashing, archiving and authoritative full-file validation.
        for name in paths:
            if Path(name).suffix not in ('.c', '.h'):
                continue
            path = self.w.ROOT / name
            original = path.read_bytes()
            lines = original.splitlines(keepends=True)
            while lines and not lines[-1].strip():
                lines.pop()
            normalized = b''.join(lines)
            if normalized != original:
                path.write_bytes(normalized)
                print(f"Removed trailing blank lines: {name}", flush=True)
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
        if paths != self.paths(tracked, session["active"], baseline):
            raise RuntimeError("Checkout changed during acceptance validation")
        if self.w.content_hashes(paths) != expected_hashes:
            raise Rejected("Candidate contents changed during acceptance validation")
        self.w.check_session(session)
        from decomp_ghidra import prepare_update
        prepare_update(self.w, active)
        self.invariant(session)
        if self.w.content_hashes(paths) != expected_hashes:
            raise Rejected("Candidate changed during Ghidra feedback planning")
        return paths, expected_hashes

    def reject(self, session, tracked, reason, *, retryable=False):
        active = session["active"]
        archive = self.w.ROOT / active["context"] / "candidate"
        archive.mkdir(parents=True, exist_ok=True)
        self.w.archive_edits(archive, session["integration_head"])
        active["failure"] = reason
        active["retryable"] = retryable
        self.w.save_state(session)
        # Never erase unexpected edits, index changes, or another process's commits.
        self.invariant(session)
        paths = self.paths(tracked, session["active"])
        if retryable and paths:
            active["candidate_archived"] = True
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
            from decomp_runtime import record_attempt
            record_attempt(self.w.ROOT, active, 'interrupted', reason)
            self.w.save_state(session)
            return
        if session["completed"]:
            self.w.command(self.w.validation_command({k:v for k,v in active.items() if k != "unit"}), "restored-baseline", directory=archive)
        active["phase"] = "deferred"
        from decomp_runtime import record_attempt
        record_attempt(self.w.ROOT, active, "deferred", reason)
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
        maintenance = {'.gitignore', 'AGENTS.md', 'FUNCTION_AGENTS.md',
                       'docs/decomp-knowledge/README.md',
                       'tools/decomp.py', 'tools/decomp_run.py', 'tools/decomp_agent.py',
                       'tools/decomp_runtime.py', 'tools/decomp_metrics.py',
                       'docs/decomp-worker.md', 'docs/token-efficiency.md', 'README.md'}
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
              and head == (active.get('checkpoint') or active.get('working_head') or active.get('base_commit'))):
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
        result_path = self.w.ROOT / active['context'] / 'result.json'
        if not result_path.exists():
            from decomp_runtime import attempt_result
            self.w.write(result_path, attempt_result(
                self.w.ROOT, active, 'interrupted', active.get('failure', 'Interrupted batch')))
        session.setdefault('interrupted', []).append(dict(active))
        session.update(active=None, retry_target=key(active), stop_reason=None,
                       phase='running',
                       limit=max(self.args.limit, len(session['completed']) + len(session['deferred']) + 1))
        session.pop('deadline', None)  # Discard deadlines from older sessions.
        self.w.save_state(session)
        print(f"Recovered interrupted batch; retrying {key(active)} first (previous attempt archived)", flush=True)
        return session

    def accept(self, session, paths, expected_hashes):
        # Git/merge errors deliberately preserve the phase, branch, candidate and
        # index for inspection. Never turn a partially completed commit into WIP.
        active = session["active"]
        self.w.check_session(session)
        self.invariant(session)
        from decomp_ghidra import record, apply
        payload = self.w.read(self.w.ROOT / active['context'] / 'ghidra-feedback/request.json')
        feedback_path = record(self.w, active, payload)
        paths = [*paths, feedback_path]
        expected_hashes = {**expected_hashes, **self.w.content_hashes([feedback_path])}
        active['ghidra_record'] = feedback_path
        active["phase"] = "committing_match"
        self.w.save_state(session)
        # Earlier WIP commits already contain part (or all) of the candidate.
        # Commit only this turn's delta on the persistent function branch.
        delta = sorted(self.w.changes())
        self.w.audit_paths(delta, delta)
        if self.w.content_hashes(paths) != expected_hashes:
            raise RuntimeError("Candidate contents changed after validation")
        # A retry may also undo an old WIP edit back to main. Such a path is
        # absent from the accepted main delta but must be committed here.
        delta_hashes = self.w.content_hashes(delta)
        commit = self.w.commit_files(delta, f"Match {active['name']} from bounded agent run",
                                     delta_hashes) if delta else session["head"]
        session["head"] = commit
        active.update(commit=commit, phase="validated_commit")
        self.w.save_state(session)
        self.invariant(session)
        self.switch(session, "main", session["integration_head"])
        active["phase"] = "integrating"
        self.w.save_state(session)
        self.invariant(session)
        # Copy the audited, validated result instead of merging WIP history.
        self.w.git("restore", "--source", commit, "--worktree", "--", *paths)
        commit = self.w.commit_files(paths, f"Match {active['name']} from bounded agent run", expected_hashes)
        active["integration_commit"] = commit
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
        from decomp_runtime import record_attempt
        record_attempt(self.w.ROOT, active, "matched")
        if key(active) in self.w.deferred_targets():
            self.w.retry_deferred(key(active))
        session["completed"].append({**active, "phase": "integrated"})
        session["active"] = None
        self.w.save_state(session)
        print(f"Integrated {active['branch']} into main at {commit[:12]}", flush=True)

    def resolve_functions(self):
        requested = getattr(self.args, "functions", None)
        if requested is None:
            return None
        targets = self.w.inventory()["targets"]
        selected = []
        for value in self.w.function_requests(requested):
            value = value.strip()
            if not value:
                raise RuntimeError("--functions contains an empty entry")
            generated = re.fullmatch(r"func_([0-9a-fA-F]{8})", value)
            address = "0x" + generated[1] if generated else value
            matches = [row for row in targets
                       if value.casefold() == key(row).casefold()
                       or value == row.get("name")
                       or address.casefold() == row["address"].casefold()]
            if len(matches) != 1:
                raise RuntimeError(f"Function {value!r} is unknown or ambiguous; use IMAGE:0xADDRESS")
            row = matches[0]
            if row["status"] not in ("needs_review", "c"):
                raise RuntimeError(f"Function {key(row)} is excluded from the C queue ({row['status']})")
            if key(row) not in selected:
                selected.append(key(row))
        return selected

    def run(self):
        started = datetime.now(timezone.utc).isoformat()
        if self.w.read(self.w.REVIEW).get("matching_paused", False):
            raise RuntimeError("C matching is paused by the boundary-audit policy; no batch started")
        previous = self.w.state() if self.w.STATE.exists() else None
        resuming = bool(previous and (previous.get('active') or previous.get('retry_target')))
        retry_target = None
        if resuming:
            self.resume(previous)
            retry_target = previous["retry_target"]
            # Keep the old run's recovery record before replacing current state.
            self.w.write(self.w.ROOT / previous["directory"] / "state.json", previous)
        if self.w.git("branch", "--show-current") != "main":
            raise RuntimeError("Automatic batches must start on the main integration branch")
        selected = self.resolve_functions()
        session = self.w.begin_batch(self.args.limit)
        if selected:
            pending = selected
            if previous and previous.get("selected_functions") == selected:
                attempted = {key(row) for row in previous["completed"] + previous["deferred"]}
                remaining = [identity for identity in previous["pending_functions"] if identity not in attempted]
                if remaining or previous.get("phase") != "finished":
                    pending = remaining
            session.update(selected_functions=selected, pending_functions=pending)
            if retry_target not in pending:
                retry_target = None
        if retry_target:
            session["retry_target"] = retry_target
            session["interrupted"] = previous.get("interrupted", [])
        batch_path = self.w.ROOT / session["directory"] / "batch.json"
        batch = self.w.read(batch_path) if batch_path.exists() else {"session": session["directory"], "runs": []}
        argv = list(getattr(self.args, "invocation", []))
        run = dict(argv=argv, command=shlex.join(argv) if argv else None, started_at=started,
                   ended_at=None, outcome="running", target_limit=self.args.limit,
                   agent_seconds=self.args.agent_seconds, agent_tokens=self.agent_tokens,
                   model=self.model, effort=self.effort, selected_functions=selected, functions=[])
        batch["runs"].append(run)
        self.w.write(batch_path, batch)
        try:
            self.run_session(session, batch_path, batch, run)
            run["outcome"] = "finished"
            run["reason"] = session.get("stop_reason")
        except KeyboardInterrupt:
            run.update(outcome="interrupted", reason="Interrupted")
            raise
        except Exception as error:
            run.update(outcome="failed", reason=str(error))
            raise
        finally:
            run["ended_at"] = datetime.now(timezone.utc).isoformat()
            self.w.write(batch_path, batch)
            from decomp_metrics import refresh_batches
            refresh_batches(self.w.ROOT)

    def run_session(self, session, batch_path, batch, run):
        session.update(branch="main", integration_head=session["head"], phase="running")
        session["sync_types"] = getattr(self.args, "sync_types", False)
        sync_types_pending = session['sync_types']
        self.w.save_state(session)
        reason = "target limit reached"
        while len(session["completed"]) + len(session["deferred"]) < session["limit"]:
            try:
                current_inventory = self.w.next_target(session, session.get('retry_target'))
                session.pop('retry_target', None)
                self.w.save_state(session)
            except self.w.NoTarget:
                reason = "requested function list exhausted" if session.get("selected_functions") else "eligible queue exhausted"
                break
            active = session["active"]
            active["agent_tokens"] = self.agent_tokens
            active["requested_model"] = self.model
            active["requested_effort"] = self.effort
            run["functions"].append(dict(identity=key(active), name=active["name"], context=active["context"]))
            self.w.write(batch_path, batch)
            self.w.save_state(session)
            number = len(session["completed"]) + len(session["deferred"]) + 1
            print(f"\nTarget {number}/{session['limit']}: {key(active)} ({active['size']} bytes)", flush=True)
            if not active.get('boundary_findings'):
                from decomp_ghidra import export
                try:
                    export(self.w, active, sync_types=sync_types_pending)
                    sync_types_pending = False
                except BaseException:
                    # An unavailable/locked Ghidra project is infrastructure
                    # failure, not evidence against reconstructing this target.
                    session.update(stop_reason='Fresh Ghidra export failed before agent launch')
                    self.w.save_state(session)
                    raise
            self.start_branch(session)
            baseline = session["integration_head"]
            before = yaml.safe_load(self.w.git("show", f"{baseline}:{active.get('build_config', 'scus_943.58.yaml')}"))
            old_symbols = self.w.git("show", f"{baseline}:{active.get('symbol_file', 'symbol_addrs.txt')}")
            tracked = set(self.w.git("ls-tree", "-r", "--name-only", baseline).splitlines())
            prior_c = coverage(current_inventory)
            launched = False
            budget_stop = None
            try:
                if session["active"].get("boundary_findings"):
                    launched = True
                    raise Rejected("Selected-target split blocker: " + json.dumps(session["active"]["boundary_findings"]))
                budget_stop = self.launch(session)
                launched = True
                self.invariant(session)
                paths, expected_hashes = self.candidate(session, tracked, before, old_symbols, prior_c)
            except KeyboardInterrupt:
                self.reject(session, tracked, "Interrupted during agent/validation; candidate archived", retryable=True)
                raise
            except AgentUsageLimited as error:
                self.reject(session, tracked, str(error), retryable=True)
                print(f"Codex usage limit reached for {key(active)}. Retrying in 30 minutes.", flush=True)
                time.sleep(USAGE_RETRY_SECONDS)
                self.resume(session)
                continue
            except AgentUnavailable as error:
                self.reject(session, tracked, str(error), retryable=True)
                # Exit before final validation and before selecting another function.
                raise
            except (RuntimeError, subprocess.CalledProcessError, OSError, UnicodeError) as error:
                reason_detail = str(error)
                if budget_stop:
                    reason_detail = (f"Worker token budget reached ({budget_stop['spent']:,}/"
                                     f"{budget_stop['budget']:,}): {reason_detail}")
                self.reject(session, tracked, reason_detail)
                if not launched and not isinstance(error, self.w.CommandTimeout):
                    # A broken/auth-failing launcher should not burn the entire queue.
                    reason = "agent launcher failed; inspect the archived log"
                    break
                continue
            # Commit errors leave the candidate/index available for inspection.
            # They must never trigger rollback after Git may have committed.
            self.accept(session, paths, expected_hashes)
        self.invariant(session)
        validation_skipped = not session["completed"]
        session.update(phase="batch_final_validation", stop_reason=reason,
                       full_validation_passed=False, full_validation_skipped=validation_skipped)
        self.w.save_state(session)
        try:
            if validation_skipped:
                print("No functions matched; skipping final all-image validation and README progress refresh.", flush=True)
                session.update(phase="finished")
            else:
                self.w.command(["make", "validate"], "batch-final", directory=self.w.ROOT / session["directory"])
                session.update(full_validation_passed=True, phase="finished")
        except (RuntimeError, subprocess.CalledProcessError, OSError) as error:
            session.update(stop_reason="Final all-image validation failed: " + str(error), phase="validation_failed")
            raise
        finally:
            from decomp_runtime import summary
            self.w.save_state(session)
            result = summary(self.w.ROOT, session)
            session["result"] = session["directory"] + "/result.json"
            self.w.write(self.w.ROOT / session["result"], result)
            self.w.save_state(session)
        # Update tracked documentation only after all candidate/checkout invariants
        # and the final all-image gate, never during acceptance of a worker edit.
        if not validation_skipped:
            from readme_progress import update
            update()
        print(f"Batch stopped: {reason}; {len(session['completed'])} committed, "
              f"{len(session['deferred'])} deferred. Result: {session['result']}", flush=True)
        print(f"Knowledge review: inspect attempt knowledge.md notes under {session['directory']} "
              "and curate reusable findings via docs/decomp-knowledge/README.md.", flush=True)
        if reason == "interrupted":
            raise KeyboardInterrupt
        if reason.startswith("agent launcher failed"):
            raise RuntimeError(reason)


def run_batch(args, worker):
    Runner(worker, args).run()
