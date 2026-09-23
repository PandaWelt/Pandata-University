"""Maintainer Linux regression runner; Windows users run Validate-Offline-Baseline.cmd.

Exercises the same JSON command sequences and real course/controller as the native
harness. Does not claim that Python/subprocess tests execute cmd.exe or PowerShell.
"""
import concurrent.futures
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
from validator_input import validate_validator

ROOT = Path(__file__).resolve().parents[2]


def static_checks():
    launch = (ROOT/'Start-Pandata-University.cmd').read_text()
    assert launch.splitlines().count(':tools') == 1
    assert launch.count('if not defined PUNOPAUSE pause') == 2
    assert 'if not "%~2"=="" exit /b 2' in launch
    assert '-tests.exe' not in launch
    assert launch.count('call "%PUROOT%Build-University.cmd"') == 1
    validate_validator((ROOT/'Validate-Offline-Baseline.cmd').read_text())
    ps = (ROOT/'university/tools/Validate-Offline-Baseline.ps1').read_text()
    assert ps.count("'Validate-Offline-Foundation.cmd'") == 1
    for name in ['launcher-first', 'launcher-restart', 'refused-commands', 'backup-readable',
                 'recovery-state', 'mismatch-state', 'fresh-state', 'read-only-actions']:
        assert "'"+name+"'" in ps
    assert 'RedirectStandardInput' in ps and 'Text.UTF8Encoding($false)' in ps
    assert 'Same $before (Map $root $protected)' in ps
    assert ps.rindex('PASS: PU-OFFLINE-1.0') > ps.index('} finally {')
    assert 'mentor-status' not in ps and 'open-vs' not in ps
    data = json.loads((ROOT/'university/tests/offline_workflow.json').read_text())
    for commands in data.values():
        assert commands[-1] == 'quit'
        assert not set(commands) & {'mentor-status', 'mentor-send', 'open-vs', 'run-check', 'failure-demo'}
    for bad in ['review-select ', 'review-select\t', 'Review-select', '\ufeffreview-select',
                'review-select\u200b', 'review-select extra', 'review-select & echo unsafe']:
        assert bad in data['refused']
    return data


def hashes(root, names):
    result = {}
    for name in names:
        p = root/name
        for f in ([p] if p.is_file() else p.rglob('*')):
            assert not f.is_symlink()
            if f.is_file(): result[str(f.relative_to(root))] = hashlib.sha256(f.read_bytes()).hexdigest()
    return result


