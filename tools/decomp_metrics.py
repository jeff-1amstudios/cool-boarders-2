#!/usr/bin/env python3
"""Rebuild per-function worker usage reports from preserved attempt records."""
import argparse
import csv
import io
import json
from collections import Counter, defaultdict
from datetime import datetime, timezone
from pathlib import Path

from decomp_runtime import usage

ROOT = Path(__file__).resolve().parents[1]


def read(path):
    return json.loads(path.read_text()) if path.exists() else {}


def atomic_text(path, text):
    temporary = path.with_suffix(path.suffix + '.tmp')
    temporary.write_text(text)
    temporary.replace(path)


def timing(directory):
    """Old log mtimes approximate launcher wall time, never exact model time."""
    measured = read(directory / 'worker-timing.json')
    phases = {name.replace('-', '_'): read(directory / name / 'worker-timing.json')
              for name in ('merge-repair', 'layout-repair')}
    samples = ([measured] if measured else []) + [v for v in phases.values() if v]
    if samples:
        return dict(started_at=min(v['started_at'] for v in samples),
                    finished_at=max(v['finished_at'] for v in samples),
                    seconds=round(sum(v['seconds'] for v in samples), 3),
                    source='measured_monotonic', **{k: v for k, v in phases.items() if v})
    prompt, log = directory / 'agent-prompt.md', directory / 'agent.log'
    if prompt.exists() and log.exists():
        start, end = prompt.stat().st_mtime, log.stat().st_mtime
        if end >= start:
            return dict(started_at=start, finished_at=end, seconds=round(end-start, 3),
                        source='estimated_file_mtimes')
    return dict(seconds=None, source='unavailable')


def attempt_rows(root):
    base = root / 'reports/decomp'
    # Include interrupted/missing-result attempts; keep retries as separate rows.
    directories = {p.parent for p in (base / 'sessions').glob('*/*/context.json')}
    directories.update(p.parent for p in (base / 'sessions').glob('*/*/result.json'))
    for directory in sorted(directories):
        context, result = read(directory / 'context.json'), read(directory / 'result.json')
        identity = result.get('identity', context.get('identity'))
        if not identity and context.get('address'):
            identity = context.get('image', 'SCUS_943.58') + ':' + context['address']
        if not identity:
            continue
        agent = result.get('agent', {})
        if not agent.get('tokens'):
            agent = usage(directory / 'agent.log')
        tokens = agent.get('tokens', {})
        inp, out = tokens.get('input_tokens'), tokens.get('output_tokens')
        cached = tokens.get('cached_input_tokens')
        clock = timing(directory)
        attempt_timing = result.get('timing', {})
        yield dict(identity=identity, name=result.get('name', context.get('name')),
                   model=agent.get('model'), requested_model=result.get('requested_model'),
                   effort=agent.get('effort'), requested_effort=result.get('requested_effort'),
                   size_bytes=result.get('size', context.get('size')),
                   status=result.get('status', 'unrecorded'),
                   input_tokens=inp, cached_input_tokens=cached,
                   uncached_input_tokens=inp-cached if inp is not None and cached is not None else None,
                   output_tokens=out, reasoning_output_tokens=tokens.get('reasoning_output_tokens'),
                   total_tokens=inp+out if inp is not None and out is not None else None,
                   usage_complete=agent.get('complete', False),
                   agent_tokens=result.get('agent_tokens'),
                   budget_reached=bool(agent.get('budget_stop')),
                   worker_seconds=clock['seconds'], timing_source=clock['source'],
                   attempt_seconds=attempt_timing.get('attempt_seconds'),
                   attempt_started_at=attempt_timing.get('started_at'),
                   session=directory.parent.relative_to(root).as_posix(),
                   context=directory.relative_to(root).as_posix())


