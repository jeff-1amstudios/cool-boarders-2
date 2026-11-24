# Cross-overlay duplicate triage

Run from the repository root:

```sh
tools/.venv/bin/python tools/overlay_duplicates.py
```

This standalone, read-only scan uses the saved headless Ghidra inventory in
`reports/overlays/<image>.json` and the original files in `orig/disc/files/`.
It checks all 14 overlay files against `config/overlays.json`, checks each
function's body hash and ranges, and checks SDK classifications against
`config/psyq-sdk.json`. It needs no new dependencies or Ghidra invocation.
Missing exports must first be regenerated using the existing overlay workflow.
Saved exports may omit functions discovered since their export; this scan does
not claim to refresh saved analysis.

Outputs are generated, ignored files:

- `reports/overlay-duplicates/duplicates.md`: counts, largest groups and overlay pairs.
- `reports/overlay-duplicates/duplicates.json`: complete groups with image-qualified identities and input hashes.

The exact pass groups identical body bytes. The normalized pass preserves
instruction order, registers, scalar constants, relative branches, resident
calls and GP/stack offsets. It replaces direct targets inside the containing
overlay and adjacent `lui`/address-use pairs pointing into that overlay with
symbolic targets. Internal targets retain their instruction positions;
external overlay targets retain their pattern of reuse within the body.
Targets are not checked for equivalent implementations or equivalent data.
Noncontiguous range layout is retained in normalized fingerprints.

Default counts exclude bodies below 32 bytes; all-size totals are also emitted.
Use `--min-bytes 64` to be stricter or `--output <directory>` to change the
report destination. SDK groups are counted separately. Normalized and exact
counts overlap and must not be added together. Extra-copy estimates count
distinct images minus one per group, excluding additional same-image copies.
Byte estimates can overcount overlapping Ghidra bodies and are not build progress.

Both passes find reuse leads, not proven semantic equivalence. Identical bytes
can refer to overlay-specific callees or data. The normalized pass also misses
nonadjacent address construction, pointers outside the file, changed register
allocation, different function boundaries and instruction changes. No matching
C, SDK identity, backlog deduplication or saved-analysis changes follow from
these results. Every adopted C implementation still needs its containing-file
exact comparison.

Further analysis can compare corresponding callee groups and data references.
Each image-qualified target and its acceptance gate remain independent.

## Accepted C reuse in automatic attempts

Every new attempt now queries live fingerprints and the freshly rebuilt target
inventory on integration main. Eligible donors are current accepted non-SDK C
functions in another overlay. Exact-byte donors rank ahead of normalized donors;
ties use image-qualified identity for deterministic selection. Held, retained
assembly and deferred candidates never become donors. There is no 32-byte cutoff
for reuse. Resident targets are outside this overlay-only lookup.

When a donor exists, the attempt receives `reuse.json`, `reuse-candidate.c`,
`reuse-donor.c` and `reuse-headers.json`. They preserve the selected donor block,
shared declarations, quoted header dependencies, compiler settings, body/source
hashes, alternatives and instruction-corresponding address/symbol hints. The
candidate has the target annotation but retains donor C identifiers: the worker
must adapt the entry and references, including aliases and pointer addends. It
then compiles the minimally adapted implementation before algorithm changes.
No whole-file source/header copying or automatic type import is performed.

An accepted donor **supersedes a deferred candidate**, including C already on
the persistent function branch. Before updating an old branch, the supervisor
checks its changes against the common ancestor: only the selected function,
its declarations/types/symbols and its split are eligible for replacement.
Unrelated edits stop the restart. It archives the WIP files and patch under
`superseded-branch/`, then fast-forwards the existing branch to a merge commit
whose contents exactly equal current accepted main. The old branch tip and main
are both parents, so no branch is deleted or rewritten and obsolete candidate
appends cannot cause text conflicts. The saved phase/commit permits recovery if
interrupted during this update. Ordinary retries without donors still merge
normally and stop on unresolved conflicts.

The launcher preserves the old selected block as
`superseded-candidate.c`; archived C/notes remain available via `resume.json`.
The worker replaces only the selected block, retains branch/history, and records
the donor, replacement reason and first-check result in `knowledge.md`. Without
a donor the existing resume policy applies. Fresh typed Ghidra output is still
supplied, and normal scope checks and full-containing-file acceptance still run.

Preview eligible targets from a saved backlog (does not launch workers):

```sh
tools/.venv/bin/python tools/decomp_reuse.py
```

The preview is diagnostic; automatic selection rebuilds the inventory before
choosing donors so newly accepted matches become available to later attempts.
