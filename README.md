# Cool Boarders 2 reverse engineering

Starting point for a PS1 matching decompilation. This repository contains project-authored C reconstructions, analysis configuration, build/install recipes and documentation. Game images, extracted assets, raw disassembly, BIOS/SDK binaries, downloaded tools and generated tool headers are not committed.

Current state (10 November 2026): reconstruction has been reset to **zero C functions** for a fresh start from Ghidra's SDK-typed pseudocode. All 15 pinned files rebuild byte for byte from retained assembly/data. Both original and rebuilt `SCUS_943.58` have SHA-256 `5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4`. Previous C bodies, provisional authored layouts, attempt/defer records and matching branches were cleared; the saved Ghidra SDK analysis, original files and reviewed boundaries remain. The GCC 2.7.2 PSX recipe remains provisional; compiler/flags are not uniquely established. See [the reconstruction workflow](docs/reconstruction-worker.md) and [SDK type audit](docs/sdk-types.md).

Project tools live under `tools/`, including the Python environment, downloaded source, build outputs, and bootstrap caches/temporary files. Ghidra is the user-authorized exception: its installation, Java runtime, build cache and application state live in Docker Desktop's storage. Only its small Docker recipe, launcher and version lock remain under `tools/`. The bootstrap uses existing host prerequisites; it does not install system packages or change shell profiles.

## Start here

The current checkout is ready to build:

```sh
make
```

This regenerates assembly, compiles the C functions with the pinned historical compiler and maspsx, links the boot EXE, then checks every byte against the unchanged reference. Output is `build/SCUS_943.58`, with a comparison report at `reports/boot-rebuild.json`. A mismatch makes the command fail. The supplied source images/extraction are already present; the steps below describe setup on a fresh checkout.

### Headless C reconstruction loop

Run `make target-list` to rebuild the complete image-qualified backlog: 3,967 detected instances across 15 images, including 1,047 SDK entries and 2,920 non-SDK instances, with zero covered by C after the reset. Review boundaries when each target is selected; duplicate detection is deferred. See the [worker workflow](docs/reconstruction-worker.md).

Each automatic attempt begins with a fresh headless decompilation from the saved Ghidra project, including current SDK names, types and accepted discoveries. The [boundary audit](docs/function-boundaries.md) records corrected shared entries, embedded data and special ABI regions. Use `make boundary-audit` to refresh that broad analysis. Accepted SDK entries and curated holds remain excluded from the matching queue.

Add or edit functions in `src/main.c` for the boot executable, or `src/overlays/<overlay>.c` for an overlay. Start each function block with `// FUNCTION: <image>:0x<ADDRESS>` (for example `// FUNCTION: SCUS_943.58:0x800121B8`). Keep all extern declarations together at the top, before the first function annotation; define structs once in guarded headers and reuse them. `src/main.h` holds the boot layouts and is included before the first annotation so every extracted function sees them. Text before the first annotation is shared. Do not create per-function source files. The build extracts blocks into generated C and keeps independent objects and diagnostics to preserve matching compiler output. For a new C function, replace its reviewed `asm` subsegment with a `c` subsegment in the image's Splat configuration, preserving the original boundaries. The subsegment name identifies the object, not an authored source file. Then run:

```sh
make validate        # also the default make / make verify workflow
```

Validation rebuilds the original reference objects and current C objects, runs objdiff in batch mode, prints per-unit scores and changed instructions, and saves full instruction/relocation diffs under `reports/objdiff/`. It then rebuilds and compares the complete executable's size and every byte against the pinned original. A mismatch exits nonzero, with object diagnostics already available for the next edit. Compilation/tool failures also stop validation; old aggregate success reports are removed before starting.

For automatic worker iterations, use `tools/.venv/bin/python tools/reconstruction_step.py` on the selected function. Once the object matches, finish the analysis-feedback proposal and stop. The supervisor validates the entire containing file, commits the accepted C and replayable analysis record, then saves recovered types and global typings into Ghidra before decompiling the next target. Objdiff scores are debugging aids; full-file comparison gates acceptance. Retained assembly and SDK identification remain separate from matching C progress.

`make objdiff` (or `make objdiff-report`) runs just the batch object diagnostics without the final EXE link/check, for faster iteration. `make build/src/queue_call.o` recompiles a single unit. No workflow opens a GUI or an interactive viewer.

For an automatic batch with one fresh agent process per function, run `tools/.venv/bin/python tools/reconstruct.py start --limit 10` from clean `main`, with Docker Desktop running and the saved project closed in interactive Ghidra. Python starts each target on a `codex/match-<image>-<address>` branch, launches the existing Codex CLI, and independently validates its candidate. Matching commits fast-forward `main`; blocked attempts remain as WIP checkpoints. See the [reconstruction worker](docs/reconstruction-worker.md) for feedback, time budgets, logs and recovery.

