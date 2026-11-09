# First bounded reconstruction pilot

9 November 2026. Ten targets attempted, nine converted to matching C and committed individually, one deferred. Compiled C grew from five functions / 236 bytes to fourteen functions / 672 bytes, an increase of **436 bytes**. The 266,240-byte boot executable remains identical to the pinned reference:

```text
5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4
```

| Address | Result | C bytes | Observation |
| --- | --- | ---: | --- |
| `0x80015F68` | Committed | 48 | Halfword test/set; explicit result variable matches branch structure. |
| `0x80015F98` | Committed | 48 | Paired halfword test/clear. |
| `0x80017044` | Committed | 48 | Four-byte table entry address passed to command 22. |
| `0x8001728C` | Committed | 44 | Command 9 followed by state clear. |
| `0x800178AC` | Committed | 32 | Single SDK-call wrapper; callee remains assembly. |
| `0x80017A00` | Deferred | 0 | Best object score 97.64%; pointer and counter allocated to opposite saved registers. |
| `0x8001BF08` | Committed | 64 | State clear and two three-argument calls. |
| `0x8001C0D4` | Committed | 32 | Indexed descending clear matches sixteen-word loop and initialization order. |
| `0x8001DA98` | Committed | 60 | First greater-than unsigned value among three twelve-byte records. |
| `0x8001DAD4` | Committed | 60 | Paired less-than search. |

The search record's first word and stride are established by accesses; remaining fields are explicitly unknown. Function names remain address-based, filenames describe observed operations, and subsystem meanings and full signatures remain provisional. These nine functions are reconstructed C, without a claim that all are game-authored rather than library wrappers. SDK signature recognition is separate from C reconstruction.

The first acquisition candidate failed with a smaller function and changed full-file output despite equivalent source behavior. The explicit-result version matched. The word-clear candidate also initially failed because its first two instructions were ordered differently; indexed C matched. Every failed attempt left object diagnostics and a failing full-file report. Every function commit ran fresh `make validate` and audited its staged source/configuration paths. The reference fingerprint and compiler recipe were unchanged.

The pointer loop was deferred after five failed attempts, its patch and candidate source archived under ignored `reports/reconstruction/0x80017A00/`, and its edits removed before the next target. A matching baseline was restored and validated. The committed queue places this function on hold so a new session does not immediately repeat the same unsuccessful search. Release the hold only after investigating allocation or new caller/type evidence. Its 88 retained assembly bytes are excluded from C progress.

The inventory initially detected 799 functions and ended at 798. Comparing the initial and final splits showed that Splat stopped treating `0x8001CD40` as a separate function and included it in `0x8001CC14`: the former boundary split a fall-through load/store sequence, without a return or a new prologue. No code disappeared. This illustrates why detected boundaries and counts remain provisional. The final inventory has fourteen C functions and 784 detected retained assembly functions; it is not a whole-game completion denominator. Overlays remain unmeasured.

The pilot supports continued bounded batches using the [worker workflow](reconstruction-worker.md). It does not resolve the O2/O3 compiler tie or establish settings for independent libraries/overlays. The compiler matrix still measures the original five probes. For the next batch, review functions with signed byte loads, arithmetic and switches rather than selecting only trivial wrappers; add reviewed SDK leads to the queue metadata as they are established.

Post-pilot worker improvements preserve separate session histories, snapshot edits and available diagnostics for each attempt, reject simultaneous worker commands and honor persistent holds. Guard tests cover byte-checked assembly context, path staging, protected recipe/HEAD/deadline checks, attempt limits, target exclusions, session archive retention and concurrency. The script prepares and gates work; an agent must still supply and review the C.

## Follow-up

The deferred function was subsequently resolved with indexed C under the unchanged recipe, followed by ten more matching functions. The current total is 25 C functions / 1,432 bytes. The compiler matrix now covers all 25. See [the loop investigation and batch learnings](reconstruction-learnings.md); the results above describe the earlier pilot.
