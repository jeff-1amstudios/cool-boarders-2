#!/usr/bin/env python3
"""Rebuild per-function worker usage reports from preserved attempt records."""
import argparse
import csv
import io
import json
from collections import Counter
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
    if measured:
        return measured
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
        yield dict(identity=identity, name=result.get('name', context.get('name')),
                   size_bytes=result.get('size', context.get('size')),
                   status=result.get('status', 'unrecorded'),
                   input_tokens=inp, cached_input_tokens=cached,
                   uncached_input_tokens=inp-cached if inp is not None and cached is not None else None,
                   output_tokens=out, reasoning_output_tokens=tokens.get('reasoning_output_tokens'),
                   total_tokens=inp+out if inp is not None and out is not None else None,
                   usage_complete=agent.get('complete', False),
                   worker_seconds=clock['seconds'], timing_source=clock['source'],
                   attempt_seconds=result.get('timing', {}).get('attempt_seconds'),
                   context=directory.relative_to(root).as_posix())


def refresh(root=ROOT):
    rows = list(attempt_rows(root))
    directory = root / 'reports/decomp'
    directory.mkdir(parents=True, exist_ok=True)
    fields = list(rows[0]) if rows else ['identity', 'context']
    stream = io.StringIO()
    writer = csv.DictWriter(stream, fieldnames=fields)
    writer.writeheader()
    writer.writerows(rows)
    atomic_text(directory / 'function-metrics.csv', stream.getvalue())
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
