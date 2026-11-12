# Cool Boarders 2

A matching decompilation of the USA PS1 release (`SCUS_943.58`) and its 14 known code overlays. The goal is to reconstruct C that rebuilds the original binaries byte for byte.

## Progress

<!-- progress:start -->
Snapshot: **13 November 2026**. Counts cover non-SDK function instances across all 15 images.

| Status | Functions | Percent |
| --- | ---: | ---: |
| Matching C | 64 | 2.19% |
| Deferred | 3 | 0.10% |
| Remaining | 2,853 | 97.71% |
| **Total** | **2,920** | **100.00%** |

Matching C requires an exact full-file comparison against the pinned original. Deferred targets remain assembly and await another attempt; remaining includes 1 curated hold.

C function instances by image: `SCUS_943.58`: 64.

The inventory contains **3,967** detected function instances, including **1,047 SDK signature matches** excluded from the table. Boundaries are provisional and duplicate bodies are counted per image. Counts come from `reports/reconstruction/targets.json` and the persistent `reports/reconstruction/deferred.json`, excluding targets already covered by C.

**Binary rebuild: 15/15 files exact (100%).** This includes retained assembly and data, so it measures build reproducibility separately from C reconstruction. Latest results: `reports/boot-rebuild.json` and `reports/overlay-rebuild.json`. The compiler recipe remains provisional.
<!-- progress:end -->

The reconstruction script refreshes this snapshot after successful batch validation. Run `make progress` to validate and refresh it manually. Review and commit the updated README before starting another batch.

## Build

With local tools and the expected disc extraction available:

```sh
make
```

The default build validates every byte of all 15 pinned files and fails on a mismatch. For a fresh checkout, follow [setup and reproduction](docs/setup.md).

## Reconstruct

Keep authored C in `src/main.c` or `src/overlays/<overlay>.c`, with image-qualified `// FUNCTION:` annotations and shared declarations/types. Review each function's boundaries before replacing assembly.

```sh
make target-list                  # refresh the function inventory
make validate IMAGE=SCUS_943.58   # validate the containing file during iteration
make validate                    # required all-image check at batch end
```

Use the [headless reconstruction workflow](docs/reconstruction-worker.md) for selected-object diagnostics, automatic batches and accepted Ghidra feedback. Only full-file byte comparison establishes correctness.

## Documentation

- [Setup and reproduction](docs/setup.md)
- [Reconstruction workflow](docs/reconstruction-worker.md)
- [Function boundaries](docs/function-boundaries.md) and [deferred reviews](docs/deferred-function-review.md)
- [Overlays and matching](docs/overlays.md)
- [Ghidra in Docker](docs/ghidra-docker.md) and [SDK types](docs/sdk-types.md)
- [Compiler evidence](docs/compiler-identification.md) and [project roadmap](docs/workflow.md)

Project tools stay under `tools/`; Ghidra runs in Docker. Supply your own disc dump. Game images, extracted assets, generated files and third-party tools are excluded from Git. See [AGENTS.md](AGENTS.md) for workspace and contribution rules.
