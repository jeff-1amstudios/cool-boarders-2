#!/usr/bin/env python3
"""Install checksum-pinned objdiff binaries locally; never install host packages."""
import hashlib
import json
import platform
import shutil
import subprocess
from pathlib import Path

from install_mips import fetch

TOOLS = Path(__file__).resolve().parent


def main():
    if (platform.system(), platform.machine()) != ("Darwin", "arm64"):
        raise RuntimeError("The objdiff lock currently pins macOS ARM64 binaries only")
    lock = json.loads((TOOLS / "objdiff.lock.json").read_text())
    destination = TOOLS / "objdiff/bin"
    destination.mkdir(parents=True, exist_ok=True)
    (TOOLS / "downloads").mkdir(exist_ok=True)
    for asset in lock["assets"]:
        archive = TOOLS / "downloads" / asset["name"]
        fetch(asset["browser_download_url"], archive, asset["digest"].removeprefix("sha256:"))
        binary = destination / "objdiff-cli"
        if not binary.exists() or hashlib.sha256(binary.read_bytes()).hexdigest() != asset["digest"].removeprefix("sha256:"):
            shutil.copyfile(archive, binary)
        binary.chmod(0o755)
        subprocess.run([binary, "--version"], check=True)
    print(f"objdiff {lock['version']} installed under {destination}")


if __name__ == "__main__":
    main()
