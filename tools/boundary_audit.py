#!/usr/bin/env python3
"""Conservative PSX boundary diagnostics. Findings are not automatic corrections."""
import argparse
from collections import Counter, defaultdict, deque
import csv
import hashlib
import json
from pathlib import Path
import re

import rabbitizer
import yaml

from rebuild import ROOT, reference

VERSION = 1
INSTRUCTION = re.compile(r"/\* ([0-9A-F]+) ([0-9A-F]+) ([0-9A-F]{8}) \*/\s*(.*)")
CALLEE_SAVED = tuple(range(16, 24)) + (28, 30)
UNKNOWN = ("unknown",)


def hx(address):
    return f"0x{address:08X}"


def signed16(word):
    value = word & 0xFFFF
    return value - 0x10000 if value & 0x8000 else value


def decode(word, pc):
    instruction = rabbitizer.Instruction(word, pc, rabbitizer.InstrCategory.R3000GTE)
    op, rs, rt, rd, function = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, word & 63
    kind, target = "ordinary", None
    if op in (2, 3):
        kind = "call" if op == 3 else "jump"
        target = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
    elif op == 0 and function in (8, 9):
        kind = "return" if function == 8 and rs == 31 else "indirect_jump"
        if function == 9 and rd != 0:
            kind = "indirect_call"
    elif instruction.isBranch():
        kind = "branch"
        target = pc + 4 + (signed16(word) << 2)
        if instruction.isFunctionCall():
            kind = "conditional_call"
    elif op == 0 and function in (12, 13):
        kind = "trap"
    destination = instruction.getDestinationGpr()
    return {"word": word, "pc": pc, "op": op, "rs": rs, "rt": rt, "rd": rd,
            "function": function, "kind": kind, "target": target,
            "valid": instruction.isValid(), "likely": instruction.isBranchLikely(),
            "destination": destination.value if destination is not None else None,
            "reads": [register for field, register in (("Rs", rs), ("Rt", rt), ("Rd", rd))
                      if getattr(instruction, "reads" + field)() and register != 0]}


def instructions(row, reference_bytes):
    result = {}
    raw = bytearray()
    for offset, address, value, _ in INSTRUCTION.findall(row["body"]):
        offset, address = int(offset, 16), int(address, 16)
        data = bytes.fromhex(value)
        if reference_bytes[offset:offset + 4] != data:
            raise RuntimeError(f"Unverified instruction context at {hx(address)}")
        if address in result:
            raise RuntimeError(f"Repeated instruction at {hx(address)}")
        result[address] = decode(int.from_bytes(data, "little"), address)
        raw.extend(data)
    if not result or len(raw) != row["size"]:
        raise RuntimeError(f"Invalid inventory size for {row['address']}")
    return result, hashlib.sha256(raw).hexdigest()


def apply_instruction(instruction, registers, slots, flag):
    """Track entry-register identities and constant stack offsets, not program values.

    ABI-conforming callees are assumed to preserve s0-s7/gp/fp/sp. Only known
    frame-relative slots are tracked: arbitrary pointer aliases and callee memory
    effects are not proved. Exceptional/indirect transfers remain review findings.
    """
    op, rs, rt, rd, fn = (instruction[k] for k in ("op", "rs", "rt", "rd", "function"))
    immediate = signed16(instruction["word"])
    base = registers[rs]
    if op in (0x2B, 0x29, 0x28, 0x2A, 0x2E):  # stores, including partial stores
        if base[0] == "sp":
            address = base[1] + immediate
            width = {0x28: 1, 0x29: 2}.get(op, 4)
            for saved in list(slots):
                if address < saved + 4 and saved < address + width:
                    del slots[saved]
            if op == 0x2B:
                slots[address] = registers[rt]
        return
    destination = instruction["destination"]
    if destination is None or destination == 0:
        return
    value = UNKNOWN
    if op in (8, 9) and base[0] == "sp":
        value = ("sp", base[1] + immediate)
    elif op in (8, 9) and rs == 0 and immediate == 0:
        value = ("zero",)
    elif op == 0 and fn in (0x20, 0x21, 0x25):  # add/addu/or used as moves
        if rt == 0:
            value = registers[rs]
        elif rs == 0:
            value = registers[rt]
    elif op == 0 and fn == 0 and ((instruction["word"] >> 6) & 31) == 0:
        value = registers[rt]
    elif op == 0x23 and base[0] == "sp":
        value = slots.get(base[1] + immediate, UNKNOWN)
    registers[destination] = value
    if destination == 29 and value[0] != "sp":
        flag("dynamic_or_unknown_stack", instruction["pc"])


