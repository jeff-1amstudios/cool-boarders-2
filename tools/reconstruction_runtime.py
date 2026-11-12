"""Stream child output and summarize recorded agent usage."""
import json
import re
import shlex
from pathlib import PurePosixPath


def file_read_command(command):
    """Recognize common file-display commands, including shell-wrapped reads."""
    try:
        lexer = shlex.shlex(command, posix=True, punctuation_chars=True)
        lexer.whitespace_split = True
        lexer.commenters = ''
        tokens = list(lexer)
    except ValueError:
        return False
    if not tokens:
        return False
    if PurePosixPath(tokens[0]).name in ('sh', 'bash', 'zsh'):
        if len(tokens) == 3 and tokens[1] in ('-c', '-lc'):
            return file_read_command(tokens[2])
        return False
    commands, current = [], []
    for token in tokens:
        if token in (';', '&&', '||', '|'):
            if not current:
                return False
            commands.append(current)
            current = []
        elif token in ('&', '>', '>>', '<', '<<', '(', ')'):
            return False
        else:
            current.append(token)
    if current:
        commands.append(current)
    return bool(commands) and all(
        PurePosixPath(words[0]).name in ('cat', 'head', 'tail', 'sed', 'rg', 'grep')
        and not (PurePosixPath(words[0]).name == 'sed'
                 and any(arg.startswith('-i') for arg in words[1:]))
        for words in commands)


class Output:
    """Render agent JSON events for humans while retaining raw JSON in the log."""
    def __init__(self, agent=False, verbose=False, ghidra=False):
        self.agent, self.verbose, self.pending = agent, verbose, ''
        self.ghidra = ghidra
        self.ghidra_level = None

    def write(self, chunk):
        self.pending += chunk
        while '\n' in self.pending:
            line, self.pending = self.pending.split('\n', 1)
            self.line(line)

    def text(self, text):
        for line in text.splitlines(keepends=True):
            if self.ghidra:
                level = re.match(r'^\s*(TRACE|DEBUG|INFO|WARN|ERROR|FATAL)\b', line)
                if level:
                    self.ghidra_level = level[1]
                elif line.strip() and not line.startswith((' ', '\t')):
                    # Unclassified launcher errors must remain visible.
                    self.ghidra_level = None
                # Retain warning/error continuations, including Java stack traces.
                if self.ghidra_level in ('TRACE', 'DEBUG', 'INFO'):
                    continue
            if self.verbose or not line.startswith('+ '):
                print(line, end='', flush=True)

    def line(self, line):
        if not self.agent:
            self.text(line + '\n')
            return
        try:
            event = json.loads(line)
        except ValueError:
            self.text(line + '\n')
            return
        if not isinstance(event, dict):
            self.text(line + '\n')
            return
        item = event.get('item', {})
        if event.get('type') == 'item.completed' and item.get('type') == 'agent_message':
            print(item.get('text', ''), flush=True)
        elif item.get('type') == 'command_execution':
            if event.get('type') == 'item.started':
                print('$ ' + item.get('command', ''), flush=True)
            elif event.get('type') == 'item.completed':
                if (self.verbose or item.get('exit_code') != 0
                        or not file_read_command(item.get('command', ''))):
                    self.text(item.get('aggregated_output', ''))
        elif event.get('type') == 'error':
            print('Agent error: ' + str(event.get('message', 'unknown error')), flush=True)
        elif event.get('type') == 'turn.failed':
            print('Agent failed: ' + str(event.get('error', {}).get('message', 'unknown error')), flush=True)

    def finish(self):
        if self.pending:
            self.line(self.pending)
            self.pending = ''


def follow_log(path, done, agent=False, verbose=False, ghidra=False):
    """Tee child output without competing with communicate() for its pipes."""
    output = Output(agent, verbose, ghidra)
    with path.open(errors='replace') as stream:
        while not done.is_set():
            chunk = stream.read(8192)
            if chunk:
                output.write(chunk)
            else:
                done.wait(.05)
        output.write(stream.read())
    output.finish()


def usage(path):
    totals, complete, error, message = {}, False, None, None
    if path.exists():
        for line in path.read_text().splitlines():
            try:
                event = json.loads(line)
            except ValueError:
                continue
            if event.get('type') == 'item.completed' and event.get('item',{}).get('type') == 'agent_message':
                message = event['item'].get('text')
            if event.get('type') == 'error':
                error = str(event.get('message', 'Agent error'))[:600]
            if event.get('type') == 'turn.failed':
                error = str(event.get('error', {}).get('message', error or 'Agent failed'))[:600]
            if event.get('type') == 'turn.completed' and 'usage' in event:
                complete = True
                for key, count in event['usage'].items():
                    totals[key] = totals.get(key, 0) + count
    return {'complete': complete, 'tokens': totals, 'error': error, 'message': message}


def attempt_result(root, row, status, reason=None):
    result = {k:row[k] for k in ('identity','image','address','name','size','context','branch','base_commit','commit','checkpoint') if k in row}
    directory = root / row['context']
    result.update(status=status, reason=reason, agent=usage(directory / 'agent.log'))
    custom_report = directory / 'agent-last-message.txt'
    if custom_report.exists():
        result['agent']['message'] = custom_report.read_text()
    return result


def summary(root, session):
    totals, missing = {}, []
    for row in session['completed'] + session['deferred']:
        recorded = usage(root / row['context'] / 'agent.log')
        for key, count in recorded['tokens'].items():
            totals[key] = totals.get(key, 0) + count
        if not recorded['complete'] and not row.get('boundary_findings'):
            missing.append(row.get('identity', row['address']))
    return {'session': session['directory'], 'stop_reason': session['stop_reason'],
            'completed': len(session['completed']), 'deferred': len(session['deferred']),
            'matching_c_bytes_added': sum(r['size'] for r in session['completed']),
            'full_validation_passed': session.get('full_validation_passed', False),
            'worker_tokens': totals, 'missing_worker_usage': missing,
            'attempts': [r['context'] + '/result.json' for r in session['completed'] + session['deferred']]}
