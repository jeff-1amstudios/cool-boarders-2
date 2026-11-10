"""Supervisor-owned fresh decompilation and accepted, replayable analysis feedback."""
import hashlib
import json
from pathlib import Path
import re
import time

from c_sources import sections
from reconstruction_backlog import source_file, slug


def digest(payload):
    return hashlib.sha256(payload).hexdigest()


def request(root, active):
    catalog = json.loads((root / 'config/psyq-sdk.json').read_text())
    return {k: active[k] for k in ('image', 'address', 'ranges', 'body_sha256')} | {
        'schema_version': 1,
        'references': {'SCUS_943.58': catalog['reference_sha256'],
                       **{image: row['reference_sha256'] for image, row in catalog['overlays'].items()}}}


def invoke(worker, directory, payload, mode):
    directory.mkdir(parents=True, exist_ok=True)
    completion = directory / 'complete.json'
    completion.unlink(missing_ok=True)
    worker.write(directory / 'request.json', payload)
    request_hash = digest((directory / 'request.json').read_bytes())
    relative = directory.relative_to(worker.ROOT).as_posix()
    if not relative.startswith('reports/'):
        raise RuntimeError('Ghidra exchange must stay under ignored reports/')
    worker.command([str(worker.ROOT / 'tools/ghidra.sh'), 'headless', '/projects', 'CB2_USA',
                    '-process', 'SCUS_943.58', '-noanalysis', *(['-readOnly'] if mode != 'apply' else []),
                    '-scriptPath', '/scripts', '-postScript', 'ReconstructionGhidra.java',
                    '/' + relative, mode, '-log', '/' + relative + '/headless.log'],
                   'ghidra-' + mode, directory=directory)
    if not completion.is_file():
        raise RuntimeError('Ghidra did not complete; inspect ' + str(directory / ('ghidra-' + mode + '.log')))
    result = json.loads(completion.read_text())
    if result.get('request_sha256') != request_hash or result.get('mode') != mode:
        raise RuntimeError('Stale Ghidra completion manifest')
    for name, expected in result.get('files', {}).items():
        if name not in ('target.ghidra.c', 'ghidra-types.h') or digest((directory / name).read_bytes()) != expected:
            raise RuntimeError('Ghidra export fingerprint differs')
    if mode == 'export' and set(result.get('files', {})) != {'target.ghidra.c', 'ghidra-types.h'}:
        raise RuntimeError('Incomplete Ghidra evidence bundle')
    if mode == 'apply' and 'Save succeeded for processed file: /SCUS_943.58' not in (directory / 'ghidra-apply.log').read_text():
        raise RuntimeError('Ghidra update ran but persistence was not confirmed; recover before continuing')
    return result


def export(worker, active):
    directory = worker.ROOT / active['context']
    result = invoke(worker, directory / 'ghidra', request(worker.ROOT, active), 'export')
    context = worker.read(directory / 'context.json')
    context['ghidra'] = result
    worker.write(directory / 'context.json', context)
    active['ghidra_literals'] = sorted(literal_tokens((directory / 'ghidra/target.ghidra.c').read_text()))


def strip_comments(text):
    return re.sub(r'/\*.*?\*/|//[^\n]*', ' ', text, flags=re.S)


def literal_tokens(text):
    # Ignore comments and character constants; retain exact C string spelling.
    pattern = r'''"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|/\*.*?\*/|//[^\n]*'''
    return {m[0] for m in re.finditer(pattern, text, re.S) if m[0].startswith('"')}


def declarations(root, active):
    source = root / source_file(active)
    shared, blocks = sections(source.read_text())
    seen = set()

    def header(path):
        if path in seen:
            return ''
        if path.is_symlink() or not path.resolve().is_relative_to((root / 'src').resolve()):
            raise RuntimeError('Feedback header leaves source tree')
        seen.add(path)
        text = strip_comments(path.read_text())
        included = ''.join(header(path.parent / name) for name in re.findall(r'^\s*#\s*include\s+"(\w+\.h)"', text, re.M))
        return included + '\n' + re.sub(r'^\s*#.*$', '', text, flags=re.M)

    headers = ''.join(header(source.parent / name) for name in re.findall(r'^\s*#\s*include\s+"(\w+\.h)"', shared, re.M))
    body = strip_comments(blocks[active['identity']])
    match = re.search(r'^\s*([\w\s*]+\b' + re.escape(active['name']) + r'\s*\([^;{}]*\))\s*\{', body)
    if not match:
        raise RuntimeError('Cannot extract selected C prototype for Ghidra')
    prototype = match[1].strip() + ';'
    externs = re.findall(r'\bextern\s+[^;{}]+;', strip_comments(shared))
    return headers + '\n' + '\n'.join(externs) + '\n' + prototype, externs


