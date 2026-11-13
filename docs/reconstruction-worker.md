# Reconstruction batches

```sh
tools/.venv/bin/python tools/reconstruct.py start --limit 2
```

The command blocks, prints each target's output to stdout, finishes that attempt, then moves to the next. Echoed compiler/assembler commands (`+ ...`) are hidden by default; use `--verbose` to show them. Full command echoes remain in the saved logs. `run` is an alias for `start`. Defaults are ten targets, 120 minutes per batch and a hard 900-second timeout per agent. Override with `--limit`, `--minutes` and `--agent-seconds`. `status` shows the current or last batch. Start on `main` with no pending game/source/config edits.

Session input fingerprints are informational snapshots, not stop conditions.
Edits to tracked tools, documentation, `AGENTS.md` and `FUNCTION_AGENTS.md` may
remain in the checkout during a batch. They are excluded from candidate scope
audits and matching commits; commit maintenance separately. Workers still follow
their supplied brief. Source/config scope checks, Git ref/index checks, pinned
reference verification and exact full-file comparisons remain enforced. Git can
still refuse a branch switch that would overwrite pending maintenance.

`start` also resumes a recorded interrupted batch. It preserves completed and
deferred counts, renews the time budget, and uses `--limit` as the total batch
target count (with room for at least the interrupted target). The interrupted
function is retried first from a fresh Ghidra export; its previous logs, candidate
archive and result remain in their original attempt directory. A retry uses a
new directory. Recovery accepts known worker/checkpoint/restore phases on the
recorded branch and commit, with an empty index. Unexpected edits, commits or
integration phases stop for inspection. Clean, linear commits changing only the
supervisor scripts, this guide or README may be made before recovery; game,
compiler and validation inputs must remain unchanged.

Codex usage limits, failed turns and launcher failures stop the entire batch with
a nonzero exit. The supervisor archives/checkpoints the candidate, restores main,
and records an interrupted attempt without deferring the function or selecting
another target. No final rebuild runs on this infrastructure exit. Once access
is available again, run `start` to retry that function and continue the batch.

Batch subprocesses default to `TQDM_DISABLE=1`, so Splat progress bars are disabled
for workers and supervisor validation alike. Warnings, errors and summaries remain
visible. This affects progress rendering only, not splitting or binary checks.
For standalone validation, use `TQDM_DISABLE=1 make validate` for the same output.

The loop is: select target → ask saved Ghidra for current typed pseudocode → run a worker matching only that function → supervisor compares the entire containing file → commit accepted C/analysis record → save recovered types, globals and prototype into Ghidra → decompile the next target. Python owns Git, Ghidra and acceptance. A custom `--agent-command` accepts shell-quoted argv and receives its prompt on stdin without a shell. Docker Desktop must be running, and the saved project must be closed in interactive Ghidra. An unavailable fresh export stops before worker launch and does not defer the target.

SDK/layout declarations are reused by default (`--sync-types` defaults to false). Use `tools/.venv/bin/python tools/reconstruct.py start --limit 2 --sync-types` to refresh all saved program type declarations, including SDK types such as `CdlLOC` and `RECT`, during the first selected function's existing headless export. The cache lives in ignored `reports/reconstruction/ghidra-types.h` with a SHA-256/catalog manifest; subsequent targets and batches reuse it without exporting declarations again. With no cache, workers use authored image headers; there is no implicit refresh. Refresh explicitly when additional Ghidra layouts are needed or the SDK catalog changes. Workers must not launch extra SDK audits/type exports. Fresh function pseudocode, current signatures/global metadata and accepted analysis feedback remain enabled on every attempt.

## Worker instructions

`AGENTS.md` governs repository maintenance and supervisor work, including tool
setup, integration and full-file acceptance. `FUNCTION_AGENTS.md` is the focused
brief for a single-function worker: allowed edits, boundaries, type propagation,
object diagnostics and analysis evidence. A missing dependency is a blocker for
the supervisor, not a worker installation task.

The supervisor embeds `FUNCTION_AGENTS.md` into each archived `agent-prompt.md`,
followed by the selected target's paths, identity and time allowance. The default
Codex launcher uses the invocation-local `--config project_doc_max_bytes=0` so
repository/global `AGENTS.md` discovery does not also inject maintenance guidance.
This leaves sandbox and approval settings unchanged. Custom `--agent-command`
launchers receive the same focused prompt on stdin and must configure their own
automatic instruction loading if they support it. Workers are instructed not to edit the brief.
Changes take effect on the next worker invocation.

## Targets and review

`make target-list` refreshes the complete byte-checked Ghidra backlog under `reports/reconstruction/targets.json`. Selection follows recorded order: boot first, then overlay filename, ascending address. IDs include the image, such as `INIT.BIN:0x8005AB6C`; equal RAM addresses in different overlays remain separate. SDK identifications and curated holds stay excluded. Unknown does not establish game authorship. Splat build spans and Ghidra function instances remain different counts.

