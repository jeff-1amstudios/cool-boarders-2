# Decomp batches

The workflow namespace is `decomp`: `tools/decomp.py`, the `decomp_*` helper
modules, `config/decomp.json`, `DecompGhidra.java`, `CB2_DECOMP_CONTEXT`, and
`reports/decomp/`. Existing queue and navigational metadata move with the report
directory. Archived logs, candidate-source snapshots and accepted Ghidra feedback
records retain their original contents, including historical command spellings.

```sh
tools/.venv/bin/python tools/decomp.py start --limit 2
```

The command blocks, prints each target's output to stdout, finishes that attempt, then moves to the next. Echoed compiler/assembler commands (`+ ...`) are hidden by default; use `--verbose` to show them. Full command echoes remain in the saved logs. `run` is an alias for `start`. Defaults are ten targets and a hard 900-second timeout per agent. Override with `--limit` and `--agent-seconds`. There is no batch time budget or `--minutes` option. `status` shows the current or last batch. Start on `main` with no pending game/source/config edits.

The agent timeout measures only the worker subprocess, including its own tool
calls. Supervisor Ghidra export, validation and feedback run separately with
their own command timeouts. Each function worker receives its full allowance;
Ghidra export, validation and feedback do not consume it.

The default Codex launcher also stops each attempt at **100,000 uncached input
plus output tokens**. Set `--agent-tokens N` to change the threshold or
`--agent-tokens 0` to use the time limit alone. The worker stops on the first
Codex usage update at or above the threshold. An individual model response can
take it past the threshold before usage is reported. The supervisor archives the
observed usage and checks any candidate already left on disk through the usual
validation and feedback gates. A retry gets a fresh worker budget. Custom
`--agent-command` launchers use the time limit and have no live token budget.

Candidate C and header files have trailing blank lines removed after source
scope review, before the whitespace check and content hashing. Other whitespace
errors still fail review. Exact full-file validation remains required.

Session input fingerprints are informational snapshots, not stop conditions.
Edits to tracked tools, documentation, `AGENTS.md` and `FUNCTION_AGENTS.md` may
remain in the checkout during a batch. They are excluded from candidate scope
audits and matching commits; commit maintenance separately. Workers still follow
their supplied brief. Source/config scope checks, Git ref/index checks, pinned
reference verification and exact full-file comparisons remain enforced. Git can
still refuse a branch switch that would overwrite pending maintenance.

Each `start` invocation creates a new batch with fresh counters and its own
`--limit`. An interrupted function is retried first, resuming its saved candidate with a fresh Ghidra export as reference.
For example, stopping at target 2/50 and starting with `--limit 10` makes that
function target 1/10. Earlier attempts retain their original logs, candidate
archives and results. Recovery archives the preceding state before creating
the new batch. Recovery accepts known worker/checkpoint/restore phases on the
recorded branch and commit, with an empty index. Unexpected edits, commits or
integration phases stop for inspection. Committed updates to main may be made
before recovery, including accepted source and tooling changes. Recovery accepts
them on main when it descends from the recorded baseline, with no pending game
edits, staged changes or unresolved merge. It archives the interrupted attempt,
then reselects from fresh inventory and refreshes donor/Ghidra inputs. Rewritten
history and updates to main while still on a function branch require inspection.
Pinned-reference checks and the full-file acceptance gate remain enforced.

When Codex reports a usage limit, the supervisor archives the attempt and
restores main. Every 30 minutes it retries the same function with a fresh
Ghidra export. The retry doubles as the availability check. The new worker
must resume its existing branch candidate (or archived edits when the branch has none), and the batch keeps its completed and
deferred counts. Ctrl-C during the wait leaves the batch available for `start`
to resume.

Other failed turns and launcher failures still exit with a nonzero status
after archiving the attempt and restoring main. Run `start` after resolving
the failure to retry that function and continue the batch.

Batch subprocesses default to `TQDM_DISABLE=1`, so Splat progress bars are disabled
for workers and supervisor validation alike. Warnings, errors and summaries remain
visible. This affects progress rendering only, not splitting or binary checks.
For standalone validation, use `TQDM_DISABLE=1 make validate` for the same output.

