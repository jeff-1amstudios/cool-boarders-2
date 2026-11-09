# Bounded reconstruction worker

`tools/reconstruct.py` owns target selection, context preparation, budgets, validation and audited commits. Its `run` command launches one fresh agent process per function on a separate Git branch and automatically advances the queue. `main` is the integration branch. The manual commands below remain available for supervised work. Work proceeds serially in the current checkout; the runner switches branches rather than creating extra worktrees.

## Complete target backlog

Run `make target-list` to rebuild `reports/reconstruction/targets.json` from the saved, byte-checked Ghidra analysis for the boot EXE and all 14 overlays. The backlog contains every detected function instance, including SDK and C entries. Stable IDs combine image and address, for example `INIT.BIN:0x8005AB6C`; equal addresses or bodies in different overlays remain separate. It records each image's build configuration, symbol file and file offset.

The current checkpoint has 3,967 instances: 1,047 accepted plugin SDK entries, 30 non-SDK entries covered by C, one held entry and 2,889 requiring review. This is 2,890 remaining non-SDK instances, including the held entry; unknown entries are not proven game-authored. Five additional SDK entries already have C and remain classified as SDK. C coverage records configured source, and must be accompanied by a fresh successful `make validate` before reporting a match.

Process the backlog in its recorded order: boot first, then overlay filename, ascending address. At each remaining entry, review its body, shared entries, callers and data dependencies; then reconstruct and validate it, or record a concrete blocker and move on. Boundary review is performed for the selected entry, rather than requiring the entire backlog to be reviewed first. Keep known shared or nonstandard regions held. Duplicate detection remains separate.

The existing `reconstruct.py inventory` and automatic runner below still operate on reviewed boot Splat build units. They are separate from this complete function backlog. Before using automatic batches against the backlog, adapt selection, session/deferral identities, context paths, allowed Splat/symbol paths and C acceptance checks to the selected image. Preserve the source-edit protections and the full 15-file validation gate. Until that adapter is complete, the matching pause prevents accidentally launching the old boot-only queue. The next pipeline session should finish this adapter and then run the first bounded batch; it should not spend time re-detecting PsyQ versions or duplicates.

## Automatic batches

Matching is currently paused in `config/reconstruction.json`. See [the boundary audit](function-boundaries.md) before deliberately resuming. A batch starts by refreshing the saved Ghidra review export; missing, incomplete, stale or divergent review evidence stops it before any agent is launched.

From a clean checkout on `main`, with the existing Codex CLI installed and authenticated:

```sh
tools/.venv/bin/python tools/reconstruct.py run --limit 10
```

