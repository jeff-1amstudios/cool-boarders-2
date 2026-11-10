"""Overlay-sized authored sources with independently compiled annotated functions.

Splat names remain object identities, not authored C filenames. Text before the
first FUNCTION marker is shared; each marker owns text up to the next marker.
"""
from pathlib import Path
import re

import yaml

ROOT = Path(__file__).resolve().parents[1]
MARKER = re.compile(r'^// FUNCTION: ([A-Za-z0-9_.]+):(0x[0-9A-Fa-f]{8})\s*$', re.M)


def source_path(unit):
    unit = Path(unit)
    if unit.parts[:1] == ('overlays',):
        if len(unit.parts) != 3:
            raise ValueError('Invalid overlay unit: ' + str(unit))
        return Path('src/overlays') / (unit.parts[1] + '.c')
    if len(unit.parts) != 1:
        raise ValueError('Invalid boot unit: ' + str(unit))
    return Path('src/main.c')


def sections(text):
    matches = list(MARKER.finditer(text))
    blocks = {}
    for i, match in enumerate(matches):
        identity = f'{match[1]}:0x{int(match[2], 16):08X}'
        if identity in blocks:
            raise ValueError('Duplicate FUNCTION annotation: ' + identity)
        blocks[identity] = text[match.start():matches[i + 1].start() if i + 1 < len(matches) else len(text)]
    return text[:matches[0].start()] if matches else text, blocks


def function_source(text, identity):
    shared, blocks = sections(text)
    if identity not in blocks:
        raise ValueError('Missing FUNCTION annotation: ' + identity)
    return shared + blocks[identity]


def unit_identity(unit, config):
    for segment in config['segments']:
        if not isinstance(segment, dict):
            continue
        for row in segment.get('subsegments', []):
            if (isinstance(row, list) and row[1] == 'c'
                    and Path(segment.get('dir', '')) / row[2] == Path(unit)):
                address = segment['vram'] + row[0] - segment['start']
                return f"{config['name']}:0x{address:08X}"
    raise ValueError('Object is not a configured C unit: ' + str(unit))


def object_source(name, config=None):
    unit = Path(name).relative_to('build/src').with_suffix('')
    source = source_path(unit)
    if config is None:
        config_path = (Path('config/overlays') / (unit.parts[1] + '.yaml')
                       if unit.parts[0] == 'overlays' else Path('scus_943.58.yaml'))
        config = yaml.safe_load((ROOT / config_path).read_text())
    return source, function_source((ROOT / source).read_text(), unit_identity(unit, config))
