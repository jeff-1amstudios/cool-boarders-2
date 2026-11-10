"""Verify image-qualified extraction without leaking adjacent function blocks."""
import unittest

from c_sources import function_source, source_path, unit_identity


class SourceTests(unittest.TestCase):
    def test_blocks_keep_shared_context_and_isolate_equal_addresses(self):
        text = ('typedef int Word;\nextern Word a;\nextern Word b;\n'
                '// FUNCTION: A.BIN:0x80010000\nWord f(void) {return a;}\n'
                '// FUNCTION: B.BIN:0x80010000\nWord g(void) {return b;}\n')
        result = function_source(text, 'A.BIN:0x80010000')
        self.assertIn('typedef int Word;', result)
        self.assertIn('return a;', result)
        self.assertIn('extern Word a;', result)
        self.assertIn('extern Word b;', result)
        self.assertNotIn('Word g(void)', result)
        with self.assertRaisesRegex(ValueError, 'Missing FUNCTION'):
            function_source(text, 'A.BIN:0x80010008')
        with self.assertRaisesRegex(ValueError, 'Duplicate FUNCTION'):
            function_source(text + text, 'A.BIN:0x80010000')

    def test_source_paths_and_reviewed_address(self):
        self.assertEqual(str(source_path('boot_main')), 'src/main.c')
        self.assertEqual(str(source_path('overlays/init/entry')), 'src/overlays/init.c')
        config = {'name': 'INIT.BIN', 'segments': [{'start': 0, 'vram': 0x8005AB68,
                  'dir': 'overlays/init', 'subsegments': [[4, 'c', 'entry']]}]}
        self.assertEqual(unit_identity('overlays/init/entry', config), 'INIT.BIN:0x8005AB6C')
        with self.assertRaisesRegex(ValueError, 'not a configured'):
            unit_identity('overlays/init/other', config)


if __name__ == '__main__':
    unittest.main()
