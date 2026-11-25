# Per-function matching worker

Your only task is to decompile the selected PS1 function as readable matching
C. The supervisor supplies its identity, allowed files, fresh Ghidra export,
reference assembly, compiler recipe and time allowance in the task prompt.
Use existing project tools only. If a tool is unavailable, report the blocker;
do not install dependencies, change tools or perform repository maintenance.

## Scope

- The supplied repository root is your only workspace. Do not search, list or
  read parent directories, sibling checkouts, home directories or other paths
  outside it, including through symlinks. Invoke existing project tools from
  this root; if required task inputs are missing, report the blocker.
- This supplied brief is complete. Do not discover, search for or read
  `AGENTS.md`, `AGENTS.override.md`, `SUPERVISOR.md` or other instruction files,
  including this brief's source file. Do not perform repository onboarding or
  instruction discovery. Begin with the selected context and supplied resume
  manifest, when present, then consult the fresh Ghidra export.
- Work on the existing branch and candidate checkout. Do not switch branches,
  stage, commit, reset, clean, invoke `decomp.py` or spawn agents.
- If `reuse.json` is supplied, its accepted exact/normalized C donor takes
  precedence over an existing deferred candidate or archived snapshot. Start
  over from `reuse-candidate.c`, replacing only the selected function block.
  Keep the persistent branch/history and reconcile declarations, types and
  symbols with current main. Prior source and notes remain reference material.
  Compile the minimally adapted donor before algorithm changes; record the
  donor, replacement reason and first-check outcome in `knowledge.md`.
- Otherwise, if the selected function already exists in the branch's image source, edit it
  directly; that working candidate takes precedence over archived snapshots.
  When it is absent and a resume manifest is supplied, start from its saved function candidate and
  read the prior diagnostics and notes. Carry forward useful shared declarations
  and header additions, reconciling them with current accepted sources, types,
  symbols and boundaries. Never replace whole current files with archived files.
  Fresh Ghidra pseudocode supplies updated SDK names and accepted analysis as
  reference; it must not reset candidate progress. Record a concrete reason for
  discarding prior work (such as changed boundaries or prohibited source forms).
  With no saved candidate, start from the supplied current Ghidra pseudocode.
  Use assembly to resolve uncertainty and refine compiler output.
- Read the selected context, current image header, cached type reference and
  relevant callers, callees and data. Keep reads focused; do not dump whole
  configurations, unrelated sources or full logs.
- Keep tool responses bounded. Search exact files first; use `rg -l` to find
  relevant files before printing bounded excerpts. `rg -m` limits each file,
  not an entire directory. Read only relevant cached type declarations and
  shared externs, rather than all previous function bodies. Use the compact
  step diagnostics first; extract selected symbols/instructions from diff JSON
  with Python instead of displaying the entire JSON. Avoid rereading unchanged
  context, assembly and declarations on each iteration.
- Review both boundaries. Boundary warnings are hints, not proof that matching C
  is impossible. Report factual split blockers and dependency conclusions.
- Do not launch Ghidra, SDK audits, GUI tools or interactive diff viewers.
  Fresh exports and saved analysis are supervisor-owned.

## Authored source

- Keep one authored C file per image. Add only the selected annotated block
  to the supplied image source, starting with `// FUNCTION: <image>:0x<ADDRESS>`.
  If restarting from an accepted donor, replace the existing selected block.
  Use the context's build symbol; the Ghidra analysis name can differ.
- Preserve existing function blocks, shared declarations, layouts and boundaries.
  Append new externs and quoted local includes to the shared top block before
  the first function annotation. Reuse existing declarations.
- Include the image header. Reuse SDK and accepted game types. Define new
  structs/unions once in guarded local headers, adding types before the image
  header's final `#endif`; do not duplicate layouts in function blocks.
  Preserve the complete existing header prefix, including blank lines: insert
  additions immediately before the final `#endif`, after any existing blank line.
- Give only the selected function a C subsegment. Change only its supplied
  split configuration, add new symbols to its supplied symbol file, and add
  headers only under its source directory. Do not change other source files,
  tools, reference bytes, compiler recipe, documentation or queue metadata.
- When the supervisor assigns a layout-repair follow-up, preserve the matching
  C and repair the selected function's generated data ownership. An explicit
  `.rodata` dictionary may carve its table out of retained `data`/`rodata`:
  name it after the selected C unit, set `linker_section_order` to the original
  section (`.data` or `.rodata`), and retain explicit data boundaries before and
  after the table. Establish the extent from pinned bytes and compiler output.
  Preserve these ownership splits on later retries. Full-file validation stays
  with the supervisor; do not edit generated linker scripts or assembly.
