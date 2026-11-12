#!/usr/bin/env python3
"""Refresh README progress after an exact all-image validation."""
from collections import Counter
from datetime import datetime
import json
import subprocess
from zoneinfo import ZoneInfo

from rebuild import ROOT
from target_queue import rebuild

START = '<!-- progress:start -->'
END = '<!-- progress:end -->'


def update():
    """Called by the supervisor only after its final all-image gate succeeds."""
    path = ROOT / 'README.md'
    text = path.read_text()
    if text.count(START) != 1 or text.count(END) != 1:
        raise RuntimeError('README must contain exactly one progress marker pair')
    before, rest = text.split(START)
    _, after = rest.split(END)
    boot = json.loads((ROOT / 'reports/boot-rebuild.json').read_text())
    overlays = json.loads((ROOT / 'reports/overlay-rebuild.json').read_text())
    if not (boot['identical'] and overlays['identical'] and overlays['validated_all_overlays']):
        raise RuntimeError('Progress requires successful full-file validation of all images')
    inventory = rebuild()
    targets = inventory['targets']
    non_sdk = {r['identity']: r for r in targets if r['status'] != 'sdk'}
    matching = {key for key, r in non_sdk.items() if r['reconstruction'] == 'c'}
    deferred_path = ROOT / 'reports/reconstruction/deferred.json'
    deferred = set(json.loads(deferred_path.read_text())) if deferred_path.exists() else set()
    deferred = (deferred & non_sdk.keys()) - matching
    remaining = non_sdk.keys() - matching - deferred
    held = sum(non_sdk[key]['status'] == 'held' for key in remaining)
    total = len(non_sdk)
    counts = [('Matching C', len(matching)), ('Deferred', len(deferred)),
              ('Remaining', len(remaining))]
    date = datetime.now(ZoneInfo('Pacific/Auckland')).strftime('%d %B %Y').lstrip('0')
    lines = [f'Snapshot: **{date}**. Counts cover non-SDK function instances across all {inventory["summary"]["images"]} images.',
             '', '| Status | Functions | Percent |', '| --- | ---: | ---: |']
    for label, count in counts:
        percent = 100 * count / total if total else 0
        lines.append(f'| {label} | {count:,} | {percent:.2f}% |')
    lines += [f'| **Total** | **{total:,}** | **{100 if total else 0:.2f}%** |', '',
              'Matching C requires an exact full-file comparison against the pinned original. '
              'Deferred targets remain assembly and await another attempt; '
              f'remaining includes {held:,} curated {"hold" if held == 1 else "holds"}.', '']
    by_image = Counter(non_sdk[key]['image'] for key in matching)
    lines += ['C function instances by image: ' + (', '.join(f'`{image}`: {count:,}' for image, count in sorted(by_image.items())) or 'none') + '.', '',
              f'The inventory contains **{len(targets):,}** detected function instances, including '
              f'**{sum(r["status"] == "sdk" for r in targets):,} SDK signature matches** excluded from the table. '
              'Boundaries are provisional and duplicate bodies are counted per image. '
              'Counts come from `reports/reconstruction/targets.json` and the persistent '
              '`reports/reconstruction/deferred.json`, excluding targets already covered by C.', '',
              f'**Binary rebuild: {1 + overlays["matching_overlays"]}/{inventory["summary"]["images"]} files exact (100%).** '
              'This includes retained assembly and data, so it measures build reproducibility separately from C reconstruction. '
              'Latest results: `reports/boot-rebuild.json` and `reports/overlay-rebuild.json`. The compiler recipe remains provisional.']
    updated = before + START + '\n' + '\n'.join(lines) + '\n' + END + after
    if updated != text:
        path.write_text(updated)
    print(f'README progress: {len(matching):,} matching, {len(deferred):,} deferred, {total:,} total')


if __name__ == '__main__':
    # Manual refreshes establish the same gate as the batch supervisor. A failed
    # validation exits before touching the README; objdiff alone never suffices.
    subprocess.run(['make', 'validate'], cwd=ROOT, check=True)
    update()
