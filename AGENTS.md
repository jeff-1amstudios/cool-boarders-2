# Workspace constraints

Install all project tools only inside this repository's `tools/` directory.

User-authorized exception: Ghidra and its PSX extension may be built and run in the existing Docker Desktop installation. Their image, build cache and named state volume live in Docker-managed storage outside the checkout. Keep Docker recipes/launchers under `tools/` and preserve analysis projects under `orig/ghidra/`. Do not install new host packages for this migration.

- Use `tools/.venv` for Python dependencies and `tools/requirements.lock` for their versions.
- Keep downloaded tool sources/binaries, build products, installer caches, and installer temporary files under `tools/`.
- Reuse existing host prerequisites where available. Do not install global packages, use Homebrew to install tools, write to system application directories, or change shell profiles/PATH persistently.
- If a required dependency cannot be installed locally under `tools/`, explain the limitation before changing the machine outside the project.
- Game images/extractions belong under ignored `orig/`; analysis outputs may live in the project outside `tools/`.

# Matching objective

The primary goal is a matching reconstruction/recompilation of the original PS1 binaries. Gate changes on an exact full-file comparison against the pinned, unchanged reference. Keep retained assembly, matching C, SDK signature matches and unloaded overlays distinct when reporting progress. Compiler recipes are provisional until enough independent functions distinguish versions/flags; libraries and game objects may use different settings. Native PC recompilation is secondary to this matching objective.

# Repository boundary

Commit project-authored C reconstructions, analysis configuration, build/install scripts, documentation and dependency/version locks. Do not commit game images, extracted assets, raw disassembly/code dumps, generated files, or downloaded/vendored third-party tools and binaries. Keep source BIN/CUE under `orig/images/`, source archives under `orig/archives/`, extraction under `orig/disc/`, and analysis projects under `orig/ghidra/`; these are ignored. Regenerate tool-provided macro headers into `build/include/` from the pinned Splat dependency. Audit the staged file list before committing.

# C reconstruction validation loop

Keep one authored C file per image: `src/main.c` for `SCUS_943.58`, and `src/overlays/<overlay>.c` for each overlay (lowercase config stem). Do not create a C file per function. Start each function block with `// FUNCTION: <image>:0x<ADDRESS>`, for example `// FUNCTION: INIT.BIN:0x8005AB6C`. Keep all extern declarations together at the top of the C file, before the first function annotation, and reuse existing declarations. Text before the first annotation is shared. Define structs and unions once in guarded `.h` files, reuse existing types, and include the image header (`main.h` or `<overlay>.h`) from the C file. Do not duplicate struct definitions between function blocks. The build extracts annotated blocks into generated C under `build/` and retains independent Splat object boundaries for matching. Preserve existing blocks when adding a function.

Use a headless edit → validate → inspect → iterate workflow. Add/edit C and update reviewed Splat segment boundaries as needed, then run `make validate IMAGE=<EXE-or-BIN>` for an exact comparison of the entire containing file. Use `tools/reconstruction_step.py` for compact selected-object diagnostics during iteration. Plain `make validate` (also the default `make`) checks all 15 pinned files and is required at batch end. Do not run a pre-batch rebuild/match check before starting reconstruction attempts. Inspect per-unit scores in `reports/objdiff.json`, instruction/relocation diffs in `reports/objdiff/<unit>.json`, and the full-file result in `reports/boot-rebuild.json`. A nonmatching EXE must fail validation while preserving object diagnostics for the next edit. `make objdiff` runs the object diagnostics alone. Do not launch GUI or interactive diff viewers. Only the pinned full-executable byte comparison establishes correctness.
