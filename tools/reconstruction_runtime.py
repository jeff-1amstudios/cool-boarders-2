"""Deterministic batch runtime: plateau budget, structural cache and detached launch."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import time

import yaml


class Stalled(subprocess.TimeoutExpired):
    def __str__(self):
        return f'No object-score improvement for {self.timeout:g} seconds; best candidate preserved'


def communicate(process, input_text, timeout, progress_path=None, stall_seconds=0):
    deadline = time.monotonic() + timeout
    started = time.time()
    while True:
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            raise subprocess.TimeoutExpired(process.args, timeout)
        try:
            process.communicate(input=input_text, timeout=min(5, max(.05, stall_seconds / 4), remaining) if stall_seconds else remaining)
            return
        except subprocess.TimeoutExpired:
            input_text = None  # communicate may be retried, but stdin must only be sent once.
            if not stall_seconds:
                raise
            progress = {}
            if progress_path and progress_path.exists():
                try:
                    progress = json.loads(progress_path.read_text())
                except (ValueError, OSError):
                    pass
            last = max(started, progress.get('improved_at', started))
            if progress.get('score', 0) < 100 and time.time() - last >= stall_seconds:
                raise Stalled(process.args, stall_seconds)


def structural_key(root, row, report):
    """Invalidate on reference/body/catalog/review/analyzer changes, not unrelated C commits."""
    inputs = {}
    for name in ('config/function-boundaries.json', 'config/psyq-sdk.json', 'config/reconstruction.json',
                 'tools/boundary_audit.py', 'tools/reconstruction_backlog.py', 'tools/reconstruction_runtime.py'):
        path = root / name
        inputs[name] = hashlib.sha256(path.read_bytes()).hexdigest() if path.exists() else None
    catalog = [(r['identity'], r['ranges'], r['body_sha256'], r['status'] == 'sdk')
               for r in report['targets'] if r['image'] == row['image']]
    mapping = []
    config_path = root / row['build_config']
    if config_path.exists():
        config = yaml.safe_load(config_path.read_text())
        for segment in config.get('segments', []):
            if not isinstance(segment, dict):
                continue
            entries = segment.get('subsegments', [])
            for current, following in zip(entries, entries[1:]):
                low = current.get('start') if isinstance(current, dict) else current[0]
                high = following.get('start') if isinstance(following, dict) else following[0]
                if low < row['file_offset'] + row['body_bytes'] and row['file_offset'] < high:
                    mapping.append([segment.get('start'), segment.get('vram'), segment.get('dir'), current, high])
    value = {'inputs': inputs, 'mapping': mapping, 'image': row['image'], 'identity': row['identity'],
             'reference': row['reference_sha256'], 'catalog': catalog}
    return hashlib.sha256(json.dumps(value, sort_keys=True).encode()).hexdigest()


def cached_structural(root, row, report):
    path = root / 'reports/reconstruction/structural-deferrals.json'
    if not path.exists():
        return False
    cache = json.loads(path.read_text())
    record = cache.get(row['identity'])
    return bool(record and record.get('key') == structural_key(root, row, report))


def save_structural(root, active, report):
    path = root / 'reports/reconstruction/structural-deferrals.json'
    cache = json.loads(path.read_text()) if path.exists() else {}
    cache[active['identity']] = {'key': structural_key(root, active, report),
                                'findings': active['boundary_findings'], 'context': active['context'],
                                'recorded_at': time.time()}
    temporary = path.with_suffix('.tmp')
    temporary.write_text(json.dumps(cache, indent=2) + '\n')
    temporary.replace(path)


def start(root, args):
    from reconstruct import exclusive_worker
    directory = root / 'reports/reconstruction'
    # Check the same lock before dispatch; the child owns it for the actual batch.
    with exclusive_worker(directory):
        log = directory / f'batch-{time.time_ns()}.log'
        argv = [sys.executable, 'tools/reconstruct.py', 'run', '--limit', str(args.limit),
                '--minutes', str(args.minutes), '--agent-seconds', str(args.agent_seconds),
                '--stall-seconds', str(args.stall_seconds)]
        if args.retry_structural:
            argv.append('--retry-structural')
        if args.agent_command:
            argv += ['--agent-command', args.agent_command]
        with log.open('w') as stream:
            process = subprocess.Popen(argv, cwd=root, stdin=subprocess.DEVNULL, stdout=stream,
                                       stderr=subprocess.STDOUT, start_new_session=True)
        launch = {'pid': process.pid, 'log': str(log.relative_to(root)), 'argv': argv, 'started_at': time.time()}
        (directory / 'launch.json').write_text(json.dumps(launch, indent=2) + '\n')
    print(json.dumps(launch, indent=2))
    print('Batch runs independently. Read status/summary.json when needed; no model polling is required.')


def usage(path):
    totals, complete, error = {}, False, None
    if path.exists():
        for line in path.read_text().splitlines():
            try:
                event = json.loads(line)
            except ValueError:
                continue
            if event.get('type') == 'error':
                error = str(event.get('message', 'Agent error'))[:600]
            if event.get('type') == 'turn.failed':
                error = str(event.get('error', {}).get('message', error or 'Agent failed'))[:600]
            if event.get('type') == 'turn.completed' and 'usage' in event:
                complete = True
                for key, count in event['usage'].items():
                    totals[key] = totals.get(key, 0) + count
    return {'complete': complete, 'tokens': totals, 'error': error}


def summary(root, session):
    totals, missing = {}, []
    for row in session['completed'] + session['deferred']:
        recorded = usage(root / row['context'] / 'agent.log')
        for key, count in recorded['tokens'].items():
            totals[key] = totals.get(key, 0) + count
        if not recorded['complete'] and not row.get('boundary_findings'):
            missing.append(row.get('identity', row['address']))
    blocked = []
    for row in session['deferred']:
        record = {k: row[k] for k in ('identity', 'address', 'reason', 'checkpoint', 'context') if k in row}
        directory = root / row['context']
        record['agent_error'] = usage(directory / 'agent.log')['error']
        message = directory / 'agent-last-message.txt'
        if message.exists():
            record['agent_result'] = message.read_text()[:1000]
        blocked.append(record)
    return {'session': session['directory'], 'stop_reason': session['stop_reason'],
            'completed': len(session['completed']), 'deferred': len(session['deferred']),
            'matching_c_bytes_added': sum(r['size'] for r in session['completed']),
            'full_validation_passed': session.get('full_validation_passed', False),
            'worker_tokens': totals, 'missing_worker_usage': missing,
            'accepted': [{k: r[k] for k in ('identity', 'address', 'commit') if k in r} for r in session['completed']],
            'blocked': blocked}