def prepare_update(worker, active):
    directory = worker.ROOT / active['context']
    proposal_path = directory / 'ghidra-update.json'
    if not proposal_path.is_file():
        raise RuntimeError('Matched candidate needs ghidra-update.json with function_evidence and globals')
    proposal = worker.read(proposal_path)
    if set(proposal) != {'function_evidence', 'globals'} or not isinstance(proposal['function_evidence'], str) or not proposal['function_evidence'].strip():
        raise RuntimeError('Invalid Ghidra feedback proposal')
    if not isinstance(proposal['globals'], list):
        raise RuntimeError('Feedback globals must be a list')
    text, externs = declarations(worker.ROOT, active)
    symbols = (worker.ROOT / active['symbol_file']).read_text()
    symbols_by_name = dict(re.findall(r'^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+);', symbols, re.M))
    globals_ = []
    # Overlay symbols may name resident globals too; require an explicit image.
    for global_ in proposal['globals']:
        if (set(global_) != {'image', 'address', 'name', 'evidence'}
                or not all(isinstance(v, str) and v.strip() for v in global_.values())
                or global_['image'] not in ('SCUS_943.58', active['image'])
                or not re.fullmatch(r'[A-Za-z_]\w*', global_['name'])):
            raise RuntimeError('Invalid reviewed global feedback')
        name = global_['name']
        if name not in symbols_by_name or int(symbols_by_name[name], 16) != int(global_['address'], 16):
            raise RuntimeError('Global feedback must agree with the accepted symbol address: ' + name)
        if not any(re.search(r'\b' + re.escape(name) + r'\b', declaration) and '(' not in declaration for declaration in externs):
            raise RuntimeError('Global feedback needs an authored extern declaration: ' + name)
        declaration = next(d for d in externs if re.search(r'\b' + re.escape(name) + r'\b', d) and '(' not in d)
        alias = 'CB2GlobalType_' + name
        declaration, replaced = re.subn(r'\b' + re.escape(name) + r'\b(?=\s*(?:\[|;))', alias, declaration)
        if replaced != 1 or ',' in declaration:
            raise RuntimeError('Declare each feedback global separately: ' + name)
        text += '\n' + re.sub(r'^extern\s+', 'typedef ', declaration)
        globals_.append({**global_, 'type_alias': alias})
    payload = request(worker.ROOT, active) | {'name': active['name'], 'declarations': text,
        'globals': globals_, 'function_evidence': proposal['function_evidence'],
        'sequence': time.time_ns(), 'identity': active['identity']}
    invoke(worker, directory / 'ghidra-feedback', payload, 'plan')
    return payload


def record(worker, active, payload):
    path = 'config/ghidra/' + slug(active) + '.json'
    if (worker.ROOT / path).exists():
        raise RuntimeError('Analysis feedback already exists for selected function')
    worker.write(worker.ROOT / path, payload)
    return path


def apply(worker, active, payload):
    return invoke(worker, worker.ROOT / active['context'] / 'ghidra-feedback', payload, 'apply')


def replay(worker):
    records = [worker.read(path) for path in (worker.ROOT / 'config/ghidra').glob('*.json')]
    for payload in sorted(records, key=lambda r: r['sequence']):
        invoke(worker, worker.ROOT / 'reports/ghidra-replay' / slug(payload), payload, 'apply')


def recover(worker):
    session = worker.state()
    active = session.get('active')
    if not active or active.get('phase') != 'syncing_ghidra':
        raise RuntimeError('No accepted match is waiting for Ghidra synchronization')
    if (worker.changes() or worker.git('diff', '--cached', '--name-only')
            or worker.git('branch', '--show-current') != 'main'
            or worker.git('rev-parse', 'HEAD') != active['commit']):
        raise RuntimeError('Recovery requires the unchanged accepted commit on clean main')
    payload = worker.read(worker.ROOT / active['ghidra_record'])
    apply(worker, active, payload)
    from reconstruction_runtime import attempt_result
    worker.write(worker.ROOT / active['context'] / 'result.json', attempt_result(worker.ROOT, active, 'matched'))
    session['completed'].append({**active, 'phase': 'integrated'})
    session.update(active=None, stop_reason='Recovered accepted Ghidra update; start a new batch', phase='recovered')
    worker.save_state(session)
