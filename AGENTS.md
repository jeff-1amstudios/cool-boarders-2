# Workspace constraints

Install all project tools only inside this repository's `tools/` directory.

User-authorized exception: Ghidra and its PSX extension may be built and run in the existing Docker Desktop installation. Their image, build cache and named state volume live in Docker-managed storage outside the checkout. Keep Docker recipes/launchers under `tools/` and preserve analysis projects under `orig/ghidra/`. Do not install new host packages for this migration.

- Use `tools/.venv` for Python dependencies and `tools/requirements.lock` for their versions.
- Keep downloaded tool sources/binaries, build products, installer caches, and installer temporary files under `tools/`.
- Reuse existing host prerequisites where available. Do not install global packages, use Homebrew to install tools, write to system application directories, or change shell profiles/PATH persistently.
- If a required dependency cannot be installed locally under `tools/`, explain the limitation before changing the machine outside the project.
- Game images/extractions belong under ignored `orig/`; analysis outputs may live in the project outside `tools/`.

# Matching objective

The primary goal is a matching decompilation/recompilation of the original PS1 binaries. Gate changes on an exact full-file comparison against the pinned, unchanged reference. Keep retained assembly, matching C, SDK signature matches and unloaded overlays distinct when reporting progress. Compiler recipes are provisional until enough independent functions distinguish versions/flags; libraries and game objects may use different settings. Native PC recompilation is secondary to this matching objective.

# Repository boundary

Commit project-authored C decompilations, analysis configuration, build/install scripts, documentation and dependency/version locks. Do not commit game images, extracted assets, raw disassembly/code dumps, generated files, or downloaded/vendored third-party tools and binaries. Keep source BIN/CUE under `orig/images/`, source archives under `orig/archives/`, extraction under `orig/disc/`, and analysis projects under `orig/ghidra/`; these are ignored. Regenerate tool-provided macro headers into `build/include/` from the pinned Splat dependency. Audit the staged file list before committing.

# C decompilation validation loop

Inline assembly in C is allowed only for PSX GTE operations: GTE commands, COP2 register/memory transfers and pipeline `nop`s. Use literal instruction templates with explicit operands/clobbers and keep surrounding logic in C. Other inline assembly, embedded instruction bytes, section attributes and preprocessor tricks remain prohibited. The exact full-file comparison remains the acceptance gate.

Keep one authored C file per image: `src/main.c` for `SCUS_943.58`, and `src/overlays/<overlay>.c` for each overlay (lowercase config stem). Do not create a C file per function. Start each function block with `// FUNCTION: <image>:0x<ADDRESS>`, for example `// FUNCTION: INIT.BIN:0x8005AB6C`. Keep all extern declarations together at the top of the C file, before the first function annotation, and reuse existing declarations. Text before the first annotation is shared. Define structs and unions once in guarded `.h` files, reuse existing types, and include the image header (`main.h` or `<overlay>.h`) from the C file. Do not duplicate struct definitions between function blocks. The build extracts annotated blocks into generated C under `build/` and retains independent Splat object boundaries for matching. Preserve existing blocks when adding a function.

Use a headless edit → validate → inspect → iterate workflow. Add/edit C and update reviewed Splat segment boundaries as needed, then run `make validate IMAGE=<EXE-or-BIN>` for an exact comparison of the entire containing file. Use `tools/decomp_step.py` for compact selected-object diagnostics during iteration. Plain `make validate` (also the default `make`) checks all 15 pinned files and is required at batch end when at least one function was accepted. If no functions matched, skip restored-baseline/full-file validation and README progress refresh; preserve attempt diagnostics and knowledge review. Do not run a pre-batch rebuild/match check before starting decomp attempts. Inspect per-unit scores in `reports/objdiff.json`, instruction/relocation diffs in `reports/objdiff/<unit>.json`, and the full-file result in `reports/boot-rebuild.json`. A nonmatching EXE must fail validation while preserving object diagnostics for the next edit. `make objdiff` runs the object diagnostics alone. Do not launch GUI or interactive diff viewers. Only the pinned full-executable byte comparison establishes correctness.

For automatic batches, supply each worker a fresh headless Ghidra decompilation with SDK names/types and current accepted analysis. Keep one persistent branch per function. On retry, update that branch with current main and edit its existing candidate directly in the regular image source; never delete/recreate the branch. Use archived candidates only when the branch has no saved function. Resume its diagnostics/notes, including across batches; use fresh Ghidra output as reference rather than replacing prior work. Reconcile candidate declarations and boundaries with current main, and record concrete reasons for discarding any prior work. Start from Ghidra pseudocode only when no saved candidate exists. The worker iterates only on its selected function with `decomp_step.py`; the supervisor owns containing-file validation, commits and transactional Ghidra feedback. Persist accepted types, global typings and prototypes for subsequent decompilations. Failed candidates must not update the saved analysis.

Do not add or run unit tests unless the user explicitly requests them. Verify changes with focused headless Ghidra runs and the required exact binary comparisons.

# Persistent decompilation knowledge

Before attempting a function, read `docs/decomp-knowledge/README.md`, search the
relevant topics and image-qualified function ID, and consult applicable entries
and prior attempt notes. Do not load the whole knowledge base or repeat recorded
failed experiments unless new evidence changes their assumptions.

After each accepted match, extract genuinely new reusable findings; after each
deferral, preserve useful failed approaches, hypotheses and the next discriminating
experiment. Follow the entry format and evidence rules in that index. Keep topic
files concise and searchable, merge duplicates, and separate verified findings
from hypotheses. Record exact compiler/assembler settings and function references;
object scores alone never establish a verified finding.

Function workers write proposals to their supplied attempt's `knowledge.md`,
including an explicit no-new-insight outcome when appropriate. They do not edit
the curated knowledge base. The supervising Codex session reviews these notes
against attempt results and exact containing-file validation, then updates the
tracked topic files as separate documentation maintenance. Review pending notes
after an automatic batch and before starting another; after interruption, resume
this review next session. Preserve useful deferred notes even when C is rejected;
never import speculative types into saved Ghidra analysis. Knowledge maintenance
does not alter compiler recipes, source-scope audits or binary acceptance gates.