Deferrals persist across batches in ignored `reports/reconstruction/deferred.json`, keyed by image-qualified identity with the reason and preserved attempt links. Repeated `start --limit 1` runs move on to other eligible targets. Exhausting the eligible queue stops the batch; it does not automatically retry deferred work. The last batch's deferrals are imported automatically when upgrading from the previous worker. Keep this file to preserve queue progress.

To make deferred work eligible again, run `tools/.venv/bin/python tools/reconstruct.py retry-deferred --target INIT.BIN:0x8005AB6C`, or use `retry-deferred --all`. These commands requeue targets without launching a batch or removing preserved attempts. A later `start` selects eligible targets in the usual recorded order.

The selected target gets `context.json`, `target.s`, `ghidra/target.ghidra.c`, `ghidra/ghidra-types.h` and `agent-prompt.md` in its attempt directory. The fresh decompilation uses existing SDK names, prototypes and structs, plus accepted prior discoveries. Context includes byte fingerprints, stored/inferred prototypes, parameter storage, referenced typed globals, named callees, the containing span, neighbours and bounded caller/callee evidence. The worker starts from that pseudocode and consults assembly to resolve uncertainties and improve the match. Cached type declarations are reference material (and may predate later accepted game types; source headers remain current); authored types belong in guarded source headers.

CFG warnings such as analysis-state limits or exceptional paths are advisory. They do not prove that C reconstruction is impossible and do not automatically skip the target. Factual split problems—noncontiguous bodies, embedded accepted SDK entries, shared interior entries or no containing assembly span—are recorded as blockers. Curated shared/nonstandard regions remain held. The former structural cache and score-plateau watchdog are no longer used.

The broad function inventory and boundary audit remain explicit analysis commands. Each ordinary attempt now launches read-only headless Ghidra to decompile its selected function from the current saved project. Request/output fingerprints and a completion manifest reject failed or stale exports. Every pass verifies the original mapped bytes for all 15 pinned images. No GUI or interactive diff viewer is involved.

## Analysis feedback

Successful worker file-display commands (`cat`, `head`, `tail`, `sed`, `rg`,
`grep`, including shell-wrapped combinations) print their command but omit the
file contents from the default console stream. `--verbose` restores that output.
Failed reads remain visible, as do compiler/object diagnostics and worker
messages. The worker still receives the full tool output, and raw agent events
remain in `agent.log`; this affects only the supervisor's console rendering.

Ghidra console output defaults to WARN/ERROR, including their continuation lines.
Full INFO logs are always retained in each attempt's `ghidra-*.log` files for
diagnosis and the supervisor's save-confirmation check. Add `--ghidra-info` to
`reconstruct.py start` (or `run`, `recover-ghidra`, `replay-ghidra`) to print INFO
output too, for example `tools/.venv/bin/python tools/reconstruct.py start --limit 2 --ghidra-info`.
This filters console output only; Ghidra's analysis and file logging are unchanged.

The worker writes `ghidra-update.json` in its ignored attempt directory with exactly `function_evidence` (a nonempty string describing parameter/return evidence and uncertainty) and `globals` (a list, empty when no global types were recovered). Each global entry has `image`, hexadecimal `address`, `name` and `evidence`; it must correspond to an authored extern and a matching address in the selected image's symbols file. Declare each global separately. Use `SCUS_943.58` for a resident global even when reconstructing an overlay.

Preserve established global data extents. For a referenced NUL-terminated string,
verify its bytes in the pinned image and use an explicit array bound including
the NUL; record that length in the global's evidence. For example, `bu00:` needs
`extern char card_format_device[6]`. Do not infer extra capacity from alignment
padding. An unsized extern array `[]` supplies an element type, not an allocation
size: feedback may attempt a one-element prefix and reject it for truncating an
existing string or larger object. This caused the otherwise matching
`0x80017DE8` and `0x8001827C` candidates to be deferred.

For other arrays, use explicit bounds only when supported by evidence. If the
extent remains unknown or the inferred type describes only a prefix or
overlapping view of a larger saved object, preserve the established declaration
and layout, document the inferred view and uncertainty in `function_evidence`,
and omit a global replacement that would shrink it. Retain the recovered type
evidence even when a replacement is unsafe. Never invent a bound or weaken the
importer's extent/conflict guards to make feedback pass.

After the supervisor's containing-file gate passes, it extracts the selected C prototype, authored headers and extern declarations. A headless planning transaction parses the types and checks that the proposal can be applied, then rolls back. The supervisor commits a replayable authored analysis record under `config/ghidra/` with the matching C. Only after integration does it apply that record transactionally and save Ghidra. The next fresh decompilation therefore sees the accepted types and global field names. A byte match supports one viable reconstruction; the evidence record must preserve remaining semantic uncertainty.

