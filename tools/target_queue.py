#!/usr/bin/env python3
"""Complete image-qualified function backlog; boundary review happens per target."""
import argparse
from collections import Counter
import hashlib
import json

import yaml

from rebuild import ROOT
from overlays import load, summarize

OUTPUT = ROOT / 'reports/reconstruction/targets.json'


def c_spans(config):
    spans = []
    for segment in config['segments']:
        if not isinstance(segment, dict) or segment.get('type') != 'code':
            continue
        rows = segment.get('subsegments', [])
        for index, row in enumerate(rows):
            if not isinstance(row, list) or row[1] != 'c':
                continue
            following = rows[index + 1] if index + 1 < len(rows) else None
            end = following.get("start") if isinstance(following, dict) else following[0] if following else None
            if end is None:
                raise RuntimeError('C unit needs an explicit following boundary')
            start = segment['vram'] + row[0] - segment['start']
            prefix = segment.get('dir', '')
            source = '/'.join(p for p in ('src', prefix, row[2] + '.c') if p)
            spans.append({'address': start, 'end': start + end - row[0], 'source': source})
    return spans


def assemble(entries, images, policy, boundaries, sdk):
    """No body deduplication: identical RAM addresses in different images stay distinct."""
    targets, seen = [], set()
    for entry in entries:
        image, address = entry['identity'].rsplit(':', 1)
        address = f'0x{int(address, 16):08X}'
        identity = f'{image}:{address}'
        if identity in seen:
            raise RuntimeError('Duplicate function identity: ' + identity)
        seen.add(identity)
        context = images[image]
        start = int(address, 16)
        ranges = [(int(a, 16), int(b, 16)) for a, b in entry['ranges']]
        raw = b''.join(context['data'][a - context['base'] + context['file_offset']:
                                       b - context['base'] + context['file_offset']]
                       for a, b in ranges)
        if (not ranges or not any(a <= start < b for a, b in ranges)
                or any(a < context['base'] or b <= a or
                       b - context['base'] + context['file_offset'] > len(context['data'])
                       for a, b in ranges)
                or hashlib.sha256(raw).hexdigest() != entry['body_sha256']
                or len(raw) != entry['body_bytes']):
            raise RuntimeError('Function bytes differ from reference: ' + identity)
        accepted = sdk['functions'] if image == 'SCUS_943.58' else sdk['overlays'][image]['functions']
        is_sdk = address in accepted
        if (entry['classification'] == 'sdk') != is_sdk:
            raise RuntimeError('Plugin SDK catalog differs: ' + identity)
        c = next((s for s in context['c_spans'] if s['address'] <= start < s['end']
                  and all(s['address'] <= a < b <= s['end'] for a, b in ranges)), None)
        review = policy.get('functions', {}).get(identity, {})
        region = boundaries.get('regions', {}).get(identity, {})
        if image == 'SCUS_943.58':
            review = policy.get('functions', {}).get(identity, policy.get('functions', {}).get(address, review))
            # Include holds on shared regions, not just their first entry.
            region = next((r for a, r in boundaries.get('regions', {}).items()
                           if ':' not in a and int(a, 16) <= start < int(a, 16) + r['size']), region)
        # A qualified shared region holds every intersecting function body.
        for region_id, candidate in boundaries.get('regions', {}).items():
            region_image, region_address = region_id.rsplit(':', 1) if ':' in region_id else ('SCUS_943.58', region_id)
            low = int(region_address, 16)
            if (region_image == image and candidate.get('kind') != 'standalone'
                    and any(a < low + candidate['size'] and low < b for a,b in ranges)):
                region = candidate
                break
        held = review.get('hold', False) or review.get('classification') == 'handwritten' or (region.get('kind') is not None
                                                   and region['kind'] != 'standalone')
        status = 'sdk' if is_sdk else 'c' if c else 'held' if held else 'needs_review'
        targets.append({'identity': identity, 'image': image, 'address': address,
                        'name': entry['name'], 'status': status,
                        'reconstruction': 'c' if c else 'retained_assembly',
                        'source': c['source'] if c else None,
                        'sdk_name': accepted.get(address),
                        'reference_sha256': context['sha256'],
                        'build_config': context['build_config'],
                        'symbol_file': context['symbol_file'],
                        'ranges': entry['ranges'], 'body_bytes': entry['body_bytes'],
                        'body_sha256': entry['body_sha256'],
                        'file_offset': start - context['base'] + context['file_offset'],
                        'boundary_review': 'required_before_reconstruction',
                        'note': review.get('note', '')})
    targets.sort(key=lambda r: (r['image'] != 'SCUS_943.58', r['image'], int(r['address'], 16)))
    counts = Counter(r['status'] for r in targets)
    return {'schema_version': 1,
            'scope': 'All saved Ghidra function instances in the boot EXE and 14 known overlays. '
                     'Function boundaries remain provisional. Non-SDK does not prove game authorship. '
                     'No duplicate elimination. Reviewed Splat spans are a separate build-unit inventory.',
            'order': 'Boot first, then overlay filename; ascending address within each image.',
            'summary': {'images': len(images), 'function_instances': len(targets),
                        **{k: counts[k] for k in ('sdk', 'c', 'held', 'needs_review')},
                        'remaining_non_sdk': sum(r['status'] not in ('sdk', 'c') for r in targets)},
            'targets': targets}


def rebuild():
    OUTPUT.unlink(missing_ok=True)
    catalog, boot = load()
    analysis = summarize()  # Checks run IDs, image hashes, completed analysis and overlay references.
    sdk = json.loads((ROOT / 'config/psyq-sdk.json').read_text())
    if sdk['reference_sha256'] != catalog['boot_sha256']:
        raise RuntimeError('SDK catalog differs from boot reference')
    images = {'SCUS_943.58': {'data': boot, 'base': 0x80010000, 'file_offset': 0x800,
                             'sha256': catalog['boot_sha256'], 'build_config': 'scus_943.58.yaml',
                             'symbol_file': 'symbol_addrs.txt'}}
    for name, item in catalog['overlays'].items():
        if sdk['overlays'][name]['reference_sha256'] != item['sha256']:
            raise RuntimeError('SDK catalog differs from overlay reference: ' + name)
        images[name] = {'data': (ROOT / 'orig/disc/files' / name).read_bytes(),
                        'base': int(item['load_address'], 16), 'file_offset': 0,
                        'sha256': item['sha256'], 'build_config': item['build_config'],
                        'symbol_file': item['build_config'].replace('.yaml', '.symbols.txt')}
    for context in images.values():
        config = yaml.safe_load((ROOT / context['build_config']).read_text())
        context['c_spans'] = c_spans(config)
        for span in context['c_spans']:
            if not (ROOT / span['source']).is_file():
                raise RuntimeError('Missing C source: ' + span['source'])
    report = assemble(analysis['entries'], images,
                      json.loads((ROOT / 'config/reconstruction.json').read_text()),
                      json.loads((ROOT / 'config/function-boundaries.json').read_text()), sdk)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report['summary'], indent=2))
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--next', action='store_true', help='Print the first target requiring review')
    args = parser.parse_args()
    report = rebuild()
    if args.next:
        target = next((r for r in report['targets'] if r['status'] == 'needs_review'), None)
        print(json.dumps(target, indent=2))


if __name__ == '__main__':
    main()
