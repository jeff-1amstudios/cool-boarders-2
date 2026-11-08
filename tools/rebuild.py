#!/usr/bin/env python3
"""Split, compile/assemble, link and byte-compare the USA boot executable."""
from pathlib import Path
import hashlib
import json
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
PREFIX = ROOT / "tools/toolchain/bin/mipsel-linux-gnu-"


def run(arguments, *, input_text=None, output_path=None):
    print("+", " ".join(map(str, arguments)), flush=True)
    if output_path is None:
        subprocess.run(list(map(str, arguments)), cwd=ROOT, check=True, input=input_text, text=True)
    else:
        with Path(output_path).open("w") as output:
            subprocess.run(list(map(str, arguments)), cwd=ROOT, check=True,
                           input=input_text, stdout=output, text=True)


def main():
    original = ROOT / "orig/disc/files/SCUS_943.58"
    target = original.read_bytes()
    expected = json.loads((ROOT / "config/usa-disc.json").read_text())["boot_executable"]["sha256"]
    if hashlib.sha256(target).hexdigest() != expected:
        raise RuntimeError("Reference executable differs from the pinned disc manifest")
    if not Path(str(PREFIX) + "as").is_file():
        raise RuntimeError("Missing local MIPS toolchain; run tools/install_mips.py first")
    run([sys.executable, "-m", "splat", "split", "scus_943.58.yaml"])
    # Obtain tool-provided macro headers from the pinned Splat installation;
    # they are generated dependencies, not files to vendor in the repository.
    run([sys.executable, "-c",
         "from pathlib import Path; from splat.util import conf,file_presets; "
         "conf.load([Path('scus_943.58.yaml')]); file_presets.write_all_files()"])
    script = ROOT / "build/scus_943.58.ld"
    objects = list(dict.fromkeys(re.findall(r"\b(build/[^\s()]+\.o)\(", script.read_text())))
    recipe = json.loads((ROOT / "config/compiler.json").read_text())
    compiler = ROOT / "tools/compilers" / recipe["compiler"]
    c_sources = []
    for name in objects:
        relative = Path(name).relative_to("build")
        (ROOT / name).parent.mkdir(parents=True, exist_ok=True)
        if relative.parts[0] == "src":
            source = relative.with_suffix(".c")
            preprocessed = ROOT / Path(name).with_suffix(".i")
            psyq = ROOT / Path(name).with_suffix(".psyq.s")
            gas = ROOT / Path(name).with_suffix(".gas.s")
            run([compiler / "cpp", "-P", "-undef", "-nostdinc", source, preprocessed])
            run([compiler / "cc1", "-quiet", *recipe["cc1_flags"], "-o", psyq, preprocessed])
            run([sys.executable, ROOT / "tools/vendor/maspsx/maspsx.py", *recipe["maspsx_flags"], "-G0"],
                input_text=psyq.read_text(), output_path=gas)
            run([str(PREFIX) + "as", *recipe["as_flags"], "-o", name, gas])
            c_sources.append(str(source))
        else:
            source = relative.with_suffix(".s")
            run([str(PREFIX) + "as", "-EL", "-march=r3000", "-mtune=r3000", "-mabi=32",
                 "-G0", "-no-pad-sections", "-I", "build/include", "-o", name, source])
    run([str(PREFIX) + "ld", "-EL", "-e", "_start", "-T", script,
         "-T", "build/undefined_funcs_auto.txt", "-T", "build/undefined_syms_auto.txt",
         "-Map", "build/scus_943.58.map", "-o", "build/scus_943.58.elf"])
    rebuilt = ROOT / "build/SCUS_943.58"
    run([str(PREFIX) + "objcopy", "-O", "binary", "build/scus_943.58.elf", rebuilt])
    if rebuilt.stat().st_size > len(target) * 2:
        raise RuntimeError("Unexpected binary size; inspect linker LMAs before comparing")
    actual = rebuilt.read_bytes()
    differences = [i for i in range(min(len(target), len(actual))) if target[i] != actual[i]]
    difference_count = len(differences) + abs(len(target) - len(actual))
    report = {
        "original": str(original.relative_to(ROOT)), "rebuilt": str(rebuilt.relative_to(ROOT)),
        "reference_sha256": expected, "rebuilt_sha256": hashlib.sha256(actual).hexdigest(),
        "reference_size": len(target), "rebuilt_size": len(actual),
        "identical": target == actual, "different_bytes": difference_count,
        "first_different_offsets": [f"0x{x:X}" for x in differences[:32]],
        "c_sources": c_sources,
        "compiler_recipe": recipe,
        "scope": f"Whole boot executable with {len(c_sources)} C translation units; remaining executable code retained as assembly",
    }
    (ROOT / "reports").mkdir(exist_ok=True)
    (ROOT / "reports/boot-rebuild.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    if target != actual:
        sys.exit(1)


if __name__ == "__main__":
    main()
