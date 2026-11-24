#!/usr/bin/env python3
"""Read-only cross-overlay duplicate triage from byte-checked saved Ghidra extents."""
import argparse
from collections import Counter, defaultdict
from datetime import datetime, timezone
import hashlib
import itertools
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]


def digest(data):
    return hashlib.sha256(data).hexdigest()


def read(path):
    return json.loads(path.read_text())


def normalized(words, pcs, base, size):
    """Only normalize direct overlay transfers and adjacent LUI/address pairs.

    Preserve scalar immediates, GP/stack offsets, registers, branch displacements,
    resident targets and address reuse. No register allocation or fuzzy matching.
    External overlay target identity is a hypothesis, even for exact raw bytes.
    """
    tokens = list(words)
    local = {pc: i for i, pc in enumerate(pcs)}
    targets = {}

    def address(value):
        if value in local:
            return ('self', local[value])
        if value not in targets:
            targets[value] = len(targets)
        return ('overlay', targets[value])

    for i, (word, pc) in enumerate(zip(words, pcs)):
        op = word >> 26
        if op in (2, 3):
            target = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            if base <= target < base + size:
                tokens[i] = ('jump', op, address(target))
        # Adjacent instructions only: avoid guessing data flow across clobbers.
        if op != 15 or i + 1 == len(words) or pcs[i + 1] != pc + 4:
            continue
        following = words[i + 1]
        next_op = following >> 26
        reg = (word >> 16) & 31
        if reg == 0 or (following >> 21) & 31 != reg:
            continue
        if next_op not in (9, 13, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 46, 50, 58):
            continue
        low = following & 0xFFFF
        value = ((word & 0xFFFF) << 16) + (low if next_op == 13 or low < 0x8000 else low - 0x10000)
        if base <= value < base + size:
            target = address(value)
            tokens[i] = ('hi', word & 0xFFFF0000, target)
            tokens[i + 1] = ('lo', following & 0xFFFF0000, target)
    # Preserve relative range layout, including gaps in noncontiguous bodies.
    layout = [pc - pcs[0] for pc in pcs]
    return digest(json.dumps([layout, tokens], separators=(',', ':')).encode())


