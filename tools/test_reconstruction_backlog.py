"""Image-qualified controller, review evidence and path protection regression tests."""
import copy
import hashlib
import json
import sys
import unittest
from pathlib import Path
from unittest.mock import patch

import yaml
import reconstruct as worker
from reconstruction_backlog import prepare, key, slug, coverage
from reconstruct_run import review_split, review_source, Runner, Rejected
import test_reconstruct_run as fixtures


class BacklogTests(unittest.TestCase):
    def setUp(self):
        self.fixture = fixtures.RunTests()
        self.fixture.setUp()
        self.addCleanup(self.fixture.doCleanups)
        self.root = self.fixture.root

    def rows(self):
        raw = bytes.fromhex('0800e00307000224')
        result = []
        for image in ('A.BIN', 'B.BIN'):
            name = image[0].lower()
            config = {'segments': [{'type': 'code', 'start': 0, 'vram': 0x80010000,
                'dir': 'overlays/'+name, 'subsegments': [[0,'asm','body'],[8,'data','tail']]},[12]]}
            path = self.root / ('config/overlays/'+name+'.yaml')
            path.parent.mkdir(exist_ok=True)
            path.write_text(yaml.safe_dump(config))
            path.with_suffix('.symbols.txt').write_text('')
            (self.root / 'orig/disc/files' / image).write_bytes(raw + bytes(4))
            result.append({'identity': image+':0x80010000', 'image': image, 'address': '0x80010000',
                'name': 'func_80010000', 'ranges': [['0x80010000','0x80010008']],
                'reference_sha256': hashlib.sha256(raw+bytes(4)).hexdigest(),
                'body_sha256': hashlib.sha256(raw).hexdigest(), 'body_bytes': 8,
                'file_offset': 0, 'build_config': 'config/overlays/'+name+'.yaml',
                'symbol_file': 'config/overlays/'+name+'.symbols.txt', 'status': 'needs_review'})
        return result

    def test_recorded_order_and_qualified_exclusions_without_size_filter(self):
        rows = self.rows()
        rows[0]['body_bytes'] = 4096
        session = {'completed': [], 'deferred': []}
        with patch.object(worker, 'read', return_value={}):
            self.assertEqual(worker.choose({'targets':rows},session), rows[0])
            session['deferred'] = [rows[0]]
            self.assertEqual(worker.choose({'targets':rows},session), rows[1])
            self.assertNotEqual(slug(rows[0]), slug(rows[1]))
            self.assertEqual(worker.choose({'targets':rows},session,key(rows[1])),rows[1])
            with self.assertRaises(worker.NoTarget):
                worker.choose({'targets':rows},session,'0x80010000')
        self.assertEqual(len(coverage({'targets':[{**r,'status':'c'} for r in rows]})),2)

    def test_selected_image_review_and_split_source_path(self):
        rows = self.rows()
        first = prepare(self.root, rows[0], {'targets':rows})
        self.assertEqual(first['boundary_findings'], {})
        (self.root/rows[0]['symbol_file']).write_text('build_entry = 0x80010000; // type:func\n')
        self.assertEqual(prepare(self.root,rows[0],{'targets':rows})['name'],'build_entry')
        source = self.root/'src/elements.c'
        source.write_text('int elements(void){int state[2];state[0] = 7;return state[0];}')
        review_source(self.root,['src/elements.c'],'src/elements.c','elements')
        source.write_text('int elements(void){int state[2] = {7,0};return state[0];}')
        with self.assertRaisesRegex(Rejected,'Initialized arrays'):
            review_source(self.root,['src/elements.c'],'src/elements.c','elements')
        before = yaml.safe_load((self.root/rows[0]['build_config']).read_text())
        after = copy.deepcopy(before)
        after['segments'][0]['subsegments'][0] = [0,'c','entry']
        self.assertEqual(review_split(before,after,first),'src/overlays/a/entry.c')
        after['segments'][0]['dir'] = 'overlays/b'
        with self.assertRaises(Rejected):
            review_split(before,after,first)
        (self.root/'orig/disc/files/A.BIN').write_bytes(bytes(12))
        with self.assertRaisesRegex(RuntimeError,'fingerprint'):
            prepare(self.root,rows[0],{'targets':rows})

    def test_noncontiguous_and_interior_incoming_are_durable_blockers(self):
        rows = self.rows()
        row = {**rows[0], 'ranges': [['0x80010000','0x80010004'],['0x80010008','0x8001000C']]}
        result = prepare(self.root,row,{'targets':rows})
        self.assertEqual(result['boundary_review'],'deferred')
        self.assertIn('noncontiguous_or_unaligned_body',result['boundary_findings'])
        # A second entry jumps into the selected return's delay slot.
        raw = bytes.fromhex('0800e003070002240140000800000000')
        path = self.root/'orig/disc/files/A.BIN'
        path.write_bytes(raw)
        row = {**rows[0], 'reference_sha256':hashlib.sha256(raw).hexdigest()}
        caller = {**row,'identity':'A.BIN:0x80010008','address':'0x80010008',
                  'ranges':[['0x80010008','0x80010010']]}
        result = prepare(self.root,row,{'targets':[row,caller,rows[1]]})
        self.assertIn('incoming_transfer_to_interior',result['boundary_findings'])

    def test_paths_and_protected_configs_are_selected_image_only(self):
        rows = self.rows()
        active = {**rows[0], 'context':'reports/selected'}
        worker.save_state({'active':active})
        runner = self.fixture.runner()
        tracked = set(self.fixture.git('ls-files').splitlines())
        # Commit fixture overlay configs so new sources are distinguished from existing inputs.
        self.fixture.git('add','config/overlays')
        self.fixture.git('commit','-qm','Overlay fixture')
        tracked = set(self.fixture.git('ls-files').splitlines())
        selected = self.root/rows[0]['build_config']
        selected.write_text(selected.read_text()+'\n')
        directory = self.root/'src/overlays/a'
        directory.mkdir(parents=True)
        (directory/'entry.c').write_text('int entry(void){return 7;}')
        self.assertEqual(set(runner.paths(tracked, active)),{rows[0]['build_config'],'src/overlays/a/entry.c'})
        other = self.root/rows[1]['build_config']
        other.write_text(other.read_text()+'\n')
        with self.assertRaisesRegex(RuntimeError,'Out-of-scope'):
            runner.paths(tracked, active)

    def test_missing_or_deferred_review_cannot_reach_validation(self):
        row = self.rows()[0]
        directory = self.root/'reports/selected'
        directory.mkdir()
        active = {**row,'context':'reports/selected','offset':0,'size':8}
        session = {'active':active}
        runner = self.fixture.runner()
        with patch.object(worker,'validate') as gate:
            with self.assertRaisesRegex(Rejected,'Missing selected-target'):
                runner.candidate(session,set(),{},'',{})
            (directory/'boundary-decision.json').write_text(json.dumps({
                'identity':key(row),'body_sha256':row['body_sha256'],
                'decision':'deferred','evidence':'Shared tail needs a grouped strategy'}))
            with self.assertRaisesRegex(Rejected,'Shared tail'):
                runner.candidate(session,set(),{},'',{})
            gate.assert_not_called()

    def test_overlay_process_gate_failure_then_equal_address_integration(self):
        rows = self.rows()
        catalog = {'overlays': {r['image']: {'build_config':r['build_config']} for r in rows}}
        (self.root/'config/overlays.json').write_text(json.dumps(catalog))
        # Extend the synthetic validation gate to compare every overlay independently.
        gate = self.root/'tools/fake_validate.py'
        gate.write_text(gate.read_text().replace('sys.exit(0 if passed else 1)', "") + "\n" + '''
for name in ('a','b'):
    image = name.upper()+'.BIN'
    config = yaml.safe_load(Path('config/overlays/'+name+'.yaml').read_text())
    units = [r[2] for r in config['segments'][0]['subsegments'] if r[1]=='c']
    expected = Path('orig/disc/files',image).read_bytes()
    matched = all('return 7;' in Path('src/overlays',name,n+'.c').read_text() for n in units)
    output = Path('build/overlays',name,image)
    output.parent.mkdir(parents=True,exist_ok=True)
    output.write_bytes(expected if matched else b'!'+expected[1:])
    details = Path('reports/overlay-rebuild')
    details.mkdir(exist_ok=True)
    (details/(name+'.json')).write_text(json.dumps({'identical':matched}))
    passed = passed and matched
sys.exit(0 if passed else 1)
''')
        self.fixture.agent.write_text('''
import json, os, sys, yaml
from pathlib import Path
sys.stdin.read()
context_path = Path(os.environ['CB2_RECONSTRUCTION_CONTEXT'])
r = json.loads(context_path.read_text())
config_path = Path(r['build_config'])
config = yaml.safe_load(config_path.read_text())
config['segments'][0]['subsegments'][0] = [0,'c','entry']
config_path.write_text(yaml.safe_dump(config))
directory = Path(r['source_directory'])
directory.mkdir(parents=True,exist_ok=True)
value = 8 if r['image']=='A.BIN' else 7
(directory/'entry.c').write_text('int func_80010000(void){return '+str(value)+';}')
(context_path.parent/'boundary-decision.json').write_text(json.dumps({
    'identity':r['identity'],'body_sha256':r['body_sha256'],'decision':'standalone',
    'evidence':'Single normal return and its delay slot; no callers or data dependencies.'}))
''')
        self.fixture.git('add','config','tools/fake_validate.py')
        self.fixture.git('commit','-qm','Overlay process fixture')
        def inventory():
            current = []
            for row in rows:
                config = yaml.safe_load((self.root/row['build_config']).read_text())
                c = config['segments'][0]['subsegments'][0][1]=='c'
                current.append({**row,'status':'c' if c else 'needs_review',
                                'reconstruction':'c' if c else 'retained_assembly'})
            return {'targets':current}
        with patch.object(worker,'inventory',side_effect=inventory), patch('overlays.load',return_value=(catalog,b'')):
            self.fixture.runner().run()
        state = worker.state()
        self.assertEqual([r['identity'] for r in state['deferred']],['A.BIN:0x80010000'])
        self.assertEqual([r['identity'] for r in state['completed']],['B.BIN:0x80010000'])
        self.assertIn('Command failed',state['deferred'][0]['reason'])
        self.assertEqual(self.fixture.git('branch','--show-current'),'main')
        self.assertFalse(worker.changes())
        self.assertFalse((self.root/'src/overlays/a/entry.c').exists())
        self.assertTrue((self.root/'src/overlays/b/entry.c').is_file())
        self.assertNotEqual(state['deferred'][0]['branch'],state['completed'][0]['branch'])

    def test_structural_cache_and_progress_reset_only_when_evidence_improves(self):
        from reconstruction_runtime import cached_structural, save_structural
        from reconstruction_step import progress
        rows = self.rows()
        row = {**rows[0], 'boundary_findings':{'shared_entry':['0x80010004']}, 'context':'reports/selected'}
        report = {'targets':rows}
        self.assertFalse(cached_structural(self.root,row,report))
        save_structural(self.root,row,report)
        self.assertTrue(cached_structural(self.root,row,report))
        self.assertFalse(cached_structural(self.root,rows[1],report))
        changed = copy.deepcopy(report)
        changed['targets'][0]['body_sha256'] = 'changed'
        self.assertFalse(cached_structural(self.root,changed['targets'][0],changed))
        first = progress({},90,100)
        self.assertEqual(progress(first,89,200)['improved_at'],100)
        self.assertEqual(progress(first,91,200)['improved_at'],200)
        self.assertEqual(worker.validation_command(row),['make','validate','IMAGE=A.BIN'])

    def test_plateau_kills_child_and_preserves_no_false_success(self):
        with self.assertRaisesRegex(worker.CommandTimeout,'No object-score improvement'):
            worker.command([sys.executable,'-c','import time; time.sleep(10)'],
                           'plateau',timeout=2,directory=self.root/'reports',stall_seconds=.1)

    def test_detached_start_returns_pid_and_log_without_model_supervision(self):
        from reconstruction_runtime import start
        args = self.fixture.runner().args
        args.stall_seconds, args.retry_structural = 300, False
        with patch('reconstruction_runtime.subprocess.Popen') as spawn:
            spawn.return_value.pid = 123
            start(self.root,args)
            self.assertTrue(spawn.call_args.kwargs['start_new_session'])
            self.assertIn('--stall-seconds',spawn.call_args.args[0])
        launch = json.loads((self.root/'reports/reconstruction/launch.json').read_text())
        self.assertEqual(launch['pid'],123)
        self.assertTrue((self.root/launch['log']).is_file())


if __name__ == '__main__':
    unittest.main()
