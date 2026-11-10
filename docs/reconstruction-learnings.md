# Resolving the deferred loop and the next ten functions

9 November 2026. The deferred `0x80017A00` now matches, followed by ten additional matching C functions. Compiled C grew from **14 functions / 672 bytes** to **25 functions / 1,432 bytes**. Fresh `make validate` reports 25/25 matching C functions and an identical 266,240-byte boot EXE, with SHA-256:

```text
5b074057008353b1f901744fe82f51b110cf2afa7b78c65394786350b11ef1a4
```

The reference, compiler executables and build flags were unchanged. Remaining boot code is retained assembly. SDK recognition, game/library authorship, unloaded overlays and runtime validation remain separate questions.

## What failed at 0x80017A00?

The function calls `func_8003CB2C`, passes eight successive words to `func_8003CABC`, then calls `func_8003CB3C`. API names and subsystem meaning remain provisional. The deferred reconstruction explicitly advanced its pointer:

```c
for (i = 0; i < 8; i++) {
    func_8003CABC(*words++);
}
```

Under our working recipe this has the right 88-byte size, but ten bytes differ. The pointer occupies `$s1` and the counter `$s0`; the original uses the opposite allocation. That affects the prologue and loop operand encodings. Functional plausibility and the earlier 97.64% object score were insufficient.

The matching reconstruction in [word_call_loop.c](../src/main.c) uses:

```c
for (i = 0; i < 8; i++) {
    func_8003CABC(words[i]);
}
```

The resulting machine code still walks a pointer, advancing it four bytes per iteration. GCC's optimization of the indexed access introduces a different internal representation and produces the original saved-register allocation: pointer in `$s0`, counter in `$s1`. The allocation change is directly observed; attributing it to the compiler's strength-reduction/allocation path is an interpretation of the source and output, not a recovered original source listing.

The indexed version passed the full-file gate before work began on the next ten functions. We did not force register names, embed assembly, change flags or relax the comparator.

## Which alternatives were tested?

Controlled isolated builds linked each candidate at `0x80017A00`, resolving the same three external addresses and comparing all function bytes with the pinned EXE. Local candidate sources, commands and results are under ignored `tools/tmp/loop-study/`; acceptance still came from `make validate`.

| Source variation | Result under PSX GCC 2.7.2, O2/G0 |
| --- | --- |
| Signed `int` counter, indexed `words[i]` | Exact 88-byte match |
| Same indexing with `do` loop | Exact match |
| Explicit `*words++`, ordinary `for`/`while`/`do` | Same ten differing bytes in these variants |
| `register` on pointer/counter, or local pointer alias with explicit advancement | Did not recover the allocation |
| Separate load, pointer increment and call | Did not recover the allocation in the tested forms |
| Increment counter before the call | Changed scheduling/control flow; did not match |
| Unsigned or narrow counter types | Did not match; compare/conversion behavior matters |
| Signed `long` counter with indexing | Also matched on this compiler; does not distinguish original `int`/`long` spelling |

A separate cross-check used identical indexed/pointer sources with O2 and O3, G0 and maspsx 2.56:

| Compiler | Indexed form, O2 / O3 | Explicit pointer form, O2 / O3 |
| --- | --- | --- |
| PSX GCC 2.7.2 | Exact / exact | 10 / 10 differing bytes, 88 bytes |
| CDK GCC 2.7.2 | Exact / exact | 10 / 10 differing bytes, 88 bytes |
| PSX GCC 2.8.1 | 10 / 10 differing bytes, 84 bytes | 20 / 20 differing bytes, 84 bytes |

This function supports retaining the working recipe, but alone cannot distinguish the two 2.7.2 candidates or O2 from O3. See [compiler evidence](compiler-identification.md) for the expanded 25-function matrix. An exact reconstruction establishes one viable C source/recipe combination; it does not prove the original spelling or every original compiler option.

## The next ten

All ten are integrated C and pass full-file comparison. The aggregate objdiff report counts 25/25 matching functions. The detailed fill-function diff has a relocation-label exception explained below. Sizes are original function extents, without counting neighboring assembly.

