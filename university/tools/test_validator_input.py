import unittest
from validator_input import input_block, validate_validator, COMMANDS

class ValidatorInputTests(unittest.TestCase):
    def test_canonical(self):
        text = input_block() + '\napp.exe < "%PUSTAGE%\\console-input.txt"\n'
        validate_validator(text)
        validate_validator(text.replace('\n','\r\n'))
        self.assertEqual(text.count(' echo('), 8)
        self.assertEqual(COMMANDS[-1], 'quit')

    def test_both_bad_pipelines(self):
        for echo in ['echo ', 'echo(']:
            text = '(' + '& '.join(echo + c for c in COMMANDS) + ') | app.exe'
            with self.assertRaises(ValueError): validate_validator(text)

    def test_extra_whitespace_and_wrong_token(self):
        good = input_block() + '\napp.exe < "%PUSTAGE%\\console-input.txt"\n'
        for bad in [good.replace('echo(review-select\n','echo(review-select \n'),
                    good.replace('echo(quit','echo(QUIT'),
                    good.replace(' echo(', ' echo '),
                    good.replace('< "%PUSTAGE%\\console-input.txt"', '')]:
            with self.assertRaises(ValueError): validate_validator(bad)

if __name__ == '__main__': unittest.main()
