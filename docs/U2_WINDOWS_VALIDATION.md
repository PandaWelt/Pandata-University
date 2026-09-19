# U2 Windows verification — local results pending

Apply the update following the archive's APPLY_U2.md. Use the existing Windows repository with completed U1 progress and local Git history. No dependency installation, AI account or product refactor is required.

1. Double-click Start-Pandata-University.cmd. It runs the original 44 engine checks, builds/runs new repository-action tests in isolated temporary folders, then starts University. Expect a U2 PASS summary; the count may be lower than Linux if Git is missing or Windows denies symlink creation. Every skip is printed. The orientation should already be complete.
2. Type `run-check`. Expect compiler exit 0, engine output `PASS: 44 checks`, test exit 0 and a saved evidence.json path. This recompiles the actual U1 core/checks, not a fake success stub. Inspect its JSON: passed=true, two command records with exit_status=0.
3. Type `failure-demo`. Expect compilation to succeed and the engine test to fail on the deliberately invalid temporary mission. Overall action must say failed and JSON passed=false with intentional_failure_probe=true. The invalid file is under build/university/u2-checks, never course/university/missions. Type run-check again to confirm normal success.
4. Type `files`, then `view 2`. Expect AppUISetup.h with line numbers. `view ../.git/config`, `view 999` and `run-check && anything` must not run a command or reveal another file. The unchanged completed lesson should still be shown on restart.
5. Type `git-status`, then `git-diff`. Check the existing branch, changed/untracked names and separate unstaged/staged diffs. No commit/add/reset is available. Empty diff output is legitimate for a clean tracked tree; new untracked file contents appear only after normal Git staging outside the app. U2 does not stage them. A configured clean filter is deliberately refused rather than executed.
6. Type `open-vs`. Expect your existing visualstudio/Pandata.bank.vcxproj to open. Confirm it yourself; a successful process handoff alone is not project-load evidence. Missing IDE or Build Tools-only installation must give a clear error and keep University running.
7. Quit and launch again. U1 stays complete; course/university/progress/u1-boundaries/1.pup through 9.pup are unchanged. Evidence for separate actions remains under course/university/evidence/u2.

## Missing-tool and path checks

The repository test executable automatically tests missing Git/compiler by temporarily clearing its own PATH and restores it afterward. It also tests nonexistent executables and literal command metacharacters without shell execution. No machine/user environment settings are changed. If an actual missing-tool error occurs, use Developer Command Prompt for VS 2022 for the compiler, or install/configure your intended Git outside University. The app never downloads or substitutes a tool.

For a Windows junction check, use a disposable COPY of the repository, never your working one. Replace that copy's course/university/evidence/u2 with a junction to an empty scratch directory (`mklink /J` from Command Prompt). An action must report evidence save refusal and write nothing through the junction. Remove only the test junction afterward. Do not redirect real learner data. Automated tests print a skip if symlink creation is unavailable; do not label that a Windows junction pass.

If the launcher/compiler/action fails, retain the first error and its evidence directory. The launcher must stop rather than launch a stale executable. U2 uses a fresh build staging directory each time. A failed compiler command must prevent its test command from running. No workaround should edit PandaBank, thirdparty, completed progress or Git history.

Return the action evidence directories and brief pass/failure notes for local launch, pass check, failure demo, viewer, Git and VS opening. Do not fill out another long mission worksheet. Keep this document's status pending until those results exist.
