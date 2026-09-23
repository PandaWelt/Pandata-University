Accepted at 114c272 from source/history/hash review and Chris's Windows results. See ../course/evidence/U4_114c272_review.md. Stop multiplying fixture stages; the combined offline foundation supersedes earlier next-step instructions.

# U4-F5 — deterministic validator input and boundary regression

Base: 812223b0052dcbf5fa0833516a458a3d723aebd6. F4 accepted; see course/evidence/U4_812223b_review.md. This is the smallest missing validation slice exposed by the Windows pipeline defect, not another fixture feature. U-09e documents the scope. University implementation, PandaBank, learner snapshots and historical evidence are unchanged.

## What is implemented

- Canonical fixed-token input-block generator and package-time validation: separate prefix-redirection echo( commands, exact spelling/whitespace, one input block, stdin redirection, no production pipelines. Maintainer Python only; no new runtime dependency.
- Standalone C++ byte probe reads at most 4096 stdin bytes, uses Windows binary stdin, returns 0 for the exact eight-command CRLF stream, 1 for mismatches, 2 for invalid invocation/read error/oversize. JSON hex/length records contain no raw control characters. It never opens paths or invokes processes.
- 203 focused rejection/state-preservation checks: spaces, tabs, CR/LF, NUL, vertical tab, form feed, BOM, NBSP, zero-width space, line separator, ANSI escape, backspace, arguments, paths, shell syntax, quotes and changed casing. Existing exact parser remains unchanged.
- F4 validator hardened; new F5 validator checks good/mismatched/fresh input before routing. Deliberate mismatch adds a blank line to a separate staging copy; it is never sent to University.
- Separate optional native diagnostic reproduces both old pipeline spellings only into the byte probe. Production validator does not depend on either pipeline. All diagnostic data is hex JSON; no command output is executed.
- F3 regression retained: unique canonical manifest/allowlist/patch sets, including rejection of duplicate Review/omitted Parser. New generator regression rejects both grouped-pipeline forms.

The fixture adapter remains in-memory, no file/Git/process/AI/write/apply capability. The compiler and test harness necessarily create executables/input/logs in fresh ignored build staging; these are not learner progress or runtime evidence. Live mentoring and all source-write/application/Rescue/Maintenance gates remain blocked.

## Apply and validate on Windows

Extract the package outside the repository. From its U4_F5 folder in PowerShell:

```powershell
.\Apply-Update.ps1 -Repository (Read-Host 'Existing PandaBank repository root')
```

The script verifies history ancestry, exact touched-file hashes, reparse restrictions, patch hash and manifest/allowlist/patch equality before applying. It preserves unrelated edits and refuses touched-file conflicts; do not reset. It does not stage, commit or push. Reapplying an already complete package is a no-op.

Then, in Developer PowerShell for VS 2022 at the repository root:

```powershell
.\Validate-Console-Input.cmd
```

Expected sequence:
1. Dependency checks: PASS 33 F1, 128 F2, 58 F3, 48 F4.
2. `PASS: 203 command-boundary checks`.
3. Full University and byte probe compile successfully.
4. Existing F4 demo / refused apply / fresh demo return **0 / 1 / 0**.
5. `PASS: exact 8-command CRLF input verified.`
6. Byte probe good / extra-blank-line mismatch / fresh good return **0 / 1 / 0**. JSON records are input-evidence.json, mismatch-evidence.json, fresh-input-evidence.json in the printed staging directory; exact flags true / false / true.
7. Actual University console receives only the verified good file, displays fixture, refuses apply, dismisses, rejects preview without selection and quits with exit 0.
8. Final line: `PASS: U4-F5 exact input / deliberate mismatch / exact input; exits 0 / 1 / 0; console routing passed.` Validator exit 0.

No U1–U3 tests run. Review operations save no progress/evidence. Validator captures are disposable build artifacts. Keep the first failure and printed staging directory if any step fails; do not alter the parser to pass it.

Optional native root-cause measurement, using the exact staging directory printed above:

```powershell
.\university\tools\Diagnose-Cmd-Pipeline.cmd (Read-Host 'Printed staging directory')
```

Expected diagnostic exit 0 and two JSON files, original-pipeline.json and echo-parenthesis-pipeline.json. Their bytes are observations, not a required false/true value: an affected cmd host is expected to show `20` before line endings where separators introduced spaces. Send these two small JSON records with validation results to confirm this host's exact behavior. They contain only fixed fixture command bytes. Do not feed them into University. This optional investigation is separate from acceptance of the deterministic path.

## Reviewer checks and limits

Linux: 48 existing F4 checks, 203 new boundary checks, 53 byte-probe checks, five maintainer regression cases, full C++20 warnings-as-errors build and focused demos/console routing. U1–U3 not rerun. No cmd.exe, MSVC or PowerShell is available here; no native Windows execution or apply-script execution is claimed. Linux patch apply/hash preservation and conflict refusal are tested separately from the pending PowerShell check. See docs/validation/u4-f5.json.

On a clean Windows result, accept this validation slice without another records-only cycle. Further runtime work must name and meet a concrete external-input/isolation/approval safety gate; F5 does not authorize expansion into real-file proposals or automation.