def refresh_batches(root, rows=None):
    directory = root / 'reports/decomp'
    history = defaultdict(list)
    for row in attempt_rows(root) if rows is None else rows:
        history[row['session']].append(row)
    batches = []
    invocations = []
    for session in sorted((directory / 'sessions').glob('*'), reverse=True):
        if not session.is_dir():
            continue
        path = session.relative_to(root).as_posix()
        runs = read(session / 'batch.json').get('runs', [])
        functions = {row['context']: row for row in history[path]}
        selected = {row['context']: row for run in runs for row in run['functions']}
        legacy = sorted(functions.keys() - selected.keys(),
                        key=lambda context: (functions[context]['attempt_started_at']
                                             or (root / context).stat().st_mtime, context))
        ordered = [*legacy, *selected]
        groups = [(None, legacy), *[(run, [row['context'] for row in run['functions']]) for run in runs]]
        for number, (run, contexts) in enumerate(groups):
            if not contexts and run is None:
                continue
            # Selected attempts are unique even if recovery revisits an identity.
            attempts = [functions[context] for context in dict.fromkeys(contexts) if context in functions]
            models = sorted({row['model'] for row in attempts if row['model']})
            efforts = sorted({row['effort'] for row in attempts if row['effort']})
            inp = sum(row['input_tokens'] or 0 for row in attempts)
            cached = sum(row['cached_input_tokens'] or 0 for row in attempts)
            out = sum(row['output_tokens'] or 0 for row in attempts)
            invocations.append(dict(
                session=path, invocation=number, command=(run or {}).get('command'),
                started_at=(run or {}).get('started_at'), ended_at=(run or {}).get('ended_at'),
                outcome=(run or {}).get('outcome', 'historical'),
                requested_model=(run or {}).get('model'), models=', '.join(models),
                requested_effort=(run or {}).get('effort'), efforts=', '.join(efforts),
                attempts=len(attempts), matched=sum(row['status'] == 'matched' for row in attempts),
                deferred=sum(row['status'] == 'deferred' for row in attempts),
                incomplete_usage=sum(not row['usage_complete'] for row in attempts),
                input_tokens=inp, cached_input_tokens=cached, output_tokens=out,
                total_tokens=inp + out, uncached_input_plus_output_tokens=inp - cached + out))
        final = session / 'result.json'
        started_at, ended_at = None, None
        started_source, ended_source = None, None
        if runs:
            started_at, started_source = runs[0]['started_at'], 'recorded'
            ended_at, ended_source = runs[-1]['ended_at'], 'recorded' if runs[-1]['ended_at'] else None
        if (not runs or legacy) and session.name.isdecimal():
            started_at = datetime.fromtimestamp(int(session.name) / 1e9, timezone.utc).isoformat()
            started_source = 'session_id'
        if not runs and final.is_file():
            ended_at = datetime.fromtimestamp(final.stat().st_mtime, timezone.utc).isoformat()
            ended_source = 'estimated_result_mtime'
        targets = []
        for context in ordered:
            row = functions.get(context) or selected[context]
            targets.append({key: row.get(key) for key in ('identity', 'name', 'status', 'context')})
        batches.append(dict(session=path, started_at=started_at, started_source=started_source,
                            ended_at=ended_at, ended_source=ended_source, runs=runs, functions=targets))
    directory.mkdir(parents=True, exist_ok=True)
    atomic_text(directory / 'batches.json', json.dumps(batches, indent=2) + '\n')
    stream = io.StringIO()
    writer = csv.DictWriter(stream, fieldnames=list(invocations[0]) if invocations else ['session', 'invocation'])
    writer.writeheader()
    writer.writerows(invocations)
    atomic_text(directory / 'batch-metrics.csv', stream.getvalue())


def refresh(root=ROOT):
    rows = list(attempt_rows(root))
    directory = root / 'reports/decomp'
    directory.mkdir(parents=True, exist_ok=True)
    fields = list(rows[0]) if rows else ['identity', 'session', 'context']
    stream = io.StringIO()
    writer = csv.DictWriter(stream, fieldnames=fields)
    writer.writeheader()
    writer.writerows(rows)
    atomic_text(directory / 'function-metrics.csv', stream.getvalue())
    refresh_batches(root, rows)
    totals = {key: sum(row[key] or 0 for row in rows) for key in
              ('input_tokens', 'cached_input_tokens', 'uncached_input_tokens',
               'output_tokens', 'reasoning_output_tokens', 'total_tokens', 'worker_seconds')}
    summary = dict(generated_at=datetime.now(timezone.utc).isoformat(), attempts=len(rows),
                   functions=len({r['identity'] for r in rows}),
                   statuses=dict(Counter(r['status'] for r in rows)),
                   incomplete_usage=sum(not r['usage_complete'] for r in rows),
                   timing_sources=dict(Counter(r['timing_source'] for r in rows)), totals=totals)
    atomic_text(directory / 'function-metrics.json', json.dumps(summary, indent=2) + '\n')
    lines = ['# Per-function decomp usage', '',
             f"Generated: {summary['generated_at']}", '',
             f"{len(rows)} attempts across {summary['functions']} functions. "
             f"Statuses: {summary['statuses']}. Incomplete usage: {summary['incomplete_usage']}.", '',
             'Full per-attempt data, including retries: [function-metrics.csv](function-metrics.csv).', '',
             'Input includes cached input; reasoning is a subset of output. Total = input + output. '
             'These are worker tokens only; supervisor/chat usage is excluded. Missing usage is blank, '
             'and interrupted turns may have unreported consumption. Totals include only reported values.', '',
             'Worker seconds cover the launcher subprocess. Historical file-mtime estimates are labelled; '
             'copied/touched files can distort them. Attempt seconds, when recorded, include fresh export, '
             'validation and integration. Summed worker time is not batch wall time.', '',
             '| Metric | Total |', '| --- | ---: |']
    lines += [f'| {key} | {value:,.3f} |' if isinstance(value, float) else f'| {key} | {value:,} |'
              for key, value in totals.items()]
    lines += ['', '## Largest recorded token counts', '',
              '| Function | Status | Total tokens | Worker seconds | Timing source | Attempt |',
              '| --- | --- | ---: | ---: | --- | --- |']
    for row in sorted(rows, key=lambda r: r['total_tokens'] or 0, reverse=True)[:15]:
        lines.append(f"| {row['identity']} | {row['status']} | {row['total_tokens'] or 'unknown'} | "
                     f"{row['worker_seconds']} | {row['timing_source']} | "
                     f"[record]({row['context'].removeprefix('reports/decomp/')}/result.json) |")
    atomic_text(directory / 'function-metrics.md', '\n'.join(lines) + '\n')
    return summary


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.parse_args()
    print(json.dumps(refresh(), indent=2))