Inline function-pointer types are assigned deterministic signature-based names under
`/CB2Matching/callbacks` during feedback import. Parameter names such as `callback`
can therefore repeat across declarations and batches, including nested callbacks,
without colliding or replacing another signature. Parsing first retains temporary
objects separately; normalization happens before any type resolution. Explicit
function typedefs retain their authored names. The complete proposed type graph is
checked against both the payload and saved analysis before insertion. Completion
reports include the normalized callback count and signature mapping for diagnosis.
Older feedback records use the same normalization when replayed; workers do not
need callback-parameter naming conventions.

Conflicting established layouts, custom function storage and globals overlapping instructions stop automatic feedback for review. The supervisor checks program bytes before and after updates. Failed/nonmatching worker attempts never update the saved project.

If a saved update fails after the C commit was accepted, the batch stops with phase `syncing_ghidra`; another target cannot launch. Resolve the cause and run `tools/.venv/bin/python tools/reconstruct.py recover-ghidra` on the unchanged clean accepted `main` commit. To rebuild accepted analysis on a compatible saved project, use `replay-ghidra` from a clean checkout with no active attempt. Records replay in acceptance order. Originals, SDK archives, generated pseudocode and Ghidra databases are not committed.

## Matching and preservation

The agent uses `tools/.venv/bin/python tools/reconstruction_step.py` for compact selected-object diagnostics. It recompiles the selected C unit, reuses a hash-checked assembly reference when possible, and prints a short diff. Its task ends when that function matches and its feedback proposal is written; it does not run containing-file validation. A score is diagnostic only. The compiler recipe remains provisional and fixed during each attempt. The agent should restore its best candidate before finishing; the hard timeout preserves whatever is actually in the checkout.

Python accepts only a new annotated function block in the image's shared C file (`src/main.c` or `src/overlays/<overlay>.c`), additions to the image header or new local headers, the exact selected asm/c/asm split, and appended symbols in the selected image. Use `// FUNCTION: <image>:0x<ADDRESS>` to begin the function block. Keep all extern declarations together at the top of the C file, before the first annotation, and reuse existing declarations. New externs and local includes may be appended to that shared top block; existing declarations must stay unchanged. Define structs once in guarded headers and reuse existing layouts. New definitions may be appended before the image header’s final `#endif`; existing definitions must stay unchanged. The build extracts that block into generated C and compiles its independent Splat object. Existing blocks and shared declarations, unrelated boundaries, game configuration, SDK policy and previous symbols stay protected by the candidate scope audit. Source checks reject assembly/byte embedding and unsupported source forms. Python runs fresh `make validate IMAGE=<EXE-or-BIN> UNIT=<unit>` and independently compares the **entire containing file** to its pinned unchanged reference. Candidate/staged/committed hashes and prior C coverage are checked. Only then is a matching commit fast-forwarded into `main`.

Startup checks the branch, clean checkout, saved attempt state and reference fingerprints, refreshes the target inventory, and prepares the selected function's context. It does not rebuild or check matching before the first attempt. Plain `make validate` checks the boot EXE and all 14 overlays at batch end. A nonmatching candidate cannot enter `main`. Object diagnostics and full-file failure reports remain available for the next edit. No interactive diff viewer is used.

After successful final all-image validation, the supervisor runs `tools/readme_progress.py`'s updater to refresh the marked progress block in `README.md`. Counts use a fresh target inventory and persistent deferrals, excluding SDK entries and already reconstructed targets. This happens after the worker acceptance checks, so documentation changes cannot interfere with candidate review. Review and commit the README snapshot before starting another batch, which requires a clean checkout. For a manual refresh (including after requeueing deferrals), run `make progress`; it validates all images before updating the snapshot and leaves the README untouched if validation fails.

Each attempt retains its prompt, context, agent/build logs, candidate patch/sources, diagnostics and one `result.json`. A blocked, timed-out or nonmatching attempt gets a clearly marked WIP checkpoint on its function branch; prohibited source forms stay only in the ignored archive, with an empty WIP marker. Python restores and validates `main` before continuing. Retries discard the selected function's old failed branches and recreate its original branch name from current `main`, starting from scratch. WIP commits never enter `main`.

One batch `result.json` records totals, recorded worker tokens, missing usage records and links to attempt results. `reports/reconstruction/state.json` identifies the current batch and any interrupted operation. Old session directories and archives remain untouched. There are no manual-session commands, detached launcher, separate boundary decisions, progress files or structural-cache updates.

Launcher/authentication failures stop after preserving work. Ctrl-C/SIGTERM kills the agent process group, preserves its in-scope candidate and returns to main with the function eligible for retry. Unexpected source/index/ref changes stop with the checkout intact. If Git fails mid-commit/integration, inspect the recorded branch/commit and working tree before starting again; the runner does not guess or reset unexpected changes. Hard-killed worker attempts in known pre-integration phases can be recovered by `start` under the recovery guards above.