The loop is: select target → ask saved Ghidra for current typed pseudocode → run a worker matching only that function → supervisor compares the entire containing file → commit accepted C/analysis record → save recovered types, globals and prototype into Ghidra → decompile the next target. Python owns Git, Ghidra and acceptance. A custom `--agent-command` accepts shell-quoted argv and receives its prompt on stdin without a shell. Docker Desktop must be running, and the saved project must be closed in interactive Ghidra. An unavailable fresh export stops before worker launch and does not defer the target.

SDK/layout declarations are reused by default (`--sync-types` defaults to false). Use `tools/.venv/bin/python tools/decomp.py start --limit 2 --sync-types` to refresh all saved program type declarations, including SDK types such as `CdlLOC` and `RECT`, during the first selected function's existing headless export. The cache lives in ignored `reports/decomp/ghidra-types.h` with a SHA-256/catalog manifest; subsequent targets and batches reuse it without exporting declarations again. With no cache, workers use authored image headers; there is no implicit refresh. Refresh explicitly when additional Ghidra layouts are needed or the SDK catalog changes. Workers must not launch extra SDK audits/type exports. Fresh function pseudocode, current signatures/global metadata and accepted analysis feedback remain enabled on every attempt.

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
The launcher also pins the working directory to the repository root and supplies the brief as
invocation-local `developer_instructions`. Workers must not manually discover or
read instruction files, or search/list/read outside this checkout (including
through symlinks). Disabling automatic loading alone does not prevent manual
shell searches. These are worker instruction boundaries; `workspace-write` is
not a repository-only read sandbox. `AGENTS.md` remains available to supervisors.
Custom `--agent-command` launchers receive the same focused prompt on stdin and
must configure their own working root, developer instructions and automatic
instruction loading if supported. Workers are instructed not to edit the brief.
Changes take effect on the next worker invocation.

The default launcher sends only the selected task on stdin: the brief is already
in developer instructions. The archived prompt retains both for review. Custom
launchers continue to receive the complete brief and task on stdin.

## Persistent matching knowledge

Before an attempt, consult the [knowledge index](decomp-knowledge/README.md)
and only the relevant topic entries and prior notes. The worker brief and launch
prompt require this lookup and an incremental `<attempt>/knowledge.md` proposal,
including a no-new-insight outcome when appropriate. These ignored sidecars
survive rejection, branch restoration and timeout alongside the attempt evidence.
They do not enter matching commits or saved Ghidra analysis.

After a batch, the supervising Codex session reviews the new sidecars against
recorded outcomes and full-file comparisons, consolidates reusable findings in
the tracked topic files, and labels unresolved observations as hypotheses.
Record the review disposition in each sidecar; resume pending reviews after an
interruption. The Python runner archives evidence but does not perform semantic
curation. This documentation review is separate maintenance and leaves the
existing source checks, exact acceptance gate and transactional feedback intact.

## Run a selected list

Use `--functions` to retry functions directly, including persistent deferrals,
in the supplied order. Prefer image-qualified identities because overlays can
share addresses. Function names and addresses are accepted when unambiguous.
Pasted multiline CSV rows in `IMAGE:0xADDRESS,function_name` format are also
accepted. Quote the complete multiline value. The identity column selects the
function and the name column serves as a label. Duplicates run once.

```sh
tools/.venv/bin/python tools/decomp.py start --model gpt-6.1-sol --effort medium \
  --functions 'BP.BIN:0x8007F1BC,func_8007F1BC
BP.BIN:0x8007F1BC,func_8007F1BC
BP.BIN:0x8007F2A8,func_8007F2A8'
```

```sh
tools/.venv/bin/python tools/decomp.py start \
  --functions BP.BIN:0x800864A4,BP.BIN:0x80086634,BP.BIN:0x80086B3C \
  --model gpt-6.1-sol --effort medium
```

The default limit covers the full supplied list. An explicit `--limit` caps the
number of attempts in this invocation. Repeating the same list continues its
remaining functions after an interruption or a partial run. The interrupted
function is retried first and counters start at 1 for the new invocation.
After the entire list finishes, submitting it again retries its deferred entries.
Already matched C is skipped. SDK entries, held functions, unknown entries and
ambiguous names/addresses are rejected before starting the new batch.

List runs preserve the ordinary backlog and other deferrals. A later invocation
without `--functions` continues the ordinary eligible queue. Successful retries
clear their persistent deferral only after full-file acceptance and Ghidra
feedback. Every invocation retains its own command, model, effort and usage rows.

