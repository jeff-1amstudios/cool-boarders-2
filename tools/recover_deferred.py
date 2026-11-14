#!/usr/bin/env python3
"""Recover a preserved 100% candidate without launching a decomp worker.

run performs the full recovery; prepare merges additions; check validates and plans;
accept commits and saves analysis; sync resumes a post-commit save interruption.
"""
import argparse
import difflib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess

import decomp as worker
import decomp_ghidra as ghidra
from c_sources import sections
from decomp_backlog import prepare, slug, source_file
from decomp_run import review_header_extension, review_shared_extension, review_source, review_split
from target_queue import c_spans
import yaml

ROOT = worker.ROOT


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def git_file(commit, path):
    return subprocess.check_output(['git', 'show', f'{commit}:{path}'], cwd=ROOT).decode()


def normalized(text):
    return re.sub(r'\s+', ' ', text).strip()


def additions(before, after):
    """Ignore separator-only changes, reject changes to established content."""
    left, right = before.splitlines(True), after.splitlines(True)
    added = []
    for op, low, high, start, end in difflib.SequenceMatcher(None, left, right, autojunk=False).get_opcodes():
        if op == 'equal':
            continue
        require(not ''.join(left[low:high]).strip(), 'Archived candidate changes existing declarations; review manually')
        added.append(''.join(right[start:end]))
    return ''.join(added)


def merge_symbols(before, after, current):
    require(after.startswith(before), 'Archived candidate changes existing symbols')
    pattern = r'(?m)^([A-Za-z_]\w*)\s*=\s*(0x[0-9a-fA-F]+);[^\n]*'
    existing = {name: (int(address, 16), line) for line in current.splitlines()
                for name, address in re.findall(pattern, line)}
    renames = {}
    for line in after[len(before):].splitlines():
        match = re.match(pattern, line)
        if not match:
            require(not line.strip() or line.lstrip().startswith('//'), 'Unsupported symbol addition')
            continue
        name, address = match[1], int(match[2], 16)
        if name in existing:
            require(existing[name][0] == address, f'Conflicting symbol address: {name}')
            continue
        aliases = [key for key, value in existing.items() if value[0] == address]
        if aliases:
            require(len(aliases) == 1, f'Ambiguous existing aliases at {address:#x}')
            renames[name] = aliases[0]
        else:
            current = current.rstrip() + '\n' + line + '\n'
            existing[name] = (address, line)
    return current, renames


def rename(text, renames):
    for old, new in renames.items():
        text = re.sub(r'\b' + re.escape(old) + r'\b', new, text)
    return text


def merge_shared(before, candidate, current):
    extra = additions(before, candidate)
    extern_pattern = r'\bextern\s+[^;{}]+;'
    declarations = re.findall(extern_pattern, extra)
    require(not re.sub(extern_pattern, '', extra).strip(), 'New shared content requires manual review')
    existing = re.findall(extern_pattern, current)
    for declaration in declarations:
        if normalized(declaration) not in map(normalized, existing):
            current += declaration + '\n'
            existing.append(declaration)
    return current


def split_target(config_text, active):
    config = yaml.safe_load(config_text)
    low, high = active['file_offset'], active['file_offset'] + active['size']
    rows = list(re.finditer(r'(?m)^([ \t]*)-\s*\[\s*(0x[0-9a-fA-F]+|\d+),\s*(\w+),\s*([^\]]+)\][^\n]*\n?', config_text))
    # Only a current retained-assembly span may be replaced. Never replay stale neighbouring splits.
    match = next((m for m in rows if int(m[2], 0) == active['containing_span']['start'] and m[3] == 'asm'), None)
    require(match is not None, 'Current assembly span was not found')
    prefix = match[1]
    replacement = ''
    if int(match[2], 0) < low:
        replacement += match[0]
    replacement += f'{prefix}- [0x{low:X}, c, {active["name"]}]\n'
    if high < active['containing_span']['end']:
        replacement += f'{prefix}- [0x{high:X}, asm, after_{active["name"]}]\n'
    updated = config_text[:match.start()] + replacement + config_text[match.end():]
    review_split(config, yaml.safe_load(updated), active)
    return updated


