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
from reconstruct_run import Runner, Rejected, review_source, review_split


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
            passed = all(re.search(r'\breturn\s+7\s*;', Path('src', n+'.c').read_text()) for n in units)
            actual = expected if passed else b'!' + expected[1:]
            Path('build').mkdir(exist_ok=True)
            Path('reports').mkdir(exist_ok=True)
            Path('build/SCUS_943.58').write_bytes(actual)
            report = {'identical': passed, 'different_bytes': int(not passed),
                      'reference_sha256': hashlib.sha256(expected).hexdigest(),
                      'c_sources': ['src/'+n+'.c' for n in units]}
            Path('reports/boot-rebuild.json').write_text(json.dumps(report))
            Path('reports/objdiff.json').write_text(json.dumps({'units': []}))
            sys.exit(0 if passed else 1)
        ''').lstrip())
        (self.root / "Makefile").write_text(f"validate:\n\t@{sys.executable} tools/fake_validate.py\n")
        self.git("init", "-q")
        self.git("config", "user.name", "Worker fixture")
        self.git("config", "user.email", "fixture@example.invalid")
        self.git("config", "commit.gpgsign", "false")
        self.git("add", ".")
        self.git("commit", "-qm", "Fixture baseline")
        self.base = self.git("rev-parse", "HEAD")
        for name, value in {"ROOT": self.root, "STATE": self.root / "reports/reconstruction/state.json",
                            "INVENTORY": self.root / "reports/reconstruction/inventory.json",
                            "REVIEW": self.root / "config/reconstruction.json"}.items():
            self.stack.enter_context(patch.object(worker, name, value))
        self.stack.enter_context(patch.object(worker, "reference", side_effect=self.reference))
        self.stack.enter_context(patch.object(worker, "inventory", side_effect=self.inventory))
        self.agent = self.root / "tools/tmp/agent.py"
        self.agent.write_text(textwrap.dedent(r'''
            import json, os, sys, yaml
            from pathlib import Path
            mode = sys.argv[1]
            prompt = sys.stdin.read()
            context = json.loads(Path(os.environ['CB2_RECONSTRUCTION_CONTEXT']).read_text())
            address = context['address']
            with Path('reports/agents.jsonl').open('a') as out:
                out.write(json.dumps({'pid': os.getpid(), 'address': address, 'prompt': prompt})+'\n')
            if mode == 'exit': sys.exit(3)
            if mode == 'timeout' and address == '0x80010000':
                import time
                time.sleep(60)
            if mode == 'blocked' and address == '0x80010000': sys.exit(0)
            if mode == 'outside': Path('README.md').write_text('unexpected edit\n')
            if mode == 'protected': Path('config/compiler.json').write_text('changed\n')
            config = yaml.safe_load(Path('scus_943.58.yaml').read_text())
            unit = 'unit_' + address[2:]
            row = next(r for r in config['segments'][0]['subsegments'] if r[0] == context['offset'])
            row[:] = [row[0], 'c', unit]
            Path('scus_943.58.yaml').write_text(yaml.safe_dump(config))
            with Path('symbol_addrs.txt').open('a') as out:
                out.write(context['name']+' = '+address+'; // type:func size:0x8\n')
            result = 8 if mode in ('lie', 'mixed') and address == '0x80010000' else 7
            extra = '__asm__("nop");' if mode == 'assembly' else ''
            Path('src', unit+'.c').write_text('int '+context['name']+'(void) {'+extra+'return '+str(result)+';}\n')
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
                "offset": offset, "size": 8, "status": "c" if row[1] == "c" else "assembly",
                "contiguous": True, "hardware_or_gp": False, "features": [], "classification": "unknown",
                "boundary_reviewed": True, "calls": [], "callers": [], "references": [], "body": "fixture target\n"})
        report = {"functions": functions}
        worker.write(worker.INVENTORY, report)
        return report

    def runner(self, mode="match", limit=2):
        return Runner(worker, SimpleNamespace(limit=limit, minutes=2, agent_seconds=10,
            agent_command=shlex.join([sys.executable, str(self.agent), mode])))

    def test_two_functions_use_distinct_processes_and_commits(self):
        self.runner().run()
        session = worker.state()
        self.assertEqual(len(session["completed"]), 2)
        self.assertIsNone(session["active"])
        events = [json.loads(line) for line in (self.root / "reports/agents.jsonl").read_text().splitlines()]
        self.assertEqual(len({e["pid"] for e in events}), 2)
        self.assertEqual(len({e["address"] for e in events}), 2)
        self.assertEqual(self.git("rev-list", "--count", f"{self.base}..HEAD"), "2")
        self.assertFalse(worker.changes())
        self.assertEqual((self.root / "build/SCUS_943.58").read_bytes(), self.original)
        for completed in session["completed"]:
            self.assertTrue((self.root / completed["context"] / "attempt-1/boot-rebuild.json").is_file())

    def test_mismatch_is_archived_restored_and_next_function_commits(self):
        self.runner("mixed").run()
        session = worker.state()
        self.assertEqual(len(session["deferred"]), 1)
        self.assertEqual(len(session["completed"]), 1)
        failed = self.root / session["deferred"][0]["context"]
        self.assertIn("return 8", (failed / "rejected/sources.json").read_text())
        self.assertFalse(worker.read(failed / "attempt-1/boot-rebuild.json")["identical"])
        self.assertFalse((self.root / "src/unit_80010000.c").exists())
        self.assertTrue((self.root / "src/unit_80010008.c").exists())
        self.assertFalse(worker.changes())

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
            source = self.root / "src/unit_80010000.c"
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
        session = worker.begin_session(1, 1, 2)
        worker.next_target(session)
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
        self.assertEqual(review_split(self.config, proposed, {"offset": 0, "size": 8}), "src/new_unit.c")
        proposed["options"]["compiler"] = "different"
        with self.assertRaises(Rejected):
            review_split(self.config, proposed, {"offset": 0, "size": 8})
        (self.root / "src/new_unit.c").write_text('int func(void) {unsigned char bytes[]={8,0,224,3}; return 7;}')
        with self.assertRaisesRegex(Rejected, "Initialized arrays"):
            review_source(self.root, ["src/new_unit.c"], "src/new_unit.c", "func")


if __name__ == "__main__":
    unittest.main()