def run():
    workflow = static_checks()
    stage = Path(tempfile.mkdtemp(prefix='pu-final-'))
    print('Linux staging:', stage, flush=True)
    records = []
    env = {k: v for k, v in os.environ.items() if not k.startswith('GIT_')}
    env.update(GIT_CONFIG_NOSYSTEM='1', GIT_CONFIG_GLOBAL='/dev/null')

    def command(name, args, cwd=stage, text=None, expected=0):
        p = subprocess.run(list(map(str, args)), cwd=cwd, input=text, text=True,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=600, env=env)
        (stage/(name+'.txt')).write_text(p.stdout)
        assert p.returncode == expected, (name, p.returncode, p.stdout[-5000:])
        records.append(dict(name=name, exit=p.returncode, expected=expected))
        print('PASS:', name, flush=True)
        return p.stdout

    before = hashes(ROOT, ['src','.git','course'])
    core = ['Mission.cpp','ProgressStore.cpp','Process.cpp','RepositoryActions.cpp','StudyDesk.cpp']
    preview = ['ProposalPreview.cpp']
    parser = ['mentor/Json.cpp', *preview, 'ProposalParser.cpp']
    review = [*parser, 'ProposalReview.cpp']
    controls = [*review, 'ReviewControls.cpp']
    sources = [x.replace('\\','/') for x in re.findall(r'%PUROOT%(university\\[^\"]+\.cpp)', (ROOT/'Build-University.cmd').read_text())]
    jobs = {
        'app': sources,
        'final-state': ['university/src/'+x for x in core] + ['university/tests/final_integration_tests.cpp'],
        'study': ['university/src/'+x for x in core] + ['university/tests/study_tests.cpp'],
        'proposal': ['university/src/'+x for x in preview] + ['university/tests/proposal_tests.cpp'],
        'parser': ['university/src/'+x for x in parser] + ['university/tests/proposal_parser_tests.cpp'],
        'review': ['university/src/'+x for x in review] + ['university/tests/proposal_review_tests.cpp'],
        'controls': ['university/src/'+x for x in controls] + ['university/tests/review_controls_tests.cpp'],
        'boundary': ['university/src/'+x for x in controls] + ['university/tests/command_boundary_tests.cpp'],
        'probe': ['university/tests/console_input_probe.cpp']}
    def build(job):
        name, files = job
        command('build-'+name, ['g++','-std=c++20','-Wall','-Wextra','-Werror','-I'+str(ROOT/'university/include'),
                               *[ROOT/f for f in files], '-o', stage/name])
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:
        list(pool.map(build, jobs.items()))
    for name in ['proposal','parser','review','controls','boundary']:
        print(command(name,[stage/name]), end='')
    print(command('study',[stage/'study',ROOT]), end='')
    command('byte-probes',[sys.executable,ROOT/'university/tools/test_console_input_probe.py',stage/'probe'])
    for name in ['test_package_update.py','test_validator_input.py','test_foundation_build.py']:
        command(name,[sys.executable,ROOT/'university/tools'/name])
    command('prepare-existing',[stage/'study','--prepare',ROOT,stage])
    for name in ['first','resume']:
        output=command('existing-'+name,[stage/'app'],stage/'workflow',(stage/('study-'+name+'.txt')).read_text())
        (stage/('study-'+name+'-output.txt')).write_text(output)
    print(command('verify-existing',[stage/'study','--verify',stage]),end='')
    copy = stage/'learner space & probe'; copy.mkdir()
    selected=['.gitignore','course/university/pilot.puc','course/university/missions/u1-boundaries.pum',
              'course/university/missions/003-object-types.pum','src/main.cpp','src/UI/AppUISetup.cpp',
              'src/UI/AppUISetup.h','visualstudio/Pandata.bank.vcxproj']
    for name in selected:
        dst=copy/name; dst.parent.mkdir(parents=True,exist_ok=True); shutil.copyfile(ROOT/name,dst)
    shutil.copytree(ROOT/'university',copy/'university',ignore=shutil.ignore_patterns('__pycache__'))
    template=stage/'empty-template'; template.mkdir()
    command('git-init',['git','init','--template='+str(template)],copy)
    (copy/'validation-diff.txt').write_text('before\n')
    command('git-add',['git','add','.'],copy)
    command('git-commit',['git','-c','user.name=Validation','-c','user.email=validation@example.invalid','-c','commit.gpgsign=false','commit','-m','synthetic'],copy)
    (copy/'validation-diff.txt').write_text('before\nafter-for-read-only-diff\n')
    copy_before=hashes(copy,['src','.git','validation-diff.txt'])
    command('definition-validation',[stage/'app','--validate'],copy)
    def session(name): return command(name,[stage/'app'],copy,'\n'.join(workflow[name])+'\n')
    session('first'); command('first-state',[stage/'final-state','--first',copy])
    assert 'SYNTHETIC final prediction' in session('restart')
    command('complete-state',[stage/'final-state','--complete',copy])
    progress=hashes(copy,['course/university/progress'])
    refused=session('refused'); assert 'REJECTED:' in refused and 'Action/step not completed:' in refused
    assert 'DISPLAYED:' not in refused and 'Action succeeded' not in refused
    assert not (copy/'course/university/evidence').exists()
    assert progress == hashes(copy,['course/university/progress'])
    views=session('views'); assert 'src/main.cpp (read-only)' in views and 'after-for-read-only-diff' in views
    evidence=[json.loads(p.read_text()) for p in (copy/'course/university/evidence/u2').rglob('evidence.json')]
    assert {e['action'] for e in evidence if e['passed']} == {'git-status','git-diff'}
    assert copy_before == hashes(copy,['src','.git','validation-diff.txt'])
    assert progress == hashes(copy,['course/university/progress'])
    backup=stage/'backup'; shutil.copytree(copy/'course',backup/'course')
    command('backup-state',[stage/'final-state','--complete',backup])
    saved=copy/'course/university/progress/003-object-types'
    bad=saved/(str(max(int(f.stem) for f in saved.glob('*.pup'))+1)+'.pup'); bad.write_text('corrupt\n')
    assert 'Recovery:' in session('report') and bad.exists()
    command('recovered-state',[stage/'final-state','--recovered',copy])
    definition=copy/'course/university/missions/003-object-types.pum'; original=definition.read_bytes()
    definition.write_bytes(original+b'\n# deliberate synthetic mismatch\n')
    saved_before=hashes(copy,['course/university/progress'])
    assert 'Action/step not completed:' in session('blocked')
    command('blocked-state',[stage/'final-state','--blocked',copy])
    assert saved_before == hashes(copy,['course/university/progress'])
    definition.write_bytes(original); bad.unlink() # Only exact synthetic corruption just created.
    session('fresh'); command('fresh-state',[stage/'final-state','--complete',copy])
    assert copy_before == hashes(copy,['src','.git','validation-diff.txt'])
    assert before == hashes(ROOT,['src','.git','course'])
    result=dict(passed=True, platform='Linux; native Windows NOT executed', records=records, real_protected_unchanged=True)
    (stage/'linux-results.json').write_text(json.dumps(result,indent=2)+'\n')
    print('PASS: final Linux workflow/static checks; Windows launcher execution remains pending.')
    print('Evidence:',stage/'linux-results.json')

if __name__ == '__main__': run()
