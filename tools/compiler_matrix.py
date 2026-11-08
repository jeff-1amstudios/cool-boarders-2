#!/usr/bin/env python3
"""Compare isolated C probes using pinned historical compilers and ASPSX modes."""
from pathlib import Path
import concurrent.futures
import hashlib
import itertools
import json
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
PREFIX = str(ROOT / "tools/toolchain/bin/mipsel-linux-gnu-")
CONFIG = json.loads((ROOT / "config/compiler-probes.json").read_text())
REFERENCE = (ROOT / "orig/disc/files/SCUS_943.58").read_bytes()


def command(args, log, input_text=None):
    result = subprocess.run(list(map(str, args)), cwd=ROOT, input=input_text,
                            text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    log.write("+ " + " ".join(map(str, args)) + "\n" + result.stderr)
    if result.returncode:
        raise RuntimeError(result.stderr[-1500:])
    return result.stdout


def trial(compiler_name, optimization, small_data, aspsx, probe):
    name = f"{compiler_name}/{optimization}_G{small_data}_as{aspsx}/{probe['name']}"
    directory = ROOT / "build/compiler-probes" / name
    directory.mkdir(parents=True, exist_ok=True)
    compiler = ROOT / "tools/compilers" / compiler_name
    source = ROOT / "src" / (probe["name"] + ".c")
    address = int(probe["address"], 16)
    expected = REFERENCE[address - 0x80010000 + 0x800:address - 0x80010000 + 0x800 + probe["size"]]
    record = {"compiler": compiler_name, "optimization": optimization, "G": small_data,
              "aspsx": aspsx, "probe": probe["name"], "address": probe["address"],
              "expected_size": probe["size"], "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest()}
    with (directory / "commands.log").open("w") as log:
        try:
            preprocessed = directory / "probe.i"
            command([compiler / "cpp", "-P", "-undef", "-nostdinc", source, preprocessed], log)
            raw = directory / "probe.psyq.s"
            command([compiler / "cc1", "-quiet", "-" + optimization, f"-G{small_data}",
                     "-mips1", "-o", raw, preprocessed], log)
            assembly = raw.read_text()
            # Historical .extern size metadata describes small-data eligibility.
            # maspsx currently discovers this from .comm rather than .extern.
            # Fixed-address --defsym equivalents below resolve these test symbols;
            # no common data is copied into the game or the compared .text bytes.
            if small_data:
                assembly = re.sub(r"(?m)^\s*\.extern\s+([\w]+),\s*(\d+)\s*$",
                                  lambda match: f"\t.comm\t{match[1]},{match[2]}" if int(match[2]) > 0 else match[0], assembly)
            translated = command([sys.executable, ROOT / "tools/vendor/maspsx/maspsx.py",
                                  "--aspsx-version", aspsx, "--use-comm-section", f"-G{small_data}"], log, assembly)
            gas = directory / "probe.gas.s"
            gas.write_text(translated)
            obj = directory / "probe.o"
            command([PREFIX + "as", "-EL", "-march=r3000", "-mtune=r3000", "-mabi=32",
                     "-O1", "-G0", "-no-pad-sections", "-o", obj, gas], log)
            linker = directory / "probe.ld"
            definitions = "\n".join(f"{key} = {value};" for key, value in CONFIG["external_symbols"].items())
            linker.write_text(definitions + f"\n_gp = 0x80050274;\nSECTIONS {{ .text 0x{address:X} : SUBALIGN(4) {{ *(.text) }} /DISCARD/ : {{ *(*) }} }}\n")
            elf = directory / "probe.elf"
            command([PREFIX + "ld", "-EL", "-T", linker, "-o", elf, obj], log)
            binary = directory / "probe.bin"
            command([PREFIX + "objcopy", "-O", "binary", "-j", ".text", elf, binary], log)
            actual = binary.read_bytes()
            record.update({"status": "compared", "actual_size": len(actual), "exact": actual == expected,
                           "different_bytes": sum(a != b for a, b in zip(expected, actual)) + abs(len(expected) - len(actual))})
        except RuntimeError as error:
            record.update({"status": "failed", "exact": False, "error": str(error)})
    return record


def main():
    expected_sha = json.loads((ROOT / "config/usa-disc.json").read_text())["boot_executable"]["sha256"]
    if hashlib.sha256(REFERENCE).hexdigest() != expected_sha:
        raise RuntimeError("Reference executable fingerprint changed")
    compilers = ["gcc-2.7.2-psx", "gcc-2.7.2-cdk", "gcc-2.8.1-psx"]
    jobs = list(itertools.product(compilers, ["O0", "O1", "O2", "O3"], [0, 8],
                                  ["2.21", "2.34", "2.56"], CONFIG["probes"]))
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
        records = list(pool.map(lambda job: trial(*job), jobs))
    scores = []
    for compiler, optimization, small_data, aspsx in itertools.product(compilers, ["O0", "O1", "O2", "O3"], [0, 8], ["2.21", "2.34", "2.56"]):
        group = [r for r in records if (r["compiler"], r["optimization"], r["G"], r["aspsx"]) == (compiler, optimization, small_data, aspsx)]
        scores.append({"compiler": compiler, "optimization": optimization, "G": small_data, "aspsx": aspsx,
                       "exact_probes": [r["probe"] for r in group if r["exact"]],
                       "failed_probes": sum(r["status"] == "failed" for r in group)})
    scores.sort(key=lambda item: -len(item["exact_probes"]))
    report = {"reference_sha256": expected_sha, "trials": len(records),
              "scope": "Empirical evidence from proposed source reconstructions; ambiguous matches do not establish the original compiler or flags",
              "flags": "cc1 -mips1, GNU as -O1 -G0 -no-pad-sections; maspsx G matches tested cc1 G",
              "small_data_note": "G8 trials translate .extern size hints to .comm for maspsx. Results depend on the assumed external declarations.",
              "scores": scores, "results": records}
    (ROOT / "reports/compiler-matrix.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"Completed {len(records)} compiler/flag/assembler/probe trials")
    for score in scores[:12]:
        print(score)
    print("Report: reports/compiler-matrix.json")


if __name__ == "__main__":
    main()
