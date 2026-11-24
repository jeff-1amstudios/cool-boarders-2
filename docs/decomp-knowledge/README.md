# Decompilation knowledge base

Persistent project-authored matching knowledge. Read this index, then only the
entries relevant to the current symptom. These are scoped observations about
viable matching C, not proof of the original source or a universal compiler recipe.

| Topic | Search terms / entries |
| --- | --- |
| [Scheduling and C constructs](scheduling.md) | inline, return cursor, delay slot, constant load, store order, short reload; SCHED-001, SCHED-002 |
| [Types and pointer arithmetic](types-and-pointers.md) | CdlLOC, alignment, struct copy, lw/sw, stride, narrow field, widened local; TYPE-001, TYPE-002 |
| [Registers and stack layout](registers-and-stack.md) | lifetime, buffer scopes, frame size, saved registers, allocation priority, scratch reuse, indexed traversal, RTL/LLDB workflow; STACK-H001, REG-H001, REG-H002, REG-002, REG-003, REG-W001 |
| [Optimization and toolchain](optimization.md) | division, traps, maspsx, provisional flags; OPT-001 |

## Before an attempt

Search IDs and symptoms, for example:

```sh
rg -n 'CdlLOC|unaligned|struct copy' docs/decomp-knowledge
rg -n -F 'SCUS_943.58:0x80017470' docs/decomp-knowledge
rg -l -F 'SCUS_943.58:0x80017470' reports/decomp/sessions -g knowledge.md
```

Read only selected prior notes; check their sibling `result.json` for outcome.
Older attempts may have no sidecar: use the deferred queue's attempt links and
[curated historical reviews](../deferred-function-review.md). Do not recursively
dump logs. On retries, resume the supplied saved candidate and its diagnostics;
consult the fresh typed Ghidra export as updated reference. Reconcile prior
declarations and boundaries with current main instead of overwriting whole files.
Record consulted IDs and avoid already failed trials unless assumptions changed.

## Evidence and maintenance

- **Verified** means the cited accepted C passed the pinned full-containing-file
  comparison. State the supported scope and cite the accepted source, validation
  record or durable review. A high object score or worker claim is insufficient.
- **Hypothesis** means plausible, partially supported or unresolved; record what
  failed, the observed difference and an experiment that could distinguish causes.
  Failed trials disprove only the exact candidate/settings tested.
- Give each finding a stable topic ID. Search before adding; extend an existing
  entry with independent evidence instead of duplicating it. Split topics only
  when needed. Keep entries around 100–250 words, with minimal illustrative
  snippets rather than disassembly dumps. Name image and address (overlays reuse
  RAM addresses). Preserve contradictory evidence and narrow or retire claims.
- A matched function need not yield a new rule. Keep one-function debugging
  details in its attempt sidecar; promote only reusable lessons. Resolved
  hypotheses should link their replacement, retaining useful failed approaches.
- Freeze the actual recipe in each entry: compiler release, cc1 flags, maspsx
  flags/version or pinned dependency reference, assembler flags and relevant ABI/
  type assumptions. A link to mutable `config/compiler.json` alone is insufficient.
  Never generalize one object's settings to libraries or other images.

Seed entries use the repository's old-gcc release 0.17 and maspsx pin
`4ffa8b5106f9780d5d4aa7609f184949433ddbbc` in `tools/compilers.lock.json`.
The historical reviews establish the cited outcomes; they are not new validation
runs. Later findings must record their own settings rather than inherit this pin.

## Worker proposal (`<attempt>/knowledge.md`)

Write incrementally so timeouts preserve discoveries. Use this compact format;
omit empty discovery fields for an explicit no-new-insight outcome:

```markdown
# IMAGE:0xADDRESS — pending supervisor review
Consulted: SCHED-001 (or none relevant); prior attempt path if used.
Outcome: object matched / deferred / interrupted; No new reusable insight if so.
Recipe: exact compiler release, cc1, maspsx and assembler flags/pins.
Proposed topic / existing ID: ...
Observation and scope: ...
Before C → after C: minimal excerpts; identify illustrative simplifications.
MIPS: minimal differing instructions, addresses; target versus candidate.
Evidence: source/context, numbered trial and diagnostic paths.
Failed approaches: variation → observed difference, not just a trial count.
Uncertainty / next experiment: ...
```

## Supervisor review

After each manual result or automatic batch, inspect that batch's attempt notes
and results, including deferrals/interruption. Check the claimed C, instruction
differences and recipe. Promote findings only after exact containing-file
acceptance; keep useful unsuccessful observations explicitly hypothetical.
Add the acceptance/commit or durable review reference and date to promoted entries.
Use a concise before/after C example and relevant MIPS where evidenced; explicitly
say when the historical record lacks a failed candidate rather than inventing it.

Curate topic files separately from matching commits and audit the documentation
diff. The runner archives sidecars; it does not semantically review or promote
them. The supervising Codex session owns that review. Mark reviewed sidecars
with `Review: promoted <IDs> / duplicate <IDs> / function-specific / no new insight`
and a documentation commit when available, so the next session can resume pending
work. Do not delete ignored archives or make them the sole home of reusable
findings. No new match or savings percentage is implied by documentation updates.

## Review records

See the [session review inventory](reviews/README.md) for completed note reviews.
Each session record preserves reviewed attempt outcomes, decisions, evidence
references and useful function-specific failed approaches. The 2026-11-25 review
covered 27 notes, followed by four notes from the interrupted batch
`1795560864174034000`. Attempts without notes are valid and are silently
skipped; they do not create pending review work.
