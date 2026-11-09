#!/usr/bin/env python3
"""Synthetic R3000 counterexamples for boundary and ABI diagnostics."""
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

import boundary_audit as audit

BASE = 0x80010000


def r(rs=0, rt=0, rd=0, function=0):
    return (rs << 21) | (rt << 16) | (rd << 11) | function


def i(op, rs, rt, immediate):
    return (op << 26) | (rs << 21) | (rt << 16) | (immediate & 0xFFFF)


def jal(address):
    return (3 << 26) | ((address >> 2) & 0x3FFFFFF)


def code(words):
    return {BASE + n * 4: audit.decode(word, BASE + n * 4) for n, word in enumerate(words)}


def findings(words):
    return audit.analyze_cfg(code(words), BASE)["findings"]


class BoundaryTests(unittest.TestCase):
    def test_leaf_function_needs_no_prologue_and_delay_slot_belongs_to_it(self):
        self.assertFalse(findings([r(31, function=8), r(0, 0, 2, 0x21)]))
        self.assertIn("delay_slot_outside_span", findings([r(31, function=8)]))

    def test_saved_registers_and_return_address_survive_a_call(self):
        words = [i(9, 29, 29, -24), i(0x2B, 29, 31, 20), i(0x2B, 29, 16, 16),
                 r(4, 0, 16, 0x21), jal(BASE + 0x1000), 0,
                 i(0x23, 29, 31, 20), i(0x23, 29, 16, 16), i(9, 29, 29, 24),
                 r(31, function=8), 0]
        self.assertFalse(findings(words))
        bad = words.copy()
        bad[6] = 0
        self.assertIn("return_address_not_entry_ra", findings(bad))
        bad = words.copy()
        bad[7] = 0
        self.assertIn("callee_saved_r16_not_restored", findings(bad))

    def test_load_immediately_before_return_does_not_supply_return_target(self):
        # lw ra; jr ra is a load-delay hazard even with a correct saved value.
        words = [i(9, 29, 29, -24), i(0x2B, 29, 31, 20), jal(BASE + 0x1000), 0,
                 i(0x23, 29, 31, 20), r(31, function=8), i(9, 29, 29, 24)]
        self.assertIn("return_address_not_entry_ra", findings(words))

    def test_save_in_call_delay_slot_saves_the_link_not_original_ra(self):
        words = [i(9, 29, 29, -24), jal(BASE + 0x1000), i(0x2B, 29, 31, 20),
                 i(0x23, 29, 31, 20), i(9, 29, 29, 24), r(31, function=8), 0]
        self.assertIn("return_address_not_entry_ra", findings(words))

    def test_one_bad_exit_is_not_hidden_by_a_good_early_return(self):
        words = [i(9, 29, 29, -8), i(4, 4, 0, 4), 0, i(9, 29, 29, 8),
                 r(31, function=8), 0, r(31, function=8), 0]
        result = findings(words)
        self.assertEqual(result["stack_not_restored_at_return"], [audit.hx(BASE + 24)])

    def test_early_return_is_not_automatically_a_function_end(self):
        words = [i(4, 4, 0, 3), 0, r(31, function=8), 0,
                 r(31, function=8), r(4, 0, 2, 0x21)]
        self.assertFalse(findings(words))
        merged = [r(31, function=8), 0, r(31, function=8), 0]
        self.assertIn("unreached_nonpadding_instruction", findings(merged))

    def test_fallthrough_and_nonstandard_register_inputs_require_review(self):
        self.assertIn("control_leaves_span", findings([i(9, 5, 7, 96), i(9, 0, 6, 0)]))
        result = findings([r(8, 9, 2, 0x21), r(31, function=8), 0])
        self.assertIn("nonstandard_entry_register_use", result)

    def test_indirect_jump_and_exception_are_not_guessed(self):
        self.assertIn("unresolved_indirect_jump", findings([r(8, function=8), 0]))
        self.assertIn("exceptional_control_flow", findings([r(function=12), r(31, function=8), 0]))

    def test_review_is_invalidated_by_any_byte_or_extent_change_or_finding(self):
        row = {"address": audit.hx(BASE), "size": 8, "end_exclusive": audit.hx(BASE + 8),
               "instruction_sha256": hashlib.sha256(b"fixture").hexdigest(), "findings": {}}
        review = {**row, "kind": "standalone", "reviewed": True, "evidence": ["Reviewed both edges"]}
        self.assertTrue(audit.review_matches(row, review))
        for change in [{"address": audit.hx(BASE + 4)}, {"size": 12}, {"end_exclusive": audit.hx(BASE + 12)},
                       {"instruction_sha256": "wrong"}, {"findings": {"control_leaves_span": []}}]:
            self.assertFalse(audit.review_matches({**row, **change}, review))
        for change in [{"kind": "multi_entry"}, {"reviewed": False}, {"evidence": []}]:
            self.assertFalse(audit.review_matches(row, {**review, **change}))

    def test_inventory_instruction_bytes_are_checked_independently(self):
        data = r(31, function=8).to_bytes(4, "little") + bytes(4)
        row = {"address": audit.hx(BASE), "size": 8,
               "body": f"/* 0 {BASE:X} {data[:4].hex().upper()} */ jr $ra\n"
                       f"/* 4 {BASE+4:X} 00000000 */ nop\n"}
        self.assertEqual(len(audit.instructions(row, data)[0]), 2)
        with self.assertRaises(RuntimeError):
            audit.instructions(row, bytes(8))

    def test_ghidra_export_from_another_payload_is_rejected(self):
        temporary = audit.ROOT / "tools/tmp"
        temporary.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=temporary) as directory:
            path = Path(directory)
            (path / "program.tsv").write_text(
                "program\tlanguage\tpayload_start\tpayload_size\tpayload_sha256\n"
                "fixture\tPSX:LE:32:default\t80010000\t8\twrong\n")
            with self.assertRaisesRegex(RuntimeError, "pinned initialized EXE payload"):
                audit.ghidra_evidence(path, bytes(0x808))

    def test_coverage_includes_noncode_words_omitted_by_function_parser(self):
        data = r(31, function=8).to_bytes(4, "little") + bytes(4)
        row = {"body": f"/* 800 {BASE:X} {data[:4].hex().upper()} */ jr $ra\n"
                       f"/* 804 {BASE+4:X} 00000000 */ nop\n"}
        config = {"segments": [{"type": "code", "start": 0x800, "vram": BASE,
                                 "subsegments": [[0x800, "asm", "unit"], [0x810, "data", "data"]]}]}
        target = bytes(0x800) + data + b"metadata"
        result = audit.coverage_gaps([row], target, config)
        self.assertEqual(result["decoded_instruction_bytes"], 8)
        self.assertEqual(result["gaps"], [{"start": audit.hx(BASE + 8), "end_exclusive": audit.hx(BASE + 16),
            "size": 8, "zero_only": False, "bytes_sha256": hashlib.sha256(b"metadata").hexdigest()}])

    def test_review_export_must_be_present_when_required_and_match_exactly(self):
        temporary = audit.ROOT / "tools/tmp"
        temporary.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=temporary) as directory:
            path = Path(directory) / "reviews.json"
            review = {"regions": {"fixture": {"kind": "standalone"}}}
            with self.assertRaisesRegex(RuntimeError, "missing"):
                audit.check_review_export(review, path, required=True)
            path.write_text(json.dumps(review))
            audit.check_review_export(review, path, required=True)
            path.write_text('{"regions": {}}')
            with self.assertRaisesRegex(RuntimeError, "differs"):
                audit.check_review_export(review, path, required=True)

    def test_partial_or_stale_headless_exports_cannot_pass(self):
        temporary = audit.ROOT / "tools/tmp"
        with tempfile.TemporaryDirectory(dir=temporary) as directory:
            path = Path(directory)
            target = bytes(0x800) + b"payload"
            with self.assertRaisesRegex(RuntimeError, "did not complete"):
                audit.check_export_complete(path, target)
            complete = {"payload_sha256": hashlib.sha256(b"payload").hexdigest()}
            for name in ("program.tsv", "functions.tsv", "references.tsv", "reviewed-units.json"):
                (path / name).write_text(name)
                complete[name] = hashlib.sha256(name.encode()).hexdigest()
            (path / "export-complete.json").write_text(json.dumps(complete))
            audit.check_export_complete(path, target)
            (path / "functions.tsv").write_text("changed")
            with self.assertRaisesRegex(RuntimeError, "changed Ghidra export"):
                audit.check_export_complete(path, target)
            (path / "keep.txt").write_text("preserve unrelated artifacts")
            audit.prepare_ghidra_export(path)
            self.assertFalse((path / "export-complete.json").exists())
            self.assertTrue((path / "keep.txt").exists())


if __name__ == "__main__":
    unittest.main()
