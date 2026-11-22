# Register allocation and stack layout

## STACK-H001 — Hypothesis: buffer lifetimes influence stack-slot reuse

Historical unresolved observation, **not current queue status**:
`SCUS_943.58:0x80017470`, [November 13 review](../deferred-function-review.md#newest-deferrals-reviewed--2026-11-13).
Separate response-buffer scopes reduced the full EXE difference from 12 bytes
to two. Frame size, saved registers, mixer and second response buffer matched;
the first buffer still occupied the wrong slot:

```mips
# Candidate                 # Target
addiu $a2, $sp, 0x10        # addiu $a2, $sp, 0x18
lbu   $v0, 0x10($sp)       # lbu   $v0, 0x18($sp)
```

Reusable experiment shape (illustrative, not an accepted candidate):

```c
/* Before: both buffers live in the outer scope. */
unsigned char first[8], second[8];
/* After: isolate distinct call/response lifetimes in separate blocks. */
{ unsigned char response[8]; /* first polling path */ }
{ unsigned char response[8]; /* later polling path */ }
```

Scope changes improved this candidate but did not establish exact matching.
649 historical C/compiler trials included types/layouts and lifetime variations;
no alternate recipe was accepted. Do not repeat a broad flag sweep. Next:
consult newer attempts, then vary one buffer's lifetime/address escape while
holding types and recipe fixed; compare both buffer offsets and saved registers.
Trial artifacts: `reports/decomp/deferred-review-20261113/`.

Baseline recipe: old-gcc 0.17 PSX GCC 2.7.2 `-O2 -G0 -mips1`; maspsx
`--aspsx-version 2.56 --expand-div -G0`; GNU as `-EL -march=r3000 -mtune=r3000
-mabi=32 -O1 -G0 -no-pad-sections`. Other trial flags are not fully enumerated in
the durable review; inspect numbered trial records before asserting they failed.

## REG-H001 — Unresolved: frame and color-expression allocation interact

Historical `SCUS_943.58:0x800143E0` review left 33 full-file byte differences:
frame `0x22A8` versus target `0x22B0`, four color instructions using `a3` versus
`t0`, and add/move versus add/mask. Inline absolute value, signed texture-index
scaling and color assignment order improved the candidate without matching.
See [remaining candidate](../deferred-function-review.md#remaining-candidate).
Recipe baseline as above; the review also tried available GCC 2.7.2/2.8.1,
O2/O3 and scheduling/CSE/frame-pointer variations without an accepted replacement.
No general rule that declaration order selects registers or padding repairs
frames is established. Next: check later resolution records, then isolate the
color expression's width/masking and live ranges under the fixed recipe. Exact
failed C must be recovered from trial archives, not reconstructed from this note.

Both hypotheses seeded 2026-11-23; recheck later evidence before retrying.