def check_entry_register_reads(instruction, registers, flag):
    op, rs, rt = (instruction[k] for k in ("op", "rs", "rt"))
    stack_save = op == 0x2B and registers[rs][0] == "sp"
    for register in instruction["reads"]:
        if (register not in (0, 4, 5, 6, 7, 28, 29, 31)
                and registers[register] == ("entry", register)
                and not (stack_save and register == rt)):
            flag("nonstandard_entry_register_use", instruction["pc"])


def step(instruction, registers, slots, pending_load, flag):
    """Respect the R3000's one-instruction integer load delay.

    This is identity/stack analysis, not an emulator. Coprocessor interlocks and
    unaligned load-pair values are deliberately not interpreted as known values.
    """
    op, rs = instruction["op"], instruction["rs"]
    check_entry_register_reads(instruction, registers, flag)
    destination = instruction["destination"]
    old = registers[destination] if destination is not None else None
    apply_instruction(instruction, registers, slots, flag)
    integer_load = op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26)
    coprocessor_load = op in (0x10, 0x12) and rs in (0, 2)
    next_load = None
    if destination not in (None, 0) and (integer_load or coprocessor_load):
        next_load = (destination, registers[destination])
        registers[destination] = old
    if pending_load is not None:
        # A subsequent explicit write to the same register wins. Consecutive
        # loads still commit the previous load before scheduling the new one.
        register, value = pending_load
        if register != destination or next_load is not None:
            registers[register] = value
    return next_load


