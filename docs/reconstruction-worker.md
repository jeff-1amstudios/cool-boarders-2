# Reconstruction batches

```sh
tools/.venv/bin/python tools/reconstruct.py start --limit 2
```

The command blocks, prints each target's output to stdout, finishes that attempt, then moves to the next. Echoed compiler/assembler commands (`+ ...`) are hidden by default; use `--verbose` to show them. Full command echoes remain in the saved logs. `run` is an alias for `start`. Defaults are ten targets, 120 minutes per batch and a hard 900-second timeout per agent. Override with `--limit`, `--minutes` and `--agent-seconds`. `status` shows the current or last batch. Run from clean `main`; do not edit the checkout concurrently.

The loop is: select target → prepare context/branch → run agent → check edits → compare the entire containing file → commit a match or preserve WIP → continue. Python owns Git and acceptance; the agent only edits C, the selected Splat span and appended symbols. A custom `--agent-command` accepts shell-quoted argv and receives its prompt on stdin without a shell.

## Targets and review

`make target-list` refreshes the complete byte-checked Ghidra backlog under `reports/reconstruction/targets.json`. Selection follows recorded order: boot first, then overlay filename, ascending address. IDs include the image, such as `INIT.BIN:0x8005AB6C`; equal RAM addresses in different overlays remain separate. SDK identifications and curated holds stay excluded. Unknown does not establish game authorship. Splat build spans and Ghidra function instances remain different counts.

The selected target gets `context.json`, `target.s` and `agent-prompt.md` in its attempt directory. Context includes byte fingerprints, the containing span, neighbours and bounded caller/callee evidence. The agent reviews boundaries and dependencies and explains any blocker in its final response; no separate review form is required.

CFG warnings such as analysis-state limits or exceptional paths are advisory. They do not prove that C reconstruction is impossible and do not automatically skip the target. Factual split problems—noncontiguous bodies, embedded accepted SDK entries, shared interior entries or no containing assembly span—are recorded as blockers. Curated shared/nonstandard regions remain held. The former structural cache and score-plateau watchdog are no longer used.

Ghidra refreshes and the separate boot build-span inventory belong to the explicit `make boundary-audit` analysis command. Normal batches use the saved, byte-checked exports and do not launch Ghidra.

## Matching and preservation

The agent uses `tools/.venv/bin/python tools/reconstruction_step.py` for compact selected-object diagnostics. It recompiles the selected C unit, reuses a hash-checked assembly reference when possible, and prints a short diff. A score is diagnostic only. The compiler recipe remains provisional and fixed during each attempt. The agent should restore its best candidate before finishing; the hard timeout preserves whatever is actually in the checkout.

Python accepts only a new annotated function block in the image's shared C file (`src/main.c` or `src/overlays/<overlay>.c`), additions to the image header or new local headers, the exact selected asm/c/asm split, and appended symbols in the selected image. Use `// FUNCTION: <image>:0x<ADDRESS>` to begin the function block. Keep all extern declarations together at the top of the C file, before the first annotation, and reuse existing declarations. New externs and local includes may be appended to that shared top block; existing declarations must stay unchanged. Define structs once in guarded headers and reuse existing layouts. New definitions may be appended before the image header’s final `#endif`; existing definitions must stay unchanged. The build extracts that block into generated C and compiles its independent Splat object. Existing blocks and shared declarations, unrelated boundaries, build/reference/compiler inputs, SDK policy and previous symbols stay protected. Source checks reject assembly/byte embedding and unsupported source forms. Python runs fresh `make validate IMAGE=<EXE-or-BIN> UNIT=<unit>` and independently compares the **entire containing file** to its pinned unchanged reference. Candidate/staged/committed hashes and prior C coverage are checked. Only then is a matching commit fast-forwarded into `main`.

Plain `make validate` checks the boot EXE and all 14 overlays at batch start and end. A nonmatching candidate cannot enter `main`. Object diagnostics and full-file failure reports remain available for the next edit. No interactive diff viewer is used.

Each attempt retains its prompt, context, agent/build logs, candidate patch/sources, diagnostics and one `result.json`. A blocked, timed-out or nonmatching attempt gets a clearly marked WIP checkpoint on its function branch; prohibited source forms stay only in the ignored archive, with an empty WIP marker. Python restores and validates `main` before continuing. Retries discard the selected function's old failed branches and recreate its original branch name from current `main`, starting from scratch. WIP commits never enter `main`.

One batch `result.json` records totals, recorded worker tokens, missing usage records and links to attempt results. `reports/reconstruction/state.json` identifies the current batch and any interrupted operation. Old session directories and archives remain untouched. There are no manual-session commands, detached launcher, separate boundary decisions, progress files or structural-cache updates.

Launcher/authentication failures stop after preserving work. Ctrl-C/SIGTERM kills the agent process group, preserves its in-scope candidate and returns to validated main. Unexpected source/index/ref changes stop with the checkout intact. If Git fails mid-commit/integration or the process is hard-killed, inspect the recorded branch/commit and working tree before starting again; the runner does not guess or reset unexpected changes.
