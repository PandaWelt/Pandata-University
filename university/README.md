U4-F4: `review-select`, `review-preview`, `review-status`, `review-dismiss` now control the compiled fixture in the normal University console. No proposal/path input or apply command exists. Run `Validate-Review-Controls.cmd` in Developer PowerShell for VS 2022. See ../docs/U4_F4_IMPLEMENTATION.md for exact results and packaging safeguards.

U4-F3: one in-memory review session can select, preview, dismiss and invalidate a fixture. Run `Validate-Proposal-Review.cmd` from Developer PowerShell for VS 2022 at the repository root. See ../docs/U4_F3_IMPLEMENTATION.md. No arbitrary input/import, approval/apply, Git, AI or persistence route is added. F2 is accepted at 6b8081d.

U4-F2: focused compiled serialized-proposal demos are available. Run `Validate-Proposal-Parser.cmd` from Developer PowerShell for VS 2022 at the repository root. See ../docs/U4_F2_IMPLEMENTATION.md. This route has no stdin/file import, Git, AI, progress save or apply capability.

U4-F1: proposal-preview and proposal-stale-demo display/reject an in-memory fixture without saving or executing anything. For focused Windows validation without repeating U1–U3, run Validate-Proposal-Preview.cmd from Developer PowerShell for VS 2022. See docs/U4_FIXTURE.md; all live/apply/automation gates remain unchanged.

# U3 current use

Launch Start-Pandata-University.cmd. U1 progress and all U2 commands remain available. New commands inside University: `mentor-status`, `mentor-preview`, `mentor-send`. Preview precedes each explicit ask; AI text is advisory and cannot run actions or award progress. Missing/unsupported Codex means offline, not a startup failure. No key, billing fallback or provider install is required for the University checks.

See docs/U3_HANDOFF.md and docs/U3_WINDOWS_VALIDATION.md. Windows U3/live provider validation is pending. U2 was verified locally by Chris; junction automation was skipped.

Current Linux build (from repository root):

```sh
mkdir -p build/university
g++ -std=c++20 -Wall -Wextra -Werror -pedantic -Iuniversity/include university/src/Mission.cpp university/src/Process.cpp university/src/mentor/*.cpp university/tests/mentor_tests.cpp -o build/university/mentor-tests
build/university/mentor-tests .
g++ -std=c++20 -Wall -Wextra -Werror -pedantic -Iuniversity/include university/src/Mission.cpp university/src/ProgressStore.cpp university/src/Process.cpp university/src/RepositoryActions.cpp university/src/mentor/*.cpp university/src/main.cpp -o build/university/pandata-university
build/university/pandata-university
```

U2 run-check still runs the original 44 engine checks. The launcher additionally runs unchanged repository tests and the U3 suite before opening the console. Mentor metadata/answer records append under the existing ignored evidence root; they never update course progress.

The following U1 narrative and old build commands are historical, not current build instructions.

# Pandata University — offline foundation

This is a separate C++20 console program, with no new dependencies. It reads one prepared step at a time and saves short responses/hints locally. It does not edit PandaBank, invoke Git/build commands during study, grade answers, connect to AI or implement the next product refactor. The five-step session is an orientation to the existing header/source boundary.

## Windows first launch

Double-click `Start-Pandata-University.cmd` in the existing repository root after applying this handoff. It locates your installed Visual Studio C++ tools (or uses the active Developer Command Prompt), compiles/runs engine checks, compiles the separate program into a fresh ignored staging directory, validates its data and starts it. Read the first step and type `next`. No additional package/download is performed. If it cannot find the compiler, run the same launcher from **Developer Command Prompt for VS 2022**, or check the installed Desktop development with C++ workload. Keep the first error for review; do not modify PandaBank to fix a University tooling error.

The Windows launcher was statically inspected but cannot be executed on this Linux host. Expected local result: `PASS: 44 checks`, schema/prerequisite validation, then the first University step. Quit/relaunch should resume your saved step. This is the next local platform verification, not a claimed Windows pass. PandaBank continues to build through the unchanged visualstudio/Pandata.bank.vcxproj; instructions in docs/BUILD_WINDOWS.md.

## Linux build actually verified

Run from the repository root with a C++20 compiler:

```sh
mkdir -p build/university
g++ -std=c++20 -Wall -Wextra -Werror -pedantic -Iuniversity/include university/src/Mission.cpp university/src/ProgressStore.cpp university/tests/engine_tests.cpp -o build/university/engine-tests
build/university/engine-tests course/university/missions/u1-boundaries.pum course/university/pilot.puc
g++ -std=c++20 -Wall -Wextra -Werror -pedantic -Iuniversity/include university/src/Mission.cpp university/src/ProgressStore.cpp university/src/main.cpp -o build/university/pandata-university
build/university/pandata-university --validate
build/university/pandata-university
```

The same test executable was separately built with `-O2 -DNDEBUG` and passed all 44 checks; tests do not use assert for their pass/fail checks. CLI integration was verified with a disposable copy of the course data. These are University builds, not a Linux PandaBank build. No CMake configuration was created because CMake is unavailable here; the planned portable migration remains separate.

## Study and recovery

Commands: `next`, `answer <one short response>`, `check`, `hint`, `more`, `syntax`, `professional`, `quit`. Optional content stays collapsed until requested. Check only verifies evidence presence and valid state; responses are explicitly ungraded. Mentor/Rescue/Maintenance are unavailable. Answers/hints/next save snapshots automatically. No test answers are bundled as your progress.

Back up `course/university/progress` together with definitions; it is personal local state ignored by Git. Invalid definitions stop; corrupt snapshots are preserved with recovery messages. Details: docs/UNIVERSITY_FORMAT.md. Definitions can be inspected in a text editor; do not modify them mid-session without preserving state and arranging explicit migration.

See docs/UNIVERSITY_HANDOFF.md for validation, scope and U2's next engineering gate. No generated executable or test cache is source or included in the delivery.
