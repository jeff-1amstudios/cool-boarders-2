# Optimization and toolchain behavior

## OPT-001 — Verified: assembler division expansion can explain extra guards

Scope: `SCUS_943.58:0x80013654` (636 bytes, four guarded divisions).
`config/compiler.json` records exact full-EXE validation with 23 resident C
functions establishing `--expand-div` for this function. Accepted C is in
`src/main.c`. Do not remove arithmetic safety sequences by changing C semantics
before checking the compiler-to-assembler pipeline.

The existing record establishes a tool-setting change, not a C rewrite:

```c
/* Before and after: signed integer division in the accepted C. */
quotient = numerator / denominator; /* illustrative expression */
```

Relevant expansion in `build/src/func_80013654.gas.s` (registers for one site):

```mips
div   $zero, $3, $2
bnez  $2, nonzero
nop
break 7
nonzero:
# Additional INT_MIN / -1 guard includes break 6 before mflo.
```

Recipe: old-gcc 0.17 `gcc-2.7.2-psx`, cc1 `-O2 -G0 -mips1`; maspsx
`--aspsx-version 2.56 --expand-div -G0`; GNU as `-EL -march=r3000 -mtune=r3000
-mabi=32 -O1 -G0 -no-pad-sections`. See pinned maspsx dependency in
`tools/compilers.lock.json` (`4ffa8b5106f9780d5d4aa7609f184949433ddbbc`)
and [compiler evidence](../compiler-identification.md).
Original compiler/ASPSX version and project-wide flags remain provisional;
O2/O3 ties in independent probes must remain ties. Historical failed output
without expansion is not reproduced here. Seeded from existing evidence 2026-11-23.
