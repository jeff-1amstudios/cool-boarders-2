#!/usr/bin/env python3
"""Negative controls for the reconstruction worker's selection and commit gates."""
import tempfile
import time
import sys
import subprocess
import unittest
from pathlib import Path
from unittest.mock import patch

import reconstruct as worker


class WorkerTests(unittest.TestCase):
    def test_staging_requires_exact_explicit_authored_paths(self):
        good = {"src/example.c", "scus_943.58.yaml"}
        self.assertEqual(set(worker.audit_paths(list(good), good)), good)
        for paths, changes in [(["src/example.c"], good),
                               (["reports/dump.json"], {"reports/dump.json"}),
                               (["tools/vendor/example.c"], {"tools/vendor/example.c"}),
                               (["src/../orig/dump.c"], {"src/../orig/dump.c"}),
                               (["config/usa-disc.json"], {"config/usa-disc.json"})]:
            with self.subTest(paths=paths), self.assertRaises(RuntimeError):
                worker.audit_paths(paths, changes)

    def test_protected_recipe_head_and_deadline(self):
        session = {"head": "a", "protected": {"recipe": "a"}, "deadline": time.time() + 60}
        with patch.object(worker, "git", return_value="a"), patch.object(worker, "fingerprints", return_value={"recipe": "a"}):
            worker.check_session(session)
            with self.assertRaises(RuntimeError):
                worker.check_session({**session, "deadline": 0})
            with self.assertRaises(RuntimeError):
                worker.check_session({**session, "head": "b"})
            with self.assertRaises(RuntimeError):
                worker.check_session({**session, "protected": {"recipe": "b"}})

    def test_attempt_limit_prevents_even_a_stale_success(self):
        session = {"active": {"attempts": 2}, "attempt_limit": 2}
        with patch.object(worker, "check_session"), patch.object(worker, "command") as build:
            with self.assertRaises(RuntimeError):
                worker.validate(session)
            build.assert_not_called()

    def test_selection_excludes_hardware_sdk_and_completed_targets(self):
        base = {"status": "assembly", "contiguous": True, "hardware_or_gp": False,
                "classification": "unknown", "size": 32, "features": [],
                "boundary_reviewed": False, "calls": []}
        rows = [{**base, "address": "0x80010000", "hardware_or_gp": True},
                {**base, "address": "0x80010004", "classification": "sdk"},
                {**base, "address": "0x80010008"},
                {**base, "address": "0x8001000A", "hold": True},
                {**base, "address": "0x8001000C"}]
        session = {"completed": [{"address": "0x80010008"}], "deferred": []}
        with patch.object(worker, "read", return_value={"pilot": []}):
            self.assertEqual(worker.choose({"functions": rows}, session)["address"], "0x8001000C")
            with self.assertRaises(RuntimeError):
                worker.choose({"functions": rows}, session, "80010000")

    def test_assembly_context_must_match_reference_bytes(self):
        directory = worker.ROOT / "tools/tmp"
        directory.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=directory) as tmp:
            path = Path(tmp) / "fixture.s"
            path.write_text("glabel example\n/* 0 80010000 0800E003 */ jr $ra\n"
                            "/* 4 80010004 00000000 */ nop\nendlabel example\n")
            rows = worker.parse_functions(path, bytes.fromhex("0800E00300000000"))
            self.assertEqual(rows[0]["size"], 8)
            self.assertTrue(rows[0]["contiguous"])
            with self.assertRaises(RuntimeError):
                worker.parse_functions(path, bytes(8))

    def test_session_archives_survive_new_sessions(self):
        directory = worker.ROOT / "tools/tmp"
        directory.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=directory) as tmp:
            root = Path(tmp)
            with patch.object(worker, "ROOT", root), patch.object(worker, "STATE", root / "state.json"):
                first = {"directory": "sessions/first", "completed": ["a"]}
                second = {"directory": "sessions/second", "completed": []}
                worker.save_state(first)
                worker.save_state(second)
                self.assertEqual(worker.read(root / "sessions/first/state.json"), first)
                self.assertEqual(worker.read(root / "state.json"), second)

    def test_concurrent_worker_is_rejected(self):
        directory = worker.ROOT / "tools/tmp"
        directory.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=directory) as tmp:
            with worker.exclusive_worker(Path(tmp)):
                result = subprocess.run([sys.executable, "-c",
                    "import sys; from pathlib import Path; sys.path.insert(0, 'tools'); "
                    "from reconstruct import exclusive_worker; "
                    "\nwith exclusive_worker(Path(sys.argv[1])): pass", tmp],
                    capture_output=True, text=True, cwd=worker.ROOT)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("Another reconstruction worker", result.stderr)


if __name__ == "__main__":
    unittest.main()
