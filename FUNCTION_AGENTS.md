# Per-function matching worker

Your only task is to reconstruct the selected PS1 function as readable matching
C. The supervisor supplies its identity, allowed files, fresh Ghidra export,
reference assembly, compiler recipe and time allowance in the task prompt.
Use existing project tools only. If a tool is unavailable, report the blocker;
do not install dependencies, change tools or perform repository maintenance.

## Scope

- Work on the existing branch and candidate checkout. Do not switch branches,
  stage, commit, reset, clean, invoke `reconstruct.py` or spawn agents.
- Start from the supplied current Ghidra pseudocode, with SDK names and accepted
  analysis. Use assembly to resolve uncertainty and refine compiler output.
  Do not restore earlier failed candidates or reconstruct from assembly alone.
- Read the selected context, current image header, cached type reference and
  relevant callers, callees and data. Keep reads focused; do not dump whole
  configurations, unrelated sources or full logs.
- Review both boundaries. Boundary warnings are hints, not proof that matching C
  is impossible. Report factual split blockers and dependency conclusions.
- Do not launch Ghidra, SDK audits, GUI tools or interactive diff viewers.
  Fresh exports and saved analysis are supervisor-owned.

## Authored source

- Keep one authored C file per image. Append only the selected annotated block
  to the supplied image source, starting with `// FUNCTION: <image>:0x<ADDRESS>`.
  Use the context's build symbol; the Ghidra analysis name can differ.
- Preserve existing function blocks, shared declarations, layouts and boundaries.
  Append new externs and quoted local includes to the shared top block before
  the first function annotation. Reuse existing declarations.
- Include the image header. Reuse SDK and accepted game types. Define new
  structs/unions once in guarded local headers, adding types before the image
  header's final `#endif`; do not duplicate layouts in function blocks.
- Give only the selected function a C subsegment. Change only its supplied
  split configuration, append symbols to its supplied symbol file, and add
  headers only under its source directory. Do not change other source files,
  tools, reference bytes, compiler recipe, documentation or queue metadata.
- No inline assembly, embedded instruction bytes, section attributes or
  preprocessor tricks. Only quoted local headers and simple header guards are
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

Preserve established data extents. Verify referenced string bytes in the pinned
image and use explicit array bounds including the NUL, recording the lengths in
evidence; `bu00:` needs `char[6]`. Do not infer capacity from alignment padding.
An unsized `extern ...[]` supplies an element type, not an allocation size;
feedback may attempt a one-element prefix and reject it for truncating saved
data. Use other array bounds only when evidenced. If a type describes only a
prefix/overlapping view of a larger saved object, preserve its established
declaration/layout, explain the inferred view in `function_evidence`, and omit a
global replacement that would shrink it. Never invent bounds or weaken guards.

## Iterate and finish

- Use `tools/.venv/bin/python tools/reconstruction_step.py --context <context>`
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
