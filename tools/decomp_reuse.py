"""Accepted cross-overlay C starting points; fingerprints never establish acceptance."""
import argparse
from collections import defaultdict
import json
from pathlib import Path
import re
import struct

from c_sources import sections
from decomp_backlog import source_file
from overlay_duplicates import ROOT, digest, inventory, read


def fingerprints(root):
    rows, _ = inventory(root)
    return {r['identity']: r for r in rows}


def donors(target, targets, hashes):
    selected = hashes.get(target['identity'])
    if selected is None:
        return []
    found = []
    for row in targets:
        other = hashes.get(row['identity'])
        if (other is None or row['image'] == target['image'] or row['status'] != 'c'
                or row.get('decomp') != 'c' or other['sdk']):
            continue
        mode = ('exact' if selected['exact'] == other['exact'] else
                'normalized' if selected['normalized'] is not None
                and selected['normalized'] == other['normalized'] else None)
        if mode:
            found.append((mode, row))
    return sorted(found, key=lambda pair: (pair[0] != 'exact', pair[1]['identity']))


def references(root, row):
    """Position-corresponding address hints, not inferred C types or equivalence."""
    data = (root / 'orig/disc/files' / row['image']).read_bytes()
    base = int(row['address'], 16) - row['file_offset']
    pcs = [pc for a, b in row['ranges'] for pc in range(int(a, 16), int(b, 16), 4)]
    words = [struct.unpack_from('<I', data, pc - base)[0] for pc in pcs]
    result = {}
    for i, (pc, word) in enumerate(zip(pcs, words)):
        op = word >> 26
        if op in (2, 3):
            result[(i, 'transfer')] = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
        if op != 15 or i + 1 == len(words) or pcs[i + 1] != pc + 4:
            continue
        following = words[i + 1]
        next_op = following >> 26
        reg = (word >> 16) & 31
        if not reg or (following >> 21) & 31 != reg:
            continue
        if next_op not in (9, 13, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 46, 50, 58):
            continue
        low = following & 0xFFFF
        address = ((word & 0xFFFF) << 16) + (low if next_op == 13 or low < 0x8000 else low - 0x10000)
        if 0x80000000 <= address < 0x80200000:
            result[(i, 'address')] = address
    return result


def symbols(root, row):
    result = defaultdict(list)
    for name, value in re.findall(r'^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+);',
                                  (root / row['symbol_file']).read_text(), re.M):
        result[int(value, 16)].append(name)
    return result


def address_hints(root, donor, target):
    before, after = references(root, donor), references(root, target)
    old_names, new_names = symbols(root, donor), symbols(root, target)
    addresses = defaultdict(set)
    addresses[int(donor['address'], 16)].add(int(target['address'], 16))
    for position, value in before.items():
        if position in after:
            addresses[value].add(after[position])
    return [dict(donor_address=f'0x{old:08X}', donor_symbols=old_names[old],
                 target_addresses=[f'0x{a:08X}' for a in sorted(values)],
                 target_symbols={f'0x{a:08X}': new_names[a] for a in sorted(values)},
                 ambiguous=len(values) != 1)
            for old, values in sorted(addresses.items())]


def headers(root, path, seen=None):
    seen = set() if seen is None else seen
    for name in re.findall(r'^\s*#include\s+"([^"]+)"', path.read_text(), re.M):
        header = next((p.resolve() for p in (path.parent / name, root / 'src' / name) if p.is_file()), None)
        if header is None or header in seen or not header.is_relative_to((root / 'src').resolve()):
            continue
        seen.add(header)
        headers(root, header, seen)
    return seen


def prepare(root, target, report, directory):
    """Snapshot donors on clean integration main, before switching to a WIP branch."""
    if target['image'] == 'SCUS_943.58':
        return None
    hashes = fingerprints(root)
    matches = donors(target, report['targets'], hashes)
    if not matches:
        return None
    mode, donor = matches[0]
    path = root / source_file(donor)
    shared, blocks = sections(path.read_text())
    if donor['identity'] not in blocks:
        raise RuntimeError('Accepted donor has no authored function: ' + donor['identity'])
    block = blocks[donor['identity']]
    # Keep identifiers for explicit adaptation: aliases, pointer addends and
    # overlay-specific types make blind textual address substitutions unsafe.
    candidate = block.replace('// FUNCTION: ' + donor['identity'], '// FUNCTION: ' + target['identity'], 1)
    (directory / 'reuse-candidate.c').write_text(candidate)
    (directory / 'reuse-donor.c').write_text(shared + block)
    snapshot = {str(p.relative_to(root)): p.read_text() for p in sorted(headers(root, path))}
    (directory / 'reuse-headers.json').write_text(json.dumps(snapshot, indent=2) + '\n')
    catalog = read(root / 'config/overlays.json')['overlays']
    recipe = lambda row: read(root / catalog[row['image']].get('compiler_config', 'config/compiler.json'))
    donor_compiler, target_compiler = recipe(donor), recipe(target)
    effective = lambda cfg: {k: cfg.get(k) for k in
                             ('compiler', 'compiler_release', 'cc1_flags', 'maspsx_flags', 'as_flags')}
    details = dict(donor=donor['identity'], target=target['identity'], match=mode,
                   fingerprint=hashes[target['identity']][mode], source=source_file(donor),
                   source_sha256=digest(path.read_bytes()), donor_block_sha256=digest(block.encode()),
                   donor_body_sha256=hashes[donor['identity']]['exact'],
                   target_body_sha256=hashes[target['identity']]['exact'],
                   donor_compiler=donor_compiler, target_compiler=target_compiler,
                   same_compiler_settings=effective(donor_compiler) == effective(target_compiler),
                   address_hints=address_hints(root, donor, target),
                   alternatives=[dict(identity=row['identity'], match=kind) for kind, row in matches[1:]],
                   policy='Accepted donor supersedes deferred/branch candidates. Preserve prior work and notes '
                   'for reference; replace only selected function body, retain branch/history. Adapt entry and '
                   'reference names, declarations and types; compile the minimally adapted donor first. '
                   'Address hints cover direct transfers and adjacent pairs, not every C reference. '
                   'Full-containing-file validation remains the acceptance gate.')
    (directory / 'reuse.json').write_text(json.dumps(details, indent=2) + '\n')
    return {k: details[k] for k in ('donor', 'match', 'fingerprint', 'same_compiler_settings')}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inventory', type=Path, default=ROOT / 'reports/decomp/targets.json')
    args = parser.parse_args()
    report = read(args.inventory)
    hashes = fingerprints(ROOT)
    found = []
    for target in report['targets']:
        if target['status'] != 'needs_review':
            continue
        matches = donors(target, report['targets'], hashes)
        if matches:
            kind, donor = matches[0]
            found.append(dict(target=target['identity'], donor=donor['identity'], match=kind))
    print(json.dumps(dict(note='Preview from saved inventory; automatic attempts rebuild it first.',
                         reusable_targets=len(found), targets=found), indent=2))


if __name__ == '__main__':
    main()