## Targets and review

Shared C declarations may gain standalone extern declarations and local includes
between existing lines. Existing shared lines must remain unchanged; changing a
prototype, helper body or existing header layout still requires manual review.
This permits grouping new externs with related declarations without requiring
them to be appended at the end of the shared text. Exact full-file validation
remains the acceptance gate.

`make target-list` refreshes the complete byte-checked Ghidra backlog under `reports/decomp/targets.json`. Selection follows recorded order: boot first, then overlay filename, ascending address. IDs include the image, such as `INIT.BIN:0x8005AB6C`; equal RAM addresses in different overlays remain separate. SDK identifications and curated holds stay excluded. Unknown does not establish game authorship. Splat build spans and Ghidra function instances remain different counts.

Deferrals persist across batches in ignored `reports/decomp/deferred.json`, keyed by image-qualified identity with the reason and preserved attempt links. Repeated `start --limit 1` runs move on to other eligible targets. Exhausting the eligible queue stops the batch; it does not automatically retry deferred work. The last batch's deferrals are imported automatically when upgrading from the previous worker. Keep this file to preserve queue progress.

To make deferred work eligible again, run `tools/.venv/bin/python tools/decomp.py retry-deferred --target INIT.BIN:0x8005AB6C`, or use `retry-deferred --all`. These commands requeue targets without launching a batch or removing preserved attempts. A later `start` selects eligible targets in the usual recorded order.

