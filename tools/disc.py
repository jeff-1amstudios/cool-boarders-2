#!/usr/bin/env python3
"""Fingerprint and extract a local raw PS1 image without changing its source."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import shlex
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]


def fingerprint(path: Path) -> dict:
    hashes = {name: hashlib.new(name) for name in ("sha1", "sha256")}
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            for digest in hashes.values():
                digest.update(block)
    return {"size": path.stat().st_size, **{k: v.hexdigest() for k, v in hashes.items()}}


def image_files(image: Path) -> list[Path]:
    if image.suffix.lower() != ".cue":
        with image.open("rb") as stream:
            header = stream.read(16)
        if (len(header) < 16 or header[:12] != b"\0" + b"\xff" * 10 + b"\0"
                or header[15] not in (1, 2) or image.stat().st_size % 2352):
            raise ValueError("Expected a raw 2352-byte/sector BIN or CUE. Preserve the original; "
                             "for CHD use chdman extractcd first. Cooked 2048-byte ISOs need a separate extraction path.")
        return [image]
    files = [image]
    for line in image.read_text(encoding="utf-8-sig").splitlines():
        if re.match(r"^\s*FILE\s", line, re.I):
            tokens = shlex.split(line, posix=True)
            if len(tokens) != 3 or tokens[2].upper() != "BINARY":
                raise ValueError("This importer expects CUE FILE entries in BINARY format.")
            track = (image.parent / tokens[1]).resolve()
            if not track.is_file():
                raise ValueError(f"CUE track missing: {track}")
            if track not in files:
                files.append(track)
    if len(files) == 1:
        raise ValueError("No FILE tracks found in CUE.")
    return files


def exe_header(path: Path) -> dict | None:
    with path.open("rb") as stream:
        header = stream.read(0x800)
    if header[:8] != b"PS-X EXE":
        return None
    if len(header) < 0x800:
        raise ValueError(f"Truncated PS-X EXE header: {path}")
    fields = {}
    for name, offset in (("entrypoint", 0x10), ("gp", 0x14), ("load_address", 0x18),
                         ("payload_size", 0x1C), ("bss_address", 0x28),
                         ("bss_size", 0x2C), ("stack_base", 0x30), ("stack_offset", 0x34)):
        fields[name] = struct.unpack_from("<I", header, offset)[0]
    if path.stat().st_size < 0x800 + fields["payload_size"]:
        raise ValueError(f"Truncated PS-X EXE payload: {path}")
    return {k: f"0x{v:08X}" for k, v in fields.items()}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("image", type=Path, help="CUE with all referenced BINs, or raw 2352-byte image")
    parser.add_argument("--out", type=Path, default=ROOT / "orig" / "disc")
    args = parser.parse_args()
    image = args.image.expanduser().resolve(strict=True)
    inputs = image_files(image)
    extractor = ROOT / "tools/build/mkpsxiso/dumpsxiso"
    if not extractor.is_file():
        raise ValueError("Build dumpsxiso first with tools/bootstrap.sh.")
    out = args.out.expanduser().resolve()
    if out.exists():
        raise ValueError(f"Output already exists; choose another --out directory: {out}")
    input_manifest = [{"path": str(p), **fingerprint(p)} for p in inputs]
    out.mkdir(parents=True)
    (out / "inputs.json").write_text(json.dumps(input_manifest, indent=2) + "\n")
    with (out / "extraction.log").open("w") as log:
        subprocess.run([str(extractor), str(image), "-x", str(out / "files"),
                        "-s", str(out / "layout.xml"), "--lba"],
                       cwd=out, stdout=log, stderr=subprocess.STDOUT, check=True)
    inventory = []
    boot_config = []
    for path in sorted((out / "files").rglob("*")):
        if not path.is_file():
            continue
        record = {"path": str(path.relative_to(out / "files")), **fingerprint(path)}
        header = exe_header(path)
        if header is not None:
            record["psx_exe"] = header
        if path.name.upper().split(";")[0] == "SYSTEM.CNF":
            boot_config.append(path.read_text(encoding="ascii", errors="replace"))
        inventory.append(record)
    manifest = {"source": str(image), "system_cnf": boot_config, "files": inventory,
                "note": "Hashes are recorded, not yet independently verified against Redump. "
                        "Raw/compressed overlays may not have PS-X EXE headers."}
    (out / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Extracted {len(inventory)} files. Manifest: {out / 'manifest.json'}")
    for entry in inventory:
        if "psx_exe" in entry:
            print(f"PS-X EXE: {entry['path']}  {entry['psx_exe']}")
    for config in boot_config:
        print(config.strip())


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        print(f"error: {error}", file=sys.stderr)
        sys.exit(1)
