# Cool Boarders 2 reverse engineering

Starting point for a PS1 matching decompilation. This repository contains project-authored C reconstructions, analysis configuration, build/install recipes and documentation. Game images, extracted assets, raw disassembly, BIOS/SDK binaries, downloaded tools and generated tool headers are not committed.

Current state (9 November 2026): the USA boot executable rebuilds byte for byte, with fourteen functions (672 bytes) compiled from C and the remaining code retained as assembly. Both original and rebuilt `SCUS_943.58` have SHA-256 `5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4`. A tested GCC 2.7.2 PSX recipe is pinned, but the original compiler/flags are not uniquely established. Overlays and runtime behavior remain to be validated. See [matching build and compiler evidence](docs/compiler-identification.md).

Project tools live under `tools/`, including the Python environment, downloaded source, build outputs, and bootstrap caches/temporary files. Ghidra is the user-authorized exception: its installation, Java runtime, build cache and application state live in Docker Desktop's storage. Only its small Docker recipe, launcher and version lock remain under `tools/`. The bootstrap uses existing host prerequisites; it does not install system packages or change shell profiles.

## Start here

The current checkout is ready to build:

```sh
make
```

This regenerates assembly, compiles the C functions with the pinned historical compiler and maspsx, links the boot EXE, then checks every byte against the unchanged reference. Output is `build/SCUS_943.58`, with a comparison report at `reports/boot-rebuild.json`. A mismatch makes the command fail. The supplied source images/extraction are already present; the steps below describe setup on a fresh checkout.

### Headless C reconstruction loop

Add or edit a function in `src/`; for a new C unit, replace its reviewed `asm` subsegment with a `c` subsegment in `scus_943.58.yaml`, preserving the original boundaries. Then run:

```sh
make validate        # also the default make / make verify workflow
```

Validation rebuilds the original reference objects and current C objects, runs objdiff in batch mode, prints per-unit scores and changed instructions, and saves full instruction/relocation diffs under `reports/objdiff/`. It then rebuilds and compares the complete executable's size and every byte against the pinned original. A mismatch exits nonzero, with object diagnostics already available for the next edit. Compilation/tool failures also stop validation; old aggregate success reports are removed before starting.

For agent work: inspect `reports/objdiff.json`, the relevant `reports/objdiff/<unit>.json` and `reports/boot-rebuild.json`, adjust the C/types/recipe, and repeat `make validate` until the full executable matches. Objdiff scores are debugging aids; only the complete EXE byte check gates correctness. The fourteen C units cover 672 bytes; retained assembly, SDK signatures and unloaded overlays are excluded from objdiff progress.

`make objdiff` (or `make objdiff-report`) runs just the batch object diagnostics without the final EXE link/check, for faster iteration. `make build/src/queue_call.o` recompiles a single unit. No workflow opens a GUI or an interactive viewer.

For an automatic batch with one fresh agent process per function, run `tools/.venv/bin/python tools/reconstruct.py run --limit 10` from a clean checkout. Python starts each target on a `codex/match-<address>` branch from `main`, launches the existing Codex CLI, and independently validates its candidate. Matching commits fast-forward `main`; blocked attempts remain as WIP checkpoints on their function branches. The runner returns to `main` before the next target. See the [reconstruction worker](docs/reconstruction-worker.md) for time budgets, custom launchers, logs and the manual workflow.

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

Ghidra and the PSX extension run headlessly in Docker. Use `./tools/ghidra.sh headless` to print CLI usage; see [Ghidra Docker setup](docs/ghidra-docker.md) for the saved project, container paths and verification command. The previous unpacked host installation and archives have been removed.

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