| Address | Source | Bytes | Useful evidence |
| --- | --- | ---: | --- |
| `0x800166A4` | [fill_slots.c](../src/main.c) | 56 | Local byte value and local base pointer recover the backward fill's address hoisting and allocation. |
| `0x8001742C` | [state_transition.c](../src/main.c) | 68 | `state == 1 || state == 2` becomes a range check; signed halfword is extended into a word. |
| `0x80017AC0` | [word_find_call.c](../src/main.c) | 88 | Indexed early-exit search keeps pointer advancement after the nonzero-result test. |
| `0x80017B18` | [four_word_calls.c](../src/main.c) | 76 | Indexing recovers another call loop's saved-register choices. |
| `0x800181D4` | [byte_fold.c](../src/main.c) | 92 | Unsigned byte loads, explicit shift masks, unsigned loop bound and seed constant. |
| `0x8001A3DC` | [copy_bytes.c](../src/main.c) | 52 | Forward copy with unsigned count and zero-count guard. |
| `0x8001ADC8` | [copy_record_payload.c](../src/main.c) | 76 | Indexed second source fixes the first loop's destination-pointer register. |
| `0x8001C0F4` | [clear_words_flag.c](../src/main.c) | 60 | Descending word clear, conditional initialization, constant return. |
| `0x8001E560` | [copy_bounded_name.c](../src/main.c) | 76 | Potential aliasing requires a second source load after the store. |
| `0x8002FCEC` | [positive_bit_length.c](../src/main.c) | 28 | Signed arithmetic shift; zero and negative inputs return zero. |

These ten add **672 bytes**; the resolved deferred function adds another **88 bytes**. Seven of the ten matched their first full validation attempt. Three required refinement:

- **Backward fill:** direct global indexing and a literal fill value did not match. A local `unsigned char` value and local base pointer followed by `slots[i]` matched. This is a viable source form; the experiment does not establish that the original author chose these exact locals.
- **Early-exit search:** `*words++` incremented the pointer in the call delay slot and produced an 84-byte function. `words[i]` produced the original 88 bytes, including advancement only on the continuing path.
- **Payload copy:** the first candidate differed only in the first loop's destination-pointer register (`$v1` versus `$t0`), scoring 98.95%. Changing the **second** loop from `*bytes++` to `bytes[i]` fixed it. Register allocation can depend on the entire function. The modeled layout claims only the observed 16-byte unknown prefix, 256 halfwords and 8,192 bytes.

The first batch validation failed, preserved all object diagnostics, and reported a four-byte size difference plus shifted output. Those downstream differences were consequences of the short search function, not evidence that thousands of later functions needed rewriting. After the three refinements, fresh validation passed with zero differing bytes.

One diagnostic distinction remains: `reports/objdiff/fill_slots.json` scores its symbol at 99.28571% because the reference uses `D_80058D07` while the C object uses `D_80058CC8 + 63` in its HI16/LO16 relocations. Both resolve to exactly `0x80058D07`. The aggregate report counts the function as matching, and the linked executable is byte-identical. This is a symbol/addend representation difference, not a remaining machine-code mismatch. Preserve readable array-base declarations and check resolved addresses rather than distorting C to satisfy a relocation label.

## Practices for the next ten and hundred

1. **Separate semantic recovery from source-shape recovery.** Establish widths, signedness, side effects, bounds and call order first. Then vary indexing, pointer updates, loop forms, locals and expression ordering systematically.
2. **Classify the mismatch before editing.** Check function size, first changed instruction, allocation, delay slots, relocations and surrounding addresses. An objdiff percentage is a navigation aid, never the acceptance criterion.
3. **Try indexing even when the assembly walks a pointer.** Compilers routinely lower indexed loops that way. Conversely, a pointer form can be correct elsewhere; keep both in the candidate set.
4. **Inspect the whole function when one register differs.** Later loops and temporary lifetimes can alter allocation in earlier loops. Avoid adding arbitrary arithmetic, volatile accesses or forced-register declarations merely to chase a score.
5. **Preserve unusual behavior.** The byte fold uses two left shifts; it is not a rotate. Its masks keep shift counts in range. The bounded copy can fill all 256 bytes without adding a terminator. Its second load can observe a potentially aliasing store; caching the byte changes that behavior.
6. **Keep source valid and narrow in its claims.** Use array indexing for a descending traversal instead of a source pointer that steps before the array. Do not infer unknown fields, API meanings, game authorship or full prototypes from instruction shape alone.
7. **Test compiler hypotheses across independent functions.** Keep the recipe stable while recovering source forms. Add successful nontrivial functions to `config/compiler-probes.json`, preserve ties, and investigate flags separately if mismatches recur across independent objects. Libraries and overlays may differ.
8. **Validate and record before advancing.** Keep the original fingerprint fixed, run `make validate`, inspect `reports/objdiff.json`, per-unit JSON and `reports/boot-rebuild.json`, and document both successful forms and failed hypotheses. Local trial sources/logs remain ignored; reviewed C, configuration and these learnings belong in Git.

