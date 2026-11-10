"""Safety checks for the complete function backlog, independent of game files."""
import hashlib
import unittest

from target_queue import assemble, c_spans


class TargetQueueTest(unittest.TestCase):
    def setUp(self):
        self.data = bytes(range(32))
        self.images = {name: {'data': self.data, 'base': 0x80000000, 'file_offset': 0,
                             'sha256': hashlib.sha256(self.data).hexdigest(), 'c_spans': [],
                             'build_config': name + '.yaml', 'symbol_file': name + '.txt'}
                       for name in ('SCUS_943.58', 'A.BIN', 'B.BIN')}
        self.sdk = {'functions': {}, 'overlays': {n: {'functions': {}} for n in ('A.BIN', 'B.BIN')}}

    def entry(self, image, start=0, low=0, end=8, sdk=False):
        return {'identity': f'{image}:0x{0x80000000 + start:08X}', 'name': 'test',
                'classification': 'sdk' if sdk else 'unidentified',
                'ranges': [[hex(0x80000000 + low), hex(0x80000000 + end)]],
                'body_bytes': end - low, 'body_sha256': hashlib.sha256(self.data[low:end]).hexdigest()}

    def build(self, entries, policy=None, boundaries=None):
        return assemble(entries, self.images, policy or {}, boundaries or {}, self.sdk)

    def test_same_address_in_different_overlays_remains_two_targets(self):
        report = self.build([self.entry('A.BIN'), self.entry('B.BIN')])
        self.assertEqual(report['summary']['needs_review'], 2)
        self.assertNotEqual(report['targets'][0]['identity'], report['targets'][1]['identity'])
        with self.assertRaisesRegex(RuntimeError, 'Duplicate function identity'):
            self.build([self.entry('A.BIN')] * 2)

    def test_sdk_interior_entry_stays_excluded_even_when_c(self):
        self.sdk['overlays']['A.BIN']['functions']['0x80000004'] = 'SdkHelper'
        self.images['A.BIN']['c_spans'] = [{'address': 0x80000000, 'end': 0x80000008, 'source': 'src/a.c'}]
        row = self.build([self.entry('A.BIN', start=4, sdk=True)])['targets'][0]
        self.assertEqual(row['status'], 'sdk')
        self.assertEqual(row['reconstruction'], 'c')

    def test_stale_bytes_and_sdk_disagreement_fail(self):
        entry = self.entry('A.BIN')
        self.images['A.BIN']['data'] = b'x' * 32
        with self.assertRaisesRegex(RuntimeError, 'Function bytes differ'):
            self.build([entry])
        self.images['A.BIN']['data'] = self.data
        entry['classification'] = 'sdk'
        with self.assertRaisesRegex(RuntimeError, 'SDK catalog differs'):
            self.build([entry])

    def test_shared_region_hold_and_partial_c_coverage(self):
        self.images['SCUS_943.58']['c_spans'] = [{'address': 0x80000004, 'end': 0x80000008, 'source': 'src/a.c'}]
        entry = self.entry('SCUS_943.58', start=4, low=0, end=12)
        boundaries = {'regions': {'0x80000000': {'size': 16, 'kind': 'multi_entry'}}}
        row = self.build([entry], boundaries=boundaries)['targets'][0]
        self.assertEqual(row['status'], 'held')
        self.assertEqual(row['reconstruction'], 'retained_assembly')
        # A boot-address hold must not leak into an overlay at the same address.
        row = self.build([self.entry('A.BIN')], boundaries=boundaries)['targets'][0]
        self.assertEqual(row['status'], 'needs_review')

    def test_overlay_c_paths_and_file_offset_to_vram(self):
        config = {'segments': [{'type': 'code', 'start': 0, 'vram': 0x8005AB68,
                               'dir': 'overlays/init', 'subsegments':
                               [[0, 'rodata', 'header'], [4, 'c', 'example'], [12, 'asm', 'rest']]}]}
        self.assertEqual(c_spans(config), [{'address': 0x8005AB6C, 'end': 0x8005AB74,
                                           'source': 'src/overlays/init.c', 'unit': 'overlays/init/example'}])


if __name__ == '__main__':
    unittest.main()
