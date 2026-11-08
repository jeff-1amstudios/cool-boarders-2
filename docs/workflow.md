# Tool choices and workflow

Research date: 9 November 2026. No existing Cool Boarders 2 decomp/recomp repository was found in the searches performed; this is not proof that none exists.

## Toolchain

| Purpose | Choice | Status / rationale |
| --- | --- | --- |
| Preserve and verify a disc | [redumper](https://github.com/superg/redumper) + [Redump](https://wiki.redump.info/Dumping_Guides) | Acquisition option; requires a suitable drive. Not installed. |
| Extract and later rebuild disc layout | [dumpsxiso / mkpsxiso](https://github.com/Lameguy64/mkpsxiso) | Built locally for arm64. Preserves file order/LBAs in XML; XA/CDDA-aware. Whole-disc identity must still be checked. |
| Split executable and disassemble MIPS | [splat](https://github.com/ethteck/splat) + [spimdisasm](https://github.com/Decompollaborate/spimdisasm) | Installed in `tools/.venv`; versions locked. Splat can seed a PS-X EXE config. |
| Static analysis and SDK recognition | [Ghidra](https://github.com/NationalSecurityAgency/ghidra) + [ghidra_psx_ldr](https://github.com/lab313ru/ghidra_psx_ldr) | Docker image: Ghidra 12.1.4 + matching PSX extension 2026.10.03, with Linux ARM64 native components. See [launcher and setup](ghidra-docker.md). |
| Runtime debugging | [PCSX-Redux](https://github.com/pcsx-redux/pcsx-redux) | Recommended next installation for stepping, breakpoints and RAM/VRAM inspection. Use a known-good emulator run as the behavior baseline. |
| Reproduce original compiler output | [old-gcc](https://github.com/decompals/old-gcc) + [maspsx](https://github.com/mkst/maspsx) + MIPS little-endian GNU binutils | Installed locally: binutils 2.42, three old-gcc candidates, pinned maspsx. Five C functions match with the provisional recipe in `config/compiler.json`; see [compiler evidence](compiler-identification.md). |
| Match individual functions | [asm-differ](https://github.com/simonlindholm/asm-differ), optionally [decomp.me](https://github.com/decompme/decomp.me) | Choose after the local compile/link loop works. Splat's assembly and linker output are useful inputs. |
| Native recomp experiment | [RetroPortingToolKit/psxrecomp](https://github.com/RetroPortingToolKit/psxrecomp) | Preferred first experiment: documented macOS ARM64 toolchain and a per-game setup workflow. Beta; this game's support has not been tested. |

The original compiler cannot be replaced with a modern homebrew SDK when the goal is matching machine code. Identify SDK/library code first so game logic and SDK routines can be measured separately. old-gcc publishes macOS build scripts/releases; verify each downloaded binary's architecture rather than assuming every historical compiler runs natively on ARM64.

## Milestone 1: establish the reference

Import one disc with `tools/disc.py`. Compare its track hashes against the correct Redump entry. Read SYSTEM.CNF instead of guessing which file boots. Inspect the EXE entrypoint, load address, GP, payload and stack fields. Inventory additional executables, archive containers, streamed data, and files that may contain overlays.

Boot the untouched image. Record region, BIOS/emulator version, title screen, a complete race, audio, inputs, and memory-card save/reload. These observations become the regression reference. Do not infer gameplay compatibility from reaching the title screen.

## Milestone 2: map code and achieve an unchanged rebuild

Load the boot EXE into Ghidra using the PSX loader. Apply PsyQ recognition and label the startup path, CD reads, memory allocation, frame loop, rendering, audio and input calls. Watch runtime loading to discover code written to reused RAM addresses. Raw/compressed overlays may have no PS-X EXE header and will not be identified by the initial importer.

Generate a Splat configuration. Review every estimated code/data boundary; separate executable header, code, rodata, data, small data, and zero-initialized regions using observed evidence. Preserve file offsets and virtual addresses, especially GP-relative data. Each overlay needs its own load address and split configuration.

Assemble and link the unchanged disassembly with matching binutils/assembler behavior. Compare the resulting executable with the original byte for byte. Only then establish the period compiler version, optimization flags, small-data settings and ABI on small representative functions. If ordinary GCC assumptions fail, investigate other compiler families before forcing matches.

## Milestone 3: replace functions incrementally

Start with leaf functions with clear inputs and few hardware dependencies. Recover types from callers, convert one function to C, compile with the pinned compiler, and compare instructions and relocations. Rebuild and run the game after coherent changes. Report byte/function matching with a documented denominator; distinguish SDK matches, handwritten C, retained assembly and unanalysed overlays.

## Parallel option: native recompilation

Static recompilation translates MIPS into host code and still needs PS1 hardware behavior. It can precede a complete readable C decomp. PSXRecomp documents an interpreter fallback for unresolved code, so measure which execution paths actually run as compiled code.

Once the image is identified, keep the experiment separate from the matching tree:

```sh
git clone --recurse-submodules https://github.com/RetroPortingToolKit/psxrecomp.git tools/vendor/psxrecomp
mkdir -p build/recomp
sh tools/vendor/psxrecomp/tools/new_project_layout/setup_project.sh \
  --disc '/absolute/path/Cool Boarders 2.cue' --dir "$PWD/build/recomp"
```

Consult the checked-out [game project setup guide](https://github.com/RetroPortingToolKit/psxrecomp/blob/master/docs/GAME_PROJECT_SETUP.md) for prompts, required tools and current flags. Pin that framework commit in the generated game's repository. The framework documents Git, Python, CMake, Ninja and a C/C++ compiler, plus SDL3; CMake and SDL3 are present on this machine but Ninja was not found during initial inspection. The bootstrap in this repository installs the analysis tools only.

OpenBIOS is the framework's documented default, with a compatible dumped retail BIOS optional; game-specific incompatibilities may change that requirement. No Cool Boarders 2 runtime build has been attempted. Verify a full race, all courses, tricks, replay, sound/streaming and save/reload before adding widescreen or changing frame timing.

[PS1Recomp/ps1-recomp](https://github.com/PS1Recomp/ps1-recomp) is a separate alternative with a Ghidra export workflow and HLE runtime. It is not the same project as PSXRecomp. Benchmark actual game behavior before switching frameworks.

## First deliverable after receiving the disc

A verified disc manifest, executable/overlay map, reviewed initial split, and reproducible unchanged executable rebuild. Native recompilation can then be assessed against the same emulator baseline. A booting native executable alone does not satisfy those decompilation milestones.