- New symbols may be inserted anywhere in the supplied symbol file. Preserve
  existing definitions and their order; do not add duplicate symbol names.
- Splat symbol metadata does not support `type:data`. For data symbols, omit
  `type` when unknown, or use a supported type such as `u32`, `u8` or `char`
  when evidenced. Keep the C extern's type in the image source.
- Inline assembly is allowed only for PSX GTE operations: GTE commands, COP2
  register/memory transfers and pipeline `nop`s. Use literal instruction
  templates with explicit operands/clobbers; keep surrounding logic in C.
  Other inline assembly, embedded instruction bytes, section attributes and
  preprocessor tricks remain prohibited. Only quoted local headers and simple header guards are
  accepted. Initialized arrays require manual review; use uninitialized local
  arrays and explicit element assignments instead.

## Types and analysis evidence

Propagate known types through locals, globals, fields, assignments, initializers,
calls and returns. For example, a destination receiving `file.pos` should first
be tried as `CdlLOC`, with a direct typed assignment. Recheck other uses for
conflicts. Distinguish typed values from intentional conversions, serialization,
byte copies and overlapping views. If a typed form cannot match, explain the
instruction difference and retain the type evidence; a byte match does not prove
that the original type was scalar.

Write the supplied `ghidra-update.json` with exactly:

```json
{"function_evidence": "observed arguments, return behavior and remaining uncertainty", "globals": []}
```

For each newly recovered global type, add an entry with `image`, hexadecimal
`address`, `name` and `evidence`, backed by an authored extern and matching symbol
address. Use `SCUS_943.58` for resident globals even when matching an overlay.
Headers and the selected prototype are imported automatically by the supervisor.
Include supported recovered globals; do not add unrelated or speculative types.

Preserve established data extents. For each referenced NUL-terminated string,
read its bytes from the pinned image and calculate the explicit array bound as
`first NUL file offset - string start file offset + 1` using a script. Do not
count displayed filename characters manually or infer length from a symbol name.
Record the image, start and terminator file offsets, non-NUL byte count and
NUL-inclusive bound in evidence; `bu00:` needs `char[6]`. Apply the measured bound
to both the authored extern and symbol size. Do not infer capacity from alignment
padding or scan beyond the evidenced containing data span; report uncertainty if
a terminator cannot be established.
An unsized `extern ...[]` supplies an element type, not an allocation size;
feedback may attempt a one-element prefix and reject it for truncating saved
data. Use other array bounds only when evidenced. If a type describes only a
prefix/overlapping view of a larger saved object, preserve its established
declaration/layout, explain the inferred view in `function_evidence`, and omit a
global replacement that would shrink it. Never invent bounds or weaken guards.

## Iterate and finish

- Before compiling, read `docs/decomp-knowledge/README.md` and search the relevant
  topic files for the selected image-qualified ID and assembly symptoms. Read
  only applicable entries. Consult prior attempt `knowledge.md` notes for this
  identity as described in the index. Treat hypotheses as experiments, not rules;
  repeat a failed approach only with a stated change in assumptions.
- Maintain `knowledge.md` in the supplied attempt directory as you learn, before
  the time/token limit. Use the index's proposal format: consulted entry IDs,
  genuinely new findings, compact before/after C and MIPS evidence, exact recipe,
  useful failed approaches, remaining uncertainty and next experiment. If nothing
  new was learned, say so. An object match is pending supervisor verification.
  This ignored evidence sidecar is allowed alongside `ghidra-update.json`; do
  not edit curated documentation or label your own findings full-file verified.
- Use `tools/.venv/bin/python tools/decomp_step.py --context <context>`
  for each compile/edit iteration. Inspect its compact diagnostics and the
  selected `object-diff.json` when necessary.
- Do not run unit tests, full-file validation or whole-project objdiff.
  The compiler recipe is fixed for this worker. Object similarity is diagnostic;
  only the supervisor's pinned full-file comparison establishes acceptance.
- Before finishing, review type propagation and write the feedback proposal.
  Once the selected object matches and evidence is recorded, stop.
- Leave candidate edits in place. If time runs out or matching is blocked,
  restore the best candidate and state the concrete remaining mismatch/blocker.
  The supervisor owns full-file checks, commits and transactional Ghidra saves.
