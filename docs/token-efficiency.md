# Decomp session efficiency review

Reviewed the repository's preserved automatic worker logs and result records on
2026-11-14. Reproduce totals with
`tools/.venv/bin/python tools/decomp_metrics.py`; the generated report
is `reports/decomp/function-metrics.md`, with all attempts in the CSV.
No new decomp workers were launched for this review.

The snapshot contains 259 attempts across 246 image-qualified functions:
187 matched, 64 deferred and 8 interrupted. Usage is incomplete or absent for
13 attempts. Recorded totals are 156,775,643 input tokens, including 146,374,784
cached input tokens (93.4%), plus 1,102,664 output tokens. Uncached input totals
10,400,859. These totals accumulate model input over successive calls; they are
not unique source text or a cost estimate. Supervisor usage is excluded.

Historical worker wall time is approximately 12.1 hours across 252 attempts,
with seven unavailable timings. This is a file-timestamp estimate and cannot
establish exact inference time. Future runs record measured launcher duration.

## Changes applied

- Supply the focused worker brief once to the default launcher, in developer
  instructions. Previously the same brief also appeared in the task input.
  Preserve the combined archived prompt and the complete custom-launcher input.
- Instruct workers to read relevant cached declarations, bound search output,
  find filenames before dumping matches, and extract selected diff data with
  Python. Keep fresh Ghidra exports, accepted types and exact acceptance gates.
- Record per-attempt usage and durations centrally, retaining failed attempts
  and retries so later comparisons include their consumption.

## Evidence and next priorities

In session `1794630539797179000`, the 28-byte `BDEDIT.BIN:0x8005FF08`
worker searched the overlay assembly directory for two global addresses and its
function address. That command produced 411,137 characters. The entire worker
needed only seven commands and ultimately matched. Prefer filename discovery,
then bounded excerpts around relevant uses. Large search results inflate context
for later calls even when the console hides them or the model receives a
truncated response. Raw output character counts are not token counts.

In session `1794619219863599000`, `BDEDIT.BIN:0x8005F4AC` printed a
202,840-character `object-diff.json`. It was deferred after 58 commands and
3,921,950 input tokens (3,845,888 cached). Start from the compact step output;
extract the relevant symbol and a small instruction range before requesting
more. Do not make whole-file JSON dumps the default diagnostic path.

Among attempts with reported usage, matched workers had median input of 358,867
tokens; deferred workers had median input of 534,484. The latter include difficult
functions and do not prove that a smaller budget would improve success. Keep the
current time allowance until measured timing and function-size comparisons can
show whether an opt-in smaller first-pass budget is worthwhile. A token cap or
score-plateau cutoff could discard solvable functions and has not been added.

Console filtering already suppresses successful file-display contents for the
supervisor, but the worker still receives those contents. Further console-only
filtering will not reduce the worker's context. Focus on bounded tool requests
and avoiding repeated reads. Relevant SDK/type declarations must remain available;
do not remove the fresh typed decompilation or weaken validation to save tokens.

A future deterministic selected-context view (shared declarations, exact split
neighbours and referenced types) could reduce broad source/header reads further.
Measure the changes above on comparable functions before adding that machinery.
No savings percentage is claimed without a subsequent measured batch.
