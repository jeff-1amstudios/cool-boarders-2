# SDK types in the saved Ghidra program

Read-only audit on 10 November 2026, using the accepted PsyQ 3.7.0 catalog and the existing pinned Docker image. No SDK types or prototypes were applied or changed by this audit. It checked all 15 mapped payloads against their pinned originals. This establishes analysis provenance, not a rebuilt-file match or new C coverage.

The plugin contains `/opt/ghidra/Ghidra/Extensions/ghidra_psx_ldr/data/psyq370.gdt`. Its analyzer loads this archive and applies function definitions by name after SDK identification. The archive contains 144 structures and 985 function definitions, among 2,133 total type entries. Ghidra's Data Type Manager exposes the archive as `psyq370`; imported types also appear beneath the program, grouped into header categories such as `LIBGPU.H`, `LIBGTE.H` and `LIBCD.H`.

The saved program contains 63 structures. All 63 are equivalent to their corresponding archive definitions. Another 81 archive structures are absent at their archive paths in the program; this does not imply failed application because types can remain in the archive until needed. The audit does not search for renamed equivalents elsewhere.

| Type | Bytes | Saved definition agrees |
| --- | ---: | --- |
| RECT | 8 | Yes |
| DRAWENV | 92 | Yes |
| DISPENV | 20 | Yes |
| VECTOR | 16 | Yes |
| SVECTOR | 8 | Yes |
| MATRIX | 32 | Yes |
| POLY_FT4 | 40 | Yes |
| SPRT | 20 | Yes |
| CdlFILE | 24 | Yes |

Among the 1,047 accepted SDK function instances, 339 have matching stored return/parameter types and variadic flags. Two more, `DecDCTvlc2` in ENDING and MODE_SEL, differ only because the archive uses the array typedef `DECDCTTAB` while the saved parameter is an element pointer: ordinary C parameter adjustment. One function with an available archive prototype lacks its stored signature: resident `bcmp` at `0x80045300`. Review and apply its archive definition in a separate analysis-repair pass.

The remaining 705 catalog entries have no same-name function definition in the archive. They include generated SDK internal-entry names and public names such as `AverageSZ3`, `AverageSZ4` and `ClipF`. They must not all be reported as failed imports or assigned guessed public signatures. No accepted catalog entry is missing a saved function object.

Examples of correctly applied public prototypes include `DrawSync`, `VSync`, `PutDrawEnv`, `PutDispEnv`, `LoadImage`, `SetDefDrawEnv` and `CdSearchFile`. `SetDefDrawEnv` has four arguments in `a0` through `a3` and its fifth at entry `sp + 16`. The saved calling-convention label is `__stdcall`; this name in the PSX language does not imply x86 stack argument passing. Inspect actual storage.

The reconstruction worker now receives fresh Ghidra pseudocode, relevant type declarations and named callee prototypes with signature provenance. Accepted authored types, prototypes and reviewed global typings are saved back into the project before the next attempt. See [the reconstruction workflow](reconstruction-worker.md). The former provisional C layouts were removed in the fresh-start reset; new reconstructions should use existing SDK definitions as their starting evidence.

## Reproduce

With Docker Desktop running and the saved project closed in Ghidra:

```sh
tools/.venv/bin/python tools/audit_sdk_types.py
```

The runner clears old output, copies the accepted catalog into ignored `reports/sdk-types/`, and runs `AuditSdkTypes.java` with `-readOnly -noanalysis`. It requires a fresh hash-checked completion manifest because Ghidra can return success after a script error. The generated `audit.md` summarizes results; `audit.json` contains type layouts, stored prototypes, parameter storage and complete per-instance findings. Downloaded archives and generated type dumps remain uncommitted.

This checks stored program signatures, not decompiler-inferred local signatures. Type agreement does not prove ABI storage, library identity, compiler settings or original game source types. The worker's exact containing-file comparison remains the matching acceptance gate.