The [objdiff](https://github.com/encounter/objdiff) CLI is pinned to v3.8.3 with its upstream SHA-256 digest in `tools/objdiff.lock.json`. The installer currently supports macOS ARM64 and keeps the binary under `tools/objdiff/bin/`; downloads stay under `tools/downloads/`. No host packages are installed.

Reference objects come from an independent all-assembly Splat split of the fingerprint-checked original, using the reviewed boundaries and symbols. That assembly is linked and compared against the entire original EXE before its objects are used. Generated references live under `build/objdiff/target/`, compiled C objects under `build/src/`, and generated project configuration in ignored `objdiff.json`. New C segments are discovered automatically each validation run.

1. Obtain a disc dump, retaining its CUE and every BIN track. Store them under ignored `orig/images/`, with any original download archive under `orig/archives/`. See [disc sourcing](docs/disc-source.md).
2. Install the local tools, if needed:

   ```sh
   PSX_PYTHON=/opt/homebrew/bin/python3.13 sh tools/bootstrap.sh
   ```

3. Extract and inventory the image:

   ```sh
   tools/.venv/bin/python tools/disc.py 'orig/images/Cool_Boarders_2_USA/Cool Boarders 2 (USA).cue'
   ```

   This writes `orig/disc/inputs.json`, `manifest.json`, `extraction.log`, `layout.xml`, and `files/`. It records track SHA-1/SHA-256, extracted-file hashes, SYSTEM.CNF, and PS-X EXE header fields. It does not modify the image or overwrite an existing output folder. Hash recording is not independent dump verification. Failed extraction leaves its logs; use a fresh `--out` for a retry.

   Install the matching toolchain locally, then build:

   ```sh
   make toolchain
   make
   ```

   This builds binutils under `tools/toolchain/` and installs candidate compilers under `tools/compilers/`. No system packages are installed. The initial toolchain build can take several minutes. Run `make compiler-matrix` to repeat the compiler/flag experiments.

4. The supplied image's boot executable has already been identified and its initial config generated. Repeat the split with:

   ```sh
   tools/.venv/bin/python -m splat split scus_943.58.yaml
   ```

   This writes generated assembly under ignored `asm/`, and linker/dependency files under ignored `build/`. The initial pointer-table/code boundary and startup symbols have been corrected from the generated defaults. Exact output bytes are validated by `make`; some semantic section boundaries still need analysis. To seed a different executable later, use `splat create_config` in a separate working directory so it does not overwrite this reviewed config.

See [tools and workflow](docs/workflow.md) for the milestones and native-port experiment.

Ghidra and the PSX extension run in Docker. For an interactive browser desktop, run `./tools/ghidra.sh web-build` once, then `./tools/ghidra.sh web` and open [Ghidra](http://127.0.0.1:6080/vnc.html?autoconnect=true&resize=scale). Use `./tools/ghidra.sh headless` to print CLI usage; see [Ghidra Docker setup](docs/ghidra-docker.md) for session controls, the saved project, container paths and verification command. The previous unpacked host installation and archives have been removed.

The [overlay workflow](docs/overlays.md) maps all 14 known raw code BINs into the existing Ghidra program as native overlay blocks, applies the PSX plugin, and produces a combined SDK-excluded function inventory. Run `make overlays` with the project closed in the GUI, or `make overlay-report` to summarize the completed analysis.

`make validate` rebuilds and byte-compares the boot EXE and all 14 raw overlay BINs. `make overlay-validate OVERLAY=init` checks one overlay; overlay C units participate in the same headless objdiff workflow. All overlay code currently remains retained assembly. See [overlay matching](docs/overlays.md#matching-overlays) for split configs and reports.

## Working rules

- Select one exact retail revision before assigning addresses or reporting matching progress. Region changes can alter code, timing, and content.
- Establish an unchanged assembly rebuild before replacing functions with C.
- Preserve original addresses and bytes as the reference; document assumptions and overlays separately.
- Track function matching and runtime compatibility independently. Generated native code does not establish a matching decompilation.
- `orig/`, `reports/`, generated assembly/assets, and downloaded tools are ignored by Git. Review any generated C before publishing it.

`tools/requirements.lock` pins the Python environment. `tools/bootstrap.sh` pins mkpsxiso to `a6b11ea86e67c189137ac50a4066044b3fdb0525`; submodules retain upstream's pins. Disc tools are built without FLAC encoding support; WAV extraction remains available.

## Repository contents and reproduction

Commit project-authored `src/`, `config/`, documentation and scripts, plus version/checksum locks. Scripts under `tools/` are our build orchestration; installed third-party code and binaries under that directory are ignored. Splat's tool-provided macro headers are regenerated into ignored `build/include/` during each build. Raw disassembly is regenerated from the supplied executable into ignored `asm/`.

On a fresh checkout with an existing Python 3.13, Git, CMake, make and C/C++ compiler on macOS ARM64: run `PSX_PYTHON=/path/to/python3.13 sh tools/bootstrap.sh`, supply the expected BIN/CUE under `orig/images/`, run the extraction command above, then `make toolchain` and `make`. Network access is needed for dependency setup. The current compiler archives target Apple Silicon; other host platforms need corresponding compiler builds. No disc image or installed tool directory needs to be copied into Git.
# cool-boarders-2
