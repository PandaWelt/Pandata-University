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

    def test_generator_rejects_pipelines(self):
        from validator_input import COMMANDS, input_block
        with tempfile.TemporaryDirectory() as td:
            root = Path(td); repo = root / 'repo'; repo.mkdir()
            baseline = root / 'base.zip'
            with zipfile.ZipFile(baseline, 'w'): pass
            path = 'Validate-Console-Input.cmd'
            for spelling in ['echo ', 'echo(']:
                (repo / path).write_text('(' + '& '.join(spelling + c for c in COMMANDS) + ') | app.exe\n')
                with self.assertRaises(ValueError):
                    generate(baseline, repo, [path], 'a'*40, 'U4-F5', path, root / 'out')
            (repo / path).write_text(input_block() + '\napp.exe < "%PUSTAGE%\\console-input.txt"\n')
            generate(baseline, repo, [path], 'a'*40, 'U4-F5', path, root / 'out')
            self.assertEqual(validate_package(root / 'out'), 1)

    def test_offline_stage_keeps_progress_and_existing_definition_protected(self):
        with tempfile.TemporaryDirectory() as td:
            root=Path(td); repo=root/'repo'; repo.mkdir()
            baseline=root/'base.zip'
            with zipfile.ZipFile(baseline,'w'): pass
            for path in ['course/university/progress/u1-boundaries/1.pup',
                         'course/university/evidence/u2/evidence.json',
                         'course/university/missions/u1-boundaries.pum',
                         'course/university/pilot.puc']:
                target=repo/path;target.parent.mkdir(parents=True,exist_ok=True);target.write_text('refuse\n')
                with self.assertRaises(ValueError):
                    generate(baseline,repo,[path],'a'*40,'OFFLINE-FOUNDATION','Validate-Offline-Foundation.cmd',root/'out')
            path='course/university/missions/003-object-types.pum'
            (repo/path).write_text('reviewed new lesson\n')
            generate(baseline,repo,[path],'a'*40,'OFFLINE-FOUNDATION','Validate-Offline-Foundation.cmd',root/'out')
            self.assertEqual(validate_package(root/'out'),1)

if __name__ == '__main__': unittest.main()