def inventory(root):
    catalog = read(root / 'config/overlays.json')
    sdk = read(root / 'config/psyq-sdk.json')
    plan = read(root / 'reports/overlays/plan.json')
    if sdk['reference_sha256'] != catalog['boot_sha256']:
        raise RuntimeError('SDK catalog differs from pinned reference')
    rows, sources = [], {}
    for image, item in catalog['overlays'].items():
        data = (root / 'orig/disc/files' / image).read_bytes()
        export_path = root / 'reports/overlays' / (image + '.json')
        export = read(export_path)
        if (len(data) != item['size'] or digest(data) != item['sha256']
                or export['source_sha256'] != item['sha256']
                or export['mapped_sha256'] != item['sha256']
                or export['run_id'] != plan['run_id'] or export['analysis_timed_out']
                or not export['native_overlay'] or export['image'] != image
                or sdk['overlays'][image]['reference_sha256'] != item['sha256']):
            raise RuntimeError('Changed reference or invalid saved export: ' + image)
        sources[image] = dict(reference_sha256=item['sha256'], export_sha256=digest(export_path.read_bytes()))
        base = int(item['load_address'], 16)
        seen = set()
        for entry in export['entries']:
            identity = entry['identity']
            start = int(entry['address'], 16)
            ranges = [(int(a, 16), int(b, 16)) for a, b in entry['ranges']]
            if (identity != f'{image}:0x{start:08X}' or identity in seen or not ranges
                    or not entry['body_inside_file']
                    or not any(a <= start < b for a, b in ranges)
                    or any(a < base or b > base + len(data) or a >= b for a, b in ranges)
                    or any(b > c for (_, b), (c, _) in zip(ranges, ranges[1:]))):
                raise RuntimeError('Invalid body: ' + identity)
            seen.add(identity)
            raw = b''.join(data[a-base:b-base] for a, b in ranges)
            if len(raw) != entry['body_bytes'] or digest(raw) != entry['body_sha256']:
                raise RuntimeError('Function bytes changed: ' + identity)
            is_sdk = entry['address'] in sdk['overlays'][image]['functions']
            if is_sdk != (entry['classification'] == 'sdk'):
                raise RuntimeError('SDK classification differs: ' + identity)
            aligned = all(a % 4 == 0 and b % 4 == 0 for a, b in ranges)
            pcs = [pc for a, b in ranges for pc in range(a, b, 4)] if aligned else []
            words = list(struct.unpack('<' + 'I' * (len(raw) // 4), raw)) if aligned else []
            rows.append(dict(identity=identity, image=image, name=entry['name'],
                             sdk=is_sdk, body_bytes=len(raw), exact=digest(raw),
                             normalized=normalized(words, pcs, base, len(data)) if aligned else None))
    return rows, sources


def groups(rows, mode, minimum):
    buckets = defaultdict(list)
    for row in rows:
        if row['body_bytes'] >= minimum and row[mode] is not None:
            buckets[row[mode]].append(row)
    result = []
    for fingerprint, members in buckets.items():
        images = {r['image'] for r in members}
        if len(images) < 2:
            continue
        # Keep one per image when estimating cross-image redundancy: additional
        # same-image instances do not inflate the advertised savings.
        redundant = len(images) - 1
        result.append(dict(fingerprint=fingerprint, body_bytes=members[0]['body_bytes'],
                           images=len(images), instances=len(members),
                           cross_image_extra_copies=redundant,
                           cross_image_extra_bytes=redundant * members[0]['body_bytes'],
                           exact_variants=len({r['exact'] for r in members}),
                           members=[{k: r[k] for k in ('identity', 'name', 'body_bytes', 'sdk')} for r in members]))
    return sorted(result, key=lambda g: (-g['cross_image_extra_bytes'], g['fingerprint']))


def summary(found):
    return dict(groups=len(found), instances=sum(g['instances'] for g in found),
                cross_image_extra_copies=sum(g['cross_image_extra_copies'] for g in found),
                cross_image_extra_bytes=sum(g['cross_image_extra_bytes'] for g in found))


def run(root, output, minimum):
    rows, sources = inventory(root)
    non_sdk = [r for r in rows if not r['sdk']]
    report = dict(generated_at=datetime.now(timezone.utc).isoformat(), sources=sources,
                  scope='14 overlays only; saved Ghidra extents, not reviewed source-function boundaries.',
                  caveats='Exact means identical body bytes, not equivalent behavior: callees/data can differ. '
                          'Normalized groups are candidates, not semantic matches. Adjacent overlay address pairs '
                          'and direct overlay transfers only; no changed registers, constants or instruction order. '
                          'No backlog, C, SDK classification or saved Ghidra analysis is changed. '
                          'Copy/byte estimates count images minus one per group; overlapping bodies can overcount bytes.',
                  minimum_bytes=minimum,
                  population=dict(overlays=len(sources), functions=len(rows), non_sdk=len(non_sdk),
                                  sdk=len(rows)-len(non_sdk), non_sdk_body_bytes=sum(r['body_bytes'] for r in non_sdk),
                                  unaligned_bodies=sum(r['normalized'] is None for r in rows)),
                  groups={}, summary={})
    for mode in ('exact', 'normalized'):
        found = groups(non_sdk, mode, minimum)
        report['groups'][mode] = found
        report['summary'][mode] = summary(found)
        report['summary'][mode + '_all_sizes'] = summary(groups(non_sdk, mode, 1))
        report['summary'][mode + '_sdk'] = summary(groups([r for r in rows if r['sdk']], mode, minimum))
    pairs = Counter()
    for group in report['groups']['normalized']:
        images = sorted({m['identity'].rsplit(':', 1)[0] for m in group['members']})
        pairs.update(itertools.combinations(images, 2))
    report['overlay_pairs'] = [dict(images=list(pair), shared_candidate_groups=count)
                               for pair, count in pairs.most_common()]
    output.mkdir(parents=True, exist_ok=True)
    (output / 'duplicates.json').write_text(json.dumps(report, indent=2) + '\n')
    lines = ['# Cross-overlay duplicate triage', '', report['scope'], '', report['caveats'], '',
             f'Non-SDK population: {len(non_sdk)} function instances. Minimum body: {minimum} bytes.', '',
             '| Pass | Groups | Instances in groups | Extra cross-image copies | Extra body bytes |',
             '| --- | ---: | ---: | ---: | ---: |']
    for mode, stats in report['summary'].items():
        lines.append(f"| {mode} | {stats['groups']} | {stats['instances']} | {stats['cross_image_extra_copies']} | {stats['cross_image_extra_bytes']} |")
    lines += ['', 'Normalized includes exact matches; do not add these totals together.', '',
              '## Largest normalized candidate groups', '']
    for group in report['groups']['normalized'][:25]:
        lines += [f"- {group['body_bytes']} bytes × {group['instances']} instances / {group['images']} overlays; "
                  f"{group['exact_variants']} distinct byte bodies: " + ', '.join(m['identity'] for m in group['members'])]
    lines += ['', '## Overlay pairs', '']
    lines += [f"- {' / '.join(p['images'])}: {p['shared_candidate_groups']} candidate groups" for p in report['overlay_pairs'][:20]]
    (output / 'duplicates.md').write_text('\n'.join(lines) + '\n')
    print(json.dumps({k: report[k] for k in ('population', 'summary')}, indent=2))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--min-bytes', type=int, default=32, help='Default 32; all-size counts also reported')
    parser.add_argument('--output', type=Path, default=ROOT / 'reports/overlay-duplicates')
    args = parser.parse_args()
    if args.min_bytes < 1:
        parser.error('--min-bytes must be positive')
    run(ROOT, args.output, args.min_bytes)


if __name__ == '__main__':
    main()