def analyze_cfg(code, entry, *, max_states=20000):
    findings = defaultdict(set)
    visited, reachable, returns, exits = set(), set(), set(), set()

    def flag(kind, address):
        findings[kind].add(address)

    initial = [("entry", register) for register in range(32)]
    initial[0], initial[29] = ("zero",), ("sp", 0)
    pending = deque([(entry, tuple(initial), (), None)])
    while pending:
        pc, values, saved, pending_load = pending.popleft()
        state = (pc, values, saved, pending_load)
        if state in visited:
            continue
        if len(visited) >= max_states:
            flag("analysis_state_limit", pc)
            break
        visited.add(state)
        if pc not in code:
            flag("control_leaves_span", pc)
            exits.add(pc)
            continue
        instruction = code[pc]
        reachable.add(pc)
        registers, slots = list(values), dict(saved)
        if not instruction["valid"]:
            flag("invalid_instruction", pc)
            continue
        kind = instruction["kind"]
        if kind == "ordinary":
            next_load = step(instruction, registers, slots, pending_load, flag)
            pending.append((pc + 4, tuple(registers), tuple(sorted(slots.items())), next_load))
            continue
        if kind == "trap":
            flag("exceptional_control_flow", pc)
            continue
        if kind == "conditional_call" or instruction["likely"]:
            flag("unsupported_conditional_transfer", pc)
            continue
        check_entry_register_reads(instruction, registers, flag)
        # A call writes its link register before executing its delay slot.
        return_target = registers[instruction["rs"]] if kind == "return" else None
        branch_rs, branch_rt = registers[instruction["rs"]], registers[instruction["rt"]]
        if kind in ("call", "indirect_call"):
            link = 31 if kind == "call" else instruction["rd"]
            registers[link] = ("link", pc + 8)
            if link != 31:
                flag("nonstandard_link_register", pc)
        # Branch decisions and jump-register targets use the old register value
        # when immediately preceded by a load. The delay instruction sees the
        # newly committed value.
        if pending_load is not None:
            register, value = pending_load
            link = 31 if kind == "call" else instruction["rd"] if kind == "indirect_call" else None
            if register != link:
                registers[register] = value
        delay = code.get(pc + 4)
        if delay is None:
            flag("delay_slot_outside_span", pc)
            continue
        reachable.add(pc + 4)
        if not delay["valid"] or delay["kind"] != "ordinary":
            flag("unsupported_delay_slot", pc + 4)
            continue
        next_load = step(delay, registers, slots, None, flag)
        if kind == "return":
            returns.add(pc)
            if return_target != ("entry", 31):
                flag("return_address_not_entry_ra", pc)
            if registers[29] != ("sp", 0):
                flag("stack_not_restored_at_return", pc)
            for register in CALLEE_SAVED:
                if registers[register] != ("entry", register):
                    flag(f"callee_saved_r{register}_not_restored", pc)
            if next_load is not None:
                flag("load_in_return_delay_slot", pc + 4)
            continue
        if kind == "indirect_jump":
            flag("unresolved_indirect_jump", pc)
            continue
        if kind in ("call", "indirect_call"):
            if kind == "indirect_call":
                flag("indirect_call_requires_review", pc)
            # Calls can return; noreturn behavior is not inferred from a name.
            for register in (1, 2, 3, *range(4, 16), 24, 25):
                registers[register] = UNKNOWN
            # The callee executes before the call returns, so a delay-slot load
            # has completed; caller-saved registers remain unknown after it.
            if next_load is not None and next_load[0] not in (1, 2, 3, *range(4, 16), 24, 25):
                registers[next_load[0]] = next_load[1]
            pending.append((pc + 8, tuple(registers), tuple(sorted(slots.items())), None))
            continue
        targets = [instruction["target"]]
        if kind == "branch":
            op, rs, rt = instruction["op"], instruction["rs"], instruction["rt"]
            equal = rs == rt or (branch_rs != UNKNOWN and branch_rs == branch_rt)
            if op == 4 and equal:  # beq of provably equal values
                pass
            elif op == 5 and equal:
                targets = [pc + 8]
            else:
                targets.append(pc + 8)
        for target in targets:
            pending.append((target, tuple(registers), tuple(sorted(slots.items())), next_load))
    if not returns:
        flag("no_resolved_return", entry)
    unreachable = [pc for pc, instruction in code.items() if pc not in reachable and instruction["word"] != 0]
    for pc in unreachable:
        flag("unreached_nonpadding_instruction", pc)
    return {"findings": {k: [hx(a) for a in sorted(v)] for k, v in sorted(findings.items())},
            "reachable_instructions": len(reachable), "return_sites": [hx(a) for a in sorted(returns)],
            "external_exits": [hx(a) for a in sorted(exits)], "states": len(visited)}


def ghidra_evidence(directory, target):
    """Compare only an export whose initialized payload matches this reference."""
    if not (directory / "program.tsv").exists():
        return None
    with (directory / "program.tsv").open() as stream:
        metadata = next(csv.DictReader(stream, delimiter="\t"))
    if (metadata["language"] != "PSX:LE:32:default"
            or int(metadata["payload_size"]) != len(target) - 0x800
            or metadata["payload_sha256"] != hashlib.sha256(target[0x800:]).hexdigest()):
        raise RuntimeError("Ghidra export does not match the pinned initialized EXE payload")
    result = defaultdict(list)
    with (directory / "functions.tsv").open() as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            result[int(row["entry"], 16)].append({"start": hx(int(row["start"], 16)),
                "end_exclusive": hx(int(row["end_inclusive"], 16) + 1),
                "name": row["name"], "source": row["source"]})
    return result


