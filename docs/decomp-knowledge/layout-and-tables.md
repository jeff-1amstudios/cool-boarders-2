# Layout and generated tables

## LAYOUT-001 — Splat text ownership for explicit switch tables

Verified configuration finding, 2026-11-26. Scope: COMB.BIN with the repository's
pinned Splat dependency; not a change to compiler selection or game semantics.

A 100% normalized text diagnostic can omit a compiler-emitted `.rodata` table.
Leaving the original words in retained data duplicates the table and shifts code.
Carve its measured original extent into a same-name `.rodata` subsegment with
`linker_section_order: .data`, retaining explicit data boundaries around it.

With `section_order: [.rodata, .text, .data, .bss]`, Splat's
`CommonSegCode.parse_subsegments` can replace the function's base segment with
its later same-name table. The reference linker then requests nonexistent
`src/overlays/comb/func_*.s` and can place the table twice. Changing only the
assembler path would mask the ownership error.

Use `.text` before `.rodata`, and explicitly order COMB's retained four-byte
`rodata/header` in `.text`. The header still precedes code; tables stay in `.data`.
No authored C or compiler recipe change is required. Configuration commit
`c92ccc9` passes the pinned full COMB comparison on main. The unchanged saved C
for COMB.BIN:0x800608D4, COMB.BIN:0x80060940 and COMB.BIN:0x80061E48 also passes
individually with the measured table splits: 308128 bytes, zero differences.
Those checks initially validated isolated candidate branches only. All three are
now integrated with transactional Ghidra feedback; the subsequent all-15-image
gate passes. See the [acceptance follow-up](reviews/1795625601628029000.md).

Recipe: old-gcc release 0.17, gcc-2.7.2-psx `-O2 -G0 -mips1`; maspsx
`4ffa8b5106f9780d5d4aa7609f184949433ddbbc`, `--aspsx-version 2.56 --expand-div -G0`;
GNU as `-EL -march=r3000 -mtune=r3000 -mabi=32 -O1 -G0 -no-pad-sections`.
See the [durable validation and review record](reviews/1795595998382409000.md).
