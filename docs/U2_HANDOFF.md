# U2 repository actions — 2026-09-18

After applying the update patch to your existing repository, **double-click Start-Pandata-University.cmd and type `run-check`.** Your completed orientation stays complete. The console remains open for repository actions instead of exiting after the last step.

## Submission review and preservation

Submitted U1 archive SHA-256: `9ba26175c2a021f1da0f9f1890e0accd539ff995c1646fa3f83e02690eab3f24`. Git HEAD is `74aef5e` (Add Pandata University U1 foundation); Mission 002 commit `21ba2af` is present in the supplied history. The nine progress snapshots end with all five steps complete, responses “linker problem”, “AppUISetup.cpp”, “object presets/tools”, and one transfer hint. Snapshot completion was read through the existing engine. This is orientation evidence, not a completed product refactor or durable mastery. Chris reports successful Windows launcher compilation and engine checks; no Windows execution was performed by this reviewer.

All submitted PandaBank source, dependencies, native Visual Studio files, Git metadata, learner snapshots and unrelated untracked files were left byte-identical. Mission definition/pilot index and pure U1 engine/store/tests were also unchanged. No commit, reset, checkout, merge or cleanup was performed in this repository. Temporary test repositories have their own fixture commits only.

## Actual U2 scope

| Command | Behavior |
|---|---|
| files / view NUMBER | Nine reviewed relevant files, numbered read-only display; no arbitrary path accepted |
| run-check | Compile current University engine tests using fixed source/argument lists into a fresh build directory, then run all 44 U1 checks against real prepared content |
| failure-demo | Same build/check action with a deliberately invalid mission in that new output directory; real test returns nonzero; original content/progress untouched |
| git-status | Read-only branch and short status, including untracked names |
| git-diff | Separate unstaged and staged tracked-file diffs; untracked file contents are not a Git diff |
| open-vs | Discover installed Visual Studio IDE and hand it the existing PandaBank .vcxproj; no shell file association |
| help / quit | Show commands / leave without changing completed U1 progress |

Every accepted repository action records a separate evidence directory with command executable, argument array, cwd, combined stdout/stderr, exit status, duration, timeout/output-limit flags, error, timestamp and action result. Raw output files and versioned JSON are human/tool-readable. Missing tools and failed commands are evidence too. An intentional failing check remains `passed: false`; it never advances a lesson. The study app does not auto-commit or grade answers.

## Restrictions and limits

- UI accepts fixed action IDs and numbered files only. No general terminal, free-form compiler flags, command templates or user-supplied executable.
- Process creation uses direct argv on POSIX / quoted CreateProcess arguments on Windows, with no shell. Tool lookup excludes empty/relative PATH entries and binaries under this repository. Installed compiler/Git/VS remain trusted tools.
- Relevant-file and output paths reject symbolic links and Windows reparse points/junctions. File reads cap at 1 MiB. Process output caps at 1 MiB per command; actions terminate on a deadline (compiler/tests 120 s; Git/discovery 15 s). Child groups/jobs are terminated on enforced limits. Terminal control bytes are escaped; CRLF displays normally.
- This is policy for a trusted local development repository, not a sandbox for hostile C++ or a defense against concurrent filesystem replacement. Compiling/running modified tests executes that reviewed project code. Do not treat arbitrary downloaded code as safe because its build command is allowlisted.
- Git runs without optional locks, pager, external diff/text conversion or fsmonitor. Inherited GIT_* overrides and global/system config are excluded; effective config keys are inspected and configured filters cause refusal. Config values/credentials are not dumped. Local Git history/index are not modified by these views. Git worktree root must equal the University root.
- Build products go under ignored build/university/u2-checks. Evidence goes under ignored course/university/evidence/u2; back it up alongside progress. No automatic deletion/compaction of old runs. Partial evidence directories may remain if saving fails; only final evidence.json represents a completed record. Save failure keeps output visible and reports that it is not durably recorded.
- Visual Studio launch is a handoff, not proof of project load or build. Its evidence has null exit status and handed_off=true. The IDE can create its normal .vs/user/cache files after launch. The University action does not edit source or compile PandaBank.

## Validation and platform gate

Observed Linux/GCC tests are in validation/u2-validation.txt; representative real pass/fail JSON is in validation/u2-success.json and u2-expected-failure.json. Test fixtures exercised actual compiler execution, literal spaces/metacharacters, missing tools, nonzero exit, deadlines/output bounds, path restrictions, successful/failing engine checks, JSON/raw-output agreement, Git staged/unstaged visibility and index/HEAD preservation. Tests operate in temporary directories; production source/progress and the submitted Git history were hash-checked afterward.

Windows Process/CreateProcess/job code, launcher changes, junction policy and Open in Visual Studio require local verification; they have not run here. U1 Windows success does not certify new U2 platform code. Follow U2_WINDOWS_VALIDATION.md. No CMake migration, editor, PandaBank refactor, AI, Rescue or Maintenance was implemented. The next prerequisite is this Windows U2 check, before considering U3's already documented read-only mentor contract.

## Files and application

U2_CHANGED_FILES.md is the exhaustive list against the submitted archive. The delivered archive contains an update patch and only its changed-file payload, not a replacement .git/source/progress tree. APPLY_U2.md explains git apply --check, apply and conflict handling. Keep the original upload/local Git checkpoint as recovery. The patch was checked/applied to an isolated copy of the submitted tree and its resulting bytes compared with the intended payload; no target history was replaced.
