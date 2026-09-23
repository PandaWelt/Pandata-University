# Pandata University Offline Foundation 1.0 — PU-OFFLINE-1.0

This is the final integration/acceptance delivery, **Windows run 2 of 2**, based on
accepted checkpoint c2d3a755a0e7e9112b384026d1b355c01ae110b9. It is not another
fixture stage. Run 1 review is in `course/evidence/OFFLINE_c2d3a75_review.md`.

## Apply and validate once

Close University. Extract this patch package outside the existing repository.
In PowerShell at the extracted package folder:

```powershell
.\Apply-Update.ps1 -Repository (Read-Host 'Existing PandaBank repository root')
```

Then, in **Developer PowerShell for VS 2022**, at that repository root, run only:

```powershell
.\Validate-Offline-Baseline.cmd
```

Requires the already-installed VS C++ tools, Git for Windows on PATH and built-in
Windows PowerShell. No Python, Codex, API key, new dependency or network needed.
Do not run the older validators separately. Do not study or edit the repository
concurrently with this preservation audit. Ordinary `.git` directory required;
linked worktrees/reparse paths and percent/quote/newline in the root path are
explicitly refused, not guessed safe. Spaces and `&` are exercised by the harness.

The apply script checks ancestry, touched-file hashes and the exact unique
manifest/allowlist/patch path set. Conflicts stop before application; keep edits,
never reset to make it apply. It does not stage, commit, push or alter progress.

## What the final run actually does

All sessions use a fresh, copied checkout under the printed ignored
`build/university/final-<unique>` stage. The validator copies reviewed definitions
and source inputs, **not real learner snapshots or Git history**. Real state is
read for preservation hashes only. It creates a local synthetic Git history in
the copy with inherited Git configuration isolated; there is no remote operation.

1. Runs the retained combined regression group **once in the disposable copy**:
   33 F1, 128 F2, 58 F3, 48 F4, 203 command-boundary checks; exact 8-command CRLF
   input, deliberate mismatch, fresh input; review demos and actual routing;
   59 offline study checks, or **58 plus explicit symlink SKIP** if denied;
   15 existing console integration checks. U1–U3 suites are not invoked.
2. Invokes the real `Start-Pandata-University.cmd --no-pause`, using its unchanged
   shared builder and schema validation. Fresh synthetic orientation unlocks
   Mission 003; catalog selection, short steps, answer, hint and report are used.
   The read-only C++ verifier checks actual saved position, answer and hint count.
3. Invokes the real launcher again, producing a **second clean build**, resuming
   the saved answer and completing the seven-step lesson with clearly synthetic
   observations. This proves study continuity, not C++ correctness or mastery.
4. Checks malformed/whitespace/invisible-character/case/argument/shell input,
   traversal and write/apply/automation refusals; no saved progress/action evidence
   or proposal display may result. Extra launcher argument exits **2** before build.
5. Uses actual file viewing and read-only Git status/diff; checks machine-readable
   successful evidence, visible known diff, unchanged source/Git/progress.
   Does not repeat run-check/failure-demo (unchanged U1 suite) or launch Visual
   Studio (previously accepted U2 handoff; IDE side effects would pollute this test).
6. Loads a byte-verified backup of synthetic progress; inserts a corrupt newest
   synthetic snapshot and verifies visible fallback without deleting it. Changes
   only the disposable definition and verifies mismatch refusal without migration.
   Restores only that test definition/removes only that inserted corrupt snapshot;
   a fresh session must resume the complete saved record.
7. Rehashes real learner state, course, University source, PandaBank `src/`, native
   project files, root launchers and `.git` bytes; any difference is failure.
   Writes command/output/exit records and hashes to `acceptance.json` in staging.

Logs are captured rather than flooding the terminal. Each stage prints
`PASS: <stage> (exit 0)` only after its expected exit; semantic/state assertions
must also pass before the final line. No timeout/output-limit condition is a skip.
Each process group is bounded to ten minutes and 8 MiB captured output.

## Exact expected result

Retained check counts above appear, followed by launcher-first, first-saved-state,
launcher-restart, complete-saved-state, refused-commands, read-only-actions,
backup-readable, recovery-session/state, mismatch-session/state, fresh-session/state.

- Retained demo and exact-input sequences: **0 / 1 / 0**. The middle exit is the
  intentional refusal; the combined regression process still exits 0.
- `EXPECTED REFUSAL: launcher-extra-argument (exit 2)`.
- `EXPECTED REFUSAL: malformed/traversal/apply/write commands; interactive session exits 0`.
- `EXPECTED REFUSAL: mismatched definition; no migration/reset`.
- If Windows denies symlinks: `SKIP: symlink creation denied; not passed.` This is
  recorded in the receipt, never counted as a passed link assertion.
- `PASS: real learner state, PandaBank source and Git history unchanged`.
- `Acceptance receipt: <printed staging path>\acceptance.json`.
- **Final line, only after all assertions and preservation checks:**

```text
PASS: PU-OFFLINE-1.0 launch-ready offline baseline; Windows run 2 of 2 complete.
```

Overall exit **0**, receipt `passed: true`, `windowsRun: 2`. C4310/C4996 warnings
are non-blocking unless compilation actually fails. A FAIL, missing final line,
timeout or preservation mismatch is a genuine failure: preserve staging and return
the first error, not another speculative run. Do not delete snapshots or relax gates.

## Acceptance without a third records commit

The committed `course/foundation_baseline.json` defines this baseline and its
conditional acceptance rule now. Native final acceptance is **pending**, not
preclaimed. A passing receipt for this reviewed source plus Chris's returned final
result closes the foundation directly in review; it needs **no further records
patch, validation-only commit, or third Windows execution**. Review/commit this
implementation after success as the last foundation implementation commit and
return its hash with the final output/receipt. Keep the receipt in your backup;
ignored logs do not need to enter Git.

## Normal use after success

Double-click `Start-Pandata-University.cmd`; normal pause behavior is preserved.
Completed real U1 should resume Mission 003. Use `lessons`, then `lesson 2` if
needed. Read the first step; make the prediction before changing PandaBank code.
`history`, `step 1`, `report`, `hint`, `more`, `syntax`, `professional` and `feedback`
provide useful offline support. Follow the existing Mission 003 instructions in
Visual Studio; no extraction solution is supplied here. Lesson evidence is not a
compiler pass or reviewer acceptance. Back up `course/university/progress` with
its matching definitions and your repository; Git alone excludes those snapshots.

No additional foundation run is needed unless an actual failure is discovered.
Next work is course content and chronological learner missions, not F6/F7/F8.

## Deferred, not offline blockers

Live mentor remains blocked by unproven exclusive native context-only reads;
managed auth/Plus/model acceptance is not claimed. No AI is invoked by validation.
Real-file proposal application, source writes, Rescue and Maintenance automation
retain their separate isolation/approval/validation/rollback gates. No paid fallback.
Full GUI/editor, CMake migration, adaptive grading and product features are outside
this console baseline. Snapshot power-loss durability, migration/compaction and
Windows junction coverage remain explicit debt, not secretly claimed guarantees.

Linux C++ builds, retained regressions, actual console workflow, package/static
checks ran here. Native cmd.exe/MSVC/PowerShell were unavailable; Windows run 2 is
the real native validation, not a Linux simulation relabeled as Windows evidence.
