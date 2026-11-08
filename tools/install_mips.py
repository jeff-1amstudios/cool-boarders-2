#!/usr/bin/env python3
"""Install/build the pinned matching toolchain under tools/, without host packages."""
from pathlib import Path
import hashlib
import json
import os
import subprocess
import tarfile
import urllib.request

TOOLS = Path(__file__).resolve().parent


def fetch(url, destination, expected):
    if not destination.exists():
        partial = destination.with_suffix(destination.suffix + ".part")
        request = urllib.request.Request(url, headers={"User-Agent": "coolboarders2-mips-setup"})
        print(f"Downloading {destination.name}", flush=True)
        with urllib.request.urlopen(request, timeout=120) as source, partial.open("wb") as target:
            for block in iter(lambda: source.read(1024 * 1024), b""):
                target.write(block)
        if hashlib.sha256(partial.read_bytes()).hexdigest() != expected:
            raise RuntimeError(f"Download checksum mismatch: {partial}")
        partial.rename(destination)
    if hashlib.sha256(destination.read_bytes()).hexdigest() != expected:
        raise RuntimeError(f"Archive checksum mismatch: {destination}")


def main():
    for directory in ["downloads", "vendor", "build", "cache", "tmp", "compilers"]:
        (TOOLS / directory).mkdir(exist_ok=True)
    lock = json.loads((TOOLS / "binutils.lock.json").read_text())
    archive = TOOLS / "downloads" / f"binutils-{lock['version']}.tar.xz"
    fetch(lock["url"], archive, lock["sha256"])
    source = TOOLS / "vendor" / f"binutils-{lock['version']}"
    if not (source / "configure").exists():
        with tarfile.open(archive) as unpack:
            unpack.extractall(TOOLS / "vendor", filter="data")
    zutil = source / "zlib/zutil.h"
    original = "#if defined(MACOS) || defined(TARGET_OS_MAC)"
    fixed = "#if defined(MACOS) || (defined(TARGET_OS_MAC) && !defined(__APPLE__))"
    if original in zutil.read_text():
        subprocess.run(["patch", "-p1", "-i", str(TOOLS / "binutils-macos.patch")], cwd=source, check=True)
    if fixed not in zutil.read_text():
        raise RuntimeError("Unexpected bundled zlib source; inspect the macOS patch")
    compilers = json.loads((TOOLS / "compilers.lock.json").read_text())
    for asset in compilers["assets"]:
        archive = TOOLS / "downloads" / asset["name"]
        fetch(asset["browser_download_url"], archive, asset["digest"].removeprefix("sha256:"))
        destination = TOOLS / "compilers" / asset["name"].removesuffix("-macos.tar.gz")
        if not (destination / "cc1").exists():
            destination.mkdir(exist_ok=True)
            with tarfile.open(archive) as unpack:
                unpack.extractall(destination, filter="data")
    maspsx = TOOLS / "vendor/maspsx"
    commit = compilers["maspsx_commit"]
    if not (maspsx / ".git").exists():
        subprocess.run(["git", "clone", "--no-checkout", "https://github.com/mkst/maspsx.git", str(maspsx)], check=True)
        subprocess.run(["git", "checkout", commit], cwd=maspsx, check=True)
    if subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=maspsx, text=True).strip() != commit:
        raise RuntimeError("maspsx revision differs from the pinned version")
    prefix = TOOLS / "toolchain"
    assembler = prefix / "bin/mipsel-linux-gnu-as"
    linker = prefix / "bin/mipsel-linux-gnu-ld"
    if not assembler.is_file() or not linker.is_file():
        build = TOOLS / "build/binutils"
        build.mkdir(parents=True, exist_ok=True)
        environment = os.environ.copy()
        environment.update(TMPDIR=str(TOOLS / "tmp"),
                           CLANG_MODULE_CACHE_PATH=str(TOOLS / "cache/clang"),
                           CFLAGS="-O2 -std=gnu17", CXXFLAGS="-O2 -std=c++17")
        commands = [
            [source / "configure", "--target=" + lock["target"], "--prefix=" + str(prefix),
             "--disable-nls", "--disable-werror", "--disable-gprofng", "--disable-gdb",
             "--disable-sim", "--disable-shared", "--without-debuginfod", "--without-zstd", "MAKEINFO=true"],
            ["make", "-j4", "all-binutils", "all-gas", "all-ld", "MAKEINFO=true"],
            ["make", "install-binutils", "install-gas", "install-ld", "MAKEINFO=true"],
        ]
        with (TOOLS / "cache/mips-toolchain-setup.log").open("w") as log:
            for command in commands:
                print("Running:", " ".join(map(str, command)), flush=True)
                subprocess.run(list(map(str, command)), cwd=build, env=environment,
                               stdout=log, stderr=subprocess.STDOUT, check=True)
    version = subprocess.check_output([str(assembler), "--version"], text=True).splitlines()[0]
    if lock["version"] not in version:
        raise RuntimeError(f"Unexpected assembler version: {version}")
    print(version)
    print("Pinned compilers and maspsx ready under tools/; run make to rebuild and compare.")


if __name__ == "__main__":
    main()