def audit_rows(rows, target, ghidra=None, references=None):
    all_code, codes, digests = {}, {}, {}
    for row in rows:
        start = int(row["address"], 16)
        code, digest = instructions(row, target)
        if set(code) & set(all_code):
            raise RuntimeError("Overlapping instruction spans")
        codes[start], digests[start] = code, digest
        all_code.update({pc: (start, instruction) for pc, instruction in code.items()})
    incoming = defaultdict(list)
    for pc, (owner, instruction) in all_code.items():
        destination = instruction["target"]
        if destination is not None:
            incoming[destination].append({"from": hx(pc), "owner": hx(owner), "kind": instruction["kind"]})
    # Raw aligned words outside decoded instructions are possible pointers, not
    # proven references: integers can happen to equal code addresses.
    pointer_words = defaultdict(list)
    for offset in range(0x800, len(target) - 3, 4):
        source = 0x80010000 + offset - 0x800
        if source not in all_code:
            destination = int.from_bytes(target[offset:offset + 4], "little")
            if destination in all_code:
                pointer_words[destination].append(hx(source))
    result = []
    for row in rows:
        start = int(row["address"], 16)
        code = codes[start]
        cfg = analyze_cfg(code, start)
        if not row["contiguous"]:
            cfg["findings"]["noncontiguous_inventory_span"] = [row["address"]]
        internal_incoming = [{"to": hx(pc), **edge} for pc in code if pc != start
                             for edge in incoming[pc] if edge["owner"] != row["address"]]
        if internal_incoming:
            cfg["findings"]["incoming_transfer_to_interior"] = sorted({e["to"] for e in internal_incoming})
        ranges = ghidra.get(start, []) if ghidra is not None else None
        structural_end = max(code) + 4
        ghidra_comparison = "unavailable"
        if ranges is not None:
            ghidra_comparison = "entry_missing" if not ranges else "different_extent"
            if len(ranges) == 1 and ranges[0]["start"] == hx(start) and ranges[0]["end_exclusive"] == hx(structural_end):
                ghidra_comparison = "same_extent"
        result.append({"address": row["address"], "name": row["name"], "size": row["size"],
                       "end_exclusive": hx(structural_end), "instruction_sha256": digests[start],
                       "status": row["status"], "unit": row["unit"],
                       "legacy_queue_candidate": row["status"] == "assembly" and row["contiguous"]
                           and not row["hardware_or_gp"] and not row.get("hold", False)
                           and row["classification"] not in ("sdk", "handwritten")
                           and row["size"] <= 160 and "indirect-control" not in row["features"],
                       "incoming_entry_transfers": incoming[start], "incoming_interior_transfers": internal_incoming,
                       "possible_pointer_words_to_entry": pointer_words[start],
                       "possible_pointer_words_to_interior": {hx(pc): pointer_words[pc] for pc in code if pc != start and pointer_words[pc]},
                       "ghidra_references_to_entry": (references or {}).get(start, []),
                       "ghidra_comparison": ghidra_comparison, "ghidra_ranges": ranges,
                       **cfg})
    return result


def review_matches(row, review):
    """Explicit review is tied to exact bytes, address, extent and unit kind."""
    return (review.get("kind") == "standalone" and review.get("reviewed") is True
            and review.get("address") == row["address"]
            and bool(review.get("evidence")) and review.get("size") == row["size"]
            and review.get("end_exclusive") == row["end_exclusive"]
            and review.get("instruction_sha256") == row["instruction_sha256"]
            and not row["findings"])