def paths_for(active):
    source = source_file(active)
    return [source, str(Path(source).with_suffix('.h')), active['build_config'], active['symbol_file'],
            'config/ghidra/' + slug(active) + '.json']


def fingerprint(paths):
    return {p: ghidra.digest((ROOT / p).read_bytes()) for p in paths if (ROOT / p).is_file()}


def validation_inputs():
    paths = set(worker.PROTECTED)
    paths.update(p.relative_to(ROOT).as_posix() for p in (ROOT / 'src').rglob('*') if p.is_file())
    paths.update(['scus_943.58.yaml', 'symbol_addrs.txt', 'tools/recover_deferred.py'])
    return fingerprint(sorted(paths))


def review_current(directory, active, recovery):
    before = worker.read(directory / 'before.json')
    source = source_file(active)
    old_shared, old_blocks = sections(before[source])
    new_shared, new_blocks = sections((ROOT / source).read_text())
    review_shared_extension(old_shared, new_shared)
    require({k: v.rstrip() for k, v in new_blocks.items() if k != active['identity']}
            == {k: v.rstrip() for k, v in old_blocks.items()}, 'Existing function blocks changed')
    require(active['identity'] in new_blocks, 'Selected function block is missing')
    for path, text in before.items():
        if path.endswith('.h'):
            review_header_extension(text, (ROOT / path).read_text())
    review_split(yaml.safe_load(before[active['build_config']]),
                 yaml.safe_load((ROOT / active['build_config']).read_text()), active)
    require((ROOT / active['symbol_file']).read_text().startswith(before[active['symbol_file']]),
            'Existing symbols changed')
    review_source(ROOT, recovery['paths'], source, active['name'])
    subprocess.run(['git', 'diff', '--check', '--', *recovery['paths']], cwd=ROOT, check=True)


def prepare_candidate(identity, directory):
    require(worker.git('branch', '--show-current') == 'main', 'Recovery requires main')
    require(not worker.git('diff', '--cached', '--name-only'), 'Recovery requires an empty index')
    entries = worker.deferred_targets()
    require(identity in entries, 'Target is not deferred')
    old = entries[identity]
    prior = ROOT / old['context']
    report = worker.read(prior / 'object-report.json')
    require(report.get('measures', {}).get('fuzzy_match_percent') == 100, 'Recovery requires a preserved 100% object candidate')
    inventory = worker.inventory()
    row = next(r for r in inventory['targets'] if r['identity'] == identity)
    require(row['status'] == 'needs_review', 'Target is not retained eligible assembly')
    active = prepare(ROOT, row, inventory)
    require(not active['boundary_findings'], f'Current boundary blockers: {active["boundary_findings"]}')
    active.update(context=directory.relative_to(ROOT).as_posix())
    allowed = paths_for(active)
    require(not any(name in worker.git('diff', '--name-only').splitlines() for name in allowed), 'Commit or preserve pending image edits before recovery')
    directory.mkdir(parents=True, exist_ok=False)
    worker.write(directory / 'context.json', active)
    worker.write(directory / 'original-deferral.json', old)
    ghidra.export(worker, active)
    archived = worker.read(prior / 'candidate/sources.json')
    result = worker.read(prior / 'result.json')
    source = source_file(active)
    current_source = (ROOT / source).read_text()
    current_shared, current_blocks = sections(current_source)
    before_shared, _ = sections(git_file(result['base_commit'], source))
    candidate_shared, candidate_blocks = sections(archived[source])
    symbols = active['symbol_file']
    proposed_symbols, renames = merge_symbols(git_file(result['base_commit'], symbols),
                                              git_file(old['checkpoint'], symbols), (ROOT / symbols).read_text())
    proposed_shared = merge_shared(rename(before_shared, renames), rename(candidate_shared, renames), current_shared)
    review_shared_extension(current_shared, proposed_shared)
    require(identity not in current_blocks, 'Selected function already exists')
    proposed_source = proposed_shared + current_source[len(current_shared):].rstrip() + '\n\n' + rename(candidate_blocks[identity], renames).rstrip() + '\n'
    updates = {source: proposed_source, symbols: proposed_symbols,
               active['build_config']: split_target((ROOT / active['build_config']).read_text(), active)}
    for path, text in archived.items():
        if path == source:
            continue
        require(path == str(Path(source).with_suffix('.h')), 'Archived extra headers need manual review')
        before = git_file(result['base_commit'], path)
        extra = additions(before, text)
        current = (ROOT / path).read_text()
        require(extra.strip() not in current, 'Header addition is already present; reconcile manually')
        closing = current.rfind('#endif')
        require(closing >= 0, 'Image header has no closing guard')
        proposed = current[:closing] + extra + current[closing:]
        review_header_extension(current, proposed)
        updates[path] = proposed
    # Archive rollback inputs before touching authored files. A failed check never saves Ghidra.
    worker.write(directory / 'before.json', {p: (ROOT / p).read_text() for p in updates})
    for path, text in updates.items():
        (ROOT / path).write_text(text)
    review_source(ROOT, list(updates), source, active['name'])
    subprocess.run(['git', 'diff', '--check', '--', *updates], cwd=ROOT, check=True)
    proposal = worker.read(prior / 'ghidra-update.json')
    proposal = json.loads(rename(json.dumps(proposal), renames))
    worker.write(directory / 'ghidra-update.json', proposal)
    worker.write(directory / 'recovery.json', {'phase': 'prepared', 'base_commit': worker.git('rev-parse', 'HEAD'),
                                               'paths': list(updates), 'renames': renames})
    print(f'Prepared {identity}; inspect {directory.relative_to(ROOT)} and git diff before check/accept.')


