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
