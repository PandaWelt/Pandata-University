"""Maintainer packaging regression; no dependency beyond Python standard library."""
import json
from pathlib import Path
import tempfile
import unittest
import zipfile
from package_update import generate, validate_package

class PackageTests(unittest.TestCase):
    def test_manifest_allowlist_regression(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td); repo = root / 'repo'; repo.mkdir()
            paths = ['Validate-Proposal-Parser.cmd', 'Validate-Proposal-Review.cmd']
            baseline = root / 'base.zip'
            with zipfile.ZipFile(baseline, 'w') as z:
                for p in paths: z.writestr('Pandata.Bank/' + p, 'before\n')
            for p in paths: (repo / p).write_text('after\n')
            out = root / 'out'
            generate(baseline, repo, paths, 'a'*40, 'U4-F4', 'Validate-Review-Controls.cmd', out)
            self.assertEqual(validate_package(out), 2)
            original = (out / 'Apply-Update.ps1').read_text()
            # Reproduce the F3 same-count duplicate and omitted Parser path exactly.
            (out / 'Apply-Update.ps1').write_text(original.replace("    'Validate-Proposal-Parser.cmd'", "    'Validate-Proposal-Review.cmd'"))
            with self.assertRaises(ValueError): validate_package(out)
            (out / 'Apply-Update.ps1').write_text(original.replace("    'Validate-Proposal-Parser.cmd'", "    'Other.cmd'"))
            with self.assertRaises(ValueError): validate_package(out)
            (out / 'Apply-Update.ps1').write_text(original)
            manifest = json.loads((out / 'manifest.json').read_text())
            manifest['files'].append(manifest['files'][0])
            (out / 'manifest.json').write_text(json.dumps(manifest))
            with self.assertRaises(ValueError): validate_package(out)
            with self.assertRaises(ValueError): generate(baseline, repo, paths+paths[:1], 'a'*40, 'U4-F4', 'Validate-Review-Controls.cmd', out)

if __name__ == '__main__': unittest.main()
