#!/usr/bin/env python3
"""Compile one selected C unit, print a compact diff, and record progress (never acceptance)."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
from itertools import zip_longest

import yaml
from rebuild import ROOT
from target_queue import c_spans


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write(path, value):
    temporary = path.with_suffix('.tmp')
    temporary.write_text(json.dumps(value, indent=2) + '\n')
    temporary.replace(path)


def selected_unit(context):
    config = yaml.safe_load((ROOT / context['build_config']).read_text())
    spans = [r for r in c_spans(config) if r['address'] == int(context['address'], 16)
             and r['end'] - r['address'] == context['size']]
    if len(spans) != 1:
        raise RuntimeError('Split exactly the selected function into a C unit first')
    return spans[0]['source'].removeprefix('src/').removesuffix('.c')


def check(context_path, full=False):
    context_path = context_path.resolve()
    context = json.loads(context_path.read_text())
    directory = context_path.parent
    unit = selected_unit(context)
    recipe_path = context.get('compiler_config', 'config/compiler.json')
    inputs = [context['build_config'], context['symbol_file'], recipe_path,
              'config/usa-disc.json', 'config/overlays.json', 'tools/objdiff.py',
              'tools/rebuild.py', 'tools/overlay_rebuild.py', 'tools/reconstruction_step.py',
              'orig/disc/files/' + context['image']]
    signature = {p: digest(ROOT / p) for p in inputs}
    cache_path = directory / 'object-reference.json'
    cache = json.loads(cache_path.read_text()) if cache_path.exists() else {}
    target_path = Path(cache.get('target_path', '/nonexistent'))
    log_path = directory / 'object-check.log'
    with log_path.open('w') as log:
        if (cache.get('inputs') != signature or cache.get('unit') != unit
                or not target_path.is_file() or digest(target_path) != cache.get('target_sha256')):
            # This preparation independently rebuilds and verifies the whole assembly-only image.
            subprocess.run([sys.executable, 'tools/objdiff.py', '--image', context['image'], '--unit', unit],
                           cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, check=True)
            project = json.loads((ROOT / 'objdiff.json').read_text())
            selected = next(u for u in project['units'] if u['name'] == unit)
            for key in ('target_path', 'base_path'):
                selected[key] = str(ROOT / selected[key])
            project['units'] = [selected]
            (directory / 'objdiff.json').write_text(json.dumps(project, indent=2) + '\n')
            target_path = Path(selected['target_path'])
            write(cache_path, {'inputs': signature, 'unit': unit, 'target_path': str(target_path),
                               'target_sha256': digest(target_path)})
        else:
            # The unchanged reference object is reused; source/headers are always compiled fresh.
            subprocess.run(['make', 'build/src/' + unit + '.o'], cwd=ROOT,
                           stdout=log, stderr=subprocess.STDOUT, check=True)
        cli = str(ROOT / 'tools/objdiff/bin/objdiff-cli')
        report_path, diff_path = directory / 'object-report.json', directory / 'object-diff.json'
        for path in (report_path, diff_path):
            path.unlink(missing_ok=True)
        subprocess.run([cli, 'report', 'generate', '-p', str(directory), '-o', str(report_path)],
                       cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, check=True)
        subprocess.run([cli, 'diff', '-p', str(directory), '-u', unit, '-o', str(diff_path), '--format', 'json-pretty'],
                       cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, check=True)
    report = json.loads(report_path.read_text())
    score = report['units'][0]['measures'].get('fuzzy_match_percent', 0)
    print(f'{unit}: {score:.3f}% (diagnostic only)')
    diff = json.loads(diff_path.read_text())
    right = {s['name']: s for s in diff.get('right', {}).get('symbols', [])}
    count = 0
    for symbol in diff.get('left', {}).get('symbols', []):
        if not symbol.get('instructions') or symbol.get('match_percent') == 100:
            continue
        other = right.get(symbol['name'], right.get(symbol['name'].removesuffix('.NON_MATCHING'), {}))
        for left, new in zip_longest(symbol.get('instructions', []), other.get('instructions', []), fillvalue={}):
            a, b = left.get('instruction', {}), new.get('instruction', {})
            if a.get('formatted') != b.get('formatted'):
                if count < 12:
                    print(f"  +0x{int(a.get('address', b.get('address', 0))):X}: {a.get('formatted', '<missing>')} | {b.get('formatted', '<missing>')}")
                count += 1
    print(f'Details: {diff_path.relative_to(ROOT)}; log: {log_path.relative_to(ROOT)}')
    if full:
        path = directory / 'image-validate.log'
        with path.open('w') as log:
            result = subprocess.run([sys.executable, 'tools/validate.py', '--image', context['image'], '--unit', unit],
                                    cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
        print(f"{context['image']}: {'EXACT' if result.returncode == 0 else 'FAILED'}; log: {path.relative_to(ROOT)}")
        return result.returncode
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--context', default=os.environ.get('CB2_RECONSTRUCTION_CONTEXT'))
    parser.add_argument('--validate', action='store_true', help='Also validate the entire selected EXE/BIN')
    args = parser.parse_args()
    if not args.context:
        parser.error('--context or CB2_RECONSTRUCTION_CONTEXT is required')
    try:
        return check(Path(args.context), args.validate)
    except (RuntimeError, OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f'Object check failed: {error}; inspect the context directory/object-check.log', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
