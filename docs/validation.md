# Local verification

9 November 2026, macOS arm64, AppleClang 21.0.0, Python 3.13.

- Built mkpsxiso and dumpsxiso 2.30 from the commit pinned in `tools/bootstrap.sh`. `file` confirmed dumpsxiso is a native arm64 Mach-O executable.
- Installed Splat 0.50.1, spimdisasm 1.42.4 and their dependencies in `tools/.venv`; `pip check` passed. Exact package versions are in `tools/requirements.lock`.
- Generated a synthetic disc containing an original minimal PS-X EXE fixture, SYSTEM.CNF, and a CD audio track using mkpsxiso. Imported the resulting CUE/BIN through `tools/disc.py`.
- Confirmed executable entrypoint and SHA-256, SYSTEM.CNF discovery, audio extraction, and unchanged source-image SHA-256.
- Confirmed a repeated import refuses to overwrite its output, and a cooked 2048-byte-sector ISO is rejected before creating output.

The synthetic fixture was created in a temporary directory and removed after the check. No commercial game content was used. These checks validate the setup and importer, not Cool Boarders 2 extraction, a matching game rebuild, emulator behavior, or native recompilation. Those require the source disc.

## Supplied USA disc

- Imported the supplied CUE and all 21 BIN tracks, extracting 346 files, including CDDA WAVs and the extractor's license-data file.
- Recorded SHA-1/SHA-256 of all tracks and the boot executable. Rehashed the entire source set after extraction and initial analysis; every hash was unchanged.
- Read SYSTEM.CNF and PS-X EXE headers: boot `SCUS_943.58`, entrypoint `0x800120FC`, load address `0x80010000`.
- Generated and ran a provisional Splat configuration with initial startup symbols. Corrected its initial pointer-table/code boundary and disabled linker section-end alignment that would shift observed addresses.
- Recorded the source/executable fingerprints in `config/usa-disc.json`. Verified Git ignores the supplied archive, disc folder and extracted/generated content.

At this initial disc-analysis stage, direct Redump verification, assembly/link byte comparison, execution in an emulator, and native recompilation had not been performed. Subsequent matching-build work is recorded below; emulator and whole-game runtime validation remain outstanding.

## Ghidra and PSX installation

- Verified official archive SHA-256 digests for Ghidra 12.1.4 and PSX loader 2026.10.03 (the extension build for 12.1.4).
- Built the macOS ARM64 native components successfully using the included Gradle wrapper. Confirmed decompile and Sleigh are arm64 Mach-O executables. Gradle and all native build products remain under `tools/`.
- Reused the existing OpenJDK 24.0.1. No JDK or package-manager installation was performed.
- Headless import of the original `SCUS_943.58` selected `PSX Executables Loader` and `PSX:LE:32:default`. Auto-analysis completed in approximately 95 seconds, including the PsyQ Signatures analyzer.
- `VerifyPsx.java` successfully decompiled the boot entrypoint at `0x800120FC`; report saved to ignored `reports/ghidra-validation.txt`. The persistent project was saved at `orig/ghidra/CB2_USA.gpr`.
- Confirmed launch-support Java settings and Ghidra preferences were created under `tools/ghidra-state/config/`. Logs, temporary files and caches are also routed under `tools/ghidra-state/` by the project wrapper.

The boot project contains uninitialized BSS and unloaded overlays, so auto-analysis emitted warnings about those addresses. Startup pseudocode also contains control-flow warnings that need manual review. Successful decompilation verifies tool operation, not correctness or completeness of recovered C. The GUI was not opened during setup.

## Docker migration