def check(directory, active, recovery):
    require(worker.git('rev-parse', 'HEAD') == recovery['base_commit'], 'HEAD changed since preparation')
    require(recovery['phase'] in ('prepared', 'checked'), 'Recovery is already committed')
    review_current(directory, active, recovery)
    inputs = validation_inputs()
    unit = next(s['unit'] for s in c_spans(yaml.safe_load((ROOT / active['build_config']).read_text())) if s['address'] == int(active['address'], 16))
    with (directory / 'validate.log').open('w') as log:
        subprocess.run(['make', 'validate', 'IMAGE=' + active['image'], 'UNIT=' + unit], cwd=ROOT,
                       stdout=log, stderr=subprocess.STDOUT, env=os.environ | {'TQDM_DISABLE': '1'}, check=True)
    report = ROOT / ('reports/boot-rebuild.json' if active['image'] == 'SCUS_943.58' else 'reports/overlay-rebuild/' + Path(active['build_config']).stem + '.json')
    require(worker.read(report)['identical'], 'Containing file does not match')
    shutil.copy2(report, directory / 'validation.json')
    shutil.copy2(ROOT / 'reports/objdiff.json', directory / 'objdiff.json')
    payload = ghidra.prepare_update(worker, active)
    require(validation_inputs() == inputs, 'Validation inputs changed during checking')
    record = 'config/ghidra/' + slug(active) + '.json'
    require(not (ROOT / record).exists() or record in recovery['paths'], 'Unrelated analysis record exists')
    worker.write(ROOT / record, payload)
    recovery.update(phase='checked', paths=list(dict.fromkeys(recovery['paths'] + [record])))
    recovery['hashes'] = fingerprint(recovery['paths'])
    recovery['validation_inputs'] = validation_inputs()
    worker.write(directory / 'recovery.json', recovery)
    print('Exact containing-file match and Ghidra plan passed; ready for accept.')


def sync(directory, active, recovery):
    require(worker.git('rev-parse', 'HEAD') == recovery['commit'], 'HEAD changed since acceptance')
    require(fingerprint(recovery['paths']) == recovery['hashes'], 'Accepted files changed')
    payload = worker.read(ROOT / ('config/ghidra/' + slug(active) + '.json'))
    ghidra.apply(worker, active, payload)
    ghidra.invoke(worker, directory / 'confirmation', ghidra.request(ROOT, active), 'export')
    entries = worker.deferred_targets()
    entries.pop(active['identity'], None)
    worker.write(worker.STATE.parent / 'deferred.json', entries)
    recovery['phase'] = 'complete'
    worker.write(directory / 'recovery.json', recovery)
    print(f'Recovered {active["identity"]}: {recovery["commit"]}; analysis saved and confirmed.')


