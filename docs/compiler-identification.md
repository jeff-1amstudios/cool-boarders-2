# Matching build and compiler evidence

Historical compiler evidence from before the 10 November 2026 fresh-start reset. The current checkout has zero reconstructed C functions; the source paths and coverage counts below describe earlier experiments. The provisional compiler recipe was preserved.

The USA boot executable now rebuilds byte for byte, including its PS-X EXE header and data. Thirty-five functions (1,560 bytes) are compiled from reconstructed C after resolving the deferred loop, completing the next ten targets and integrating a subsequent automatic ten-function batch; all other executable code remains assembly. The compiler matrix below covers the 25-function probe set from the preceding batch; the latest ten have full-file validation but have not been added to that matrix. This establishes a matching boot rebuild, not a complete game decompilation.

## Repeat the build

```sh
make toolchain       # only needed to prepare missing local tools
make                 # split, compile/assemble, link, and compare every byte
make compiler-matrix # repeat the 1,800 compiler/flag/assembler/probe trials
```

The reference is `orig/disc/files/SCUS_943.58`. Its pinned SHA-256 and the rebuilt file's SHA-256 are both:

```text
5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4
```

The file size is 266,240 bytes. Build output is `build/SCUS_943.58`; comparison details are in ignored `reports/boot-rebuild.json`. The build checks the reference's fingerprint first and returns a failure on any size or byte difference. It assembles retained code and compiles each C translation unit before linking; it does not copy the reference executable into the output.

## What identifies the compiler?

These tools provide evidence and comparison, rather than a universal compiler/flag detector:

