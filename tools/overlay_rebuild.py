#!/usr/bin/env python3
"""Rebuild raw overlays with Splat and gate each complete BIN against its pinned reference."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import yaml

from rebuild import ROOT, PREFIX, compile_c, run
from overlays import load

SUMMARY = ROOT / "reports/overlay-rebuild.json"
DETAILS = ROOT / "reports/overlay-rebuild"
ASM_FLAGS = ["-EL", "-march=r3000", "-mtune=r3000", "-mabi=32", "-G0", "-no-pad-sections", "-I", "build/include"]


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + "\n")


def slug(name):
    return name.removesuffix(".BIN").lower()


def clear_reports():
    SUMMARY.unlink(missing_ok=True)
    if DETAILS.exists():
        for path in DETAILS.glob("*.json"):
            path.unlink()


def configs():
    catalog, _ = load()
    return catalog["overlays"]


def image_config(name, item):
    path = ROOT / item["build_config"]
    config = yaml.safe_load(path.read_text())
    options = config["options"]
    expected_path = (ROOT / "orig/disc/files" / name).resolve()
    base = (path.parent / options.get("base_path", ".")).resolve()
    if (base / options["target_path"]).resolve() != expected_path:
        raise RuntimeError("Overlay split targets a different reference: " + name)
    segments = [s for s in config["segments"] if isinstance(s, dict)]
    if (not segments or segments[0]["start"] != 0 or segments[0]["vram"] != int(item["load_address"], 16)
            or config["segments"][-1] != [item["size"]]):
        raise RuntimeError("Overlay split changes its pinned load address or file extent: " + name)
    if any(options.get(key) != value for key, value in (("asm_path", "asm"), ("src_path", "src"), ("build_path", "build"))):
        raise RuntimeError("Overlay source/object roots must use the shared repository layout")
    for segment in segments:
        directory = Path(segment.get("dir", ""))
        if directory.parts[:2] != ("overlays", slug(name)) or ".." in directory.parts:
            raise RuntimeError("Overlay source/object directory is not isolated: " + name)
    # Every image must have distinct outputs despite sharing the same virtual addresses.
    expected_dir = "build/overlays/" + slug(name)
    for option in ("ld_script_path", "elf_path", "undefined_funcs_auto_path", "undefined_syms_auto_path"):
        if Path(options[option]).parent.as_posix() != expected_dir:
            raise RuntimeError("Overlay build output is not isolated: " + option)
    return path, config


def c_units(config):
    result = []
    for segment in config["segments"]:
        if not isinstance(segment, dict):
            continue
        for sub in segment.get("subsegments", []):
            kind = sub[1] if isinstance(sub, list) and len(sub) > 1 else sub.get("type") if isinstance(sub, dict) else None
            if kind == "c":
                name = sub[2] if isinstance(sub, list) else sub["name"]
                directory = Path(segment.get("dir", "")) / (sub.get("dir", "") if isinstance(sub, dict) else "")
                result.append(directory / name)
    return result


def split(name, item, *, reference=False):
    path, config = image_config(name, item)
    if reference:
        config = copy.deepcopy(config)
        for segment in config["segments"]:
            if not isinstance(segment, dict):
                continue
            for sub in segment.get("subsegments", []):
                if isinstance(sub, list) and len(sub) > 1 and sub[1] == "c":
                    sub[1] = "asm"
                elif isinstance(sub, dict) and sub.get("type") == "c":
                    sub["type"] = "asm"
        directory = "build/overlays/" + slug(name) + "/reference"
        config["options"].update(base_path=str(ROOT), asm_path=directory + "/asm", build_path=".",
                                 asset_path=directory + "/assets", cache_path=directory + "/splat-cache",
                                 ld_script_path=directory + "/overlay.ld", elf_path=directory + "/overlay.elf",
                                 undefined_funcs_auto_path=directory + "/undefined_funcs.txt",
                                 undefined_syms_auto_path=directory + "/undefined_syms.txt")
        path = ROOT / directory / "splat.yaml"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(yaml.safe_dump(config, sort_keys=False))
    directory = ROOT / "reports/overlay-rebuild" / slug(name)
    directory.mkdir(parents=True, exist_ok=True)
    log = directory / ("reference-split.log" if reference else "split.log")
    with log.open("w") as output:
        subprocess.run([sys.executable, "-m", "splat", "split", str(path)], cwd=ROOT,
                       stdout=output, stderr=subprocess.STDOUT, check=True)
    # Macro headers are generated dependencies of the pinned Splat installation.
    run([sys.executable, "-c", "from pathlib import Path; from splat.util import conf,file_presets; "
         "import sys; conf.load([Path(sys.argv[1])]); file_presets.write_all_files()", path])
    return config


def build_objects(config, recipe, *, reference=False):
    options = config["options"]
    script = ROOT / options["ld_script_path"]
    objects = list(dict.fromkeys(re.findall(r"([^\s()]+\.o)\(", script.read_text())))
    sources = []
    for name in objects:
        path = Path(name)
        (ROOT / path).parent.mkdir(parents=True, exist_ok=True)
        if not reference and path.parts[:2] == ("build", "src"):
            compile_c(name, recipe)
            sources.append(path.relative_to("build").with_suffix(".c").as_posix())
        else:
            source = path.with_suffix(".s") if reference else path.relative_to("build").with_suffix(".s")
            run([str(PREFIX) + "as", *ASM_FLAGS, "-o", name, source])
    return sources


def link(config, output):
    options = config["options"]
    elf = ROOT / options["elf_path"]
    run([str(PREFIX) + "ld", "-EL", "-e", "func_8005AB6C", "-T", options["ld_script_path"],
         "-T", options["undefined_funcs_auto_path"], "-T", options["undefined_syms_auto_path"],
         "-Map", elf.with_suffix(".map"), "-o", elf])
    run([str(PREFIX) + "objcopy", "-O", "binary", elf, output])


def compare(expected, actual):
    offsets = [i for i, (a, b) in enumerate(zip(expected, actual)) if a != b]
    first = (offsets + list(range(min(len(expected), len(actual)),
                                   min(max(len(expected), len(actual)), min(len(expected), len(actual)) + 32))))[:32]
    return {"reference_sha256": hashlib.sha256(expected).hexdigest(),
            "rebuilt_sha256": hashlib.sha256(actual).hexdigest(),
            "reference_size": len(expected), "rebuilt_size": len(actual),
            "identical": expected == actual,
            "different_bytes": len(offsets) + abs(len(expected) - len(actual)),
            "first_different_offsets": [f"0x{i:X}" for i in first]}


def rebuild_image(name, item):
    output = ROOT / "build/overlays" / slug(name) / name
    output.parent.mkdir(parents=True, exist_ok=True)
    output.unlink(missing_ok=True)
    config = split(name, item)
    recipe_path = item.get("compiler_config", "config/compiler.json")
    recipe = json.loads((ROOT / recipe_path).read_text())
    sources = build_objects(config, recipe)
    link(config, output)
    expected = (ROOT / "orig/disc/files" / name).read_bytes()
    # Recheck after the build too; never accept a changed reference.
    if hashlib.sha256(expected).hexdigest() != item["sha256"]:
        raise RuntimeError("Overlay reference changed during build: " + name)
    return {"image": name, "original": "orig/disc/files/" + name,
            "rebuilt": output.relative_to(ROOT).as_posix(), "load_address": item["load_address"],
            **compare(expected, output.read_bytes()), "c_sources": sources,
            "compiler_config": recipe_path,
            "scope": "Full raw BIN, including header word, retained assembly and data. "
                     "Initial code/data boundaries are provisional; SDK recognition is not reconstructed C."}


def rebuild_all(only=None):
    clear_reports()
    catalog = configs()
    selected = {name: item for name, item in catalog.items()
                if only is None or only.lower() in (name.lower(), slug(name))}
    if not selected:
        raise ValueError("Unknown overlay: " + str(only))
    results = []
    for name, item in selected.items():
        try:
            report = rebuild_image(name, item)
        except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as error:
            report = {"image": name, "identical": False, "error": str(error)}
        write(DETAILS / (slug(name) + ".json"), report)
        results.append(report)
        print(f"{name}: {'EXACT' if report['identical'] else 'FAILED'}", flush=True)
    aggregate = {"identical": all(r["identical"] for r in results),
                 "validated_all_overlays": set(selected) == set(catalog),
                 "overlay_count": len(results), "matching_overlays": sum(r["identical"] for r in results),
                 "c_translation_units": sum(len(r.get("c_sources", [])) for r in results), "images": results}
    write(SUMMARY, aggregate)
    return aggregate


def prepare_objdiff_units():
    """Independently rebuild assembly-only references for overlay C diagnostics."""
    units = []
    for name, item in configs().items():
        _, original_config = image_config(name, item)
        names = c_units(original_config)
        if not names:
            continue
        reference = split(name, item, reference=True)
        build_objects(reference, {}, reference=True)
        output = ROOT / "build/overlays" / slug(name) / "reference" / name
        output.unlink(missing_ok=True)
        link(reference, output)
        expected = (ROOT / "orig/disc/files" / name).read_bytes()
        if output.read_bytes() != expected:
            raise RuntimeError("Assembly-only objdiff reference differs from pinned overlay: " + name)
        split(name, item)
        recipe = json.loads((ROOT / item.get("compiler_config", "config/compiler.json")).read_text())
        for unit in names:
            base = Path("build/src") / unit.with_suffix(".o")
            target = Path(reference["options"]["asm_path"]) / unit.with_suffix(".o")
            compile_c(base, recipe)
            units.append({"name": unit.as_posix(), "target_path": target.as_posix(), "base_path": base.as_posix(),
                          "metadata": {"source_path": (Path("src") / unit.with_suffix(".c")).as_posix()}})
    return units


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--overlay", help="Validate a single BIN name or slug; default checks all 14")
    parser.add_argument("--object", help="Compile one build/src/overlays/<image>/<unit>.o for objdiff")
    args = parser.parse_args()
    if args.object:
        path = Path(args.object)
        if len(path.parts) < 5 or path.parts[:3] != ("build", "src", "overlays") or ".." in path.parts or path.suffix != ".o":
            raise ValueError("Not an overlay C object")
        catalog = configs()
        name = next((n for n in catalog if slug(n) == path.parts[3]), None)
        if name is None:
            raise ValueError("Unknown overlay object")
        _, config = image_config(name, catalog[name])
        if path.relative_to("build/src").with_suffix("") not in c_units(config):
            raise ValueError("Object is not a configured overlay C unit")
        compile_c(path, json.loads((ROOT / catalog[name].get("compiler_config", "config/compiler.json")).read_text()))
        return 0
    report = rebuild_all(args.overlay)
    return 0 if report["identical"] else 1


if __name__ == "__main__":
    sys.exit(main())
