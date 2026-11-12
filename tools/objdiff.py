#!/usr/bin/env python3
"""Generate original-disassembly reference objects and objdiff's project config."""
import argparse
import json
import re
import sys
from itertools import zip_longest
from pathlib import Path

import yaml

from rebuild import ROOT, PREFIX, absolute_symbols_script, compile_c, reference, run
from overlay_rebuild import prepare_objdiff_units
from c_sources import source_path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--summary", action="store_true", help="Summarize the existing batch report")
    parser.add_argument("--image", help="SCUS_943.58 or one overlay BIN")
    parser.add_argument("--unit", help="Prepare only this namespaced C unit")
    args = parser.parse_args()
    if args.unit and not args.image:
        parser.error("--unit requires --image")
    if args.summary:
        report = json.loads((ROOT / "reports/objdiff.json").read_text())
        details = ROOT / "reports/objdiff"
        details.mkdir(exist_ok=True)
        for unit in report.get("units", []):
            measures = unit["measures"]
            print(f"{unit['name']}: {measures.get('fuzzy_match_percent', 0):.2f}% "
                  f"({measures.get('matched_functions', 0)}/{measures.get('total_functions', 0)} functions)")
            output = details / f"{unit['name']}.json"
            output.parent.mkdir(parents=True, exist_ok=True)
            run([ROOT / "tools/objdiff/bin/objdiff-cli", "diff", "-p", ".",
                 "-u", unit["name"], "-o", output, "--format", "json-pretty"])
            diff = json.loads(output.read_text())
            right = {s["name"]: s for s in diff.get("right", {}).get("symbols", [])}
            for symbol in diff.get("left", {}).get("symbols", []):
                if "instructions" not in symbol or symbol.get("match_percent", 100) == 100:
                    continue
                print(f"  {symbol['name']}: original | compiled C", flush=True)
                left_rows = symbol["instructions"]
                right_rows = right.get(symbol["name"], {}).get("instructions", [])
                changed = 0
                for left_row, right_row in zip_longest(left_rows, right_rows, fillvalue={}):
                    left_ins = left_row.get("instruction", {})
                    right_ins = right_row.get("instruction", {})
                    left_text = left_ins.get("formatted", "<missing>")
                    right_text = right_ins.get("formatted", "<missing>")
                    if left_text != right_text:
                        print(f"    +0x{int(left_ins.get('address', right_ins.get('address', 0))):X}: "
                              f"{left_text} | {right_text}")
                        changed += 1
                        if changed == 40:
                            print("    Further instructions are in the JSON diff.")
                            break
            print(f"  Instruction/relocation details: {output.relative_to(ROOT)}")
        measures = report["measures"]
        print(f"C units only: {measures.get('matched_code', 0)}/{measures.get('total_code', 0)} code bytes matching.")
        print("Details: reports/objdiff.json. Run make for full-file boot and overlay checks.")
        return
    units = []
    boot_config = yaml.safe_load((ROOT / "scus_943.58.yaml").read_text())
    boot_has_c = any(isinstance(row, list) and row[1] == 'c'
                     for segment in boot_config['segments'] if isinstance(segment, dict)
                     for row in segment.get('subsegments', []))
    if args.image in (None, "SCUS_943.58") and boot_has_c:
        _, expected, _ = reference()
        directory = ROOT / "build/objdiff"
        directory.mkdir(parents=True, exist_ok=True)
        (ROOT / "reports").mkdir(exist_ok=True)
        # Split independently of reconstructed C, preserving the reviewed boundaries
        # and symbols. Never obtain target bytes from the current compiled objects.
        config = yaml.safe_load((ROOT / "scus_943.58.yaml").read_text())
        names = []
        for segment in config["segments"]:
            if isinstance(segment, dict):
                for subsegment in segment.get("subsegments", []):
                    if isinstance(subsegment, list) and subsegment[1] == "c":
                        names.append(subsegment[2])
                        subsegment[1] = "asm"
        config["options"].update(
            base_path=str(ROOT), asm_path="build/objdiff/target", build_path=".",
            asset_path="build/objdiff/assets", cache_path="build/objdiff/splat-cache",
            ld_script_path="build/objdiff/reference.ld", ld_dependencies=False,
            elf_path="build/objdiff/reference.elf",
            undefined_funcs_auto_path="build/objdiff/undefined_funcs.txt",
            undefined_syms_auto_path="build/objdiff/undefined_syms.txt",
        )
        split_config = directory / "splat.yaml"
        split_config.write_text(yaml.safe_dump(config, sort_keys=False))
        run([sys.executable, "-m", "splat", "split", split_config])
        run([sys.executable, "-c",
             "from pathlib import Path; from splat.util import conf,file_presets; "
             "conf.load([Path('build/objdiff/splat.yaml')]); file_presets.write_all_files()"])
        script = directory / "reference.ld"
        objects = list(dict.fromkeys(re.findall(r"([^\s()]+\.o)\(", script.read_text())))
        for name in objects:
            run([str(PREFIX) + "as", "-EL", "-march=r3000", "-mtune=r3000", "-mabi=32",
                 "-G0", "-no-pad-sections", "-I", "build/include", "-o", name,
                 Path(name).with_suffix(".s")])
        run([str(PREFIX) + "ld", "-EL", "-e", "_start", "-T", script,
             "-T", absolute_symbols_script(),
             "-T", "config/sdk/resident.ld",
             "-T", directory / "undefined_funcs.txt", "-T", directory / "undefined_syms.txt",
             "-o", directory / "reference.elf"])
        binary = directory / "SCUS_943.58"
        run([str(PREFIX) + "objcopy", "-O", "binary", directory / "reference.elf", binary])
        if binary.read_bytes() != expected:
            raise RuntimeError("Original-disassembly objdiff reference rebuild differs from the pinned EXE")
        recipe = json.loads((ROOT / "config/compiler.json").read_text())
        for name in names:
            if args.unit and name != args.unit:
                continue
            base = f"build/src/{name}.o"
            compile_c(base, recipe)
            units.append({"name": name, "target_path": f"build/objdiff/target/{name}.o",
                          "base_path": base, "metadata": {"source_path": str(source_path(name))}})
    if args.image != "SCUS_943.58":
        units.extend(prepare_objdiff_units(args.image, args.unit))
    if args.unit and not any(u["name"] == args.unit for u in units):
        raise RuntimeError("Selected C unit is not configured in the selected image")
    project = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/v3.8.3/config.schema.json",
        "min_version": "3.8.3", "custom_make": "make",
        "build_target": False, "build_base": True,
        "watch_patterns": ["*.c", "*.h", "config/compiler.json", "config/overlays/**", "config/overlays.json",
                           "tools/rebuild.py", "tools/overlay_rebuild.py", "tools/c_sources.py", "Makefile"],
        "ignore_patterns": ["build/**", "orig/**", "asm/**", "reports/**", "tools/vendor/**",
                            "tools/.venv/**", "tools/cache/**", "tools/tmp/**"],
        "units": units,
    }
    (ROOT / "objdiff.json").write_text(json.dumps(project, indent=2) + "\n")
    print(f"Prepared {len(units)} boot/overlay C units; assembly-only references match their pinned files.")
    print("objdiff scores are debugging aids. Run make for full-file boot and overlay correctness checks.")


if __name__ == "__main__":
    main()
