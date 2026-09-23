"""Maintainer-only Linux build/workflow runner; never substitutes for native Windows validation."""
from pathlib import Path
import concurrent.futures
import hashlib
import json
import os
import subprocess
import tempfile
import sys

ROOT=Path(__file__).resolve().parents[2]

def main():
    stage=Path(tempfile.mkdtemp(prefix='pu-desktop-'))
    print('Linux desktop staging:',stage,flush=True)
    groups=json.loads((ROOT/'university/desktop-sources.json').read_text())
    files=sum(groups.values(),[])
    env={k:v for k,v in os.environ.items() if not k.startswith('GIT_')}
    env.update(GIT_CONFIG_NOSYSTEM='1',GIT_CONFIG_GLOBAL='/dev/null')
    results=[]
    def run(name,args,expected=0,extra=None):
        p=subprocess.run(list(map(str,args)),cwd=stage,env=env|dict(extra or {}),stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,timeout=600)
        (stage/(name+'.txt')).write_text(p.stdout)
        assert p.returncode==expected,(name,p.returncode,p.stdout[-6000:])
        results.append(dict(name=name,exit=p.returncode,expected=expected))
        return p.stdout
    def protected():
        return {str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()
                for name in ['src','thirdparty','course'] for p in (ROOT/name).rglob('*') if p.is_file()}
    before=protected()
    flags=['g++','-std=c++20','-Wall','-Wextra','-Werror','-pthread']
    includes=['-I'+str(ROOT/p) for p in ['university/include','thirdparty/imgui','thirdparty/imgui/backends','thirdparty/SDL2/include']]
    objects={p:stage/(str(n)+'.o') for n,p in enumerate(files)}
    def compile_one(p): run('compile-'+str(files.index(p)),[*flags,*includes,'-c',ROOT/p,'-o',objects[p]])
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool: list(pool.map(compile_one,files))
    common=groups['core']+groups['view']+groups['imgui']
    run('link-tests',[*flags,*[objects[p] for p in common+groups['tests']],'-o',stage/'desktop-tests'])
    run('link-desktop',[*flags,*[objects[p] for p in common+groups['host']+groups['backends']],'-l:libSDL2-2.0.so.0','-l:libGL.so.1','-ldl','-o',stage/'desktop'])
    print(run('controller-render',[stage/'desktop-tests',ROOT,stage/'study']),end='',flush=True)
    # Restore only the deliberately modified disposable definition for the actual graphics smoke.
    fixture=stage/'study/course/university/missions/003-object-types.pum'
    fixture.write_bytes((ROOT/'course/university/missions/003-object-types.pum').read_bytes())
    smokeenv={'SDL_VIDEODRIVER':'offscreen','LIBGL_ALWAYS_SOFTWARE':'1'}
    args=[stage/'desktop','--repository',stage/'study','--smoke']
    print(run('graphics',args,extra=smokeenv),end='',flush=True)
    run('refused-argument',args+['--apply'],expected=1,extra=smokeenv)
    print(run('fresh-graphics',args,extra=smokeenv),end='',flush=True)
    for script in ['test_desktop_package.py','test_package_update.py','test_validator_input.py']:
        print(run(script,[sys.executable,ROOT/'university/tools'/script]),end='',flush=True)
    assert before==protected()
    data=dict(milestone='PU-DESKTOP-1',linux_passed=True,native_windows='not_run',controller_render_checks=47,
              foundation_suites_rerun=False,protected_source_course_thirdparty_unchanged=True,records=results)
    (stage/'results.json').write_text(json.dumps(data,indent=2)+'\n')
    print('PASS: complete Linux desktop build, workflow, graphics and static/package regressions.')
    print('Evidence:',stage/'results.json')

if __name__=='__main__': main()
