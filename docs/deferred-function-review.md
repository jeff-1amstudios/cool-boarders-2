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

## Newest deferrals reviewed — 2026-11-13

The newest persistent deferrals were `0x80017470` and `0x800178CC`, from
batch `1794509183087661000`. Fresh read-only headless exports used the current
saved SDK names and accepted analysis before retrying them.

| Function | Original deferral | Review result |
| --- | --- | --- |
| `0x80017470` (1,036 bytes) | Whole-executable validation failed with 12 differing bytes: the eight-byte CD response buffer and mixer occupied reversed stack slots. | Separate response-buffer scopes improved the candidate to 99.99228% diagnostic similarity and **two full-file byte differences**. Retained assembly. |
| `0x800178CC` (308 bytes) | Its object and whole executable matched, but Ghidra feedback failed with `Conflicting existing type: /functions/callback *`. | Renamed the `OpenEvent` callback parameter to `event_handler`, avoiding the parser-generated type-name collision. Retained matching C and transactional analysis feedback. |

The event function opens eight BIOS events for two event classes and enables all
eight handles. Its calling convention and function-pointer types are unchanged;
no type-conflict checks were weakened. `OpenEvent` and `EnableEvent` aliases name
existing retained SDK assembly at `0x8003CAAC` and `0x8003CADC`; they are not
additional C reconstructions.

The CD callback handles polling, fading, response copying and playback state.
The best candidate's remaining differences are the first polling buffer's two
address instructions: `addiu a2, sp, 0x10` and `lbu v0, 0x10(sp)` must use
`0x18`. Its second response buffer, mixer, frame size and saved registers match.
The whole 266,240-byte executable differs only at file offsets `0x7CB0` and
`0x7CC0`. This remains a failed acceptance gate regardless of the high score.

The review recorded 649 C/compiler trials (631 completed object comparisons),
including the available PSX/CDK GCC 2.7.2 and PSX GCC 2.8.1 compilers, optimization
and CSE/scheduling options, local types/layouts and separate buffer lifetimes.
Some options were rejected by particular compilers. No compiler recipe change
was accepted. The failed candidate did not update saved Ghidra analysis.

Evidence is preserved under ignored
`reports/reconstruction/deferred-review-20261113/`: `best-full-checked-main.c`,
`best-object-diff.json`, `best-boot-rebuild.json`, numbered trial records,
fresh exports and validation/feedback logs. The remaining queue entry links this
review while preserving the original attempt. The resolved event entry is removed
only after its full-file gate and saved Ghidra feedback succeed.

Final `make validate` passed all 15 pinned full-file comparisons. The resident
executable contains 64 matching C objects, with the remaining code retained as
assembly. The compiler flags remain unchanged.

### Generic callback import fix

The subsequent importer fix removes the need for the `event_handler` workaround;
the authored `OpenEvent` declaration again uses `callback`. Inline function-pointer
types receive deterministic signature-based identities before resolution. Parsing
without immediate insertion also prevents different callback signatures with the
same parameter name from replacing each other inside one import. Explicit named
function typedefs and established data layouts retain conflict checks.

Focused read-only headless checks cover the original failed payload, repeated and
nested callback parameter names, deterministic replay, real struct-layout and
named callback typedef conflicts, and representative accepted global/type records.
Artifacts are under ignored `reports/reconstruction/callback-import-fix/`.

The generic importer passed the original collision payload and replay checks for
five accepted GPU, sound, CD-global and event records. An older sound record was
still rejected by the existing global-extent guard because subsequent analysis
expanded `D_80058F80`; the importer does not relax that protection. Final
`make validate` again passed all 15 pinned full-file comparisons.

## Two newest deferrals — card functions, 2026-11-13

The newest persistent entries were `0x80017DE8` (604 bytes) and `0x8001827C`
(904 bytes), both from batch `1794521977450840000`. Neither was deferred for
nonmatching instructions: the archived candidates had already passed the pinned
full-executable comparison. Both were rejected during the subsequent read-only
Ghidra feedback plan.

| Function | Deferral cause | Resolution |
| --- | --- | --- |
| `0x80017DE8` | `Global would truncate existing data: card_format_device`. The unsized `char []` declaration supplied only an element type, and the importer attempted a one-byte prefix over an existing six-byte string. | Declare `card_format_device[6]`, covering the reference's `bu00:` and NUL. Restore matching C and its inline event/load helpers. |
| `0x8001827C` | `Global would truncate existing data: card_saving_text`. The unsized text declarations encountered the same saved-string extent guard. | Give all seven text arrays explicit NUL-inclusive lengths verified from the pinned bytes: 15, 16, 33, 13, 14, 15 and 17. Reuse the now-accepted `TILE *` drawing-helper prototype with explicit cursor casts. |

Fresh read-only headless exports used current saved SDK names and accepted
analysis before restoration. Both selected objects match with the unchanged
provisional `gcc-2.7.2-psx -O2 -G0 -mips1` recipe; no compiler flag change was
needed. The full 266,240-byte executable matches reference SHA-256
`5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4`
with zero differing bytes. The two functions add 1,508 bytes of matching C,
taking resident C objects from 72 to 74. BIOS/SDK calls remain retained assembly.

The first function manages eight card events, polls card-load results with up to
sixteen retries, closes the events, and invokes BIOS formatting only for result
code 1. The second draws automatic-save status and error messages, then waits
up to 120 or 180 frames, allowing a newly pressed controller button to end the
wait. Recovered layouts and return codes are viable matching representations;
the original aggregate spelling and complete card-status semantics remain
uncertain.

The importer and its conflict/extent guards are unchanged. Explicit verified
array bounds let both feedback plans pass without reducing existing data.
Replayable feedback records are under `config/ghidra/`; exports, object
comparisons, full-file logs and feedback transactions are preserved under
ignored `reports/reconstruction/deferred-card-review/`. Original failed-session
records remain intact.

Final plain `make validate` passed all 15 pinned full-file comparisons. Both
transactional Ghidra saves succeeded, and subsequent fresh read-only exports
confirmed `int func_80017DE8(void)` and `void func_8001827C(void)`. Both entries
were removed from the persistent deferred queue only after those saves and
confirmation; three older deferrals remain. The progress snapshot now records
74 matching resident C functions.

## Extern-insertion recoveries — 2026-11-14

Recovered four preserved candidates that had reached 100% selected-object
diagnostic similarity but were rejected before full-file acceptance:

| Image | Function | C bytes |
| --- | --- | ---: |
| SCUS_943.58 | `0x8003CA24` | 24 |
| BDEDIT.BIN | `0x8005C204` | 292 |
| BDEDIT.BIN | `0x8005D0F0` | 164 |
| BDEDIT.BIN | `0x8005E90C` | 224 |

The old shared-source guard required the entire previous declaration area to be
an exact prefix. Adding an extern among related declarations therefore failed
even when no existing declaration changed. The guard now permits insertion-only
extern declarations and local includes between existing lines, outside helper
bodies. Existing lines, function blocks and header-layout protections remain
enforced. The archived declaration additions pass this revised source review.

Each recovered candidate passed an exact comparison of its containing file
under the unchanged compiler recipe and pinned references, adding 704 bytes of
matching C. Replayable analysis records are under `config/ghidra/`; recovery
logs and feedback transactions are under ignored
`reports/reconstruction/extern-recovery/`. Original attempt records are preserved.

Final plain `make validate` passed all 15 pinned full-file comparisons. All four
Ghidra saves succeeded; fresh read-only decompilations confirmed the accepted
prototypes. The persistent queue now contains 38 deferred functions, and the
progress snapshot records 187 matching C function instances.
