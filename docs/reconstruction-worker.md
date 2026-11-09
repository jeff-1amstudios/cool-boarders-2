# Bounded reconstruction worker

`tools/reconstruct.py start` runs a serial Python batch in the foreground; `run` is equivalent. Both block until the batch finishes and stream build output, agent messages and per-target results to stdout. Both process the complete saved Ghidra function backlog in recorded order: boot first, then overlay filename, ascending address. Main is the integration branch. Work proceeds serially in the existing checkout, on a fresh function branch for each attempt.

## Complete target backlog

`make target-list` regenerates `reports/reconstruction/targets.json` from the saved, byte-checked boot and 14 native-overlay exports. It checks the reference fingerprints, export run IDs, SDK catalog and configured C coverage. All 3,967 function instances remain in the denominator; equal addresses and identical bodies in different images stay separate. Stable IDs look like `INIT.BIN:0x8005AB6C`. Unknown does not establish game authorship. SDK reconstruction and duplicate detection are excluded.

The starting checkpoint has 1,047 accepted SDK instances, 30 non-SDK instances covered by C, one held entry and 2,889 requiring review. Five SDK entries also have C. Report C coverage only alongside fresh successful full-file validation. Overlay code initially remains entirely retained assembly.

`reconstruct.py inventory` retains the separate legacy boot Splat-span inventory for boundary auditing. Those spans are build units, not a replacement function denominator. Automatic and manual target selection use the complete backlog. Legacy pilot lists and small-leaf preferences do not restrict or reorder that backlog.

## Run a bounded batch

Matching has deliberately resumed after integrating the backlog adapter. From clean `main`:

```sh
tools/.venv/bin/python tools/reconstruct.py start --limit 10
```

Defaults are ten selected targets, 120 minutes per session, 900 seconds per agent, and a 300-second plateau timeout. The plateau clock resets only when the selected object score improves; `--stall-seconds 0` disables it. The runner refreshes the resident Ghidra boundary export and validates the baseline before selection. Override with `--minutes`, `--agent-seconds`, and `--stall-seconds`. `start --limit 2` finishes and prints the first target before running the second, then exits. `start --detach` explicitly opts into background execution and returns a PID/log path. Use `status` for compact progress (`--verbose` for full state), or read `reports/reconstruction/summary.json` after completion. Raw per-command logs are retained alongside the stdout stream. No chat polling is needed. The summary records exact matches, WIP blockers, recorded worker tokens and any missing usage records. A custom `--agent-command` accepts shell-quoted argv, executed without a shell; the prompt arrives on stdin. The installed Codex CLI is reused without installing host packages.

Each selected instance gets byte-bound context under `reports/reconstruction/sessions/<session>/<image_address>/`: identity, reference/body hashes, exact ranges, containing Splat span, instruction listing, neighbour instructions, incoming direct transfers, control references and possible pointer words. Pointer words are leads, not proven callers. Only that selected function undergoes structural review. Context includes bounded dependency snippets and neighbouring instructions, without repeating the target body. Noncontiguous bodies, shared interior entries, embedded accepted SDK entries, ABI/control-flow findings or a missing containing assembly span produce a durable concrete deferral. Curated shared/nonstandard regions remain held. Structural blockers are cached under ignored `reports/reconstruction/structural-deferrals.json`, keyed to the image, byte-bound catalog, reviews and analysis tools. Unchanged blockers are skipped in later batches without consuming another target slot; `--retry-structural` or an explicit manual address forces review. Ordinary C mismatch deferrals remain retryable.

The agent must review boundaries, callers, callees and data dependencies and write `boundary-decision.json` in its context directory, containing `identity`, `body_sha256`, `decision` (`standalone` or `deferred`) and nonempty `evidence`. The controller requires a standalone decision bound to the selected body before acceptance. These records are durable local analysis, not proof of original source spelling. Ghidra boundaries remain estimates; the agent must defer if a safe standalone conversion is not established.

Attempts may add one C unit and local headers under `src/` for boot or `src/overlays/<slug>/` for overlays. Only the selected image's containing assembly span may be split into asm / c / asm; only its symbols file may be appended to. Options, unrelated boundaries, existing source, other image configs, previous symbol text, references, compiler/build tools, SDK policy and queue metadata stay protected. Simple source checks reject assembly/byte embedding, section attributes, initialized arrays and unsupported preprocessor tricks. Broader source changes require supervised work.

