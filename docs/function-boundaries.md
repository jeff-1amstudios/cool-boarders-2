# Function-boundary audit

9 November 2026. C reconstruction is paused. This pass corrected analysis and selection; it added no C functions. The boot executable still rebuilds to the pinned 266,240 bytes and SHA-256 `5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4`. The C total remains 35 functions / 1,560 bytes. SDK identification, original source language and overlay coverage are separate questions.

## Which representation is authoritative?

Use the saved **reviewed Ghidra analysis** as the primary semantic workspace. Its code references, data definitions, function bodies and reviewed unit annotations support the decisions. The versioned curated export is [config/function-boundaries.json](../config/function-boundaries.json); Splat is the build/disassembly projection. The unchanged executable is authoritative for bytes.

Ghidra's automatic function objects and Splat's generated labels are both hypotheses until reviewed. An entry label is not necessarily an independently compilable C function. A shared reconstruction region can contain several entry points; those entry labels remain distinct even when they must be reconstructed together.

The catalog contains **83 reviewed region dispositions**: 45 standalone extents (35 existing C and ten future candidates), nine shared-entry regions, seven helpers with nonstandard register inputs, BIOS dispatch stubs, embedded data, startup and unresolved special regions. A disposition such as `multi_entry` or `unresolved_code_region` is not approval to compile it as ordinary C. Source language remains unconfirmed where provenance has not been recovered.

The Ghidra project is under ignored `orig/ghidra/CB2_USA.*`. Before applying changes, a complete snapshot was preserved under `orig/ghidra/backups/before-boundary-review/`; its file hashes are recorded in ignored `reports/boundary-audit/ghidra/project-backup.json`. The read-only plan found zero overlaps with unreviewed functions. It identified 26 reviewed standalone starts absent from the saved analysis, mostly existing byte-matching C functions. Those definitions were added in a transaction; existing SDK names and prototypes were preserved. All initialized program bytes were checked before and after.

Reviewed groups are recorded in the Ghidra `CB2ReviewedBoundaryUnits` property map and `CB2 Boundary Review` bookmarks. Standalone function bodies are checked against their reviewed extents. Shared groups preserve their component entry definitions; they are not forced into overlapping standalone Ghidra functions. Unreviewed jump-table cases are not promoted merely because Ghidra assigned them function names.

## Findings and corrections

### Shared polygon entries

The following eight prefixes fall through into the following body. Existing direct calls, recursive calls and Ghidra's `RCpoly…` / `RCpoly…A` names support preserving both entry points in a shared region. The Ghidra names are SDK leads, not independent signature verification.

| Setup entry | Body entry | Region end, exclusive |
| --- | --- | --- |
| `0x80024CC0` | `0x80024CC8` | `0x8002503C` |
| `0x800250C0` | `0x800250C8` | `0x800254CC` |
| `0x80037BF8` | `0x80037C00` | `0x80037F18` |
| `0x80037F68` | `0x80037F70` | `0x80038314` |
| `0x80038378` | `0x80038380` | `0x800387A0` |
| `0x800387F8` | `0x80038800` | `0x80038C88` |
| `0x80038D18` | `0x80038D20` | `0x800391F4` |
| `0x80039268` | `0x80039270` | `0x800397D4` |

Explicit root sizes and global alternate labels (`type:alabel`) keep these regions together in Splat. Ordinary branch labels were insufficient because callers in other objects need global symbols. The prefix alone is excluded from the C queue. These observations establish control flow; they do not by themselves prove handwritten assembly.

`0x8001F9C8..0x8001FA38` is another shared region. Its entries at `0x8001F9C8`, `0x8001F9DC` and `0x8001FA08` have different input/output widths and share a helper/tail. The first entry branches over the middle entry. RA is preserved in `a3` across a helper call, which requires a contract beyond the normal C ABI. Ghidra's `VectorNormalS`, `VectorNormal`, and `VectorNormalSS` labels are retained as leads; the three labels are not treated as three isolated C extents.