- **Ghidra's PSX/PsyQ signatures** identify known SDK routines and suggest library versions. The actual stored Program Information property is `PsyQ Version = 3.7.0`; the analyzer's separate fallback preference is 4.7.0. The fallback is not used as evidence of this game's version.
- **[old-gcc](https://github.com/decompals/old-gcc)** supplies historical compiler candidates. The staged candidates are PSX GCC 2.7.2, CDK/Cygnus GCC 2.7.2, and PSX GCC 2.8.1, from release 0.17.
- **[maspsx](https://github.com/mkst/maspsx)** translates period compiler assembly for GNU as and models selected ASPSX behaviors. Its version-mode option is experimental compatibility behavior, not proof that the original assembler had that exact version.
- **The project's compiler matrix** compiles proposed C, links it at the original address with known external addresses, and compares the entire function's bytes. Candidate selection and C/type reconstruction still need analysis.
- **[decomp-permuter](https://github.com/simonlindholm/decomp-permuter)** can search C source variations against a target once a compile command exists. It is not installed here yet and does not by itself establish the original compiler/flags.

We accept the PSX plugin's **PsyQ 3.7.0** detection for the boot executable. Its existing SDK identifications are recorded in `config/psyq-sdk.json` and excluded from the reconstruction queue, including `DrawSyncCallback` at `0x800296F0`. This uses the plugin's result directly; no independent version/signature audit is required for this selection policy. It does not establish one compiler for all game objects and libraries. Different translation units, linked libraries and handwritten routines may require different settings.

## Empirical results

`tools/compiler_matrix.py` tests three compilers × four optimization levels (`O0`–`O3`) × two small-data limits (`G0`, `G8`) × three maspsx ASPSX modes (2.21, 2.34, 2.56) × 25 reconstructed functions: **1,800 trials**.

| Candidate | Best exact matches among the 25 C reconstructions |
| --- | ---: |
| PSX GCC 2.7.2 | 25/25 |
| CDK/Cygnus GCC 2.7.2 | 13/25 |
| PSX GCC 2.8.1 | 6/25 |

The working recipe, stored in `config/compiler.json`, is:

```text
cc1:       -O2 -G0 -mips1
maspsx:    --aspsx-version 2.56 -G0
GNU as:    -EL -march=r3000 -mtune=r3000 -mabi=32 -O1 -G0 -no-pad-sections
```

**Unresolved:** `-O3` also matches all 25 functions. Among the tested combinations, only PSX GCC 2.7.2 with G0 and maspsx 2.56 reaches 25/25, at either O2 or O3. Other assembler modes not tested may produce the same results. A different source/type reconstruction can change a candidate's score, so the table does not conclusively exclude another compiler. We have selected an evidence-backed working build recipe, not uniquely recovered every original option.

1,620 trials reached byte comparison. Another 180 G8 trials failed linking with GP displacement errors across six reconstructions (`flag_acquire`, `flag_release`, `fill_slots`, `clear_globals`, `two_command_calls`, `copy_bounded_name`): with the proposed scalar declarations, some GP-relative addresses exceed the available signed displacement. Those failures are recorded separately from byte mismatches. G8 trials preserve positive `.extern` size metadata through a common-symbol shim for maspsx; unknown-size arrays are not treated as small data. These results depend on the proposed declarations and do not establish a project-wide `-G0` policy. The retained assembly also contains a small group of GP-relative stores around `0x80020F00` that needs separate analysis.

Before the reset, full scores and trial artifacts were recorded under ignored `reports/compiler-matrix.json` and `build/compiler-probes/`. The earlier five-function matrix scored 5/5, 3/5 and 1/5 respectively; the expanded sample strengthens the PSX 2.7.2 lead but preserves the O2/O3 tie. Selection remains source-dependent because these C reconstructions were developed against the working recipe. The old matching sources, generated trial artifacts and reconstruction notes were cleared in the 10 November fresh-start reset; Git history retains the earlier experiments.

## First five C functions

| Address | Source | Bytes |
| --- | --- | ---: |
| `0x80013B30` | `src/main.c` (`queue_call`) | 48 |
| `0x80014388` | `src/main.c` (`poll_loop`) | 88 |
| `0x8001787C` | `src/main.c` (`sdk_calls`) | 48 |
| `0x8001B59C` | `src/main.c` (`clear_globals`) | 24 |
| `0x8001B5B4` | `src/main.c` (`table_exchange`) | 28 |

These total **236 bytes of compiled C functions**, linked into their original places by Splat's generated linker script. Types and descriptive filenames remain provisional. Retained assembly and SDK signature matches are not counted as reconstructed C. No full-game percentage is asserted because overlay code and the denominator still need mapping.

## Reproducible local tools

All new tools are under `tools/`. `tools/install_mips.py` verifies archive checksums, installs native Apple Silicon compiler candidates, pins maspsx commit `4ffa8b5106f9780d5d4aa7609f184949433ddbbc`, and builds GNU binutils 2.42 targeting `mipsel-linux-gnu` into `tools/toolchain/`. The GNU source archive SHA-256 was recorded from the direct HTTPS download and is pinned in `tools/binutils.lock.json`; compiler archives are checked against their upstream GitHub release digests in `tools/compilers.lock.json`.

The host build uses `-O2 -std=gnu17` for C and `-O2 -std=c++17` for C++. `tools/binutils-macos.patch` prevents the bundled zlib from confusing modern Apple headers with classic Mac OS. These are host-tool build accommodations, not flags used to compile game C. No host packages or shell profiles were changed. Ghidra remains in Docker.

## Next discrimination work

Expand the sample to functions with signed/unsigned byte accesses, division, switches, floating point, pointer arithmetic, inlining opportunities and more complex loops. Recover types from callers, compare the same settings across independent objects, and retain ties when the samples cannot distinguish them. Map and rebuild overlays before assuming the boot executable's recipe applies to them. Every integrated C change must continue to pass the full-file comparison.

## Deferred text sprite builder (`0x80012C30`)

The fresh-start reconstruction of the 624-byte text sprite builder matches with the unchanged PSX GCC 2.7.2 recipe above. The preserved flat implementation matched the sprite loop but compiled to 620 bytes: GCC moved the final packet advance past the first `GetGraphType` call, filled the conditional branch delay slot with that advance, and shared the command constant load. The reference advances the cursor before the call and loads `0xE1000000` in both paths.

The matching C uses `static __inline__ append_draw_tpage`, with an explicit `DR_TPAGE *next = packet + 1` local, and assigns the helper's result back to the caller's `packet` variable before returning it. Returning the helper call directly, advancing its parameter with `packet++`, or flattening the helper back into the caller does not produce the same code. This is evidence that inline expansion and return-value handling can determine the instruction schedule even when the operations are equivalent; it does not uniquely establish the original source spelling. No inline assembly, register pinning, compiler change, flag change, reference change or binary patch is needed.

Focused object diagnostics report 100%. The containing 266,240-byte EXE compares exactly with the pinned reference (`5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4`), and `make validate` passes all 15 pinned files. The current fresh-start build contains 12 independently compiled boot C functions; the 14 overlay rebuilds still retain their assembly and data and do not add reconstructed overlay C.
