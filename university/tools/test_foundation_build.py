"""Static Windows packaging checks; does not pretend to execute cmd.exe."""
from pathlib import Path
import re
import unittest
from validator_input import validate_validator
ROOT = Path(__file__).resolve().parents[2]

class FoundationBuildTests(unittest.TestCase):
    def test_ordinary_launcher_never_replays_suites(self):
        launch = (ROOT/'Start-Pandata-University.cmd').read_text()
        # Regression: extracting ':tools' at CALL instead of the label copied the old launcher.
        self.assertEqual(launch.splitlines().count(':tools'), 1)
        self.assertNotIn('-tests.exe', launch)
        self.assertNotIn('Validate-', launch)
        self.assertEqual(launch.count('call "%PUROOT%Build-University.cmd"'), 1)
        build = (ROOT/'Build-University.cmd').read_text()
        paths = re.findall(r'%PUROOT%(university\\[^\"]+\.cpp)', build)
        self.assertEqual(len(paths), len(set(paths)))
        self.assertEqual(len(paths), 13)
        for relative in paths:
            self.assertTrue((ROOT/relative.replace('\\','/')).is_file(), relative)

    def test_one_combined_regression_entry(self):
        text = (ROOT/'Validate-Offline-Foundation.cmd').read_text()
        validate_validator(text)
        self.assertNotIn('engine-tests.exe', text)
        self.assertNotIn('repository-tests.exe', text)
        self.assertNotIn('mentor-tests.exe', text)
        for name in ['proposal-tests.exe', 'proposal-parser-tests.exe', 'proposal-review-tests.exe',
                     'review-controls-tests.exe', 'command-boundary-tests.exe']:
            self.assertEqual(text.splitlines().count(name), 1, name)
        self.assertEqual(text.count('study-tests.exe "%PUROOT%."'), 1)
        self.assertEqual(text.count('study-tests.exe --prepare'), 1)
        self.assertEqual(text.count('study-tests.exe --verify'), 1)
        self.assertIn('< "%PUSTAGE%\\study-first.txt"', text)
        self.assertIn('< "%PUSTAGE%\\study-resume.txt"', text)

if __name__ == '__main__': unittest.main()
