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

## REG-H002 — Resolved by REG-002: packet grouping and scratch reuse

Historical `BP.BIN:0x8008510C` candidates scored 99.641% with a saved-register
permutation. The table pointer's allocation priority (2697) fell below two
blink predicates (2702/2941). A delayed alias raised its priority but added a
move. Grouping the first page construction in `do { ... } while (0)` raised its
weighted reference count from 23 to 26, fixing the saved registers without
additional instructions. The resulting 99.868% candidate still swapped
`v0`/`v1` in eight first-packet instructions. A typed scratch union resolved that
last conflict; see REG-002. Earlier failed trials remain documented under
`reports/decomp/{rtl-20261124-8008510C,priority-search-8008510C}/`.

## REG-002 — Verified: typed scratch reuse changes local/global allocation

Scope: `BP.BIN:0x8008510C`, 2026-11-24. The entire 283,488-byte `BP.BIN` matches
pinned SHA-256 `b5422780197f468835494e495079871823470f9fe314f2666d1e32c431ddb797`.
Accepted C is the annotated block in `src/overlays/bp.c`; `BpTagScratch` is
shared once in `src/overlays/bp.h`. Matching source/analysis commit: `e63b94b`.
No compiler recipe change was needed.

In the preceding packet-block candidate, LLDB inspection of the unmodified
compiler confirmed a local-allocation tie: the old primitive tag and masked OT
value each scored 60000. Earlier quantity numbering gave the old tag `v0`,
which then conflicted with the global command result. Names, literal casts and
ordinary temporary copies did not resolve this.

The matching C reuses a union's word view for command constants in both GPU
branches, then its `PrimitiveTag` view for updating the primitive's address:

```c
scratch.value = command_bits;
page_code = scratch.value | (tpage & mask);
/* ... */
scratch.value = page->tag;
scratch.tag.address = ordering_table->p;
page->tag = scratch.value;
```

RTL now treats the scratch as a global quantity across branches. It takes
`v1`; the command quantity regains its `v0` preference. This validates one viable
source representation, not the original variable names or macro spelling.
Renaming the union member `word` to `value` left the entire object unchanged.

Recipe: old-gcc 0.17 PSX GCC 2.7.2 `-O2 -G0 -mips1`; maspsx pin
`4ffa8b5106f9780d5d4aa7609f184949433ddbbc`, `--aspsx-version 2.56 --expand-div -G0`;
GNU as `-EL -march=r3000 -mtune=r3000 -mabi=32 -O1 -G0 -no-pad-sections`.
Full diagnostics and comparison evidence:
`reports/decomp/link-quantity-8008510C/knowledge.md` and sibling validation logs.

## REG-W001 — Workflow: use RTL to diagnose register-only mismatches

Use RTL when instruction shape/size already align but register choices do not.
For `BP.BIN:0x8008510C`, comparing the original candidate against a delayed alias
was the discriminating step after many equivalent C rewrites failed. Preserve
both exact C candidates and compile each under the same pinned recipe. Request
GCC 2.7.2 dumps from its generated preprocessed input, for example:

```sh
tools/compilers/gcc-2.7.2-psx/cc1 -quiet -O2 -G0 -mips1 -da \
  -o reports/<attempt>/function.s reports/<attempt>/function.i
```

Keep all generated dumps under ignored analysis output. Select the target
function's section: early dumps can also contain unused inline helpers.
Compare `.flow`/`.lreg` reference counts and live lengths, `.sched` instruction
ordering, and `.greg` conflicts, hard-register preferences and assignments.
Check whether a proposed C change actually alters those inputs before doing
more source permutations. The delayed alias explained the saved-register
priority change, but its extra move made it unsuitable as the final source.

When RTL did not expose local allocation's combined quantities, batch LLDB
tracing of the **unmodified installed compiler** revealed a priority tie and
its quantity-number tie-break. That directed the successful typed-scratch
reuse in REG-002. Trace copies and lifetimes, rather than guessing local names
or declaration order. Do not patch the allocator or retune the compiler to
force a result. RTL/object diagnostics explain candidates; exact containing-file
comparison remains the acceptance gate.

## REG-003 — Verified: indexed traversal can change saved-register allocation

Scope: `BP.BIN:0x8009479C`, accepted commit
`88aacb07e3501280ff2765371c399b5d649e558f`,
[2026-11-25 review](reviews/1795538906012286000.md). The per-attempt full-file
record reports BP.BIN identical to pinned SHA-256
`b5422780197f468835494e495079871823470f9fe314f2666d1e32c431ddb797`.

```c
/* Worker-reported previous second loop: */
for (i = 0; i < object_count; i++, objects++)
    if (objects->pmd) func_800940A8(objects);
/* Accepted second loop: */
for (i = 0; i < object_count; i++)
    if (objects[i].pmd) func_800940A8(&objects[i]);
```

The accepted indexed form is still strength-reduced to an advancing pointer:
`addu s0,a1,zero` at `0x800947A4`, counter initialization in s1 at
`0x800947B4`, `lw v0,8(s0)` at `0x80094824`, and `addiu s0,s0,0x18`
at `0x80094848`. The worker reported these pointer/counter registers exchanged
in the earlier form; its intermediate diagnostics were overwritten. Declaration
reordering did not fix that permutation, and indexing the first initialization
loop changed its instruction shape. Vary the source representation of the
specific problematic traversal, preserving unaffected loops and SDK field types.
Use REG-W001 to inspect lifetime/priority differences; indexing does not prescribe
which register another function will receive.

Recipe: decompals/old-gcc 0.17 `gcc-2.7.2-psx`, cc1 `-O2 -G0 -mips1`;
maspsx pin `4ffa8b5106f9780d5d4aa7609f184949433ddbbc`,
`--aspsx-version 2.56 --expand-div -G0`; GNU as
`-EL -march=r3000 -mtune=r3000 -mabi=32 -O1 -G0 -no-pad-sections`.
