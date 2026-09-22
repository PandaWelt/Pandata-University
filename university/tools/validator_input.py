"""Canonical command-input bytes for production Windows validators (maintainer only)."""
import re

COMMANDS = ('review-status', 'review-select', 'review-preview', 'review-apply',
            'review-dismiss', 'review-preview', 'review-status', 'quit')
BEGIN = 'rem BEGIN DETERMINISTIC CONSOLE INPUT'
END = 'rem END DETERMINISTIC CONSOLE INPUT'


def input_block():
    lines = [BEGIN]
    for index, command in enumerate(COMMANDS):
        # Fixed tokens only; never interpolate user input or commands with shell metacharacters.
        if not re.fullmatch(r'[a-z-]+', command):
            raise ValueError('Not a fixed safe command token')
        lines.append(('>' if index == 0 else '>>') + ' "%PUSTAGE%\\console-input.txt" echo(' + command)
        lines.append('if errorlevel 1 goto inputfailed')
    return '\n'.join([*lines, END])


def validate_validator(text):
    text = text.replace('\r\n', '\n')
    # Production validators need no pipelines, even with different echo spelling.
    for line in text.splitlines():
        if not line.lstrip().lower().startswith(('rem ', '::')) and '|' in line:
            raise ValueError('Pipelines forbidden in production validators')
    if 'console-input.txt' in text:
        if text.count(BEGIN) != 1 or text.count(END) != 1:
            raise ValueError('Canonical console-input block missing/duplicated')
        actual = text[text.index(BEGIN):text.index(END) + len(END)]
        if actual != input_block():
            raise ValueError('Noncanonical input generation (including whitespace)')
        if '< "%PUSTAGE%\\console-input.txt"' not in text:
            raise ValueError('Deterministic stdin redirection missing')
