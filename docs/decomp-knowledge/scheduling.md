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
