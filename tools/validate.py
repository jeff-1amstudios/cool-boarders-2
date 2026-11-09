#!/usr/bin/env python3
"""Run object diagnostics, then exact full-file checks for boot and all overlays."""
import argparse
import subprocess
import sys

from rebuild import ROOT, run
from overlay_rebuild import clear_reports


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--image", help="Validate only this entire EXE/BIN")
    parser.add_argument("--unit", help="Limit object diagnostics to this unit; still compare the entire image")
    args = parser.parse_args()
    if args.unit and not args.image:
        parser.error("--unit requires --image")
    image_args = ["--image", args.image] if args.image else []
    # Never let a failed compilation leave an old success report for an agent
    # to mistake for the result of the current edit.
    for path in [ROOT / "reports/objdiff.json", ROOT / "reports/boot-rebuild.json"]:
        path.unlink(missing_ok=True)
    clear_reports()
    run([sys.executable, "tools/objdiff.py", *image_args, *(["--unit", args.unit] if args.unit else [])])
    run([ROOT / "tools/objdiff/bin/objdiff-cli", "report", "generate",
         "-o", "reports/objdiff.json"])
    run([sys.executable, "tools/objdiff.py", "--summary"])
    # Diagnostics are already saved if linking or the exact-byte gate fails.
    result = subprocess.run([sys.executable, "tools/rebuild.py"], cwd=ROOT) if args.image in (None, "SCUS_943.58") else None
    overlays = subprocess.run([sys.executable, "tools/overlay_rebuild.py",
                               *(["--overlay", args.image] if args.image else [])], cwd=ROOT) if args.image != "SCUS_943.58" else None
    return (result.returncode if result else 0) or (overlays.returncode if overlays else 0)


if __name__ == "__main__":
    sys.exit(main())