Seven small polygon helpers also consume temporary-register values supplied by their callers: `0x8002503C`, `0x800254CC`, `0x80037F18`, `0x80038314`, `0x800387A0`, `0x80038C88`, `0x800391F4`. They have dedicated nonstandard-ABI dispositions and remain outside ordinary C selection.

### BIOS entries merged with following code

A constant BIOS-vector load, non-linking jump through `t2`, and service number in the delay slot form a 12-byte dispatch entry. The BIOS returns through the incoming RA, so the following aligned code is not reached by falling through this entry.

| Dispatch entry | Following code begins |
| --- | --- |
| `0x8003CBBC` | `0x8003CBCC` |
| `0x80044600` | `0x80044610` |
| `0x80044968` | `0x80044978` |
| `0x80044998` | `0x800449A8` |
| `0x800449B8` | `0x800449C8` |
| `0x800449F8` | `0x80044A08` |
| `0x80044A18` | `0x80044A28` |

Those seven confirmed boundaries are now explicit. The setter at `0x8003CBCC` was previously hidden inside the preceding span. The region at `0x80044610` still needs indirect-tail analysis. Internal boundaries in the following `sprintf`/helper region at `0x80044A28` remain unresolved; no guessed full-function size is forced there.

Coverage checks also recovered two 12-byte BIOS entries at `0x8003CA7C` and `0x80044938`. Splat had emitted them together with following non-code metadata as data words, so the function-label parser missed them entirely. Ghidra independently has the same 12-byte extents (`_bu_init` and `exit` names are leads).

### Data mistaken for code, and parser coverage

`0x8003D0B8..0x8003D0C8` is a 16-byte template read four words at a time by `func_8003D03C` and copied to two patch locations. The preceding routine returns before this storage. Another twelve zero bytes follow it. This region is now explicitly data in the original text section, rather than a four-byte standalone function followed by unaccounted zeros.

The Splat data island specifies both `linker_section: .text` and `linker_section_order: .text`. Moving it to the ordinary data section changes addresses and fails the independent assembly rebuild. Its original location and bytes are preserved.

All six nonzero gaps omitted by the function-label parser have byte-bound data reviews: startup options at `0x800121A4`, inline metadata at `0x8001F81C`, `0x8003CA88`, `0x80044944`, the patch template region, and trailing data at `0x8004534C`. The precise encoding of the inline metadata is not claimed. Zero-only gaps are reported separately; zero bytes alone do not prove that a region is unreachable padding.

A counterexample to blindly adopting Ghidra boundaries is the indirect dispatch in `0x8002FE10`: entries at `0x8002FE88`, `0x8002FEA0` and `0x8002FEB8` are jump-table cases despite having separate Ghidra function labels. Their labels do not justify separate C functions. No speculative split was applied.

## What the scanner establishes

[tools/boundary_audit.py](../tools/boundary_audit.py) checks every recorded instruction against the pinned file, then follows direct control-flow paths from each entry. It models branch/return delay slots and integer load delays, stack offsets, saved-register identities, call clobbers under the ordinary ABI, and return-address restoration. It reports direct transfers into another span's interior, nonstandard entry-register uses, unreached instructions and unresolved transfers. Coverage accounting includes words omitted by function-label parsing.

The final scan covers **798 generated code units**, including shared regions, rather than 798 proven original functions. It reports 189 units with review findings. Those are **not 189 proven boundary errors**: BIOS jumps, jump tables, exceptions, custom call contracts, potentially nonreturning calls, and the analysis-state limit all require more evidence. No structural warning was found in the 35 existing C units. Non-flagged spans are not automatically approved.

After reconciling reviewed Ghidra metadata, 539 generated units have the same extent as a Ghidra function, 145 have different extents, and 114 lack a Ghidra function entry. These counts describe agreement, not correctness.

The analysis does not emulate the game, resolve every indirect jump, recover overlay references, prove pointer-word candidates are pointers, or infer original source language. Arbitrary pointer aliasing and callee memory effects are not proved by the stack-slot model. GTE timing, exceptional paths and unusual ABIs require dedicated review. Potential address references and normal-ABI call assumptions are explicitly weaker than proven runtime reachability.

