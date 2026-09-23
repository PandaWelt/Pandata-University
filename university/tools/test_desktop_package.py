"""Desktop build/package boundary regressions. Maintainer only; not a learner dependency."""
import json
from pathlib import Path
import tempfile
import unittest
import zipfile
from package_update import generate, validate_package
from validator_input import validate_validator
ROOT=Path(__file__).resolve().parents[2]

class DesktopPackageTests(unittest.TestCase):
    def test_source_boundary(self):
        groups=json.loads((ROOT/'university/desktop-sources.json').read_text())
        files=sum(groups.values(),[])
        self.assertEqual(len(files),len(set(files)))
        for file in files:
            self.assertTrue((ROOT/file).is_file())
            self.assertFalse(file.startswith('src/'))
            self.assertNotIn('mentor/',file)
        for file in groups['core']:
            text=(ROOT/file).read_text()
            self.assertNotIn('#include "imgui',text)
            self.assertNotIn('#include <SDL',text)
        host=(ROOT/'university/src/desktop/desktop_main.cpp').read_text()
        self.assertIn('io.IniFilename=nullptr',host)
        self.assertIn('io.LogFilename=nullptr',host)

    def test_staging_and_launcher(self):
        text=(ROOT/'university/tools/Validate-Desktop.ps1').read_text()
        self.assertIn('[IO.Path]::GetTempPath()',text)
        self.assertIn('$stage.Length -lt 100',text)
        self.assertIn('Unchanged $before',text)
        self.assertNotIn('Validate-Offline-',text)
        self.assertNotIn('run-check',text)
        for name in ['controller-render','graphics-smoke','invalid-argument','fresh-graphics']:
            self.assertIn("'"+name+"'",text)
        validate_validator((ROOT/'Validate-University-Desktop.cmd').read_text())
        launch=(ROOT/'Start-Pandata-University-Desktop.cmd').read_text()
        self.assertEqual(launch.splitlines().count(':tools'),1)
        self.assertNotIn('-tests.exe',launch)

    def test_desktop_stage_and_unchanged_protection(self):
        with tempfile.TemporaryDirectory() as td:
            root=Path(td); archive=root/'before.zip'
            with zipfile.ZipFile(archive,'w'): pass
            repo=root/'repo'; repo.mkdir()
            p='Validate-University-Desktop.cmd'; (repo/p).write_text('@echo off\nexit /b 0\n')
            generate(archive,repo,[p],'a'*40,'PU-DESKTOP-1',p,root/'package')
            self.assertEqual(validate_package(root/'package'),1)
            for protected in ['src/main.cpp','thirdparty/imgui/imgui.cpp','course/university/progress/u1-boundaries/1.pup']:
                target=repo/protected; target.parent.mkdir(parents=True,exist_ok=True); target.write_text('forbidden\n')
                with self.assertRaises(ValueError): generate(archive,repo,[protected],'a'*40,'PU-DESKTOP-1',p,root/'bad')

if __name__=='__main__': unittest.main()
