#!/usr/bin/env python3
"""Prepare byte-checked overlay analysis inputs and summarize PSX plugin results."""
import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import uuid
import shutil

from rebuild import ROOT, reference

CONFIG = ROOT / "config/overlays.json"
DIRECTORY = ROOT / "reports/overlays"


def sha(data):
    return hashlib.sha256(data).hexdigest()


def load():
    config = json.loads(CONFIG.read_text())
    _, boot, fingerprint = reference()
    if fingerprint != config["boot_sha256"]:
        raise RuntimeError("Overlay configuration differs from the pinned boot executable")
    actual = {p.name for p in (ROOT / "orig/disc/files").glob("*.BIN")}
    if actual != set(config["overlays"]):
        raise RuntimeError("Root BIN inventory differs from the reviewed overlay catalog")
    for name, item in config["overlays"].items():
        data = (ROOT / "orig/disc/files" / name).read_bytes()
        if len(data) != item["size"] or sha(data) != item["sha256"]:
            raise RuntimeError("Changed overlay reference: " + name)
    destination = struct.unpack_from("<I", boot, 0x830)[0]
    end_table = Counter(struct.unpack_from("<14I", boot, 0x8B0))
    ends = Counter((int(item["load_address"], 16) + item["size"] + 3) & ~3
                   for item in config["overlays"].values())
    if any(int(item["load_address"], 16) != destination for item in config["overlays"].values()) or ends != end_table:
        raise RuntimeError("Overlay mappings disagree with the boot destination/end tables")
    return config, boot


def prepare():
    config, boot = load()
    DIRECTORY.mkdir(parents=True, exist_ok=True)
    programs = {"SCUS_943.58": {"load_address": "0x80010000", "size": len(boot) - 2048,
                               "container_source": "/inputs/files/SCUS_943.58", "sha256": sha(boot)}}
    for name, item in config["overlays"].items():
        programs[name] = {**item, "container_source": "/inputs/files/" + name}
    (DIRECTORY / "plan.json").write_text(json.dumps({"run_id": str(uuid.uuid4()), "programs": programs}, indent=2) + "\n")
    print(f"Prepared {len(config['overlays'])} native overlay mappings; original files unchanged", flush=True)


def analyze():
    prepare()
    project = ROOT / "orig/ghidra/CB2_USA"
    if project.with_suffix(".lock").exists():
        raise RuntimeError("Close the CB2_USA project in Ghidra before importing overlays")
    backup = ROOT / "orig/ghidra/backups/before-overlay-import"
    if not backup.exists():
        backup.mkdir(parents=True)
        shutil.copy2(project.with_suffix(".gpr"), backup / "CB2_USA.gpr")
        shutil.copytree(project.with_suffix(".rep"), backup / "CB2_USA.rep")
        hashes = {p.relative_to(backup).as_posix(): sha(p.read_bytes()) for p in backup.rglob("*") if p.is_file()}
        (backup / "hashes.json").write_text(json.dumps(hashes, indent=2) + "\n")
    print("Boot project backup: " + str(backup), flush=True)
    launcher = ROOT / "tools/ghidra-docker.sh"
    subprocess.run([str(launcher), "headless", "/projects", "CB2_USA", "-process", "SCUS_943.58",
                    "-noanalysis", "-scriptPath", "/scripts", "-postScript", "ImportOverlays.java",
                    "/reports/overlays", "-max-cpu", "4"], check=True, cwd=ROOT)
    summarize()


def summarize():
    config, _ = load()
    plan = json.loads((DIRECTORY / "plan.json").read_text())
    references = json.loads((DIRECTORY / "references.json").read_text())
    if references["run_id"] != plan["run_id"] or references["local_targets_in_default_space"]:
        raise RuntimeError("Missing current overlay reference audit or incorrectly scoped local targets")
    names = ["SCUS_943.58", *config["overlays"]]
    programs, instances = [], []
    for name in names:
        report = json.loads((DIRECTORY / (name + ".json")).read_text())
        expected = config["boot_sha256"] if name == "SCUS_943.58" else config["overlays"][name]["sha256"]
        if (report["source_sha256"] != expected or report["analysis_timed_out"] or report["run_id"] != plan["run_id"]
                or report["native_overlay"] != (name != "SCUS_943.58")):
            raise RuntimeError("Stale or incomplete Ghidra analysis: " + name)
        entries = report.pop("entries")
        if any(not entry["body_inside_file"] for entry in entries):
            raise RuntimeError("Function crosses file scope: " + name)
        instances.extend(entries)
        programs.append(report)
    groups = defaultdict(list)
    for entry in instances:
        if entry["classification"] != "sdk":
            groups[entry["body_sha256"]].append(entry["identity"])
    root = ROOT / "orig/disc/files"
    files = [p for p in root.rglob("*") if p.is_file()]
    executables = []
    referenced = set()
    for path in files:
        with path.open("rb") as stream:
            if stream.read(8) == b"PS-X EXE":
                executables.append(path.relative_to(root).as_posix())
        if path.name in names:
            referenced.update(value.decode("ascii") for value in re.findall(rb"[A-Z_0-9]+\.BIN", path.read_bytes()))
    report = {"scope": "Resident boot plus all 14 known raw root BIN overlays; Ghidra estimates, not reviewed source-function boundaries. "
                       "Distinct overlay/address identities retained. Identical body hashes group bytes only, not proven equivalent behavior. "
                       "Reconstruction and full-file build status are reported separately by target_queue.py and overlay_rebuild.py; opaque asset containers may require further format analysis.",
              "project": "CB2_USA", "program": "SCUS_943.58",
              "summary": {"programs": 1, "code_images": len(programs), "overlays": len(programs) - 1,
                          "function_instances": len(instances),
                          "sdk_function_instances": sum(p["sdk_functions"] for p in programs),
                          "non_sdk_function_instances": len(instances) - sum(p["sdk_functions"] for p in programs),
                          "non_sdk_exact_body_groups": len(groups)},
              "disc_coverage": {"files_checked": len(files), "psx_executables": sorted(executables),
                                "referenced_raw_binaries": sorted(referenced),
                                "unmapped_bin_references": sorted(referenced - set(config["overlays"]))},
              "reference_audit": {key: references[key] for key in
                                  ("same_overlay_flow_references", "resident_flow_references",
                                   "local_targets_in_default_space", "boot_to_overlay_region")},
              "images": programs, "non_sdk_exact_body_groups": dict(groups), "entries": instances}
    (DIRECTORY / "inventory.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report["summary"], indent=2))
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=["prepare", "analyze", "report"])
    args = parser.parse_args()
    {"prepare": prepare, "analyze": analyze, "report": summarize}[args.mode]()


if __name__ == "__main__":
    main()
