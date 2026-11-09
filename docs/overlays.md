# Overlay inventory

The PS1 has 2 MiB of main RAM. This game retains its boot executable and replaces the code above it with scene-specific BIN files. Different functions therefore occupy the same RAM address at different times. A function identity must include its overlay file, not just its address.

The installed [PSX plugin](https://github.com/lab313ru/ghidra_psx_ldr) provides an Overlay Manager for importing a binary at a supplied address and resolving overlay references. It does not infer this game's CD-loading protocol or discover every raw overlay automatically. We use its PSX language, loader, automatic analysis, PsyQ recognition and native decompiler after supplying the game's mapping. We accept its PsyQ 3.7.0 detection.

## Mapping

All 14 root BIN files load at **`0x8005AB68`**. Their initial function starts at **`0x8005AB6C`**, following a four-byte data word. The first word is retained; it is not used as an index into the destination table.

The boot routine at `0x800123F8` selects a file at `0x800124BC`, obtains a fixed destination from `[0x80010030]` at `0x800124C4`, and queues the read through `0x80013B60`. `boot_main` calls `0x8005AB6C`. The 14 loaded file ends, rounded to four bytes, match the end-address table at `0x800100B0`. Local direct calls corroborate the common base. This is static evidence; no emulator breakpoint trace is claimed.

Sizes, hashes, addresses and mapping evidence are pinned in [the overlay configuration](../config/overlays.json). The original extracted files remain under ignored `orig/disc/files/`.

## Reproduce

With Docker Desktop running and the existing boot project closed in Ghidra:

```sh
make overlays       # prepare, import/analyze all 14 files, export and summarize
make overlay-report # summarize the completed analysis without running Ghidra
```

The existing `orig/ghidra/CB2_USA.gpr` project is backed up under `orig/ghidra/backups/before-overlay-import/`, then its `SCUS_943.58` program receives 14 named **native overlay memory blocks**. To view it using the browser desktop:

```sh
./tools/ghidra.sh web
```

This automates the plugin Overlay Manager's import operation: each original binary becomes an initialized memory block with the overlay flag enabled, its own address space and the shared physical base `0x8005AB68`. The header word is data, `+4` is an entry point, and GP retains `0x80050274`. Boot code, SDK functions and every overlay are browsable together. The PSX plugin analyzes each overlay range directly, avoiding rescanning the already identified resident SDK. Normal automatic analysis discovers local functions and calls. The resident boundary exporter filters out overlay spaces so the existing boot-only matching workflow stays usable.

Every export compares the resident payload and each overlay memory block against their original files. The boot and all 14 overlay reference hashes are checked on every preparation/report run. Run IDs prevent a failed analysis from silently reusing old successful reports. Script failures, missing exports, cross-file function bodies and analysis timeouts fail the report. The originals are not modified.

## Results and limits

The completed native-block analysis estimates **3,967 function instances**: **1,047 SDK** and **2,920 non-SDK** (187 resident and 2,733 overlay instances). Thirty non-SDK resident functions already have byte-matching C, leaving **2,890 non-SDK candidate instances**. There are **2,632 byte-identical-body groups** among the 2,920 non-SDK instances; this is not a proven original-source function count. All 14 overlay entries pass a native decompiler smoke check. The disc scan finds only one PS-X EXE header and exactly the 14 mapped BIN filenames referenced by code, with no unmapped BIN references.

`reports/overlays/inventory.json` contains the combined inventory, per-image counts, plugin-identified SDK exclusions, file/address identities and byte-identical non-SDK body groups. Per-image exports and the headless log are alongside it. Synthetic GTE helpers are excluded; resident boot functions are counted once, separately from overlay functions.

The body-hash groups expose possible reuse between overlays; identical bytes do not prove identical behavior when calls resolve to different overlay implementations. Function instances remain distinct. Ghidra function boundaries are analysis estimates, including plugin-created SDK internal entries, and differ from the boot reconstruction's Splat units. The combined count uses Ghidra functions consistently and does not add a Ghidra overlay count to a Splat boot count.

The report also enumerates PS-X EXE headers across extracted files and BIN names referenced by the boot/overlays. All known raw code files are covered. Opaque or compressed asset containers may still need format analysis; automatic disassembly is not proof that no other code exists. All 14 overlays now have exact retained-assembly/data rebuilds. No overlay C reconstructions are retained yet. `make validate` gates the boot and all 14 raw BIN files against their pinned references.

## Matching overlays

```sh
make validate                        # objdiff diagnostics, then exact boot + all 14 BIN comparisons
make overlay-validate                # all 14 BIN comparisons only
make overlay-validate OVERLAY=init   # one BIN only; not a whole-game validation
make objdiff                         # diagnostics for all configured boot/overlay C units
```

Each `config/overlays/<image>.yaml` is a versioned Splat split with its own symbols file and outputs under `build/overlays/<image>/`. The four-byte prefix is retained as data before the code. The initial text/data boundary uses the end of Ghidra's identified functions and remains provisional. `MAIN_SEL.BIN` ends with two bytes beyond the word-aligned data; the small project-authored `bytes` Splat extension emits them explicitly. Matching covers every byte, including that tail, without truncating or padding a mismatching result into success.

Splat regenerates assembly under ignored `asm/overlays/` and `asm/data/overlays/`; GNU as assembles it, the linker resolves local and resident addresses, and objcopy emits the raw BIN. No rebuilt image is made by copying or patching the original BIN. References are protected by SHA-256 in `config/overlays.json`, checked before and after building. The aggregate result is `reports/overlay-rebuild.json`, with per-image reports and Splat logs under `reports/overlay-rebuild/`. A failure in any image fails validation while preserving diagnostics; the overlay checks still run if the boot's full-file comparison fails. Old aggregate results and rebuilt outputs are removed before their respective builds.

To reconstruct a reviewed overlay function, split its assembly span into `asm` / `c` / `asm` subsegments in that image's YAML, and add the C file under `src/overlays/<image>/`. For example, a C subsegment named `entry` in INIT uses `src/overlays/init/entry.c` and `build/src/overlays/init/entry.o`. Extend that image's symbols file with reviewed names/addresses as needed. Retained assembly/data preserves the remaining bytes.

Overlay C units appear in `reports/objdiff.json` under names such as `overlays/init/entry`; instruction/relocation diagnostics are at `reports/objdiff/overlays/init/entry.json`. The target objects come from an independent assembly-only split, whose complete BIN is checked against the original before it is used for diagnostics. The ordinary build then compiles C and links the final overlay. Objdiff remains diagnostic; only the complete raw BIN comparison establishes matching.

The current compiler recipe is a provisional default for overlay C, not evidence that every overlay originally used the same flags. An image can specify its own `compiler_config` in `config/overlays.json`. The retained-assembly baseline does not identify a C compiler. The automatic reconstruction worker still selects reviewed boot targets; overlay C edits can use the build/diagnostic workflow above, with overlay boundary review before conversion. Duplicate detection/grouping is unchanged and outside this build change.

## References between resident code and overlays

`AuditOverlayReferences.java` verifies actual direct flow references and runs during overlay import. The saved project has 47,806 flows within the correct overlay spaces and 7,390 flows to resident code. No target inside the caller's loaded overlay range was incorrectly left in default RAM. For example, `BDEDIT.BIN::8005B638` calls resident `DrawSync` at `ram::800297E8`; subsequent calls resolve to `VSync`, `PutDispEnv` and `PutDrawEnv`. These references require no manual reassignment.

The resident call at `ram::8001224C` targets `ram::8005AB6C`. It sits in `boot_main`'s scene-loading loop, and all 14 overlays have a different entry function at that physical address. INIT is loaded first, but this instruction is reused after subsequent loads. Assigning it one permanent overlay target would misrepresent the shared dispatch. `reports/overlays/references.json` records all 14 candidate entry functions; a correct override requires a particular execution context. The reference stays unresolved in default RAM rather than arbitrarily preferring INIT or another scene.

Indirect calls and computed pointer targets are outside this direct-flow audit. Context-specific reference fixes should preserve their evidence and overlay identity. The plugin also provides overlay-address-space support for the decompiler; UI reference display or pseudocode alone does not establish which scene is active at runtime.

## INIT to MODE_SEL handoff

The normal startup path loads `MODE_SEL.BIN` immediately after INIT returns. This is established from the decompiler and instruction listings, not only the filenames:

1. `boot_main` finds `INIT.BIN`, initializes a 64-byte context using `0x8001C0F4`, and starts with context word zero set to `0` (overlay/file ID 0).
2. After the loader and wait path complete, the call at `0x8001224C` enters `INIT.BIN::8005AB6C` with that context pointer.
3. INIT performs its first-run setup when context offset `+8` is `-1`. It registers the resource filename table at `INIT.BIN::8005F280` through resident `0x80013AA0`. Entries 0 and 1 point to `INIT.BIN` and `MODE_SEL.BIN`, respectively; the first file was already found by boot.
4. INIT calls resident `0x8001C130(context, 0)` at `INIT.BIN::8005ABDC`. That helper resets the context, writes **`1` to context offset `+0`** at `ram::8001C178`, and queues menu resource loads. INIT returns **`6`**, a loading/transition-path selector, not the next overlay ID.
5. The boot loop continues because the return was not `-1`. At `0x80012220` it reads the new context word (`1`), then calls `0x800123F8` at `0x80012228`. The helper indexes the file table and queues file ID 1, `MODE_SEL.BIN`, at destination `0x8005AB68`. It also configures the workspace using that overlay's end address (`0x80082C80`).
6. The resident wait/transition routine `0x800142F0` uses the previous result `6` to select `0x80014388`, which services the queued reads until complete. The same call site at `0x8001224C` then enters **`MODE_SEL.BIN::8005AB6C`**, now replacing INIT at that address.

MODE_SEL initializes its display/menu state from the supplied resources and enters title/menu rendering and input loops. The load itself follows INIT's completed setup; the subsequent title-screen button handling is inside MODE_SEL. Read-only trace artifacts are under ignored `reports/overlays/trace-mode-select/`; `ExportFunctionTrace.java` can regenerate selected function evidence without changing the project.
