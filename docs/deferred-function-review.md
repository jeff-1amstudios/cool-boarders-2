# Deferred function review — 2026-11-12

The four deferrals from batch `1794447328651698000` had different causes. A worker's object similarity score did not establish acceptance.

| Resident function | Original deferral | Result of this review |
| --- | --- | --- |
| `0x800143E0` | The candidate was nonmatching (98.732% object similarity), and the supervisor also failed to link missing GPU SDK names. | Improved to 99.855% diagnostic similarity; the entire executable still differs by 33 bytes. Retained assembly. |
| `0x80015508` | The 660-byte candidate matched its object, but the executable could not link `PutDrawEnv`, `PutDispEnv`, `DrawPrim`, and `MoveImage`. | Added symbol addresses from the existing pinned SDK catalog; retained matching C. |
| `0x8001579C` | The 120-byte candidate passed the full-executable comparison, but Ghidra rejected replacing the filename data with an unsized array's shorter prefix. | Declared the independently evidenced 17-byte filename extent; retained matching C and its analysis feedback. |
| `0x80015948` | The 552-byte candidate passed the full-executable comparison, but Ghidra rejected shrinking the existing four-byte object at `D_80057FC0` to a halfword. | Preserved that saved object's extent and omitted its unsupported global replacement from feedback. Retained matching C and the other recovered global types. |

The GPU aliases use the existing SDK catalog's addresses: `MoveImage` at `0x80029B74`, `ClearOTag` at `0x80029C38`, `DrawPrim` at `0x80029D88`, `DrawOTag` at `0x80029DE8`, `PutDrawEnv` at `0x80029E5C`, and `PutDispEnv` at `0x8002A0B4`. These identify retained SDK assembly; they are not additional C reconstructions.

The later accepted playback function already declared `D_80058F84` and `D_80058F8C`. Their shared declarations now carry the volatile qualification required by the recovered transfer function's instruction sequence. Full-file comparison checks that playback and every other accepted C function remain exact. This supports a viable reconstruction, not proof of the original source qualifiers. `D_80057FC0` is accessed as an unsigned halfword in C; this does not establish the saved four-byte object's total layout.

## Remaining candidate

For `0x800143E0`, inline absolute value, explicit signed texture-index scaling, and color assignment order improved the candidate. The best candidate's remaining instruction differences are:

- A `0x22A8` stack frame versus the reference's `0x22B0`, including saved-register offsets.
- Four color-calculation instructions using `a3` where the reference uses `t0`.
- The color result's add/move sequence versus the reference's add/mask sequence.

The selected object has the reference's instruction length; its relocated full-file build still differs by 33 bytes. No nonmatching C, temporary padding experiment, or failed candidate analysis was retained in the accepted source or saved Ghidra analysis.

Trials covered the available PSX/CDK GCC 2.7.2 and PSX GCC 2.8.1 compilers, O2/O3, scheduling, strength reduction, CSE, frame-pointer and related flag variations, plus focused C expression/type/lifetime changes. Unsupported historical compiler options were recorded as rejected trials. No trial established a better compiler recipe; `config/compiler.json` remains unchanged and provisional.

Fresh headless exports and review artifacts are under ignored `reports/reconstruction/deferred-retry/`. The remaining candidate, its matching header/symbol/split inputs, instruction diagnostics and failed full-file comparison are preserved under `reports/reconstruction/deferred-retry/800143E0/`. Original batch evidence is unchanged except the selected-object iteration diagnostics, which are copied into the new directory for this review. Prior candidate patches and supervisor logs remain available in the original session.

Final acceptance uses plain `make validate`: exact full-file comparison of all 15 pinned images. The accepted source adds three resident C functions (1,332 bytes), increasing resident C objects from 31 to 34. Remaining executable assembly, SDK identifications and overlays retain their existing status.

## Two newest deferrals — batch `1794470751902582000`

Both resident functions now compile to matching C with the unchanged provisional
`gcc-2.7.2-psx -O2 -G0 -mips1` recipe.

| Function | Why it was deferred | Resolution |
| --- | --- | --- |
| `0x80015EC8` (160 bytes) | The candidate was nonmatching (83.5% diagnostic similarity), and containing-file validation failed to link the missing `CdMix` alias. The worker preserved an earlier `unsigned short` parameter declaration. | Full-width `int` parameters, explicit low-16-bit range checks, and `short` store locals reproduce the reference's register copies without extra sign extensions. Added `CdMix = 0x80040BEC` from the pinned SDK catalog. |
| `0x80016010` (528 bytes) | The object diagnostic reported 100%, but the source checker mistook the C union member access `.word` for an assembler directive. No embedded assembly was present. | Reused the subsequently accepted `SoundDiscTransfer` layout and copied the aligned `CdlLOC` representation through `unsigned long` accesses. The resulting C passes the existing source check and reproduces the original aligned loads/stores. |

The volume function preserves a stored value when its argument's low 16 bits are
outside `0..255`, then applies the signed stored CD volume divided by two to both
active mixer channels. Full-width parameters also accommodate the observed `-1`
sentinel caller. Matching establishes a viable source representation, not the
original declaration spelling.

The transfer function allocates a `0x81C`-byte request, initializes one or two
sound banks, copies their disc positions and sizes, and starts the transfer.
The third argument's low byte is stored in the existing layout's `voice` field;
its precise mode/voice meaning remains uncertain. The overlapping file-record
view supplies file indices at a 16-byte stride. Existing saved global layouts,
including the four-byte object containing the pitch halfword, are preserved;
feedback records explain these overlapping views without replacing their saved
extents. Volatile aliases preserve the reference's initialization order without
altering prior users.

Fresh read-only headless exports, compiler/C trials, selected-object diagnostics,
full-file validation logs, and Ghidra feedback are preserved under ignored
`reports/reconstruction/newest-deferred/`. The focused C experiment found the
volume match after 74 compiler/C trials; the broader sweep recorded 995 successful
trials before it was stopped. No compiler recipe change was needed.

The full 266,240-byte `SCUS_943.58` matches the pinned reference SHA-256
`5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4` with
zero differing bytes. These two functions add 688 bytes of matching C, taking
resident C objects from 42 to 44. `CdMix` remains retained SDK assembly. The older
`0x800143E0` deferral and the overlays' reconstruction status are unchanged.

Final `make validate` passed all 15 pinned full-file comparisons. Fresh read-only
exports after transactional Ghidra saves confirmed both accepted prototypes.
The two resolved entries were removed from the persistent deferred queue.