This starts a new session, verifies the baseline, selects one eligible target, prepares its context, creates `codex/match-<address>` from the latest passing `main`, and launches a fresh `codex exec` process with the prompt on stdin. There is no session resume or shared conversation between functions. The launcher uses `--ephemeral`, `--sandbox workspace-write`, `--ask-for-approval never`, JSON event logs and a final-message file. These flags follow the [official OpenAI noninteractive documentation](https://learn.chatgpt.com/docs/non-interactive-mode) and the installed CLI's help. It reuses the existing CLI account/configuration and does not install anything. Temporary files, diagnostic logs and SQLite job state are routed under ignored `tools/tmp/` and `tools/cache/` using per-process overrides documented in the [configuration reference](https://learn.chatgpt.com/docs/config-file/config-reference); user settings are not edited.

The agent may use `make objdiff` and `make validate` while refining its C. When it exits, Python checks its source and split edits and runs **fresh `make validate` independently**. It verifies the actual rebuilt bytes, selected C address/size and unchanged previous C coverage. Agent messages and reports do not authorize a commit. Candidate content hashes must remain unchanged across validation, and staged blobs must have those same hashes. Python stages the audited paths, commits the matching function on its own branch, then switches to `main` and fast-forwards it to that exact validated commit. The function branch is retained.

The defaults are ten selected targets, a two-hour session and fifteen minutes per agent. Override them explicitly:

```sh
tools/.venv/bin/python tools/reconstruct.py run \
  --limit 5 --minutes 60 --agent-seconds 600
```

`run` performs one final controller validation per candidate. Its agent's local compile/edit loop is bounded by wall time; the manual `begin --attempts` counter does not count those local build commands. Rejected or timed-out candidates count toward the target limit. Python archives the candidate and creates a clearly marked WIP checkpoint on its function branch. It switches back to unchanged `main`, validates that baseline, then selects the next function. Cleanup may extend beyond the session deadline so the checkout can be returned to a verified state. A launcher/authentication error stops the batch after recovery instead of repeatedly consuming targets. No available target ends the batch normally.

Automatic edits are restricted to one new `src/*.c` translation unit, new local `src/*.h` files, splitting the selected assembly span in `scus_943.58.yaml`, and appending to `symbol_addrs.txt`. Existing source files, options, unrelated boundaries, previous symbols, the reference and compiler/build tools must remain unchanged. Basic source checks reject inline assembly, instruction/byte embedding directives, section attributes, initialized arrays, macro tricks and includes outside the local source headers. Only simple header guards and quoted local includes are accepted. These are conservative checks, not a proof of source readability; candidates requiring broader changes should use the manual workflow.

If the agent changes HEAD, switches branches, moves `main`, stages files, changes a protected file or makes an out-of-scope edit, the batch stops with the candidate preserved for inspection. It does not reset unexpected changes. Do not edit the checkout concurrently. Ctrl-C/SIGTERM during agent work kills its process group, checkpoints its in-scope candidate on the function branch, and returns to verified `main`. Interruption or failure during Git commit or integration leaves the recorded phase, branches and candidate/index for inspection because the operation may already have happened. A hard process kill can leave an active target; inspect `status`, logs and any running launcher before using the manual recovery commands or starting a new session.

Prompts, launcher arguments, agent logs/final messages, candidate patches/sources and controller validation reports live under `reports/reconstruction/sessions/<session>/<address>/`. Session state includes the integration tip, each function branch and base commit, accepted commit or WIP checkpoint, deferred reason, transaction phase and final stop reason. `status` prints the latest session. A new automatic batch preserves previous session directories; session deferrals reset between batches, while curated `hold: true` entries stay excluded.

A different launcher can be supplied as a shell-quoted argv string. Python executes it directly, with no shell expansion, substitution or pipelines:

```sh
tools/.venv/bin/python tools/reconstruct.py run --limit 1 \
  --agent-command '/absolute/path/to/agent-wrapper --noninteractive'
```

The full prompt is supplied on stdin and the working directory is the repository root. `CB2_RECONSTRUCTION_CONTEXT` names the target's absolute `context.json` path and `CB2_AGENT_REPORT` names an optional final-message output file. Exit zero when the agent has finished its candidate, including an ordinary blocked attempt; nonzero indicates a launcher/runtime failure. Custom launchers must supply their own authentication, sandbox, ephemeral-session and local-cache settings. They must not stage, commit, switch branches or update `main`. Python still applies every acceptance gate.

## Function branches and recovery

`run` requires `main` at startup and leaves it checked out after every ordinary accepted or deferred target. It never deletes or force-updates a function branch. If `codex/match-80017A00` already exists, a new attempt uses `codex/match-80017A00-2`, then `-3`, and so on, always starting from the latest passing `main`. It does not resume or overwrite an old attempt automatically.

Matching commits are the only reconstruction commits the runner integrates into `main`. The user-authorized exception for nonmatching **WIP checkpoint commits applies only to the function branches**. A WIP records the address, baseline commit and blocker in its commit message; its source need not compile or match. It is excluded from matching progress and is never merged into `main`. Empty blocked attempts receive an empty WIP commit so the blocker remains available in Git.

Checkpoints still obey the repository boundary. Raw assembly, byte embeddings, binary source and other rejected source forms stay only in the ignored local patch/source archive. In that case, the branch receives an empty WIP marker explaining the exclusion. Out-of-scope edits, unexpected index changes and changed Git refs cause a stop with the checkout preserved instead of an automatic checkpoint/reset.

To list and restore a checkpoint, first make sure the checkout is clean:

```sh
git branch --list 'codex/match-*'
git switch codex/match-80017A00
git log -1
make validate
```

This restores the authored source/configuration from that attempt's baseline. Logs and diagnostics remain in `reports/reconstruction/`; they are not committed. Run validation after switching branches because existing reports/build outputs describe the last build, not necessarily the current branch.

To integrate recovered WIP work, copy or squash its candidate onto a **new branch from current `main`**, validate it, then create a single matching commit. Do not merge the WIP history: an otherwise matching tip would bring unvalidated checkpoint commits into `main`. Old function branches remain available for comparison and recovery.

Branch creation, checkpointing and integration phases are recorded in `status`. If a Git operation fails or the process is killed mid-transaction, inspect the recorded commit IDs, current branch and index before continuing manually. The runner intentionally does not guess whether an interrupted commit/merge completed.

## Manual sessions

Manual commands do not automate this branch lifecycle. Create or switch to the intended function branch yourself before beginning a manual session. A manual `finish` creates a validated commit on that branch; it does not integrate it into `main`.

```sh
tools/.venv/bin/python tools/reconstruct.py inventory
tools/.venv/bin/python tools/reconstruct.py begin --limit 10 --attempts 6 --minutes 120
tools/.venv/bin/python tools/reconstruct.py next
# Inspect the prepared context, recover types, edit C and review Splat boundaries.
tools/.venv/bin/python tools/reconstruct.py check
# Inspect reports/objdiff.json, reports/objdiff/<unit>.json and reports/boot-rebuild.json.
# Edit and repeat check within the budget, then review the source and diff.
tools/.venv/bin/python tools/reconstruct.py finish \
  --paths src/example.c scus_943.58.yaml symbol_addrs.txt \
  --message 'Reconstruct example function with exact boot match'
tools/.venv/bin/python tools/reconstruct.py next
```

The example paths must be replaced by the exact set of changed files; omit unchanged paths. `finish` consumes an attempt and always runs fresh `make validate`, regardless of earlier success. It requires the selected address and size to be represented by a C segment, independently compares the rebuilt bytes, rejects reference/recipe/gate changes, checks whitespace, stages only the explicit authored paths, audits the index and commits. Begin requires a clean checkout and index; finish requires an empty index. Existing user changes must be handled separately before starting. Do not edit the checkout concurrently with a worker session.

The inventory is regenerated from an independently rebuilt, full-file-verified assembly reference. Each instruction's recorded bytes are also checked against the pinned executable. Generated assembly, context, logs, progress and attempt state remain under ignored `build/` and `reports/reconstruction/`. The committed `config/reconstruction.json` contains only reviewed metadata and pilot priorities. Splat-detected boundaries remain provisional unless explicitly reviewed; the inventory count is not a complete-game denominator. Unknown functions must not be called game-authored merely because they lack an SDK label.

`config/psyq-sdk.json` accepts the PSX plugin's PsyQ 3.7.0 detection and 989 resident SDK entry names, including its generated internal-entry names. It also records 58 SDK entries across the native overlays, keyed by overlay identity and reference fingerprint. No independent version comparison or manual SDK decompilation is required. These identifications are tied to the pinned binaries and protected from worker edits. Any boot reconstruction span containing an accepted SDK entry is classified `sdk` and excluded even if library candidates are enabled; boundaries and existing C/assembly status are preserved. The inventory records SDK names/version and counts SDK units separately, including any already reconstructed C units. The boot C worker still selects only reviewed boot units; the combined overlay inventory is documented in [the overlay workflow](overlays.md).

The queue requires a byte-bound `standalone` review from `config/function-boundaries.json` and no structural findings. It excludes curated library candidates unless deliberately enabled. Among reviewed candidates it prefers the pilot, then known call/address references and small leaf functions. Unreferenced does not mean unused; indirect calls, callbacks and overlays remain possible. The review catalog, policy, audit and Ghidra export scripts are protected from matching agents.

The former `boundary_reviewed` preference is now a hard requirement computed from the current bytes and extent, rather than an unverified boolean. Shared-entry regions, data templates and custom-ABI helpers need a separate reviewed strategy.

The queue It excludes entries on hold, known SDK/handwritten entries, GP/COP instructions, indirect control flow, noncontiguous bodies and functions larger than 160 bytes. Use `next --address 0x800...` to select an eligible target explicitly. Review both ends of each split and nearby data before converting a segment; the script does not automatically infer or edit boundaries. Read callers, callees and data references in the context; Ghidra pseudocode may be consulted as a hypothesis, not copied as established source.

If a target stalls:

```sh
tools/.venv/bin/python tools/reconstruct.py defer --reason 'Need caller evidence for the structure layout'
tools/.venv/bin/python tools/reconstruct.py status
```

Each validation attempt archives the tracked patch, candidate sources, build log and available object/full-file diagnostics. Defer also archives the current edits alongside the target context. It never resets files. Restore only this attempt's edits after checking the archive, then run `next`; the clean-checkout gate prevents contamination of the next target. Deferred targets are skipped for the remainder of the session. Sessions have separate directories under `reports/reconstruction/sessions/`, preserving context and state when a new batch starts. A new session resets its session exclusions; persistent `hold: true` entries in `config/reconstruction.json` stay excluded until reviewed and released. An active target must be finished or deferred before beginning again. Concurrent worker commands are rejected by a process lock.

Both successful and deferred targets count toward the session limit. Manual attempts include final validation, controller builds have a five-minute timeout, and the session deadline bounds checks and target selection. These limits govern worker commands; a manual agent must also stop editing or defer when its analysis budget expires. The worker does not run a background scheduler. Start another bounded session deliberately when the batch ends. Do not relax the reference fingerprint, compiler recipe or full-file gate to obtain a commit. Investigate compiler discrimination separately when independent functions show a consistent mismatch pattern.

Run the guard tests with `tools/.venv/bin/python tools/test_reconstruct.py` and the subprocess/controller integration tests with `tools/.venv/bin/python tools/test_reconstruct_run.py`. The latter uses temporary Git repositories, scripted agents and a synthetic executable gate; it tests real process launch, commits, rejection, restoration, deadlines and interruption without paid model calls. The real game's acceptance check remains `make validate`.

Matching C bytes, retained assembly, SDK recognition, overlay coverage and runtime behavior remain separate measures. The full executable comparison establishes the boot binary match; this worker does not establish runtime coverage or identify unloaded overlays.

`make validate` now also assembles/links and compares all 14 pinned overlay BINs. An unrelated overlay failure therefore blocks a boot worker attempt, and its aggregate/per-image diagnostics are archived with the attempt. All overlay build recipes are protected from this boot-only worker. Manual reviewed overlay C conversions use namespaced objdiff units and the same full validation gate; see [overlay matching](overlays.md#matching-overlays). No overlay C is counted as matched merely because its SDK signature or Ghidra decompilation is available.
