# Initial USA disc analysis

Observed locally on 9 November 2026 from the supplied BIN/CUE. The source disc and archive are preserved unchanged and ignored by Git.

## Disc and executable

| Item | Observed value |
| --- | --- |
| Volume | COOLBOARDERS2 |
| Track layout | Track 1 MODE2/2352; tracks 2–21 AUDIO |
| Extracted file count | 346, including audio WAVs and generated license-data dump |
| SYSTEM.CNF boot | `cdrom:\SCUS_943.58;1` |
| Boot file size | 266,240 bytes (`0x41000`) |
| EXE load address | `0x80010000` |
| EXE payload size | `0x40800` |
| Entrypoint | `0x800120FC` (file offset `0x28FC`) |
| Header GP | `0`; startup sets GP to `0x80050274` |
| Header stack base | `0x801FFFF0`; startup later sets its own stack |
| Track 1 SHA-1 | `a6e71a783e06e6862c2d406eb8dc885201aa6108` |
| EXE SHA-1 | `0f49672190fead08b59abdcb0122a83864fefe32` |
| EXE SHA-256 | `5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4` |

Full fingerprints are in `config/usa-disc.json`; extracted inventory is in ignored `orig/disc/manifest.json`, and file LBAs/timestamps are in `orig/disc/layout.xml`. The disc volume has a 3 October 1997 mastering timestamp and the EXE a 25 September 1997 file timestamp. These are observed metadata, not proof of a particular pressing. Direct comparison with the primary Redump database is still pending because its disc search was inaccessible during research.

The payload begins with a game build string dated 17 September 1997. Embedded SDK source-identification strings include `sys.c`, `intr.c`, and `bios.c` revisions from December 1996. These are useful SDK fingerprint leads; they do not establish the compiler or SDK release by themselves.

## Startup findings

`_start` at `0x800120FC` clears words from `0x80050278` up to `0x8005AB68`, sets the stack and GP, invokes a setup routine at `0x8003CA4C`, then calls `boot_main` at `0x800121B8`. This memory clearing is visible in the code even though the EXE header's BSS fields are zero.

`boot_main` references `\\INIT.BIN;1`, stores its search-result fields, invokes the initial loading path, and eventually calls `0x8005AB6C`. The table at `0x80010030` starts with `0x8005AB68`; INIT.BIN's first MIPS function starts at file offset 4. Together these support an initial overlay loaded at `0x8005AB68` and entered at `0x8005AB6C`. This remains static analysis until confirmed by runtime loading/breakpoints.

The other 14 entries in that table and the raw BIN headers need mapping to scene transitions. Do not assume every BIN loads at INIT's address or has the same lifetime. Several table entries share an address, suggesting reused memory, but runtime confirmation is needed.

## Code-bearing BIN candidates

All 14 root BINs start with a small word followed immediately by plausible MIPS function prologues. They have no PS-X EXE header. The first word's meaning has not been established.

| File | Bytes | First word |
| --- | ---: | --- |
| BDEDIT.BIN | 34,464 | `0x00000007` |
| BP.BIN | 283,488 | `0x00000008` |
| COMB.BIN | 308,128 | `0x00000008` |
| ENDING.BIN | 108,172 | `0x00000008` |
| FREE.BIN | 299,608 | `0x00000008` |
| HP.BIN | 266,504 | `0x00000008` |
| INIT.BIN | 19,496 | `0x00000004` |
| MAIN_SEL.BIN | 139,850 | `0x00000006` |
| MODE_SEL.BIN | 164,120 | `0x00000005` |
| OMT.BIN | 267,712 | `0x00000008` |
| SBC1.BIN | 221,832 | `0x00000008` |
| SBC2.BIN | 265,768 | `0x00000008` |
| TM.BIN | 291,432 | `0x00000008` |
| VS.BIN | 297,720 | `0x00000008` |

Assets are already separated into character/course data, models, textures, sound banks, movies and CD audio. File extensions and directories give useful leads, but their detailed formats have not been decoded.

## Provisional split

`scus_943.58.yaml` and `symbol_addrs.txt` are the starting configuration. Splat's automatic detector originally interpreted a pointer table at file offset `0x283C` as instructions and classified the startup as data. The initial code boundary is now `0x28F4` (a two-instruction return stub), and `_start` is explicitly identified at `0x28FC`.

The estimated code/data boundary at `0x35BA8` and internal rodata boundaries still need semantic review. Splat also suggests several rodata file boundaries. Its generated linker script preserves section ends without adding default 16-byte padding. The subsequent full rebuild now validates exact byte preservation of this layout; that does not prove every semantic boundary is correctly classified.

Repeat the split from the repository root:

```sh
tools/.venv/bin/python -m splat split scus_943.58.yaml
```

Generated assembly goes to `asm/`; linker and auto-symbol files go to `build/`. These are ignored. Five functions have subsequently been converted to matching C; see [the build/compiler findings](compiler-identification.md).

## Next milestone

The local MIPS toolchain and byte-identical boot rebuild are now complete. Next, extend compiler discrimination to more independent functions, confirm INIT's load/entry addresses in a debugger, and map the raw overlays. Retain all 20 audio tracks for emulator and recomp runtime testing.
