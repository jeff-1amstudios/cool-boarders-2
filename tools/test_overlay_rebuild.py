#!/usr/bin/env python3
"""Full-file overlay gates, stale-report handling and real C/objdiff integration."""
import contextlib
import io
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import yaml

import overlay_rebuild as overlay
import validate as validation


class GateTests(unittest.TestCase):
    def test_complete_file_comparison_includes_header_and_partial_word_tail(self):
        original = bytes.fromhex("06000000112233445566")
        self.assertTrue(overlay.compare(original, original)["identical"])
        for changed, offset in [(b"\x07" + original[1:], "0x0"),
                                (original[:-1] + b"\x67", "0x9"), (original[:-2], "0x8")]:
            with self.subTest(changed=changed):
                report = overlay.compare(original, changed)
                self.assertFalse(report["identical"])
                self.assertIn(offset, report["first_different_offsets"])

    def test_failed_image_does_not_skip_siblings_or_leave_an_old_success(self):
        (overlay.ROOT / "tools/tmp").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=overlay.ROOT / "tools/tmp") as tmp:
            root = Path(tmp)
            summary, details = root / "summary.json", root / "details"
            details.mkdir()
            summary.write_text('{"identical":true}')
            (details / "stale.json").write_text('{"identical":true}')
            with patch.object(overlay, "SUMMARY", summary), patch.object(overlay, "DETAILS", details), \
                 patch.object(overlay, "configs", return_value={"A.BIN": {}, "B.BIN": {}}), \
                 patch.object(overlay, "rebuild_image", side_effect=[RuntimeError("assembler failed"),
                     {"image": "B.BIN", "identical": True, "c_sources": []}]):
                with contextlib.redirect_stdout(io.StringIO()):
                    report = overlay.rebuild_all()
            self.assertFalse(report["identical"])
            self.assertEqual(report["matching_overlays"], 1)
            self.assertFalse((details / "stale.json").exists())
            self.assertIn("assembler failed", json.loads((details / "a.json").read_text())["error"])

    def test_changed_reference_cannot_leave_aggregate_success(self):
        (overlay.ROOT / "tools/tmp").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=overlay.ROOT / "tools/tmp") as tmp:
            summary = Path(tmp) / "summary.json"
            summary.write_text('{"identical":true}')
            with patch.object(overlay, "SUMMARY", summary), patch.object(overlay, "DETAILS", Path(tmp) / "details"), \
                 patch.object(overlay, "configs", side_effect=RuntimeError("Changed overlay reference")):
                with self.assertRaisesRegex(RuntimeError, "Changed overlay reference"):
                    overlay.rebuild_all()
            self.assertFalse(summary.exists())


    def test_default_validation_does_not_hide_overlay_failure_behind_boot_success(self):
        (overlay.ROOT / "tools/tmp").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=overlay.ROOT / "tools/tmp") as tmp:
            root = Path(tmp)
            (root / "reports").mkdir()
            for name in ("objdiff.json", "boot-rebuild.json"):
                (root / "reports" / name).write_text("stale success")
            with patch.object(validation, "ROOT", root), patch.object(validation, "clear_reports") as clear, \
                 patch.object(validation, "run"), patch.object(validation.subprocess, "run", side_effect=[
                     subprocess.CompletedProcess([], 0), subprocess.CompletedProcess([], 1)]) as build:
                self.assertEqual(validation.main(), 1)
                clear.assert_called_once()
                self.assertEqual(build.call_count, 2)
                self.assertTrue(any("overlay_rebuild.py" in str(arg) for arg in build.call_args.args[0]))
                self.assertFalse((root / "reports/boot-rebuild.json").exists())


class CIntegrationTests(unittest.TestCase):
    @unittest.skipUnless(Path(str(overlay.PREFIX) + "as").exists() and
                         (overlay.ROOT / "orig/disc/files/TM.BIN").exists(), "Requires repository-local toolchain and disc")
    def test_real_overlay_c_and_objdiff_accept_matching_code_and_reject_changed_code(self):
        # This original TM function is an eight-byte jr-ra/nop stub. No C is retained.
        item = dict(overlay.configs()["TM.BIN"])
        _, config = overlay.image_config("TM.BIN", item)
        temporary = overlay.ROOT / "tools/tmp"
        temporary.mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="overlay-c-", dir=temporary) as tmp:
            directory = Path(tmp)
            unit = directory.name.replace("-", "_")
            source = overlay.ROOT / "src/overlays/tm" / (unit + ".c")
            source.parent.mkdir(parents=True, exist_ok=True)
            self.assertFalse(source.exists())
            sections = config["segments"][0]["subsegments"]
            config["segments"][0]["subsegments"] = [sections[0], [4, "asm", "smoke_before"],
                [0x831C, "c", unit], [0x8324, "asm", "smoke_after"], sections[-1]]
            config["options"]["base_path"] = str(overlay.ROOT)
            split = directory / "tm.yaml"
            split.write_text(yaml.safe_dump(config, sort_keys=False))
            item["build_config"] = split.relative_to(overlay.ROOT).as_posix()
            log = overlay.ROOT / "reports/overlays/c-build-smoke.log"
            log.parent.mkdir(parents=True, exist_ok=True)
            try:
                source.write_text("void func_80062E84(void) {}\n")
                with log.open("w") as output, contextlib.redirect_stdout(output), \
                     patch.object(overlay, "configs", return_value={"TM.BIN": item}):
                    units = overlay.prepare_objdiff_units()
                    self.assertEqual(len(units), 1)
                    report = overlay.rebuild_image("TM.BIN", item)
                    self.assertTrue(report["identical"])
                    self.assertEqual(report["c_sources"], [source.relative_to(overlay.ROOT).as_posix()])
                    # Use objdiff itself with independent, byte-verified reference objects.
                    project = {"units": [{**units[0],
                        "target_path": str(overlay.ROOT / units[0]["target_path"]),
                        "base_path": str(overlay.ROOT / units[0]["base_path"])}]}
                    (directory / "objdiff.json").write_text(json.dumps(project))
                    diff = directory / "diff.json"
                    subprocess.run([str(overlay.ROOT / "tools/objdiff/bin/objdiff-cli"), "diff", "-p", str(directory),
                                    "-u", units[0]["name"], "-o", str(diff), "--format", "json-pretty"],
                                   check=True, stdout=output, stderr=output)
                    symbols = json.loads(diff.read_text())["left"]["symbols"]
                    function = next(s for s in symbols if s["name"] == "func_80062E84")
                    self.assertEqual(function["match_percent"], 100)
                    source.write_text("int func_80062E84(void) { return 1; }\n")
                    mismatch = overlay.rebuild_image("TM.BIN", item)
                    self.assertFalse(mismatch["identical"])
                    self.assertGreater(mismatch["different_bytes"], 0)
                    subprocess.run([str(overlay.ROOT / "tools/objdiff/bin/objdiff-cli"), "diff", "-p", str(directory),
                                    "-u", units[0]["name"], "-o", str(diff), "--format", "json-pretty"],
                                   check=True, stdout=output, stderr=output)
                    symbols = json.loads(diff.read_text())["left"]["symbols"]
                    function = next(s for s in symbols if s["name"] == "func_80062E84")
                    self.assertLess(function["match_percent"], 100)
            finally:
                source.unlink(missing_ok=True)


if __name__ == "__main__":
    unittest.main()