def coverage_gaps(rows, target, config):
    """Account for bytes omitted by the function-label parser as well as code.

    Zero padding and declared data are not functions. A nonzero gap is a review
    item even when Splat emitted it as .word instead of decoded instructions.
    """
    covered = {int(address, 16) for row in rows for _, address, _, _ in INSTRUCTION.findall(row["body"])}
    ranges = []
    for segment in config["segments"]:
        if not isinstance(segment, dict) or segment.get("type") != "code":
            continue
        subsegments = segment.get("subsegments", [])
        bounds = []
        for index, row in enumerate(subsegments[:-1]):
            start = row[0] if isinstance(row, list) else row["start"]
            kind = row[1] if isinstance(row, list) else row["type"]
            if kind not in ("asm", "c"):
                continue
            following = subsegments[index + 1]
            end = following[0] if isinstance(following, list) else following["start"]
            base = segment["vram"] - segment["start"]
            bounds.append((base + start, base + end))
        if bounds:
            ranges.append((min(a for a, _ in bounds), max(b for _, b in bounds)))
    gaps = []
    for low, high in ranges:
        start = None
        for pc in range(low, high + 4, 4):
            missing = pc < high and pc not in covered
            if missing and start is None:
                start = pc
            if not missing and start is not None:
                offset = start - 0x80010000 + 0x800
                data = target[offset:offset + pc - start]
                if len(data) != pc - start:
                    raise RuntimeError("Configured code coverage lies outside reference")
                gaps.append({"start": hx(start), "end_exclusive": hx(pc), "size": len(data),
                             "zero_only": not any(data), "bytes_sha256": hashlib.sha256(data).hexdigest()})
                start = None
    return {"configured_text_ranges": [{"start": hx(a), "end_exclusive": hx(b)} for a, b in ranges],
            "decoded_instruction_bytes": len(covered) * 4,
            "gaps": gaps}


def check_review_export(review, path, required=False):
    if not path.exists():
        if required:
            raise RuntimeError("Reviewed Ghidra metadata export is missing")
        return
    if json.loads(path.read_text()) != review:
        raise RuntimeError("Saved Ghidra review/export differs from config/function-boundaries.json; reconcile deliberately")


def prepare_ghidra_export(directory):
    directory.mkdir(parents=True, exist_ok=True)
    for name in ("export-complete.json", "program.tsv", "functions.tsv", "references.tsv", "reviewed-units.json"):
        (directory / name).unlink(missing_ok=True)