- Built image `coolboarders2/ghidra-psx:12.1.4-2026.10.03-arm64` with the same checksum-verified Ghidra and PSX archives. The Java image is pinned by digest; Linux ARM64 native components were compiled inside Docker.
- Opened the existing `CB2_USA` project in the container with analysis disabled and program processing read-only. Preserved the host's Ghidra owner name through Java options; no project-ownership changes were needed.
- `VerifyPsx.java` passed with `PSX:LE:32:default` and decompiled `0x800120FC`. The existing function-count estimate remained 1,215.
- Compared hashes of both stored program/user database files (`*.gbf`) before and after the container check: unchanged. Ghidra refreshed some index/journal housekeeping files while opening the project; therefore the entire project directory is not byte-identical.
- Preserved the old macOS settings as `legacy-macos-config/` in Docker volume `coolboarders2-ghidra-state-v1`, then removed the host application, two release ZIPs, and local Ghidra/Gradle state. Removed approximately 1.63 GiB from the checkout. Previous build/import logs were retained under ignored `tools/cache/`.
- Docker runs mount the extracted inputs and helper scripts read-only, with project/report mounts writable. Runtime networking is disabled. No new host packages were installed.

Docker image metadata, migration sizes and decompiler output are recorded under ignored `reports/`. GUI operation is not part of this headless container setup. Removing files from the checkout does not remove the image or build cache from Docker Desktop's disk.

## First matching boot rebuild and compiler trials

- Built GNU binutils 2.42 for `mipsel-linux-gnu` locally under `tools/toolchain/`, using the pinned GNU source and tracked modern-macOS zlib compatibility patch. Candidate old-gcc compiler archives were verified against their upstream release SHA-256 digests; maspsx was pinned to its recorded commit. Native tools are arm64; no host packages were installed.
- Established an all-assembly baseline, then replaced five functions with 236 bytes of compiled C. The integrated build produced exactly 266,240 bytes with SHA-256 `5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4`, identical to the reference file. The linker map places every C function at its original address and size.
- Ran 360 compiler/optimization/small-data/assembler-mode/function trials. PSX GCC 2.7.2 with G0 and maspsx 2.56 mode matched all five functions at both O2 and O3. CDK GCC 2.7.2 matched at most three of these reconstructions; GCC 2.8.1 matched at most one. 330 trials reached comparison, and 30 G8/global-scalar trials failed linking with GP displacement errors. Those failures depend on the assumed declarations and are not treated as project-wide flag identification.
- Exported Ghidra's stored metadata showing PsyQ Version 3.7.0, separately from its default fallback-version preference. This is an SDK lead, not proof of a single compiler/flag set for all game objects.
- Negative control: temporarily changed one reconstructed C assignment from zero to one. The resulting build failed the full-file byte comparison. Restored the exact original C source and reran `make`; the integrated output again matched every byte. The final source and report contain the restored, passing version.
- `tools/install_mips.py` was rerun against the local installation to verify pinned downloads/revisions and assembler version. A fresh toolchain compilation was not repeated after its successful build because no toolchain source changed.

Reports are `reports/boot-rebuild.json`, `reports/boot-negative-control.json`, `reports/compiler-matrix.json`, and `reports/compiler-evidence/`. The current recipe is provisional: O2/O3 remain tied and an ASPSX compatibility mode does not uniquely identify the original assembler. Remaining assembly, overlays, detailed types, and runtime validation are unfinished.

## Bounded reconstruction pilot

The [ten-target pilot](reconstruction-pilot.md) added nine matching C functions (436 bytes), bringing the total to fourteen functions and 672 bytes. One 88-byte function was deferred with register-allocation differences and remains assembly. Each C commit passed fresh full-file validation; the final executable retains the pinned hash above. Worker guard tests passed, and blocked-attempt restoration recovered the matching baseline before continuing. No compiler-recipe or reference changes were needed. Overlay mapping and runtime validation remain outstanding.

## Commit preparation

Moved the supplied BIN/CUE folder to ignored `orig/images/` and its archive to ignored `orig/archives/`. Rehashed the entire source set and confirmed no input bytes changed. Updated local inventory paths without changing their recorded fingerprints.

Removed the generated Splat macro headers from the source tree. The build now regenerates them from the pinned dependency into ignored `build/include/`, alongside the other generated build inputs. A matching rebuild passed after this change. Raw disassembly, all game assets and downloaded tool implementations remain outside the commit; the tracked build recipes fetch or regenerate them as needed.
