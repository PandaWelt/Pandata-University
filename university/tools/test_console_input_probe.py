"""Run against a compiled console_input_probe binary; Python is maintainer-only."""
import json
from pathlib import Path
import subprocess
import sys
from validator_input import COMMANDS

probe = str(Path(sys.argv[1]).resolve())
expected = ('\r\n'.join(COMMANDS) + '\r\n').encode('ascii')
checks = 0
for mode, data, code in [
    ('--verify', expected, 0), ('--verify', expected.replace(b'\r\n', b' \r\n'), 1),
    ('--verify', expected.replace(b'\r\n', b'\n'), 1), ('--verify', b'', 1),
    ('--verify', expected + b'\r\n', 1), ('--verify', expected + b'\0', 1),
    ('--verify', b'x'*4096, 1), ('--verify', b'x'*4097, 2),
    ('--diagnose', expected, 0), ('--diagnose', b'\x00\xff\r\n', 0),
    ('--diagnose', b'x'*4097, 2), ('--wrong', expected, 2), ('--verify', expected, 0)
]:
    result = subprocess.run([probe, mode], input=data, capture_output=True, timeout=5)
    assert result.returncode == code, (mode, len(data), result.returncode)
    checks += 1
    if code != 2:
        record = json.loads(result.stdout)
        assert record['exact'] == (data == expected)
        assert record['bytes'] == len(data)
        assert bytes.fromhex(record['hex']) == data
        assert b''.join(bytes.fromhex(line['hex']) for line in record['lines']) == data
        checks += 4
print(f'PASS: {checks} byte-probe checks')
