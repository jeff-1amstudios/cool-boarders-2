# Bounded reconstruction worker

`tools/reconstruct.py` owns target selection, context preparation, budgets, validation and audited commits. Its `run` command launches one fresh agent process per function and automatically advances the queue. The manual commands below remain available for supervised work. Both modes proceed serially in the current checkout, with one active target and a clean baseline between targets.

## Automatic batches

From a clean checkout, with the existing Codex CLI installed and authenticated:

```sh
tools/.venv/bin/python tools/reconstruct.py run --limit 10
```

This starts a new session, verifies the baseline, selects one eligible target, prepares its context, and launches a fresh `codex exec` process with the prompt on stdin. There is no session resume or shared conversation between functions. The launcher uses `--ephemeral`, `--sandbox workspace-write`, `--ask-for-approval never`, JSON event logs and a final-message file. These flags follow the [official OpenAI noninteractive documentation](https://learn.chatgpt.com/docs/non-interactive-mode) and the installed CLI's help. It reuses the existing CLI account/configuration and does not install anything. Temporary files, diagnostic logs and SQLite job state are routed under ignored `tools/tmp/` and `tools/cache/` using per-process overrides documented in the [configuration reference](https://learn.chatgpt.com/docs/config-file/config-reference); user settings are not edited.

The agent may use `make objdiff` and `make validate` while refining its C. When it exits, Python checks its source and split edits and runs **fresh `make validate` independently**. It verifies the actual rebuilt bytes, selected C address/size and unchanged previous C coverage. Agent messages and reports do not authorize a commit. Candidate content hashes must remain unchanged across validation, and staged blobs must have those same hashes. Python stages the audited paths and commits each accepted function separately.

The defaults are ten selected targets, a two-hour session and fifteen minutes per agent. Override them explicitly:

```sh
tools/.venv/bin/python tools/reconstruct.py run \
  --limit 5 --minutes 60 --agent-seconds 600
```

`run` performs one final controller validation per candidate. Its agent's local compile/edit loop is bounded by wall time; the manual `begin --attempts` counter does not count those local build commands. Rejected or timed-out candidates count toward the target limit. Python archives the candidate, restores only the permitted edits, validates the restored baseline, then selects the next function. Cleanup may extend beyond the session deadline so the checkout can be returned to a verified state. A launcher/authentication error stops the batch after recovery instead of repeatedly consuming targets. No available target ends the batch normally.

Automatic edits are restricted to one new `src/*.c` translation unit, new local `src/*.h` files, splitting the selected assembly span in `scus_943.58.yaml`, and appending to `symbol_addrs.txt`. Existing source files, options, unrelated boundaries, previous symbols, the reference and compiler/build tools must remain unchanged. Basic source checks reject inline assembly, instruction/byte embedding directives, section attributes, initialized arrays, macro tricks and includes outside the local source headers. Only simple header guards and quoted local includes are accepted. These are conservative checks, not a proof of source readability; candidates requiring broader changes should use the manual workflow.

If the agent changes HEAD, stages files, changes a protected file or makes an out-of-scope edit, the batch stops with the candidate preserved for inspection. It does not reset unexpected changes. Do not edit the checkout concurrently. Ctrl-C/SIGTERM during agent work kills its process group, archives its candidate and restores a verified baseline when the edits are within scope. Interruption or failure during Git commit leaves the candidate/index for inspection because the commit may already have happened. A hard process kill can leave an active target; inspect `status`, logs and any running launcher before using the manual recovery commands or starting a new session.

Prompts, launcher arguments, agent logs/final messages, candidate patches/sources and controller validation reports live under `reports/reconstruction/sessions/<session>/<address>/`. Session state includes each accepted commit, deferred reason and the final stop reason. `status` prints the latest session. A new automatic batch preserves previous session directories; session deferrals reset between batches, while curated `hold: true` entries stay excluded.

A different launcher can be supplied as a shell-quoted argv string. Python executes it directly, with no shell expansion, substitution or pipelines:

```sh
tools/.venv/bin/python tools/reconstruct.py run --limit 1 \
  --agent-command '/absolute/path/to/agent-wrapper --noninteractive'
```

The full prompt is supplied on stdin and the working directory is the repository root. `CB2_RECONSTRUCTION_CONTEXT` names the target's absolute `context.json` path and `CB2_AGENT_REPORT` names an optional final-message output file. Exit zero when the agent has finished its candidate, including an ordinary blocked attempt; nonzero indicates a launcher/runtime failure. Custom launchers must supply their own authentication, sandbox, ephemeral-session and local-cache settings. They must not stage or commit files. Python still applies every acceptance gate.

## Manual sessions

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

The inventory is regenerated from an independently rebuilt, full-file-verified assembly reference. Each instruction's recorded bytes are also checked against the pinned executable. Generated assembly, context, logs, progress and attempt state remain under ignored `build/` and `reports/reconstruction/`. The committed `config/reconstruction.json` contains only reviewed metadata and pilot priorities. Splat-detected boundaries remain provisional unless explicitly reviewed; the inventory count is not a complete-game denominator. Unknown functions must not be called game-authored merely because they lack an SDK label. SDK classification is curated, not inferred from an address cutoff or a Ghidra name alone.

The default queue prefers the reviewed pilot, then small leaf functions. It excludes entries on hold, known SDK/handwritten entries, GP/COP instructions, indirect control flow, noncontiguous bodies and functions larger than 160 bytes. Use `next --address 0x800...` to select an eligible target explicitly. Review both ends of each split and nearby data before converting a segment; the script does not automatically infer or edit boundaries. Read callers, callees and data references in the context; Ghidra pseudocode may be consulted as a hypothesis, not copied as established source.

If a target stalls:

```sh
tools/.venv/bin/python tools/reconstruct.py defer --reason 'Need caller evidence for the structure layout'
tools/.venv/bin/python tools/reconstruct.py status
```

Each validation attempt archives the tracked patch, candidate sources, build log and available object/full-file diagnostics. Defer also archives the current edits alongside the target context. It never resets files. Restore only this attempt's edits after checking the archive, then run `next`; the clean-checkout gate prevents contamination of the next target. Deferred targets are skipped for the remainder of the session. Sessions have separate directories under `reports/reconstruction/sessions/`, preserving context and state when a new batch starts. A new session resets its session exclusions; persistent `hold: true` entries in `config/reconstruction.json` stay excluded until reviewed and released. An active target must be finished or deferred before beginning again. Concurrent worker commands are rejected by a process lock.

Both successful and deferred targets count toward the session limit. Manual attempts include final validation, controller builds have a five-minute timeout, and the session deadline bounds checks and target selection. These limits govern worker commands; a manual agent must also stop editing or defer when its analysis budget expires. The worker does not run a background scheduler. Start another bounded session deliberately when the batch ends. Do not relax the reference fingerprint, compiler recipe or full-file gate to obtain a commit. Investigate compiler discrimination separately when independent functions show a consistent mismatch pattern.

Run the guard tests with `tools/.venv/bin/python tools/test_reconstruct.py` and the subprocess/controller integration tests with `tools/.venv/bin/python tools/test_reconstruct_run.py`. The latter uses temporary Git repositories, scripted agents and a synthetic executable gate; it tests real process launch, commits, rejection, restoration, deadlines and interruption without paid model calls. The real game's acceptance check remains `make validate`.

Matching C bytes, retained assembly, SDK recognition, overlay coverage and runtime behavior remain separate measures. The full executable comparison establishes the boot binary match; this worker does not establish runtime coverage or identify unloaded overlays.
