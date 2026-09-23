"""Maintainer-only patch generator; Python is not needed to apply or run University.

Render the fixed template once, with a canonical reviewed path list. Never rename
validator strings in a generated script: that caused the F3 duplicate allowlist.
"""
import argparse
import difflib
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import zipfile
from validator_input import validate_validator


def digest(data):
    return hashlib.sha256(data).hexdigest()


def validate_package(folder):
    folder = Path(folder)
    manifest = json.loads((folder / 'manifest.json').read_text())
    script = (folder / 'Apply-Update.ps1').read_text()
    block = re.search(r'\$allowed = @\(\n(.*?)\n\)', script, re.S)
    if not block:
        raise ValueError('Missing literal allowlist')
    allowed = re.findall(r"^    '([A-Za-z0-9_./-]+)'[,]?$", block[1], re.M)
    if len(allowed) != len(block[1].splitlines()):
        raise ValueError('Invalid literal allowlist')
    paths = [f['path'] for f in manifest['files']]
    patch = (folder / 'implementation.patch').read_bytes()
    patch_paths = re.findall(r'^diff --git a/(\S+) b/\1$', patch.decode(), re.M)
    if not paths or any(len(x) != len(set(x)) for x in [paths, allowed, patch_paths]):
        raise ValueError('Duplicate/empty manifest, allowlist or patch paths')
    if paths != allowed or set(paths) != set(patch_paths):
        raise ValueError('Manifest, allowlist and patch paths differ')
    if digest(patch) != manifest['patch_sha256']:
        raise ValueError('Patch hash mismatch')
    return len(paths)


def generate(baseline_zip, repository, paths, base, stage, validator, output):
    if not re.fullmatch(r'[0-9a-f]{40}', base) or not re.fullmatch(r'(?:U4-F[0-9]+|OFFLINE-FOUNDATION|OFFLINE-INTEGRATION)', stage):
        raise ValueError('Invalid base/stage')
    if not re.fullmatch(r'Validate-[A-Za-z0-9-]+\.cmd', validator):
        raise ValueError('Invalid validator')
    paths = sorted(paths)
    if len(paths) != len(set(paths)) or not paths:
        raise ValueError('Duplicate/empty reviewed path list')
    root, out = Path(repository), Path(output)
    patch, files = '', []
    with zipfile.ZipFile(baseline_zip) as archive:
        for relative in paths:
            pure = PurePosixPath(relative)
            if (not re.fullmatch(r'[A-Za-z0-9_./-]+', relative) or pure.is_absolute()
                    or any(part in ['.', '..'] for part in relative.split('/'))
                    or relative.startswith(('.git/', 'src/', 'thirdparty/'))
                    or (relative.startswith('course/university/') and relative != 'course/university/missions/003-object-types.pum')):
                raise ValueError('Unsafe/protected path: ' + relative)
            target = root / relative
            if any(p.is_symlink() for p in [target, *target.parents]):
                raise ValueError('Symlink target refused')
            name = 'Pandata.Bank/' + relative
            exists = name in archive.namelist()
            before = archive.read(name) if exists else b''
            after = target.read_bytes()
            if relative.startswith("Validate-") and relative.endswith(".cmd"):
                validate_validator(after.decode())
            if exists and before == after:
                raise ValueError('Unchanged listed file: ' + relative)
            patch += f'diff --git a/{relative} b/{relative}\n'
            if not exists:
                patch += 'new file mode 100644\n'
            patch += ''.join(difflib.unified_diff(before.decode().splitlines(True), after.decode().splitlines(True),
                             fromfile='a/' + relative if exists else '/dev/null', tofile='b/' + relative))
            files.append(dict(path=relative, before=digest(before) if exists else None, after=digest(after)))
    manifest = dict(base_commit=base, patch_sha256=digest(patch.encode()), files=files)
    tokens = dict(BASE=base, STAGE=stage, VALIDATOR=validator,
                  ALLOWLIST=',\n'.join("    '" + p + "'" for p in paths))
    template = Path(__file__).with_name('Apply-Update.ps1.in').read_text()
    script = re.sub(r'@@([A-Z]+)@@', lambda m: tokens[m[1]], template)
    out.mkdir(parents=True, exist_ok=True)
    (out / 'implementation.patch').write_bytes(patch.encode())
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    (out / 'Apply-Update.ps1').write_text(script)
    validate_package(out)
    return manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for key in ['baseline', 'repository', 'paths', 'base', 'stage', 'validator', 'output']:
        parser.add_argument('--' + key, required=True)
    args = parser.parse_args()
    generate(args.baseline, args.repository, json.loads(Path(args.paths).read_text()),
             args.base, args.stage, args.validator, args.output)
    print('PASS: generated manifest, allowlist and patch paths match uniquely')
