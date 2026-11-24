# Types and pointer arithmetic

## TYPE-001 — Verified: aligned word view versus SDK struct assignment

Scope: `SCUS_943.58:0x80016010`, accepted `src/main.c` block and
[transfer review](../deferred-function-review.md#two-newest-deferrals--batch-1794470751902582000).
The review records exact EXE and all-15-file validation under unchanged settings.
The source comment records the unsuccessful direct SDK struct copy.

```c
/* Before: emits unaligned load/store pairs with this recipe. */
request->positions[0] = disc_file_records[file_index].pos;
/* After: matching aligned four-byte representation copy. */
*(unsigned long *)&request->positions[0] =
    *(unsigned long *)&disc_file_records[file_index].pos;
```

Relevant target (`target.s` under `reports/decomp/newest-deferred/80016010/`):

```mips
/* 800160C8 */ lw $v0, -0x7F58($at)
/* 800160CC */ nop
/* 800160D0 */ sw $v0, 4($a1)
```

Check evidenced alignment and representation when an aggregate assignment emits
`lwl/lwr` or `swl/swr` instead of `lw/sw`. Keep the recovered `CdlLOC` fields;
a matching scalar view does not prove the original type was scalar. This is a
PS1 recipe-specific representation technique requiring four-byte `unsigned long`
and aligned addresses, not a portable aliasing recommendation. Pointer stride
and signedness claims require their own comparisons; do not infer them from this.

Recipe: old-gcc 0.17 `gcc-2.7.2-psx`, cc1 `-O2 -G0 -mips1`; maspsx
`--aspsx-version 2.56 --expand-div -G0`; GNU as `-EL -march=r3000 -mtune=r3000
-mabi=32 -O1 -G0 -no-pad-sections`. Seeded from the existing review on 2026-11-23.

Independent aggregate-copy evidence, reviewed 2026-11-25: accepted BP.BIN
functions `0x80093A94`, `0x80093D74`, `0x80093E9C`, `0x800944E8`,
`0x8009456C` and `0x80094868` match with ordinary SDK aggregate assignments.
SVECTOR copies use unaligned pairs; MATRIX, SPRT, DR_MODE and GsF_LIGHT use
aligned word copies. Their acceptance commits and full-file records are in
the [batch review](reviews/1795538906012286000.md); recipe is the same as above,
with maspsx pin `4ffa8b5106f9780d5d4aa7609f184949433ddbbc`.
These first-candidate successes qualify the technique: retain direct typed
copies when they already reproduce the reference; scalar views are a targeted
response to evidenced alignment differences, not a prerequisite for matching.

## TYPE-002 — Verified: widen a local without changing its stored field

Scope: `BP.BIN:0x80094910`, accepted commit
`f821670746bd4c7469da38913d927dd1f13aa837`,
[2026-11-25 review](reviews/1795538906012286000.md). Archived validation records
the entire BP.BIN identical to pinned SHA-256
`b5422780197f468835494e495079871823470f9fe314f2666d1e32c431ddb797`.

```c
/* Worker-reported first candidate: */
unsigned short count = frame->command_count;
/* Accepted; command_count remains an unsigned-short struct field: */
unsigned int count = frame->command_count;
```

The accepted C loads directly into the saved counter with
`lhu s1,2(a1)` at `0x80094940`. The worker reported an extra `andi ...,0xffff`
and move with the narrow local; the original intermediate diagnostic is no
longer saved. The final C and load are independently inspectable. When a narrow
field feeds a word-sized loop, distinguish the field's layout from the local's
arithmetic width before changing shared types. This proves a viable local type
for this function, not a universal rule that short locals require masking.

Recipe: decompals/old-gcc 0.17 `gcc-2.7.2-psx`, cc1 `-O2 -G0 -mips1`;
maspsx pin `4ffa8b5106f9780d5d4aa7609f184949433ddbbc`,
`--aspsx-version 2.56 --expand-div -G0`; GNU as
`-EL -march=r3000 -mtune=r3000 -mabi=32 -O1 -G0 -no-pad-sections`.
