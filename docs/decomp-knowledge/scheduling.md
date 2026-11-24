# Scheduling and C constructs

## SCHED-001 — Verified: inline return cursor changes scheduling

Scope: `SCUS_943.58:0x80012C30`; historical exact EXE and all-15-file validation
documented in [compiler evidence](../compiler-identification.md#deferred-text-sprite-builder-0x80012c30).
Accepted C: `src/main.c`, `append_draw_tpage` and the annotated caller.

The flat implementation produced 620 instead of 624 bytes. Explicitly computing
the next cursor in a `static __inline__` helper and assigning its return to the
caller's cursor reproduced the target schedule. Simplified shape:

```c
/* Before: flat packet updates, or return helper(...) directly. */
/* After: helper body keeps this explicit local. */
DR_TPAGE *next = packet + 1;
/* packet writes and GetGraphType calls */
return next;
/* Caller: */
packet = (unsigned char *)append_draw_tpage(ot, (DR_TPAGE *)packet, tpage);
return packet;
```

Relevant target instructions (intervening calculations omitted):

```mips
/* 80012E00 */ addiu $s3, $s0, 8  # cursor before first call
/* 80012E18 */ jal GetGraphType
/* 80012E28 */ lui $v0, 0xE100   # conditional branch delay slot
/* 80012E2C */ jal GetGraphType
/* 80012E34 */ lui $v0, 0xE100
```

The flat form delayed cursor advancement and shared the constant load. Directly
returning the helper or using `packet++` inside it also failed. Try preserving
inline boundaries and return-value lifetimes when equivalent flat C mis-schedules;
this does not establish a general register-allocation rule.

Recipe: old-gcc 0.17 `gcc-2.7.2-psx`, cc1 `-O2 -G0 -mips1`, maspsx
`--aspsx-version 2.56 -G0` (current pipeline also uses `--expand-div`, irrelevant to
this division-free function); GNU as `-EL -march=r3000 -mtune=r3000 -mabi=32
-O1 -G0 -no-pad-sections`. Seeded from the existing review on 2026-11-23.

## SCHED-002 — Verified: source store order can change a short reload

Scope: `BP.BIN:0x80093514`, accepted commit `71287a23b46aa59b80479e60968011c7f5d11c4f`;
[2026-11-25 review](reviews/1795538906012286000.md). Its archived validation
compares all 283,488 bytes of BP.BIN identically against pinned SHA-256
`b5422780197f468835494e495079871823470f9fe314f2666d1e32c431ddb797`.

The worker reported a 372-byte candidate that preserved the X arithmetic result
and sign-extended it with `sll`/`sra`. Reordering the authored updates produced
the accepted 368-byte function, including `lh v1,8(s2)` at `0x800935F0` after
the width load. Simplified change:

```c
/* Before (worker-reported; intermediate diagnostics were overwritten): */
sprite->u0 = saved_u; sprite->v0 = saved_v;
sprite->x0 += advance_x;
/* Accepted: */
sprite->x0 += advance_x;
sprite->u0 = saved_u; sprite->v0 = saved_v;
/* Both subsequently compare the signed-short x0 against the wrap limit. */
```

The accepted machine stores still put U/V before X. Thus source order can
change value reuse/reloads even when final store order appears unchanged.
Try this only where independent field updates may safely move; this single
accepted function does not establish an optimizer rule or original source order.
Unsigned addition casts and narrowing advance_x did not resolve this case.

Recipe: decompals/old-gcc 0.17 `gcc-2.7.2-psx`, cc1 `-O2 -G0 -mips1`;
maspsx pin `4ffa8b5106f9780d5d4aa7609f184949433ddbbc`,
`--aspsx-version 2.56 --expand-div -G0`; GNU as
`-EL -march=r3000 -mtune=r3000 -mabi=32 -O1 -G0 -no-pad-sections`.
