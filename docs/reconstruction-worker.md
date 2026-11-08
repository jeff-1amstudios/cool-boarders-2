# Bounded reconstruction worker

`tools/reconstruct.py` owns target selection, context preparation, budgets, validation and audited commits. The agent running it supplies reviewed C and split edits. It does not invoke an LLM or repeatedly spawn agents. Work proceeds serially in the current checkout, with one active target and a clean baseline between targets.

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

The default queue prefers the reviewed pilot, then small leaf functions. It excludes known SDK/handwritten entries, GP/COP instructions, indirect control flow, noncontiguous bodies and functions larger than 160 bytes. Use `next --address 0x800...` to select an eligible target explicitly. Review both ends of each split and nearby data before converting a segment; the script does not automatically infer or edit boundaries. Read callers, callees and data references in the context; Ghidra pseudocode may be consulted as a hypothesis, not copied as established source.

If a target stalls:

```sh
tools/.venv/bin/python tools/reconstruct.py defer --reason 'Need caller evidence for the structure layout'
tools/.venv/bin/python tools/reconstruct.py status
```

Defer archives the tracked patch and candidate source contents alongside the target context. It never resets files. Restore only this attempt's edits after checking the archive, then run `next`; the clean-checkout gate prevents contamination of the next target. Deferred targets are skipped for the remainder of the session. Logs are numbered by target and attempt. A new session after a completed/deferred batch resets its queue exclusions; consult the previous state before replacing it. An active target must be finished or deferred before beginning again.

Both successful and deferred targets count toward the session limit. Attempts include final validation, builds have a five-minute timeout, and the session deadline bounds checks and target selection. These limits govern worker commands; an agent must also stop editing or defer when its analysis budget expires. The worker does not run a background scheduler. Start another bounded session deliberately when the batch ends. Do not relax the reference fingerprint, compiler recipe or full-file gate to obtain a commit. Investigate compiler discrimination separately when independent functions show a consistent mismatch pattern.

Run the guard tests with `tools/.venv/bin/python tools/test_reconstruct.py`. Matching C bytes, retained assembly, SDK recognition, overlay coverage and runtime behavior remain separate measures. The full executable comparison establishes the boot binary match; this worker does not establish runtime coverage or identify unloaded overlays.
