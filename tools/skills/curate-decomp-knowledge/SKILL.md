---
name: curate-decomp-knowledge
description: Review pending Cool Boarders 2 decompilation attempt notes, curate evidence-backed reusable findings, and record completed knowledge reviews. Use for periodic knowledge maintenance after decomp.py batches, including deferred and interrupted attempts.
---

# Curate decompilation knowledge

Perform the supervisor knowledge review that `tools/decomp.py start` leaves
pending. This is documentation maintenance, not a new decompilation batch.

## Locate and scope

Use the current Cool Boarders 2 checkout. If invoked elsewhere, resolve this
skill's real path: its repository is three directories above this skill folder.
Read that checkout's `AGENTS.md` and `docs/decomp-knowledge/README.md`; the latter
defines entry format, evidence standards, topic IDs and review markers.
Inspect Git status and preserve unrelated work.

Honor a requested session or function scope. Otherwise discover existing
`knowledge.md` sidecars under `reports/decomp/sessions/`, oldest sessions first,
and review the pending notes. Silently skip functions/attempts without notes:
having no notes is valid. Do not list or count them as gaps, create review records
for them, reconstruct missing notes, or report them as pending. A session with no
notes needs no review record. Read compact session/attempt metadata for existing
notes before loading their contents; do not dump logs or the entire knowledge
base. Include notes from matched, deferred, rejected and interrupted attempts,
even when final results are unavailable.

Check `reports/decomp/state.json`, the relevant `batch.json` and live process
state if needed to distinguish finished/interrupted work from an active writer.
Do not edit a running attempt's sidecars or runner state; review other completed
sessions and report active work as pending. A lock file's presence alone is not
proof that a process is active.

Skip attempts with an existing completed review whose durable documentation
reference still exists. Revisit when explicitly requested or later evidence
changes the conclusion. For an existing note, a broken reference or incomplete
review remains pending. Do not silently limit the review to the latest batch.
If interrupted, leave clear durable progress for resumption.

## Review evidence

For each attempt read `knowledge.md`, its `result.json` when available and the
session outcome. Search the image-qualified function ID and relevant topic
terms in the curated knowledge and prior notes. Read only applicable entries.
Worker notes are proposals, including claims of “no new insight.”

Inspect the cited C and diagnostics when evaluating a finding: use accepted
commit objects via `git show` and archived candidates/patches without checking
out or altering persistent function branches. Compare instruction differences
where material. Correlate exact containing-file validation records with that
candidate's acceptance/commit; a session flag, current matching source or 100%
object score by itself does not verify a worker's historical claim.

Recover the actual compiler release, cc1 flags, maspsx pin/options, assembler
flags and relevant type assumptions from attempt context, logs, or configuration
at the accepted commit. Do not substitute today's mutable configuration for
the historical recipe. Do not invent intermediate candidates or overwritten
trial evidence. Record evidence gaps and narrow claims accordingly.

Decide whether the finding is new and reusable, duplicates an existing topic,
is specific to one function, or yields no new insight. Promote only genuinely
reusable findings, merging independent evidence into existing IDs where useful.
An unsuccessful attempt can support an explicitly labelled hypothesis with
the exact failed approach, observed difference and next discriminating experiment.
Only exact containing-file acceptance supports a verified matching finding.
Keep missing-evidence claims unresolved rather than upgrading them to verified.

## Save durable results

Update concise topic entries under `docs/decomp-knowledge/` using the index's
format and evidence rules. Include image-qualified IDs, exact recipes, scope,
date, acceptance/commit references and a minimal C/MIPS example where supported.
Keep useful failed observations explicitly hypothetical. Update the topic index
when adding IDs or topics, and preserve contradictory evidence.

Maintain a compact tracked review record at
`docs/decomp-knowledge/reviews/<session-id>.md`. Record each reviewed note's attempt identity,
outcome, disposition, promoted/duplicate IDs, evidence references and any pending
gaps. Preserve useful function-specific failed approaches and next experiments
there when they are not suitable for a topic entry. This record makes review
completion durable even when there is no new topic entry or ignored reports
are later unavailable. Do not copy raw disassembly dumps or whole worker notes.
Link review records from the knowledge index so future sessions can find them.

After saving a decision, append or update a review annotation in the ignored
attempt sidecar, preserving the worker's original observations:

```text
Review: promoted <IDs> / duplicate <IDs> / function-specific / no new insight
Review date: YYYY-MM-DD
Review reference: docs/decomp-knowledge/reviews/<session-id>.md
```

Choose the applicable disposition(s), not the literal list above. Add a
documentation commit reference when available. For missing evidence use an
explicit `Review: pending — <reason>` instead of a completed disposition. Do not
fabricate a worker note when one is missing; silently skip that attempt. Missing
evidence supporting a claim in an existing note is distinct from having no note.

Audit the documentation diff and Git status, checking references and ensuring
ignored attempt data stays untracked. Do not change game C, boundaries, compiler
recipes, saved Ghidra analysis, acceptance results or progress counts. No rebuild,
Ghidra run, unit tests or new matching attempts are needed for this review.
If committing is requested, keep curation in a separate documentation commit,
audit the staged list and exclude unrelated pre-existing changes.

Report sessions/attempts reviewed, findings promoted or merged, and remaining
pending items with reasons. Link the durable review records. If no pending work
exists, say so without creating empty maintenance files.