def accept(directory, active, recovery):
    require(recovery['phase'] == 'checked', 'Run check first')
    require(worker.git('branch', '--show-current') == 'main', 'Acceptance requires main')
    require(worker.git('rev-parse', 'HEAD') == recovery['base_commit'], 'HEAD changed since validation')
    require(fingerprint(recovery['paths']) == recovery['hashes'], 'Candidate changed since validation; rerun check')
    require(validation_inputs() == recovery['validation_inputs'], 'Validation inputs changed; rerun check')
    require(not worker.git('diff', '--cached', '--name-only'), 'Acceptance requires an empty index')
    paths = recovery['paths']
    subprocess.run(['git', 'add', '--', *paths], cwd=ROOT, check=True)
    staged = worker.git('diff', '--cached', '--name-only').splitlines()
    require(set(staged) <= set(paths), 'Staged files exceed reviewed candidate scope')
    require(all(p.startswith(('src/', 'config/ghidra/')) or p in (active['symbol_file'], active['build_config']) for p in staged), 'Staged artifact is outside authored scope')
    print('Staged file audit:', ', '.join(staged), flush=True)
    subprocess.run(['git', 'diff', '--cached', '--check'], cwd=ROOT, check=True)
    subprocess.run(['git', 'commit', '-m', 'Recover matching ' + active['identity'] + ' from preserved candidate'], cwd=ROOT, check=True)
    recovery.update(phase='committed', commit=worker.git('rev-parse', 'HEAD'))
    worker.write(directory / 'recovery.json', recovery)
    sync(directory, active, recovery)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('phase', choices=['run', 'prepare', 'check', 'accept', 'sync'])
    parser.add_argument('--target', required=True, action='append', help='Image-qualified identity; repeat for a run batch')
    parser.add_argument('--directory', help='Ignored recovery directory relative to the repository')
    args = parser.parse_args()
    require(len(args.target) == 1 or args.phase == 'run', 'Multiple targets require run')
    require(len(args.target) == 1 or not args.directory, 'Multiple targets use separate default directories')
    with worker.exclusive_worker(worker.STATE.parent):
        require(not worker.state().get('active'), 'Automatic decomp has an active attempt')
        for identity in args.target:
            directory = (ROOT / (args.directory or 'reports/decomp/recoveries/' + slug({'identity': identity}))).resolve()
            require(directory.is_relative_to(ROOT / 'reports'), 'Recovery directory must stay under reports/')
            if args.phase == 'prepare' or (args.phase == 'run' and not directory.exists()):
                prepare_candidate(identity, directory)
            if args.phase == 'prepare':
                continue
            active = worker.read(directory / 'context.json')
            require(active['identity'] == identity, 'Recovery directory belongs to another function')
            recovery = worker.read(directory / 'recovery.json')
            if args.phase == 'run':
                if recovery['phase'] == 'prepared':
                    check(directory, active, recovery)
                if recovery['phase'] == 'checked':
                    accept(directory, active, recovery)
                elif recovery['phase'] == 'committed':
                    sync(directory, active, recovery)
                continue
            if args.phase == 'sync':
                require(recovery['phase'] == 'committed', 'No interrupted save to resume')
            globals()[args.phase](directory, active, recovery)
        if args.phase == 'run':
            log_path = worker.STATE.parent / 'recovery-final-validate.log'
            with log_path.open('w') as log:
                subprocess.run(['make', 'validate'], cwd=ROOT, stdout=log, stderr=subprocess.STDOUT,
                               env=os.environ | {'TQDM_DISABLE': '1'}, check=True)
            print('Final all-image validation passed; log:', log_path.relative_to(ROOT))


if __name__ == '__main__':
    main()