The selected target gets `context.json`, `target.s`, `ghidra/target.ghidra.c`, `ghidra/ghidra-types.h` and `agent-prompt.md` in its attempt directory. The fresh decompilation uses existing SDK names, prototypes and structs, plus accepted prior discoveries. Context includes byte fingerprints, stored/inferred prototypes, parameter storage, referenced typed globals, named callees, the containing span, neighbours and bounded caller/callee evidence. On a retry, the launcher finds the latest archived candidate for the exact image-qualified identity across batches, skipping attempts with no saved function. It supplies the selected annotated block as `resume-candidate.c` and a `resume.json` linking prior source snapshots, diagnostics, notes and body fingerprints. The worker resumes the function already present on its persistent branch; the archive supplies prior diagnostics and notes. If the branch has no function candidate, the saved annotated block is the recovery starting point. Reconcile declarations, headers, symbols and splits with current main, and use fresh pseudocode as updated reference. Whole archived image files are never restored over current sources. Discarding prior work requires a concrete reason in `knowledge.md`. An accepted cross-overlay C donor now takes precedence over both branch and archived deferred candidates: the worker starts over from `reuse-candidate.c`, preserves the old block as `superseded-candidate.c` when present, and records the donor/replacement reason and first-check result. `reuse.json` supplies exact/normalized fingerprint provenance, compiler settings and address hints; `reuse-donor.c` and `reuse-headers.json` supply declarations and types for reconciliation. Donors are chosen on clean integration main from current accepted C, exact before normalized, and the minimally adapted donor is compiled before algorithm changes. With no accepted donor or saved candidate, the worker starts from fresh pseudocode. See [accepted C reuse](overlay-duplicates.md#accepted-c-reuse-in-automatic-attempts). Assembly resolves uncertainties and guides matching. Cached type declarations are reference material (and may predate later accepted game types; source headers remain current); authored types belong in guarded source headers.

CFG warnings such as analysis-state limits or exceptional paths are advisory. They do not prove that C decomp is impossible and do not automatically skip the target. Factual split problems—noncontiguous bodies, embedded accepted SDK entries, shared interior entries or no containing assembly span—are recorded as blockers. Curated shared/nonstandard regions remain held. The former structural cache and score-plateau watchdog are no longer used.

The broad function inventory and boundary audit remain explicit analysis commands. Each ordinary attempt now launches read-only headless Ghidra to decompile its selected function from the current saved project. Request/output fingerprints and a completion manifest reject failed or stale exports. Every pass verifies the original mapped bytes for all 15 pinned images. No GUI or interactive diff viewer is involved.

## Recover preserved 100% candidates

Reuse a deferred attempt's preserved candidate without launching another model worker:

```sh
tools/.venv/bin/python tools/recover_deferred.py run \
  --target SCUS_943.58:0x8001B59C \
  --target SCUS_943.58:0x8001B64C
```

Each target requires a preserved 100% object report, current byte-checked
inventory and boundary review, and a fresh read-only Ghidra export. Recovery
merges the archived annotated block, externs and header additions into current
sources. It preserves existing separator whitespace, reuses symbols established
at the same address, and derives splits from current boundaries. It does not
replay old source files or neighbouring split changes. Conflicting declarations,
established header edits, ambiguous aliases and nonmatching files stop for manual
review. The object score only selects candidates for recovery.

Recovery runs the existing source/split checks, exact containing-file validation,
a rollback-only feedback plan, an audited matching commit, transactional Ghidra
saving and a confirming fresh export. Only then is the deferral removed. Each
target is accepted before the next export. `run` finishes with plain
`make validate` for all 15 images. Documentation/tool edits remain outside
matching commits.

Resumable state and artifacts live under
`reports/decomp/recoveries/<image-qualified-slug>/`; original sessions
remain unchanged. Repeating `run` resumes a prepared, checked or committed
recovery. For inspection between stages, use `prepare`, `check` and `accept`
with one `--target`; `sync` retries a save after an accepted commit. A partial
preparation without `recovery.json` needs manual inspection; authored-file
rollback inputs, if edits began, are archived in `before.json`. Use a fresh
`--directory` after resolving a partial preparation. After phased recovery,
run plain `make validate` at batch end if any functions were accepted. Skip final validation and README progress refresh when none matched. Refresh README progress only after validation
succeeds, and commit maintenance separately.

## Analysis feedback

Successful worker file-display commands (`cat`, `head`, `tail`, `sed`, `rg`,
`grep`, including shell-wrapped combinations) print their command but omit the
file contents from the default console stream. `--verbose` restores that output.
Failed reads remain visible, as do compiler/object diagnostics and worker
messages. The worker still receives the full tool output, and command and usage
events remain in `agent.log`.

Ghidra console output defaults to WARN/ERROR, including their continuation lines.
Full INFO logs are always retained in each attempt's `ghidra-*.log` files for
diagnosis and the supervisor's save-confirmation check. Add `--ghidra-info` to
`decomp.py start` (or `run`, `recover-ghidra`, `replay-ghidra`) to print INFO
output too, for example `tools/.venv/bin/python tools/decomp.py start --limit 2 --ghidra-info`.
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

After the supervisor's containing-file gate passes, it extracts the selected C prototype, authored headers and extern declarations. A headless planning transaction parses the types and checks that the proposal can be applied, then rolls back. The supervisor commits a replayable authored analysis record under `config/ghidra/` with the matching C. Only after integration does it apply that record transactionally and save Ghidra. The next fresh decompilation therefore sees the accepted types and global field names. A byte match supports one viable decomp; the evidence record must preserve remaining semantic uncertainty.

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

If a saved update fails after the C commit was accepted, the batch stops with phase `syncing_ghidra`; another target cannot launch. Resolve the cause and run `tools/.venv/bin/python tools/decomp.py recover-ghidra` on the unchanged clean accepted `main` commit. To rebuild accepted analysis on a compatible saved project, use `replay-ghidra` from a clean checkout with no active attempt. Records replay in acceptance order. Originals, SDK archives, generated pseudocode and Ghidra databases are not committed.

## Matching and preservation

The agent uses `tools/.venv/bin/python tools/decomp_step.py` for compact selected-object diagnostics. It recompiles the selected C unit, reuses a hash-checked assembly reference when possible, and prints a short diff. Its task ends when that function matches and its feedback proposal is written; it does not run containing-file validation. A score is diagnostic only. The compiler recipe remains provisional and fixed during each attempt. The agent should restore its best candidate before finishing; the hard timeout preserves whatever is actually in the checkout.

If fresh supervisor diagnostics report 100% for the selected unit but the full-file
comparison fails, Python launches one layout-repair agent on the same branch and
candidate. This follow-up receives the same configured time/token allowance and
model/effort, separately from the first worker. It preserves the matching C and
may carve the selected unit's generated read-only data out of retained data using
explicit `.rodata` ownership and the original `linker_section_order`. Unrelated
boundaries, options, declarations and compiler settings remain protected. This
addresses cases such as a compiler-emitted switch table being linked before code
while its original bytes remain in retained data.

The first failure's reports and candidate archive are preserved under
`<attempt>/layout-repair/before/`; the follow-up has its own prompt, agent log and
timing under `layout-repair/`. Token totals and worker time include both agents.
The follow-up updates the original attempt's `knowledge.md`. Python reruns all
candidate audits and the exact containing-file gate before acceptance. One failed
repair ends in a normal preserved deferral; there is no recursive agent loop.
Compilation failures without a fresh full-file mismatch report, scope failures
and scores below 100% do not trigger this repair pass. Saved ownership splits
remain subject to the same narrow audit on later persistent-branch retries.

Retries merge the saved branch with current main before any decompiler export.
An existing candidate is recompiled with `decomp_step.py` first. At 100%, the
supervisor skips both Ghidra decompilation and the reconstruction worker and goes
directly to full-file validation and layout repair if needed. Prior notes and
analysis evidence are carried forward when reference body fingerprints agree.
Below 100% (or if the check fails), the ordinary fresh Ghidra export and C worker
run. Accepted-donor restarts retain their existing precedence. This skip applies
to decompiler exports; accepted-analysis feedback planning and import still run
after the exact full-file gate. A requested `--sync-types` refresh remains pending
until a reconstruction worker actually needs an export.

Python accepts only a new annotated function block in the image's shared C file (`src/main.c` or `src/overlays/<overlay>.c`), additions to the image header or new local headers, the exact selected asm/c/asm split with narrowly audited selected-unit read-only data ownership, and new symbols in the selected image. New symbol definitions may be inserted anywhere in that image's symbol file; existing lines and their order must remain unchanged, and duplicate symbol names are rejected. Use `// FUNCTION: <image>:0x<ADDRESS>` to begin the function block. Keep all extern declarations together at the top of the C file, before the first annotation, and reuse existing declarations. New externs and local includes may be appended to that shared top block; existing declarations must stay unchanged. Define structs once in guarded headers and reuse existing layouts. New definitions may be appended before the image header’s final `#endif`; existing definitions must stay unchanged. The build extracts that block into generated C and compiles its independent Splat object. Existing blocks and shared declarations, unrelated boundaries, game configuration, SDK policy and previous symbols stay protected by the candidate scope audit. Source checks allow literal PSX GTE inline assembly templates containing GTE commands (named mnemonics or numeric `cop2` commands), COP2 register/memory transfers and pipeline `nop`s, with explicit operands/clobbers. Surrounding logic stays in C. Other assembly, byte embedding and unsupported source forms remain rejected. Python runs fresh `make validate IMAGE=<EXE-or-BIN> UNIT=<unit>` and independently compares the **entire containing file** to its pinned unchanged reference. Candidate/staged/committed hashes and prior C coverage are checked. Only then are the audited candidate files committed on `main`; function-branch WIP history stays on the function branch. Scope and coverage checks compare the complete candidate against current `main`, including edits committed in earlier attempts.

Startup checks the branch, clean checkout, saved attempt state and reference fingerprints, refreshes the target inventory, and prepares the selected function's context. It does not rebuild or check matching before the first attempt. When at least one function was accepted, plain `make validate` checks the boot EXE and all 14 overlays at batch end. If none matched, skip final validation and README progress refresh, retaining attempt results and knowledge review. A nonmatching candidate cannot enter `main`. Object diagnostics and full-file failure reports remain available for the next edit. Objdiff runs with `-L warn`. No interactive diff viewer is used.

After successful final all-image validation, the supervisor runs `tools/readme_progress.py`'s updater to refresh the marked progress block in `README.md`. Counts use a fresh target inventory and persistent deferrals, excluding SDK entries and already reconstructed targets. This happens after the worker acceptance checks, so documentation changes cannot interfere with candidate review. Review and commit the README snapshot before starting another batch, which requires a clean checkout. For a manual refresh (including after requeueing deferrals), run `make progress`; it validates all images before updating the snapshot and leaves the README untouched if validation fails.

Each attempt retains its prompt, context, agent/build logs, candidate patch/sources, diagnostics and one `result.json`. A blocked, timed-out or nonmatching attempt gets a clearly marked WIP checkpoint on its function branch; prohibited source forms stay only in the ignored archive, with an empty WIP marker. Python restores the clean `main` checkout before continuing; it skips the restored-baseline full-file check while the batch has no accepted functions. Each function has one persistent `codex/match-<identity>` branch. Retries switch to that existing branch and merge current `main` into it, retaining candidate C, headers, symbols, split configuration and WIP history. Merge conflicts stop before worker launch with the branch and index intact for resolution. The worker edits the existing function directly in the regular image source. Archived candidates are fallback recovery material when no function exists on the branch; they do not replace existing branch source. The harness never deletes or recreates a function branch on retry. WIP commits never enter `main`.

One batch `result.json` records totals, recorded worker tokens, missing usage records and links to attempt results. `reports/decomp/state.json` identifies the current batch and any interrupted operation. Old session directories and archives remain untouched. There are no manual-session commands, detached launcher, separate boundary decisions, progress files or structural-cache updates.

Each new batch writes `reports/decomp/sessions/<id>/batch.json` as it runs.
It records the command line, UTC start and end times, exit outcome, budget
settings, requested model/effort and functions in selection order. Each new command
invocation gets its own batch directory. Each selection includes the image-qualified function ID
and the attempt directory. Join `context` to `function-metrics.csv` for an
individual attempt or use `session` to group the whole batch. A forced process
kill can leave the invocation without an end time.

## Function usage report

`tools/.venv/bin/python tools/decomp_metrics.py` rebuilds
`reports/decomp/function-metrics.csv` (one row per attempt, including
retries), `function-metrics.json` (totals) and `function-metrics.md` (readable
summary). It also rebuilds `reports/decomp/batches.json`, the cross-session
index of batch commands, timestamps and selected functions. Earlier batch
entries use their session IDs for start times and final result file timestamps
for estimated end times where available. Their command lines are unavailable.
These ignored reports cover preserved automatic attempts under
`sessions/`; they refresh whenever an attempt result is recorded, including
interruption and accepted Ghidra recovery. Existing historical results are read
without rewriting them. Manual chat work without an attempt record is excluded.

Rows identify image/function, outcome, size, input/cached/uncached/output/reasoning
tokens, total tokens, token budget and whether it was reached, worker seconds,
timing source, session and attempt directory. The `model` column records the
resolved model reported by Codex. `requested_model` records the explicit
override, when supplied. Historical attempts without model metadata stay blank.

Use `--model gpt-6.1-luna` to select a model for a batch. Omit the argument to
use the Codex configuration. The model ID must be available to the account.
The default launcher records the resolved model even with `--agent-tokens 0`.
Use `--effort medium` (or another effort supported by the chosen model) to
override reasoning effort for the invocation. Omit it to use Codex configuration.
The function CSV records `requested_effort` and the resolved `effort` reported
by Codex. The batch summary includes requested and observed efforts.

Custom launchers manage their own model and effort selection.

`reports/decomp/batch-metrics.csv` aggregates the function metric rows by
command invocation, including matched/deferred counts, requested and observed
models, input/cached/output totals and uncached input plus output. It refreshes
with the function report. Incomplete usage counts identify partial totals.
Historical attempts without a command record appear together within their
original session.

Input already includes cached input; output already includes reasoning output.
Total means input plus output, not a billing estimate. Counts cover workers only,
not the supervising chat. Incomplete turns include the last observed usage when
available. Missing usage stays blank.

New launches save `worker-timing.json` using a monotonic subprocess timer,
including timeouts. New results also record elapsed attempt time from selection
through validation/integration or deferral. Recovery delays can extend attempt
time. Historical worker times use prompt/log modification times and are marked
`estimated_file_mtimes`; unavailable timing stays blank. File copies or timestamp
edits can invalidate estimates. Summed worker time is not batch wall time.

See [the session efficiency review](token-efficiency.md) for findings and changes.

Launcher/authentication failures stop after preserving work. Ctrl-C/SIGTERM kills the agent process group, preserves its in-scope candidate and returns to main with the function eligible for retry. Unexpected source/index/ref changes stop with the checkout intact. If Git fails mid-commit/integration, inspect the recorded branch/commit and working tree before starting again; the runner does not guess or reset unexpected changes. Hard-killed worker attempts in known pre-integration phases can be recovered by `start` under the recovery guards above.
