"""Adapter between Ghidra function instances and bounded reconstruction attempts."""
import hashlib
import re
from pathlib import Path

import rabbitizer
import yaml
from boundary_audit import analyze_cfg, decode
from target_queue import rebuild


def key(row):
    return row.get('identity', row['address'])


def slug(row):
    return re.sub(r'[^A-Za-z0-9_-]', '_', key(row))


def source_dir(row):
    if row.get('image', 'SCUS_943.58') == 'SCUS_943.58':
        return 'src'
    return 'src/overlays/' + Path(row['build_config']).stem


def coverage(report):
    return {key(r): r.get('body_bytes', r.get('size')) for r in report.get('targets', report.get('functions', []))
            if r['status'] == 'c' or r.get('reconstruction') == 'c'}


def inventory():
    return rebuild()


def prepare(root, row, report):
    """Byte-bound structural evidence only for the selected function, never a new denominator."""
    image = row['image']
    data = (root / 'orig/disc/files' / image).read_bytes()
    if hashlib.sha256(data).hexdigest() != row['reference_sha256']:
        raise RuntimeError('Selected image fingerprint changed')
    start = int(row['address'], 16)
    base = start - row['file_offset']
    ranges = [(int(a, 16), int(b, 16)) for a, b in row['ranges']]
    findings = {}
    if len(ranges) != 1 or ranges[0][0] != start or any((b-a) % 4 or a % 4 for a,b in ranges):
        findings['noncontiguous_or_unaligned_body'] = row['ranges']
    embedded_sdk = [key(r) for r in report['targets'] if r['image'] == image and r['status'] == 'sdk'
                    and any(a <= int(r['address'],16) < b for a,b in ranges)]
    if embedded_sdk:
        findings['contains_accepted_sdk_entries'] = embedded_sdk
    code, lines = {}, []
    for a,b in ranges:
        for pc in range(a,b-3,4):
            raw = data[pc-base:pc-base+4]
            word = int.from_bytes(raw, 'little')
            code[pc] = decode(word, pc)
            text = rabbitizer.Instruction(word, pc, rabbitizer.InstrCategory.R3000GTE).disassemble()
            lines.append(f'/* {pc-base:X} {pc:08X} {raw.hex().upper()} */ {text}')
    if code and not findings:
        findings.update(analyze_cfg(code, start)['findings'])
    incoming, calls = [], []
    # Scan only byte-checked Ghidra code ranges in this image, not arbitrary data.
    for other in report['targets']:
        if other['image'] != image:
            continue
        for a,b in other['ranges']:
            for pc in range(int(a,16), int(b,16)-3,4):
                ins = decode(int.from_bytes(data[pc-base:pc-base+4], 'little'), pc)
                destination = ins['target']
                if destination in code and not any(lo <= pc < hi for lo,hi in ranges):
                    incoming.append({'from': f'0x{pc:08X}', 'to': f'0x{destination:08X}', 'owner': key(other)})
                if pc in code and destination is not None:
                    calls.append({'from': f'0x{pc:08X}', 'to': f'0x{destination:08X}', 'kind': ins['kind']})
    interior = [r for r in incoming if int(r['to'],16) != start]
    if interior:
        findings['incoming_transfer_to_interior'] = interior
    config = yaml.safe_load((root / row['build_config']).read_text())
    span = None
    for segment in config['segments']:
        if not isinstance(segment, dict):
            continue
        rows = segment.get('subsegments', [])
        for i,r in enumerate(rows[:-1]):
            if isinstance(r,list) and r[1] == 'asm' and r[0] <= row['file_offset'] and row['file_offset'] + row['body_bytes'] <= rows[i+1][0]:
                span = {'start': r[0], 'end': rows[i+1][0], 'name': r[2]}
    if span is None:
        findings['no_containing_assembly_span'] = row['build_config']
    neighbours = []
    for pc in range(max(0,row['file_offset']-32), min(len(data)-3,row['file_offset']+row['body_bytes']+32),4):
        neighbours.append(f'{pc+base:08X}: ' + rabbitizer.Instruction(int.from_bytes(data[pc:pc+4],'little'), pc+base, rabbitizer.InstrCategory.R3000GTE).disassemble())
    pointers = []
    for offset in range(0, len(data)-3, 4):
        value = int.from_bytes(data[offset:offset+4], 'little')
        if any(a <= value < b for a,b in ranges):
            pointers.append({'at': f'0x{offset+base:08X}', 'value': f'0x{value:08X}'})
    return {**row, 'possible_pointer_words': pointers, 'offset' : row['file_offset'], 'size': row['body_bytes'],
            'body': '\n'.join(lines), 'containing_span': span, 'incoming': incoming,
            'control_references': calls, 'neighbour_listing': neighbours,
            'source_directory': source_dir(row), 'boundary_findings': findings,
            'boundary_review': 'deferred' if findings else 'requires_agent_review'}
