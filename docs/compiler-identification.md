# Matching build and compiler evidence

The USA boot executable now rebuilds byte for byte, including its PS-X EXE header and data. Five functions are compiled from reconstructed C; all other executable code remains assembly. This is the first matching milestone, not a complete game decompilation.

## Repeat the build

```sh
make toolchain       # only needed to prepare missing local tools
make                 # split, compile/assemble, link, and compare every byte
make compiler-matrix # repeat the 360 compiler/flag/assembler/probe trials
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

PsyQ 3.7.0 is a useful SDK lead, not a guarantee of one compiler for all game objects and libraries. Different translation units, linked libraries and handwritten routines may require different settings.

## Empirical results

`tools/compiler_matrix.py` tests three compilers × four optimization levels (`O0`–`O3`) × two small-data limits (`G0`, `G8`) × three maspsx ASPSX modes (2.21, 2.34, 2.56) × five reconstructed functions: **360 trials**.

| Candidate | Best exact matches among the five C reconstructions |
| --- | ---: |
| PSX GCC 2.7.2 | 5/5 |
| CDK/Cygnus GCC 2.7.2 | 3/5 |
| PSX GCC 2.8.1 | 1/5 |

The working recipe, stored in `config/compiler.json`, is:

```text
cc1:       -O2 -G0 -mips1
maspsx:    --aspsx-version 2.56 -G0
GNU as:    -EL -march=r3000 -mtune=r3000 -mabi=32 -O1 -G0 -no-pad-sections
```

**Unresolved:** `-O3` also matches all five functions. Other assembler modes not tested may produce the same results. A different source/type reconstruction can change a candidate's score, so the table does not conclusively exclude another compiler. We have selected an evidence-backed working build recipe, not uniquely recovered every original option.

330 trials reached byte comparison. Thirty G8 trials failed to link the `clear_globals` reconstruction with its assumed independent scalar declarations: their GP-relative addresses exceed the available signed displacement. Those failures are recorded separately from byte mismatches. G8 trials preserve positive `.extern` size metadata through a common-symbol shim for maspsx; unknown-size arrays are not treated as small data. These results depend on the proposed declarations and do not establish a project-wide `-G0` policy. The retained assembly also contains a small group of GP-relative stores around `0x80020F00` that needs separate analysis.

Full scores, failed commands, source hashes and trial artifacts are under ignored `reports/compiler-matrix.json` and `build/compiler-probes/`.

## First five C functions

| Address | Source | Bytes |
| --- | --- | ---: |
| `0x80013B30` | `src/queue_call.c` | 48 |
| `0x80014388` | `src/poll_loop.c` | 88 |
| `0x8001787C` | `src/sdk_calls.c` | 48 |
| `0x8001B59C` | `src/clear_globals.c` | 24 |
| `0x8001B5B4` | `src/table_exchange.c` | 28 |

These total **236 bytes of compiled C functions**, linked into their original places by Splat's generated linker script. Types and descriptive filenames remain provisional. Retained assembly and SDK signature matches are not counted as reconstructed C. No full-game percentage is asserted because overlay code and the denominator still need mapping.

## Reproducible local tools

All new tools are under `tools/`. `tools/install_mips.py` verifies archive checksums, installs native Apple Silicon compiler candidates, pins maspsx commit `4ffa8b5106f9780d5d4aa7609f184949433ddbbc`, and builds GNU binutils 2.42 targeting `mipsel-linux-gnu` into `tools/toolchain/`. The GNU source archive SHA-256 was recorded from the direct HTTPS download and is pinned in `tools/binutils.lock.json`; compiler archives are checked against their upstream GitHub release digests in `tools/compilers.lock.json`.

The host build uses `-O2 -std=gnu17` for C and `-O2 -std=c++17` for C++. `tools/binutils-macos.patch` prevents the bundled zlib from confusing modern Apple headers with classic Mac OS. These are host-tool build accommodations, not flags used to compile game C. No host packages or shell profiles were changed. Ghidra remains in Docker.

## Next discrimination work

Expand the sample to functions with signed/unsigned byte accesses, division, switches, floating point, pointer arithmetic, inlining opportunities and more complex loops. Recover types from callers, compare the same settings across independent objects, and retain ties when the samples cannot distinguish them. Map and rebuild overlays before assuming the boot executable's recipe applies to them. Every integrated C change must continue to pass the full-file comparison.
