# Setup and reproduction

The matching target is the USA release, boot executable `SCUS_943.58`, plus 14 known raw code overlays. The pinned boot SHA-256 is `5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4`; image fingerprints are recorded in `config/usa-disc.json` and `config/overlays.json`.

## Prerequisites

Use existing Python 3.13, Git, CMake, make and a C/C++ compiler on macOS ARM64. The current compiler archives target Apple Silicon; other hosts need corresponding compiler builds. Network access is needed for dependency setup.

All installed project tools, sources, build products and download caches stay under `tools/`. Python dependencies use `tools/.venv` and `tools/requirements.lock`. Setup does not install host packages or modify shell profiles. Ghidra and its PSX extension use the existing Docker Desktop installation; see [Ghidra Docker setup](ghidra-docker.md).

## Fresh checkout

1. Obtain a disc dump with its CUE and every BIN track. Keep them under ignored `orig/images/`, with any source archive under `orig/archives/`. See [disc sourcing](disc-source.md).
2. Bootstrap tools using your existing Python:

   ```sh
   PSX_PYTHON=/path/to/python3.13 sh tools/bootstrap.sh
   ```

3. Extract and inventory the disc, adjusting the input path:

   ```sh
   tools/.venv/bin/python tools/disc.py 'orig/images/Cool_Boarders_2_USA/Cool Boarders 2 (USA).cue'
   ```

   Extraction writes `orig/disc/inputs.json`, `manifest.json`, `extraction.log`, `layout.xml` and `files/`. It records track and extracted-file hashes, SYSTEM.CNF and PS-X EXE header fields. Recorded hashes are not independent dump verification. The command preserves the input image and refuses to overwrite an existing output directory; use a fresh `--out` for retries.

4. Install the local matching toolchain and validate:

   ```sh
   make toolchain
   make
   ```

   Binutils builds under `tools/toolchain/`; candidate compilers install under `tools/compilers/`. The initial build can take several minutes. `make compiler-matrix` repeats compiler/flag experiments; the working recipe is provisional.

## Build outputs and diagnostics

`make` / `make validate` rebuilds and byte-compares all 15 pinned files. The boot output is `build/SCUS_943.58`; reports are `reports/boot-rebuild.json` and `reports/overlay-rebuild.json`. A mismatch fails validation. `make validate IMAGE=SCUS_943.58` checks only the boot executable; `make overlay-validate OVERLAY=init` checks one overlay.

Validation prepares independent reference objects from a fingerprint-checked all-assembly split, checks that reference link against the original, compiles current C and runs the pinned objdiff CLI. Per-unit scores live in `reports/objdiff.json`, with instruction/relocation diffs under `reports/objdiff/`. Object scores guide iteration; only the full-file comparison establishes a match. Old aggregate success reports are removed before validation begins.

`make objdiff` runs object diagnostics alone. For selected-function iteration, use `tools/reconstruction_step.py` as described in the [reconstruction workflow](reconstruction-worker.md). These workflows are headless.

Regenerate the reviewed boot split with:

```sh
tools/.venv/bin/python -m splat split scus_943.58.yaml
```

Generated assembly goes under ignored `asm/`; linker files, extracted C blocks, reference objects and tool-provided macro headers go under ignored `build/`. Preserve the reviewed configuration rather than overwriting it with `splat create_config`.

## Analysis and repository boundaries

Ghidra projects belong under `orig/ghidra/`. Follow the [Docker guide](ghidra-docker.md) to restore or operate analysis, and the [overlay guide](overlays.md) to import overlays and regenerate their inventories. `make overlays` requires the project to be closed in the interactive GUI; `make overlay-report` summarizes completed analysis.

Commit project-authored C, headers, analysis configuration, scripts, documentation and version/checksum locks. Keep game images, extracted assets, raw disassembly, generated outputs and downloaded third-party tools out of Git. The bootstrap pins mkpsxiso and its submodules; FLAC encoding is disabled, while WAV extraction remains available. The objdiff version and digest are pinned in `tools/objdiff.lock.json`.
