#!/usr/bin/env python3
"""Exercise real child processes, Git commits and rollback with a synthetic EXE gate."""
import copy
import hashlib
import json
import shlex
import subprocess
import sys
import tempfile
import textwrap
import time
import unittest
from contextlib import ExitStack
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import reconstruct as worker
from reconstruct_run import Runner, Rejected, review_source, review_split, review_header_extension, review_shared_extension


class RunTests(unittest.TestCase):
    def setUp(self):
        temporary = worker.ROOT / "tools/tmp"
        temporary.mkdir(parents=True, exist_ok=True)
        self.stack = ExitStack()
        self.addCleanup(self.stack.close)
        self.root = Path(self.stack.enter_context(tempfile.TemporaryDirectory(dir=temporary)))
        for name in ("config", "src", "orig/disc/files", "tools/tmp", "reports/reconstruction"):
            (self.root / name).mkdir(parents=True, exist_ok=True)
        self.config = {"options": {}, "segments": [{"type": "code", "start": 0, "vram": 0x80010000,
            "subsegments": [[0, "asm", "first"], [8, "asm", "second"], [16, "data", "data"]]}, [24]]}
        import yaml
        self.yaml = yaml
        (self.root / "scus_943.58.yaml").write_text(yaml.safe_dump(self.config))
        (self.root / "symbol_addrs.txt").write_text("")
        (self.root / "README.md").write_text("Synthetic test repository\n")
        (self.root / ".gitignore").write_text("/orig/\n/reports/\n/build/\n/tools/tmp/\n/tools/cache/\n")
        for name in worker.PROTECTED:
            path = self.root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("{}\n")
        (self.root / "config/reconstruction.json").write_text('{"pilot": [], "functions": {}}\n')
        self.original = bytes(range(24))
        self.sha = hashlib.sha256(self.original).hexdigest()
        (self.root / "orig/disc/files/SCUS_943.58").write_bytes(self.original)
        (self.root / "tools/fake_validate.py").write_text(textwrap.dedent(r'''
            from pathlib import Path
            import hashlib, json, re, sys, yaml
            config = yaml.safe_load(Path('scus_943.58.yaml').read_text())
            units = [r[2] for r in config['segments'][0]['subsegments'] if r[1] == 'c']
            expected = Path('orig/disc/files/SCUS_943.58').read_bytes()
            passed = all(re.search(r'\breturn\s+7\s*;', Path('src/main.c').read_text().split('// FUNCTION: SCUS_943.58:0x'+n.removeprefix('unit_'))[1].split('// FUNCTION:')[0]) for n in units)
            actual = expected if passed else b'!' + expected[1:]
            Path('build').mkdir(exist_ok=True)
            Path('reports').mkdir(exist_ok=True)
            Path('build/SCUS_943.58').write_bytes(actual)
            report = {'identical': passed, 'different_bytes': int(not passed),
                      'reference_sha256': hashlib.sha256(expected).hexdigest(),
                      'c_sources': ['src/main.c'] if units else []}
            Path('reports/boot-rebuild.json').write_text(json.dumps(report))
            Path('reports/objdiff.json').write_text(json.dumps({'units': []}))
            sys.exit(0 if passed else 1)
        ''').lstrip())
        (self.root / "Makefile").write_text(f"validate:\n\t@{sys.executable} tools/fake_validate.py\n")
        self.git("init", "-q", "--initial-branch=main")
        self.git("config", "user.name", "Worker fixture")
        self.git("config", "user.email", "fixture@example.invalid")
        self.git("config", "commit.gpgsign", "false")
        self.git("add", ".")
        self.git("commit", "-qm", "Fixture baseline")
        self.base = self.git("rev-parse", "HEAD")
        for name, value in {"ROOT": self.root, "STATE": self.root / "reports/reconstruction/state.json",
                            "REVIEW": self.root / "config/reconstruction.json"}.items():
            self.stack.enter_context(patch.object(worker, name, value))
        self.stack.enter_context(patch.object(worker, "reference", side_effect=self.reference))
        self.stack.enter_context(patch.object(worker, "inventory", side_effect=self.inventory))
        self.agent = self.root / "tools/tmp/agent.py"
        self.agent.write_text(textwrap.dedent(r'''
            import json, os, subprocess, sys, yaml
            from pathlib import Path
            mode = sys.argv[1]
            prompt = sys.stdin.read()
            context = json.loads(Path(os.environ['CB2_RECONSTRUCTION_CONTEXT']).read_text())
            address = context['address']
            with Path('reports/agents.jsonl').open('a') as out:
                out.write(json.dumps({'pid': os.getpid(), 'address': address, 'prompt': prompt,
                    'branch': subprocess.check_output(['git', 'branch', '--show-current'], text=True).strip(),
                    'main': subprocess.check_output(['git', 'rev-parse', 'main'], text=True).strip()})+'\n')
            if mode == 'exit': sys.exit(3)
            if mode == 'timeout' and address == '0x80010000':
                import time
                time.sleep(60)
            if mode == 'blocked' and address == '0x80010000': sys.exit(0)
            if mode == 'outside': Path('README.md').write_text('unexpected edit\n')
            if mode == 'protected': Path('config/compiler.json').write_text('changed\n')
            if mode == 'branch': subprocess.run(['git', 'switch', '-c', 'unexpected'], check=True)
            if mode == 'move-main':
                rogue = subprocess.check_output(['git', 'commit-tree', 'HEAD^{tree}', '-p', 'HEAD'],
                    input='Unexpected main update\n', text=True).strip()
                subprocess.run(['git', 'update-ref', 'refs/heads/main', rogue], check=True)
            config = yaml.safe_load(Path('scus_943.58.yaml').read_text())
            unit = 'unit_' + address[2:]
            row = next(r for r in config['segments'][0]['subsegments'] if r[0] == context['offset'])
            row[:] = [row[0], 'c', unit]
            Path('scus_943.58.yaml').write_text(yaml.safe_dump(config))
            with Path('symbol_addrs.txt').open('a') as out:
                out.write(context['name']+' = '+address+'; // type:func size:0x8\n')
            result = 8 if mode in ('lie', 'mixed') and address == '0x80010000' else 7
            extra = '__asm__("nop");' if mode == 'assembly' else ''
            Path('src').mkdir(exist_ok=True)
            with Path('src/main.c').open('a') as out:
                out.write('// FUNCTION: '+context['identity']+'\nint '+context['name']+'(void) {'+extra+'return '+str(result)+';}\n')
            if mode == 'tamper':
                source = Path('src/main.c')
                source.write_text(source.read_text().replace('return 7', 'return 8', 1))
            if mode == 'lie':
                Path('reports/boot-rebuild.json').write_text('{"identical": true}')
                Path('build/SCUS_943.58').write_bytes(Path('orig/disc/files/SCUS_943.58').read_bytes())
            Path(os.environ['CB2_AGENT_REPORT']).write_text('Everything matches; commit it.')
            if mode in ('index', 'head'):
                import subprocess
                subprocess.run(['git', 'add', '.'], check=True)
                if mode == 'head': subprocess.run(['git', 'commit', '-qm', 'Unauthorized agent commit'], check=True)
        ''').lstrip())

    def git(self, *args):
        return subprocess.check_output(["git", *args], cwd=self.root, text=True, stderr=subprocess.STDOUT).strip()

    def reference(self):
        path = self.root / "orig/disc/files/SCUS_943.58"
        data = path.read_bytes()
        if hashlib.sha256(data).hexdigest() != self.sha:
            raise RuntimeError("Fixture reference changed")
        return path, data, self.sha

    def inventory(self):
        config = self.yaml.safe_load((self.root / "scus_943.58.yaml").read_text())
        rows = config["segments"][0]["subsegments"]
        functions = []
        for i, offset in enumerate((0, 8)):
            row = next(r for r in rows if r[0] == offset)
            functions.append({"name": f"func_{0x80010000 + offset:08X}", "address": f"0x{0x80010000 + offset:08X}",
                "offset": offset, "size": 8, "status": "c" if row[1] == "c" else "needs_review",
                "identity": f"SCUS_943.58:0x{0x80010000 + offset:08X}", "image":"SCUS_943.58",
                "build_config":"scus_943.58.yaml", "symbol_file":"symbol_addrs.txt",
                "file_offset":offset, "body_bytes":8, "reference_sha256":self.sha,
                "body_sha256":hashlib.sha256(self.original[offset:offset+8]).hexdigest(),
                "ranges":[[f"0x{0x80010000+offset:08X}", f"0x{0x80010008+offset:08X}"]],
                "contiguous": True, "hardware_or_gp": False, "features": [], "classification": "unknown",
                "boundary_reviewed": True, "boundary_kind": "standalone", "boundary_findings": {},
                "calls": [], "callers": [], "references": [], "body": "fixture target\n"})
        report = {"targets": functions}
        return report

    def runner(self, mode="match", limit=2):
        return Runner(worker, SimpleNamespace(limit=limit, minutes=2, agent_seconds=10,
            agent_command=shlex.join([sys.executable, str(self.agent), mode])))

    def test_paused_policy_stops_before_session_or_agent_launch(self):
        (self.root / "config/reconstruction.json").write_text('{"matching_paused": true}\n')
        with patch.object(worker, "begin_batch") as begin:
            with self.assertRaisesRegex(RuntimeError, "paused.*no batch started"):
                self.runner().run()
            begin.assert_not_called()
        self.assertFalse((self.root / "reports/agents.jsonl").exists())
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)

    def test_two_functions_use_distinct_processes_and_commits(self):
        self.runner().run()
        session = worker.state()
        self.assertEqual(len(session["completed"]), 2)
        self.assertIsNone(session["active"])
        events = [json.loads(line) for line in (self.root / "reports/agents.jsonl").read_text().splitlines()]
        self.assertEqual(len({e["pid"] for e in events}), 2)
        self.assertEqual(len({e["address"] for e in events}), 2)
        self.assertEqual([e["branch"] for e in events], ["codex/match-SCUS_943_58_0x80010000", "codex/match-SCUS_943_58_0x80010008"])
        self.assertEqual(events[0]["main"], self.base)
        self.assertEqual(events[1]["main"], session["completed"][0]["commit"])
        self.assertEqual(self.git("branch", "--show-current"), "main")
        self.assertEqual(self.git("rev-list", "--count", f"{self.base}..HEAD"), "2")
        self.assertFalse(worker.changes())
        self.assertEqual((self.root / "build/SCUS_943.58").read_bytes(), self.original)
        for completed in session["completed"]:
            self.assertTrue((self.root / completed["context"] / "validation.json").is_file())
            self.assertEqual(self.git("rev-parse", completed["branch"]), completed["commit"])
            self.assertEqual(self.git("rev-parse", completed["commit"] + "^"), completed["base_commit"])

    def test_mismatch_is_archived_restored_and_next_function_commits(self):
        self.runner("mixed").run()
        session = worker.state()
        self.assertEqual(len(session["deferred"]), 1)
        self.assertEqual(len(session["completed"]), 1)
        failed = self.root / session["deferred"][0]["context"]
        self.assertIn("return 8", (failed / "candidate/sources.json").read_text())
        self.assertFalse(worker.read(failed / "validation.json")["identical"])
        self.assertNotIn("func_80010000", (self.root / "src/main.c").read_text())
        self.assertTrue((self.root / "src/main.c").exists())
        self.assertFalse(worker.changes())
        deferred = session["deferred"][0]
        self.assertEqual(self.git("rev-parse", deferred["branch"]), deferred["checkpoint"])
        self.assertIn("return 8", self.git("show", deferred["branch"] + ":src/main.c"))
        self.assertNotIn(deferred["checkpoint"], self.git("rev-list", "main").splitlines())
        self.assertEqual(self.git("branch", "--show-current"), "main")

    def test_existing_function_block_is_protected_in_shared_source(self):
        self.runner(limit=1).run()
        previous = (self.root / 'src/main.c').read_text()
        self.runner('tamper', limit=1).run()
        self.assertEqual((self.root / 'src/main.c').read_text(), previous)
        self.assertIn('Existing overlay source', worker.state()['deferred'][0]['reason'])

    def test_agent_claim_and_forged_success_do_not_authorize_commit(self):
        self.runner("lie", limit=1).run()
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)
        self.assertEqual(len(worker.state()["deferred"]), 1)
        self.assertFalse(worker.changes())
        self.assertEqual((self.root / "build/SCUS_943.58").read_bytes(), self.original)

    def test_assembly_embedding_is_rejected_even_if_fixture_gate_would_pass(self):
        self.runner("assembly", limit=1).run()
        self.assertIn("embedding", worker.state()["deferred"][0]["reason"])
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)
        self.assertFalse(worker.changes())
        deferred = worker.state()["deferred"][0]
        self.assertIn("checkpoint_excluded", deferred)
        self.assertEqual(self.git("ls-tree", "-r", "--name-only", deferred["branch"], "src"), "")

    def test_blocked_function_does_not_contaminate_next_target(self):
        self.runner("blocked").run()
        self.assertEqual(len(worker.state()["deferred"]), 1)
        self.assertEqual(len(worker.state()["completed"]), 1)

    def test_unexpected_edits_stop_without_resetting_them(self):
        with self.assertRaisesRegex(RuntimeError, "Out-of-scope"):
            self.runner("outside").run()
        self.assertEqual((self.root / "README.md").read_text(), "unexpected edit\n")
        self.assertIsNotNone(worker.state()["active"])
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)

    def test_protected_recipe_change_stops_before_validation_or_commit(self):
        with self.assertRaisesRegex(RuntimeError, "Protected"):
            self.runner("protected").run()
        self.assertEqual((self.root / "config/compiler.json").read_text(), "changed\n")
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)

    def test_failing_launcher_stops_after_one_target(self):
        with self.assertRaisesRegex(RuntimeError, "launcher failed"):
            self.runner("exit").run()
        self.assertEqual(len(worker.state()["deferred"]), 1)
        self.assertFalse(worker.changes())

    def test_agent_timeout_is_deferred_and_next_target_runs(self):
        runner = self.runner("timeout")
        runner.args.agent_seconds = 1
        runner.run()
        self.assertEqual(len(worker.state()["deferred"]), 1)
        self.assertEqual(len(worker.state()["completed"]), 1)
        self.assertIn("timed out", worker.state()["deferred"][0]["reason"])
        self.assertFalse(worker.changes())

    def test_agent_index_changes_stop_without_unstaging_them(self):
        with self.assertRaisesRegex(RuntimeError, "index"):
            self.runner("index").run()
        self.assertTrue(self.git("diff", "--cached", "--name-only"))
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)

    def test_agent_commits_stop_without_resetting_head(self):
        with self.assertRaisesRegex(RuntimeError, "HEAD"):
            self.runner("head").run()
        self.assertNotEqual(self.git("rev-parse", "HEAD"), self.base)
        self.assertEqual(self.git("log", "-1", "--format=%s"), "Unauthorized agent commit")
        self.assertIsNotNone(worker.state()["active"])
        self.assertEqual(self.git("rev-parse", "main"), self.base)

    def test_agent_branch_switch_at_same_head_stops(self):
        with self.assertRaisesRegex(RuntimeError, "changed branch"):
            self.runner("branch").run()
        self.assertEqual(self.git("branch", "--show-current"), "unexpected")
        self.assertEqual(self.git("rev-parse", "main"), self.base)

    def test_unexpected_main_update_is_not_overwritten(self):
        with self.assertRaisesRegex(RuntimeError, "main moved"):
            self.runner("move-main").run()
        self.assertNotEqual(self.git("rev-parse", "main"), self.base)
        self.assertEqual(self.git("log", "-1", "--format=%s", "main"), "Unexpected main update")
        self.assertEqual(self.git("branch", "--show-current"), "codex/match-SCUS_943_58_0x80010000")

    def test_wip_helper_refuses_main(self):
        with self.assertRaisesRegex(RuntimeError, "WIP checkpoints require"):
            worker.commit_files([], "WIP", {}, checkpoint=True)
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)

    def test_failed_fast_forward_keeps_validated_function_branch(self):
        git = worker.git
        def failed_merge(*args):
            if args[:1] == ("merge",):
                raise RuntimeError("Fixture merge failure")
            return git(*args)
        with patch.object(worker, "git", side_effect=failed_merge), self.assertRaisesRegex(RuntimeError, "merge failure"):
            self.runner(limit=1).run()
        session = worker.state()
        self.assertEqual(session["active"]["phase"], "integrating")
        self.assertEqual(self.git("rev-parse", session["active"]["branch"]), session["active"]["commit"])
        self.assertEqual(self.git("rev-parse", "main"), self.base)
        self.assertEqual(self.git("branch", "--show-current"), "main")
        self.assertFalse(session["completed"])
        self.assertFalse(worker.changes())

    def test_commit_hook_cannot_change_validated_contents_and_reach_main(self):
        hook = self.root / ".git/hooks/pre-commit"
        hook.write_text("#!/bin/sh\n" + shlex.quote(sys.executable) + " - <<'PY'\n"
            "from pathlib import Path\n"
            "p=Path('src/main.c')\n"
            "p.write_text(p.read_text().replace('return 7', 'return 8'))\n"
            "PY\ngit add -- src/main.c\n")
        hook.chmod(0o755)
        with self.assertRaisesRegex(RuntimeError, "Committed contents differ"):
            self.runner(limit=1).run()
        self.assertEqual(self.git("rev-parse", "main"), self.base)
        self.assertEqual(self.git("branch", "--show-current"), "codex/match-SCUS_943_58_0x80010000")
        self.assertIn("return 8", self.git("show", "HEAD:src/main.c"))
        self.assertFalse(worker.state()["completed"])

    def test_retry_reuses_branch_and_starts_from_latest_main(self):
        self.runner("mixed").run()
        previous = worker.state()["deferred"][0]
        updated_main = self.git("rev-parse", "main")
        self.runner("match", limit=1).run()
        accepted = worker.state()["completed"][0]
        self.assertEqual(accepted["branch"], "codex/match-SCUS_943_58_0x80010000")
        self.assertEqual(accepted["base_commit"], updated_main)
        self.assertEqual(self.git("rev-parse", previous["branch"]), accepted["commit"])
        self.assertNotIn(previous["checkpoint"], self.git("rev-list", "main").splitlines())
        self.assertEqual(self.git("branch", "--show-current"), "main")

    def test_batch_cannot_start_on_function_branch(self):
        self.git("switch", "-c", "codex/manual")
        with self.assertRaisesRegex(RuntimeError, "main integration"):
            self.runner().run()
        self.assertFalse(worker.STATE.exists())

    def test_interruption_archives_and_restores_candidate(self):
        runner = self.runner()
        launch = runner.launch
        def interrupted(session):
            launch(session)
            raise KeyboardInterrupt
        with patch.object(runner, "launch", side_effect=interrupted), self.assertRaises(KeyboardInterrupt):
            runner.run()
        self.assertEqual(len(worker.state()["deferred"]), 1)
        self.assertFalse(worker.changes())
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)

    def test_deadline_expiring_during_agent_prevents_commit(self):
        runner = self.runner()
        launch = runner.launch
        def expired(session):
            launch(session)
            session["deadline"] = 0
        with patch.object(runner, "launch", side_effect=expired):
            runner.run()
        self.assertEqual(worker.state()["stop_reason"], "session time budget exhausted")
        self.assertFalse(worker.changes())
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)

    def test_same_path_edit_after_build_cannot_commit_stale_success(self):
        validate = worker.validate
        def changed_after_build(session):
            result = validate(session)
            source = self.root / "src/main.c"
            source.write_text(source.read_text().replace("return 7", "return 8"))
            return result
        with patch.object(worker, "validate", side_effect=changed_after_build):
            self.runner(limit=1).run()
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)
        self.assertIn("contents changed", worker.state()["deferred"][0]["reason"])
        self.assertFalse(worker.changes())

    def test_deadline_expiring_during_validation_restores_candidate(self):
        validate = worker.validate
        def expired_after_build(session):
            result = validate(session)
            session["deadline"] = 0
            return result
        with patch.object(worker, "validate", side_effect=expired_after_build):
            self.runner().run()
        self.assertEqual(self.git("rev-parse", "HEAD"), self.base)
        self.assertEqual(worker.state()["stop_reason"], "session time budget exhausted")
        self.assertFalse(worker.changes())

    def test_empty_or_misquoted_launcher_rejected_before_session(self):
        for text in ("", "'"):
            args = SimpleNamespace(limit=1, minutes=1, agent_seconds=1, agent_command=text)
            with self.subTest(text=text), self.assertRaises(RuntimeError):
                Runner(worker, args)
        self.assertFalse(worker.STATE.exists())

    def test_default_launcher_is_fresh_ephemeral_and_sandboxed(self):
        runner = self.runner()
        runner.args.agent_command = None
        runner.launcher = ["codex"]
        session = worker.begin_batch(1, 2)
        session.update(branch="main", integration_branch="main", integration_head=session["head"])
        worker.next_target(session)
        runner.start_branch(session)
        with patch.object(worker, "command") as command:
            runner.launch(session)
        argv = command.call_args.args[0]
        self.assertIn("--ephemeral", argv)
        self.assertIn("workspace-write", argv)
        self.assertNotIn("resume", argv)
        self.assertEqual(argv[-1], "-")
        self.assertIn("Do not stage, commit", command.call_args.kwargs["input_text"])

    def test_timeout_kills_child_process_group(self):
        code = ("import subprocess,sys,time; subprocess.Popen([sys.executable,'-c',"
                "\"import time; from pathlib import Path; time.sleep(.4); Path('reports/escaped').write_text('bad')\"]); time.sleep(60)")
        with self.assertRaises(worker.CommandTimeout):
            worker.command([sys.executable, "-c", code], "timeout", timeout=.1)
        time.sleep(.5)
        self.assertFalse((self.root / "reports/escaped").exists())

    def test_unrelated_boundary_and_raw_array_rejected(self):
        proposed = copy.deepcopy(self.config)
        proposed["segments"][0]["subsegments"][0] = [0, "c", "new_unit"]
        self.assertEqual(review_split(self.config, proposed, {"offset": 0, "size": 8}), "src/main.c")
        proposed["options"]["compiler"] = "different"
        with self.assertRaises(Rejected):
            review_split(self.config, proposed, {"offset": 0, "size": 8})
        (self.root / "src/new_unit.c").write_text('int func(void) {unsigned char bytes[]={8,0,224,3}; return 7;}')
        with self.assertRaisesRegex(Rejected, "Initialized arrays"):
            review_source(self.root, ["src/new_unit.c"], "src/new_unit.c", "func")

    def test_struct_definitions_live_in_headers_and_existing_layouts_are_protected(self):
        header = '#ifndef TYPES_H\n#define TYPES_H\ntypedef struct { int x; } Point;\n\n#endif\n'
        extended = header.removesuffix('#endif\n') + 'typedef struct { Point start; } Line;\n\n#endif\n'
        review_header_extension(header, extended)
        with self.assertRaisesRegex(Rejected, 'Existing header'):
            review_header_extension(header, extended.replace('int x;', 'short x;'))
        source = self.root / 'src/example.c'
        source.write_text('typedef struct { int x; } Point;\nint example(void) {return 7;}\n')
        with self.assertRaisesRegex(Rejected, 'belong in shared headers'):
            review_source(self.root, ['src/example.c'], 'src/example.c', 'example')
        (self.root / 'src/types.h').write_text(extended)
        source.write_text('#include "types.h"\nint example(void) {Point p; p.x = 7; return p.x;}\n')
        review_source(self.root, ['src/example.c', 'src/types.h'], 'src/example.c', 'example')

    def test_externs_are_shared_at_top_and_existing_declarations_are_protected(self):
        shared = 'extern int existing;\n\n'
        review_shared_extension(shared, shared + 'extern int added;\n\n')
        with self.assertRaisesRegex(Rejected, 'Existing shared declarations'):
            review_shared_extension(shared, shared.replace('int existing', 'short existing'))
        with self.assertRaisesRegex(Rejected, 'Only extern declarations'):
            review_shared_extension(shared, shared + 'int definition = 7;\n')
        source = self.root / 'src/example.c'
        block = '// FUNCTION: SCUS_943.58:0x80010000\nint example(void) {return existing;}\n'
        source.write_text(shared + block)
        review_source(self.root, ['src/example.c'], 'src/example.c', 'example')
        source.write_text('// FUNCTION: SCUS_943.58:0x80010000\n' + shared + 'int example(void) {return existing;}\n')
        with self.assertRaisesRegex(Rejected, 'Extern declarations belong at the top'):
            review_source(self.root, ['src/example.c'], 'src/example.c', 'example')

    def test_split_preserves_embedded_data_dictionary_and_its_position(self):
        before = copy.deepcopy(self.config)
        data = {"start": 8, "type": "data", "name": "template",
                "linker_section": ".text", "linker_section_order": ".text"}
        before["segments"][0]["subsegments"] = [[0, "asm", "first"], data,
                                                     [12, "asm", "second"], [20, "data", "data"]]
        # A target immediately before a data island and one after it both work.
        for offset, index in [(0, 0), (12, 2)]:
            proposed = copy.deepcopy(before)
            proposed["segments"][0]["subsegments"][index] = [offset, "c", "unit"]
            self.assertEqual(review_split(before, proposed, {"offset": offset, "size": 8}), "src/main.c")
            proposed["segments"][0]["subsegments"][1]["linker_section"] = ".data"
            with self.assertRaises(Rejected):
                review_split(before, proposed, {"offset": offset, "size": 8})


if __name__ == "__main__":
    unittest.main()
