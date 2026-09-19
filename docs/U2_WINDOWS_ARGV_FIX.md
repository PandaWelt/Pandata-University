# U2 Windows argv probe correction — 2026-09-18

Scope: apply on top of the already-applied, uncommitted U2 update. Do not reapply U2, replace the checkout, reset Git or repeat U1. The runtime argument quoting, process launch and raw-output capture are unchanged.

## Finding

The failing assertion combines process success with exact output equality. The test child writes four LF-terminated argument records through std::cout. Windows CRT text output translates each LF into CRLF; the parent correctly captures raw bytes. The test expected only LF. This is a sufficient, deterministic fixture defect even with perfect argv. The screenshot does not include actual output/process flags, so it cannot independently exclude an additional launch failure.

The Windows quote function was checked against Microsoft's CRT rules: arguments are quoted, embedded quotes receive an escape, preceding backslashes are doubled appropriately, and trailing backslashes are doubled before the closing quote. CreateProcessW receives an explicit executable and no shell. The provided spaces/quote/trailing-backslash/metacharacter arguments do not justify changing that implementation.

Fix: in the test child's --child branch only, put stdout into _O_BINARY before any output, using _setmode(_fileno(stdout), _O_BINARY). Failure to set that mode returns nonzero. Keep child.ok() and exact byte equality, the same expected bytes and all safety checks. Do not strip CR, trim whitespace, normalize production output, skip Windows or accept alternative corrupted output. Parent/test-console and real compiler/test capture behavior are unchanged.

On another failure, the test prints started/exit/timeout/output-limit/error and escaped expected/captured bytes, and preserves argv-probe-output.bin in its scratch directory. This separates a launch error from argument corruption or byte translation. No environment variables or authentication values are dumped. The existing C4996 environment-access warnings are unrelated and are not suppressed in this fix.

## Validation

Linux GCC C++20 warning-as-error compile and all 46 U2 checks passed. Direct child invocation emitted the exact expected bytes. A temporary negative test deliberately split “space value” into two argv elements; the original safety gate still failed and displayed byte diagnostics. The negative mutation is not delivered. A simulated CRLF translation demonstrates the old byte mismatch; that is not native Windows execution.

Windows/MSVC and Wine are unavailable here. Native corrected launch still requires Chris's rerun; U2 remains under Windows validation and U3 is not started. Original submitted source/history/snapshots and existing U2 changes are not rollback targets. The corrective patch touches only repository_tests.cpp, this note and course/PROGRESS.md.

After applying the corrective patch, rerun Start-Pandata-University.cmd. Keep the existing scratch directory as evidence; no cleanup is needed. If the safety test fails again, send the new “argv probe”, “expected bytes” and “captured bytes” lines (or argv-probe-output.bin). If the app opens, type run-check at its own prompt. run-check is not a PowerShell command.

References: [Microsoft _setmode](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmode?view=msvc-170) documents text translation, binary mode and setting the mode before I/O. [Microsoft command-line parsing](https://learn.microsoft.com/en-us/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170) documents quote/backslash handling. No implementation was copied from external source.
