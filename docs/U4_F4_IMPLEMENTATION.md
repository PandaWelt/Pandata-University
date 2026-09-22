# U4-F4 — usable fixture review controls and reliable packaging

Base: d35d5529a9541ba029f82a76ca86cae07e445c53, verified direct child of 6b8081d. F3 accepted; see ../course/evidence/U4_d35d552_review.md. F4 is the next presentation step: Chris can now drive the reviewed lifecycle in the existing University console instead of only watching compiled sequences. It adds no real-file proposal source or write authority.

## Four exact commands

| Command | Behavior |
|---|---|
| review-select | Select the compiled fixture; status becomes proposed |
| review-preview | Revalidate through unchanged F3/F2/F1 and display; without selection, NO ACTIVE PROPOSAL |
| review-status | Show last operation's state; explicitly not approval or real-file freshness |
| review-dismiss | Clear selection; later preview cannot resurrect it |

Only these exact strings, with no arguments, are accepted by the pure adapter. It refuses commands over 32 bytes, paths, proposal text, shell punctuation, apply requests and unknown variants without echoing raw input or dispatching anything. Refused commands leave the selection unchanged. The existing console reads command lines as before; the adapter itself has no input stream or external interface. This is not arbitrary stdin proposal import. F1/F2/F3 implementation and tests remain byte-identical.

The existing normal console loads its missions/progress and creates its existing repository-action object at startup. Review commands do not access those objects or save anything. This does not claim the entire University executable lacks I/O. Compiled focused demos return before normal startup. No file lookup, Git, child process, AI, persistence, approval/apply, Rescue or Maintenance automation is added to the controls. Native live-mentor isolation remains blocked. U-09d records this presentation-only scope before implementation.

## Apply and validate

Extract the package outside the repository. In PowerShell in its U4_F4 directory:

```powershell
.\Apply-Update.ps1 -Repository (Read-Host 'Existing PandaBank repository root')
```

Keep affected files closed during application. The script requires base ancestry, exact touched-file hashes, no reparse paths and a clean patch preflight. Unrelated dirty files are allowed. It does not reset/stage/commit/push. A conflict refuses; preserve the error and your files. Reapplying a complete package is a no-op.

Then in **Developer PowerShell for VS 2022**, at the repository root:

```powershell
.\Validate-Review-Controls.cmd
```

This builds focused dependencies/tests and actual University into a fresh ignored build/university/controls-* directory. Missing cl.exe reports the required Developer shell. Existing directories are not overwritten. Compiler/installer/validator I/O and captured console output are separate from runtime review operations.

## Exact expected results

1. PASS: 33 U4-F1 checks (in-memory preview only)
2. PASS: 128 U4-F2 checks (in-memory parser only)
3. PASS: 58 U4-F3 review-session checks
4. PASS: 48 U4-F4 review-control checks
5. Full University builds successfully.
6. `--review-controls-demo`: empty status -> proposed -> displayed -> dismissed -> NO ACTIVE PROPOSAL -> dismissed status -> proposed -> displayed. Exit 0.
7. `--review-controls-refusal`: proposed -> REJECTED (review-apply) -> still proposed status. Exit 1, expected.
8. Fresh `--review-controls-demo`: same successful lifecycle. Exit 0.
9. Actual console is fed fixed commands: status/select/preview/refused apply/dismiss/preview/status/quit. Validator checks the empty/displayed/refusal/dismissed/no-active output markers; console exits 0. Its stdout is captured to ignored staging console.txt, not runtime evidence storage.

Final line:

    PASS: U4-F4 controls / refused apply / fresh controls; exits 0 / 1 / 0; console routing passed.

Send the four PASS counts and final line, or the first failure. No U1–U3 suites run; earlier checks are dependency regressions, not reopened learner assignments. Runtime review commands save no progress/evidence.

To use the controls afterward without rerunning suites, execute the built pandata-university.exe from the repository root (its staging path is printed). Type review-select, review-preview, review-status, review-dismiss, then quit. A new process starts with empty review state while completed U1 remains completed. This optional interaction is not an additional acceptance round trip.

## Packaging defect fixed

The previous F3 generator renamed a validator filename globally after inserting the allowlist. That corrupted one allowed path despite a valid patch. Chris's outside-repository fix worked; this delivery fixes the cause rather than relying on repeating it.

Maintainer-only university/tools/package_update.py and Apply-Update.ps1.in now render fixed tokens once from a single reviewed path list. No post-render filename replacements. test_package_update.py recreates the exact duplicate Review/omitted Parser and also tests mismatched membership and duplicate manifest/source entries. Final validation compares manifest, literal apply allowlist, patch paths and uniqueness plus patch hash before archive creation. The generated PowerShell repeats scope checks before application. Both Validate-Proposal-Parser.cmd and Validate-Proposal-Review.cmd must appear exactly once when changed. CHANGED_FILES.md and manifest.json list this package's edits. Python is not needed to apply, validate or run University; it is optional maintainer packaging tooling only.

## Reviewer evidence and limits

Linux: 33/128/58 dependencies, 48 F4 checks, full C++20 -Wall -Wextra -Werror build, ASan/UBSan with leak detection disabled, demo exits 0/1/0, extra/unknown flag rejection, actual console routing/quit/relaunch. Completed U1 stayed complete, review restarted empty, and every repository file remained byte-identical during runtime tests. The package undergoes disposable-copy application, delivered-code tests, conflict refusal and protected-byte preservation checks. See validation/u4-f4.json and package VALIDATION.json.

No Windows/MSVC/PowerShell execution or native syscall isolation is claimed by the reviewer. Leak detection is not claimed passed. The Windows console marker checks do not themselves hash every file; Linux preservation checks and source review provide that evidence. Review controls have only F3/F1 and standard string dependencies; no external authority is introduced. Live mentoring and full U4 remain unavailable. After acceptance, identify the next concrete capability gate instead of multiplying fixture-only stages without a useful outcome.