Remaining discrimination gaps include switches, division, floating point, inlining across functions and independently identified library objects. This batch supplies more signed/unsigned access and loop evidence, but does not close those gaps.

## Automatic queue follow-up

The first automatic run after this work selected a four-byte copied patch fragment (`0x8003D0B8`) and short handwritten fall-through entries (`0x80024CC0`, `0x800250C0`). None has an independent return, and a standalone C wrapper changes the shared control flow and byte extent. The controller correctly rejected them, retained WIP checkpoints on their function branches and restored matching `main`. The run was interrupted cleanly as it reached another equivalent entry, `0x80037BF8`.

Reviewed persistent exclusions now cover the copied fragment and eight observed `addi a3,a1,offset; addiu a2,zero,0` fall-through entries, including those at `0x80037F68`, `0x80038378`, `0x800387F8`, `0x80038D18` and `0x80039268`. They remain retained assembly, not matching C. This uses the existing queue's classification/hold mechanism; no comparator, compiler recipe or worker acceptance rule changed. Automatic session deferrals alone are temporary, so curated exclusions are necessary to prevent the next run repeating these unsuitable targets.

## Completed automatic ten-function batch

After the reviewed exclusions were committed as `cb26c58`, `tools/.venv/bin/python tools/reconstruct.py run --limit 10` completed with **ten matching commits, zero deferrals**. Work was serial: one fresh agent per function branch, independent controller validation, audited commit, then fast-forward integration into `main` before selecting the next target. All ten function branches remain at their matching commits. No WIP checkpoint was integrated.

| Address | Bytes | Commit | Observed behavior |
| --- | ---: | --- | --- |
| `0x8003D14C` | 8 | `9a019eb` | Empty return stub; preserves the return register. |
| `0x8003E15C` | 8 | `88d5937` | Return zero. |
| `0x8003E164` | 8 | `e47dd33` | Return zero. |
| `0x8003E2F8` | 8 | `9d9b493` | Return zero. |
| `0x8001EC68` | 16 | `1359792` | Store argument in D_8005802C. |
| `0x800296D0` | 16 | `722bac3` | Return zero-extended unsigned byte D_8004E050. |
| `0x800296E0` | 16 | `0fbb5da` | Return zero-extended unsigned byte D_8004E052. |
| `0x8002C3BC` | 16 | `057b102` | Store pointer in D_80054C08 and return zero; pointed-to type remains provisional. |
| `0x8002E158` | 16 | `87775b4` | Return sign-extended halfword D_8005AB3C. |
| `0x8002ECA0` | 16 | `16af9a8` | Store argument in D_8005AB24. |

This batch adds **128 bytes**, taking the total from 25 functions / 1,432 bytes to **35 functions / 1,560 bytes**. Each candidate passed fresh controller `make validate`; final read-only verification confirmed the pinned 266,240-byte file and SHA-256 were unchanged, and all ten commits were present on `main`. The final inventory has 763 detected retained assembly functions. This count is provisional and excludes unloaded overlays; it is not a whole-game denominator.

The queue favors small leaf functions, so this batch contains four return stubs and six simple accessors/setters. It exercises the automation and recovers C but adds little compiler discrimination. Do not confuse a function count increase with equivalent byte coverage or new evidence resolving O2 versus O3. The 1,800-trial compiler matrix still covers the preceding 25-function probe set. Types describe the observed accesses; full API meaning and game-versus-SDK authorship remain provisional.

The existing `--limit` counts selected attempts, including deferrals, rather than guaranteed matches. The interrupted discovery run is preserved separately under ignored `reports/reconstruction/sessions/1794188313715899000/`; the successful run is under `reports/reconstruction/sessions/1794188719862651000/`. A generic queue must exclude noncallable fragments before its small-function ordering can provide useful targets.
