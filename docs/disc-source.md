# Source image and revision

The supplied CUE and 21 BIN tracks are stored under ignored `orig/images/Cool_Boarders_2_USA/`, with the download archive under `orig/archives/`. They have been imported under `orig/disc/`; the boot configuration identifies the USA executable `SCUS_943.58`. Per-track hashes are pinned in `config/usa-disc.json`. The files were moved intact without changing their contents. Direct Redump verification of the exact disc revision remains pending. No standalone official ISO download was identified during the initial sourcing research.

## Obtaining the source

- **Original disc:** make a raw dump with [redumper](https://github.com/superg/redumper), using a supported optical drive. Its current upstream documentation includes macOS and ARM64 releases. Follow the [Redump dumping guides](https://wiki.redump.info/Dumping_Guides) and retain the logs, CUE, all BINs, and subchannel information if produced.
- **Existing personal dump:** supply the CUE and all referenced tracks together. A CHD can be expanded using MAME's `chdman extractcd`; preserve the CHD master as well. Check `chdman help extractcd` for the installed version's options.
- **PlayStation Classic:** Sony's [official lineup announcement](https://blog.playstation.com/2018/10/29/announcing-playstation-classics-full-lineup-of-20-games/) confirms that Cool Boarders 2 was included. If you already own that edition and have exported its game files, inventory them as a separate candidate; do not assume they match an original retail dump.

Redump is the reference for disc identification/checksums, not a source of game downloads. Its main disc search could not be accessed during this research, so no Redump checksum is asserted here. The importer records SHA-1 and SHA-256 for comparison once the correct database entry is accessible.

## Region choice

Provisional identifiers to confirm against the actual SYSTEM.CNF and disc:

| Release | Catalogue serial | Expected boot filename to check |
| --- | --- | --- |
| USA | SCUS-94358 | SCUS_943.58 |
| Europe/PAL | SCES-00992 | SCES_009.92 |

The [official Sony-hosted PAL manual](https://secure.cdn.us.playstation.com/manuals/classic/games/cool-boarders-2-manual-es.pdf) documents SCES-00992; the [PSX Data Center USA catalogue](https://www.psxdatacenter.com/ulist.html) lists SCUS-94358. Boot names above are expectations based on serial convention, not observed files. A serial alone does not distinguish revisions or prove integrity.

The supplied USA image is now the initial analysis target. Treat other regions as separate configurations; do not mix addresses or assets between them.

## Preserve the whole disc

Use BIN/CUE as the source of truth. A cooked 2048-byte-sector ISO contains the data filesystem but can omit CD audio, XA sector details, and layout information needed for runtime behavior. This project's dumpsxiso importer accepts raw 2352-byte-sector images or CUE/BIN; it deliberately does not silently convert a cooked ISO. Keep every track, even when the immediate analysis only needs the boot executable.

Track hashes, boot filename, EXE hash/load address and file/LBA inventory are now recorded. Remaining reference checks: independent exact-revision verification and a successful emulator baseline.
