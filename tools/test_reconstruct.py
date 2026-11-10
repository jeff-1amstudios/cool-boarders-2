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
from boundary_audit import parse_functions


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

    def test_assembly_context_must_match_reference_bytes(self):
        directory = worker.ROOT / "tools/tmp"
        directory.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=directory) as tmp:
            path = Path(tmp) / "fixture.s"
            path.write_text("glabel example\n/* 0 80010000 0800E003 */ jr $ra\n"
                            "/* 4 80010004 00000000 */ nop\nendlabel example\n")
            rows = parse_functions(path, bytes.fromhex("0800E00300000000"))
            self.assertEqual(rows[0]["size"], 8)
            self.assertTrue(rows[0]["contiguous"])
            with self.assertRaises(RuntimeError):
                parse_functions(path, bytes(8))

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