The per-image configured compiler recipe remains provisional and fixed during an attempt. SDK recognition, retained assembly and matching C are separate progress measures.

## Acceptance, branches and recovery

Python owns branch changes, validation, staging and commits. Branch/context/cache names include the image and address. Existing function branches are retained; retries get numeric suffixes. The launcher uses ephemeral workspace-write execution and routes logs, SQLite state and temporary files into ignored `tools/cache/` and `tools/tmp/`. It supplies `CB2_RECONSTRUCTION_CONTEXT` and `CB2_AGENT_REPORT`.

During iteration the agent uses `tools/.venv/bin/python tools/reconstruction_step.py` (the context path is supplied in its environment). It recompiles only the selected unit and emits at most twelve formatted instruction differences plus a JSON detail path. Its independently rebuilt assembly reference is cached with hashes of its inputs and object; source and headers are always recompiled. `--validate` additionally compares the entire containing file. A score is never an acceptance gate.

After the agent exits, the controller checks edit scope, exact selected split, source form and prior C coverage, then runs fresh `make validate IMAGE=<EXE-or-BIN> UNIT=<unit>`. This limits object diagnostics to the selected unit but rebuilds and independently compares the **entire containing file** against its pinned reference. Protected inputs and edit-scope guards establish that other images are unchanged. Full `make validate` checks all 15 files at batch start and end. Object scores and agent claims never authorize a commit. Candidate hashes must survive validation and match staged/committed blobs. Only then does Python commit and fast-forward `main` to the validated function commit.

A blocked or nonmatching attempt counts toward the limit. Its sources, patch, logs, boundary evidence and diagnostics are archived locally; in-scope authored work receives a clearly marked WIP checkpoint on its function branch. Disallowed source forms stay only in the ignored archive, with an empty WIP marker. Python returns to unchanged `main`, validates the affected image, and advances. WIP commits never enter `main`. A launcher failure stops after recovery instead of consuming the queue.

Unexpected edits, index changes, HEAD/branch changes or movement of `main` stop with the checkout preserved. Commit/integration failures preserve the transaction phase and index because Git may already have completed the operation. Do not edit concurrently or reset unexpected changes. `status` shows the latest session, branch/base/checkpoint IDs and stop reason. Ctrl-C/SIGTERM during agent work kills its process group, preserves in-scope work and returns to validated main. Recovery may extend past the deadline.

To inspect a WIP, first ensure the checkout is clean, then switch to its recorded function branch and run validation. Recover its candidate onto a **new branch from current main**, validate and create one matching commit; never merge WIP history. All session directories remain available after subsequent batches. Ordinary attempt deferrals reset between batches; cached structural blockers and curated holds persist until their evidence changes or they are explicitly retried.

## Manual sessions

Manual commands use the same complete backlog but do not manage branches. Create the desired function branch first:

```sh
tools/.venv/bin/python tools/reconstruct.py begin --limit 10 --attempts 6 --minutes 120
tools/.venv/bin/python tools/reconstruct.py next
# Or: next --address 'INIT.BIN:0x8005AB6C'
# Review context, boundaries and dependencies; edit C and selected image config.
tools/.venv/bin/python tools/reconstruct.py check
tools/.venv/bin/python tools/reconstruct.py finish --paths <all-authored-paths> --message '<message>'
```

`finish` always performs fresh validation and audits the exact explicit changed paths. It commits on the current function branch without integrating main. `defer --reason '<concrete blocker>'` archives edits but never resets them; restore only the archived attempt's edits before `next`. `begin` and selection require clean checkout/index. Manual checks consume the attempt budget; automatic agent compile loops are bounded by wall time and score progress, then receive one independent containing-file acceptance build.

Use existing guard/controller tests and the focused `tools/test_reconstruction_backlog.py` adapter checks. The latter covers image-qualified selection, review/defer behavior, image/source isolation, and a synthetic overlay full-file failure followed by successful integration at the same RAM address in another image. The actual game's correctness gate remains the pinned full-file comparison (`make validate IMAGE=...` per attempt and `make validate` for all-file batch boundaries), with diagnostics in `reports/objdiff.json`, per-unit JSON, `reports/boot-rebuild.json` and `reports/overlay-rebuild.json`. No interactive diff viewer is used.
