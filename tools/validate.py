#!/usr/bin/env python3
"""Run headless object diagnostics before the mandatory full-EXE comparison."""
import subprocess
import sys

from rebuild import ROOT, run


def main():
    # Never let a failed compilation leave an old success report for an agent
    # to mistake for the result of the current edit.
    for path in [ROOT / "reports/objdiff.json", ROOT / "reports/boot-rebuild.json"]:
        path.unlink(missing_ok=True)
    run([sys.executable, "tools/objdiff.py"])
    run([ROOT / "tools/objdiff/bin/objdiff-cli", "report", "generate",
         "-o", "reports/objdiff.json"])
    run([sys.executable, "tools/objdiff.py", "--summary"])
    # Diagnostics are already saved if linking or the exact-byte gate fails.
    result = subprocess.run([sys.executable, "tools/rebuild.py"], cwd=ROOT)
    return result.returncode


if __name__ == "__main__":
    sys.exit(main())