def check_export_complete(directory, target):
    path = directory / "export-complete.json"
    if not path.exists():
        raise RuntimeError("Ghidra export did not complete successfully; inspect its log")
    complete = json.loads(path.read_text())
    if complete.get("payload_sha256") != hashlib.sha256(target[0x800:]).hexdigest():
        raise RuntimeError("Ghidra export completion is for a different payload")
    for name in ("program.tsv", "functions.tsv", "references.tsv", "reviewed-units.json"):
        file = directory / name
        if not file.exists() or complete.get(name) != hashlib.sha256(file.read_bytes()).hexdigest():
            raise RuntimeError(f"Incomplete or changed Ghidra export file: {name}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", type=Path, default=ROOT / "reports/reconstruction/inventory.json")
    parser.add_argument("--require-ghidra", action="store_true", help="Require matching saved Ghidra payload and reviewed-unit export")
    parser.add_argument("--prepare-ghidra-export", action="store_true", help="Remove old generated export files so a failed headless script cannot reuse stale evidence")
    args = parser.parse_args()
    directory = ROOT / "reports/boundary-audit"
    if args.prepare_ghidra_export:
        prepare_ghidra_export(directory / "ghidra")
        return
    _, target, sha = reference()
    inventory = json.loads(args.inventory.read_text())
    if inventory["reference_sha256"] != sha:
        raise RuntimeError("Inventory fingerprint differs from pinned reference")
    if args.require_ghidra:
        check_export_complete(directory / "ghidra", target)
    ghidra = ghidra_evidence(directory / "ghidra", target)
    if args.require_ghidra and ghidra is None:
        raise RuntimeError("Fresh Ghidra boundary export required; run make boundary-audit")
    references = defaultdict(list)
    if ghidra is not None:
        with (directory / "ghidra/references.tsv").open() as stream:
            for row in csv.DictReader(stream, delimiter="\t"):
                try:
                    destination = int(row["to"], 16)
                except ValueError:  # external/address-space references are not RAM addresses
                    continue
                references[destination].append({"from": row["from"], "type": row["type"], "source": row["source"]})
    rows = audit_rows(inventory["functions"], target, ghidra, references)
    review_path = ROOT / "config/function-boundaries.json"
    review = json.loads(review_path.read_text()) if review_path.exists() else {"regions": {}}
    if review.get("reference_sha256", sha) != sha:
        raise RuntimeError("Boundary review is for a different reference")
    reviewed_export = directory / "ghidra/reviewed-units.json"
    check_review_export(review, reviewed_export, args.require_ghidra)
    for row in rows:
        row["review"] = review["regions"].get(row["address"], {})
        row["reviewed_standalone"] = review_matches(row, row["review"])
        row["library_candidate"] = row["review"].get("library_candidate", False)
        row["direct_call_count"] = sum(edge["kind"] == "call" for edge in row["incoming_entry_transfers"])
        row["reference_status"] = ("direct_call_seen" if row["direct_call_count"] else
            "possible_address_reference_seen" if row["possible_pointer_words_to_entry"] or row["ghidra_references_to_entry"]
            else "no_known_static_reference")
    policy = json.loads((ROOT / "config/reconstruction.json").read_text())
    coverage = coverage_gaps(inventory["functions"], target, yaml.safe_load((ROOT / "scus_943.58.yaml").read_text()))
    for gap in coverage["gaps"]:
        reviewed = review["regions"].get(gap["start"], {})
        gap["reviewed_data"] = (reviewed.get("kind") == "embedded_data"
            and reviewed.get("reviewed") is True and reviewed.get("size") == gap["size"]
            and reviewed.get("end_exclusive") == gap["end_exclusive"]
            and reviewed.get("bytes_sha256") == gap["bytes_sha256"])
    report = {"audit_version": VERSION, "reference_sha256": sha,
              "scope": "Boot EXE inventory only. Diagnostics under normal o32 call/return assumptions; no automatic boundary edits or proof of original source language. Unresolved control flow remains flagged.",
              "summary": {"spans": len(rows), "spans_with_findings": sum(bool(r["findings"]) for r in rows),
                          "legacy_queue_candidates": sum(r["legacy_queue_candidate"] for r in rows),
                          "legacy_queue_candidates_with_findings": sum(r["legacy_queue_candidate"] and bool(r["findings"]) for r in rows),
                          "reviewed_standalone": sum(r["reviewed_standalone"] for r in rows),
                          "reviewed_standalone_queue_candidates": sum(r["legacy_queue_candidate"] and r["reviewed_standalone"] for r in rows),
                          "unreviewed_legacy_queue_candidates": sum(r["legacy_queue_candidate"] and not r["reviewed_standalone"] for r in rows),
                          "matching_paused": policy.get("matching_paused", False),
                          "reference_status": dict(Counter(r["reference_status"] for r in rows)),
                          "nonzero_coverage_gaps": sum(not g["zero_only"] for g in coverage["gaps"]),
                          "unreviewed_nonzero_coverage_gaps": sum(not g["zero_only"] and not g["reviewed_data"] for g in coverage["gaps"]),
                          "nonlibrary_reviewed_queue_candidates": sum(r["legacy_queue_candidate"] and r["reviewed_standalone"] and not r["library_candidate"] for r in rows),
                          "finding_counts": dict(Counter(k for row in rows for k in row["findings"])),
                          "ghidra_comparison": dict(Counter(r["ghidra_comparison"] for r in rows))},
              "coverage": coverage, "spans": rows}
    directory.mkdir(parents=True, exist_ok=True)
    (directory / "audit.json").write_text(json.dumps(report, indent=2) + "\n")
    lines = ["# Boundary audit diagnostics", "", report["scope"], "", "| Entry | Bytes | State | Findings | Ghidra |", "| --- | ---: | --- | --- | --- |"]
    for row in rows:
        lines.append(f"| {row['address']} | {row['size']} | {row['status']} | {', '.join(row['findings']) or 'none detected'} | {row['ghidra_comparison']} |")
    (directory / "audit.md").write_text("\n".join(lines) + "\n")
    print(json.dumps(report["summary"], indent=2))


if __name__ == "__main__":
    main()
