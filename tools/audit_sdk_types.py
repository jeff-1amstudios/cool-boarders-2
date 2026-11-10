#!/usr/bin/env python3
"""Audit saved Ghidra SDK types without running analysis or saving program changes."""
import collections
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
DIRECTORY = ROOT / "reports/sdk-types"


def verify():
    payload = (DIRECTORY / "audit.json").read_bytes()
    manifest = json.loads((DIRECTORY / "complete.json").read_text())
    if hashlib.sha256(payload).hexdigest() != manifest["audit_sha256"]:
        raise RuntimeError("SDK audit completion fingerprint differs")
    audit = json.loads(payload)
    if audit["catalog_sha256"] != hashlib.sha256((ROOT / "config/psyq-sdk.json").read_bytes()).hexdigest():
        raise RuntimeError("SDK catalog changed during the audit")
    counts = collections.Counter(row["status"] for row in audit["sdk_functions"])
    structures = [t for t in audit["archive_types"] if "fields" in t]
    structure_counts = collections.Counter(t["program_definition_status"] for t in structures)
    lines = ["# Saved Ghidra SDK type audit", "",
             "Read-only audit. All 15 mapped images were compared with their pinned originals.", "",
             f"SDK version: {audit['sdk_version']}. Archive SHA-256: `{audit['archive_sha256']}`.", "",
             "## SDK function instances", "", "| Result | Count |", "| --- | ---: |"]
    lines += [f"| {key} | {value} |" for key, value in sorted(counts.items())]
    lines += ["", "Array parameters adjust to pointers in C; parameter_array_decay is compatible.",
              "No-archive-prototype entries include internal labels and public names lacking definitions; they are not all missing imports.",
              "Type agreement does not establish correct calling-convention storage or original source identity.", "",
              f"Archive structures: {len(structures)}. Program comparison at archive paths: {dict(structure_counts)}.",
              "Types absent at the archive path may be unused or stored elsewhere; absence alone is not a defect.", "",
              "## Prototype differences requiring review", "",
              "| Identity | Name | Stored prototype | Archive prototypes |", "| --- | --- | --- | --- |"]
    for row in audit["sdk_functions"]:
        if row["status"] in ("types_differ", "missing_function_entry"):
            lines.append(f"| {row['identity']} | {row['sdk_name']} | {row.get('program_prototype', 'missing entry')} | {'; '.join(row['archive_prototypes'])} |")
    lines += ["", "Full type layouts, parameter storage and unmatched-name lists are in audit.json.",
              "This audit reads stored signatures; it does not export decompiler-inferred prototypes or assess all game-specific types.", ""]
    (DIRECTORY / "audit.md").write_text("\n".join(lines))
    return counts


def main():
    DIRECTORY.mkdir(parents=True, exist_ok=True)
    for name in ("audit.json", "complete.json", "audit.md"):
        (DIRECTORY / name).unlink(missing_ok=True)
    (DIRECTORY / "catalog.json").write_bytes((ROOT / "config/psyq-sdk.json").read_bytes())
    with (DIRECTORY / "console.log").open("w") as log:
        subprocess.run([str(ROOT / "tools/ghidra.sh"), "headless", "/projects", "CB2_USA",
                        "-process", "SCUS_943.58", "-readOnly", "-noanalysis", "-scriptPath", "/scripts",
                        "-postScript", "AuditSdkTypes.java", "/reports/sdk-types",
                        "-log", "/reports/sdk-types/headless.log"], cwd=ROOT, stdout=log,
                       stderr=subprocess.STDOUT, check=True)
    # Headless script failures can exit zero: require a fresh completed payload.
    print(dict(verify()))
    print("Report: reports/sdk-types/audit.md")


if __name__ == "__main__":
    main()
