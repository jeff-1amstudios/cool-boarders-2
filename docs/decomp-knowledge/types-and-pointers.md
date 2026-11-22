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