## SDK triage and whether a function is used

Subsequent SDK selection policy: we now accept the PSX plugin's PsyQ 3.7.0 detection and its existing SDK identifications cataloged in `config/psyq-sdk.json` (989 resident entries, including generated internal-entry names, plus 58 entries across native overlays). These are excluded from matching without requiring a separate signature audit. The paragraphs below describe the more cautious provenance treatment used during the boundary audit; it no longer prevents accepting the plugin's result for work selection. Boundary reviews and source-language claims remain separate.

Ghidra names such as `CloseEvent`, `CheckCallback`, `RCpolyFT3`, `CdStatus` and `VectorNormalS` are useful library leads. The saved project provides no per-function signature score or explanatory comment for these examples. `IMPORTED` and `USER_DEFINED` symbol-source tags are not treated as independent proof of a library/version match or of manual review. No SDK identity is established from an address cutoff.

Curated library candidates are excluded from ordinary game-code selection unless `include_library_candidates` is deliberately enabled. The eight library-looking future leaf candidates keep their boundary reviews but are deprioritized by this separate provenance policy. Two reviewed future candidates remain outside that library-candidate category; this does not establish game authorship for either.

The static reference report has 408 units with direct calls, another 115 with possible address/reference uses, and 275 with no known static reference. The latter are not proven unused: callbacks, interrupts, function pointers, computed calls and unloaded overlays can reach code without a direct boot-EXE call. Referenced code receives higher priority. Unreferenced code remains preserved in the exact rebuild even when it receives lower reconstruction priority.

## Queue policy

`matching_paused: true` in [config/reconstruction.json](../config/reconstruction.json) stops a new automatic batch before session creation or agent launch. No matching agents were run during this audit.

Once matching is deliberately resumed, selection requires a reviewed `standalone` disposition with the same address, size, end and instruction SHA-256, plus no outstanding structural findings. Multi-entry groups, templates, custom-ABI helpers and unreviewed spans cannot enter the ordinary queue. The catalog, analysis scripts and policy are protected from matching-agent edits. A batch first refreshes the saved Ghidra review export and checks its agreement with the versioned catalog; subsequent inventories also check that complete export.

There are 236 candidates under the old size/hardware filters. Only ten have standalone boundary reviews, and eight of those are library candidates excluded by default. The remaining 226 require review. The pause is independent of all these filters and remains enabled.

## Reproduce and continue the audit

```sh
make validate        # exact full-file comparison, independent of Ghidra
make boundary-audit  # fresh read-only Ghidra export, independent reference, diagnostics
```

`make boundary-audit` requires the existing Docker image and saved project. It clears old generated exports before starting and requires a completion manifest with hashes of all exported files. Ghidra headless script errors cannot silently reuse stale reports even if the process exits successfully.

Reports remain ignored under `reports/boundary-audit/`: `audit.json`, `audit.md`, coverage gaps, Ghidra function ranges/references, the reviewed-unit export and the plan/apply records. The exporter checks the initialized payload and every reviewed region; it also rejects a reviewed standalone function whose saved Ghidra body has changed.

For a new correction, inspect control flow, delay slots, register contracts and references in Ghidra and the original bytes. Record evidence and exact extents; preserve alternate entry labels and unresolved cases. `ReviewBoundaries.java` supports a read-only `plan` and a transactional `apply` from a reviewed catalog. Snapshot the project before applying a new set. Export the saved reviewed metadata and reconcile the versioned catalog deliberately; do not overwrite differences silently. Then rerun both commands above. Keep bulk disassembly, project databases and generated reports out of Git.

Validation includes 14 synthetic boundary/freshness counterexamples, nine worker guard tests and 28 subprocess/controller tests. The latter use fake agents and synthetic repositories; no real matching agent is launched. A read-only Ghidra negative control rejected a deliberately wrong region fingerprint before any transaction; its headless process still exited successfully, confirming why the completion manifest is necessary. These tests supplement the byte comparison and review evidence; they do not prove every original function boundary.
