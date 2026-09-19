# U2 current use

U1 stays completed. Launch Start-Pandata-University.cmd, then type run-check. Use files/view NUMBER, failure-demo, git-status, git-diff, open-vs, help and quit. Windows launcher now builds/tests the new adapter too; native U2 validation is pending. See docs/U2_HANDOFF.md and docs/U2_WINDOWS_VALIDATION.md. The old U1 scope/build commands below are historical; use these updated Linux commands:

```sh
mkdir -p build/university
g++ -std=c++20 -Wall -Wextra -Werror -pedantic -Iuniversity/include university/src/Mission.cpp university/src/Process.cpp university/src/RepositoryActions.cpp university/tests/repository_tests.cpp -o build/university/repository-tests
build/university/repository-tests .
g++ -std=c++20 -Wall -Wextra -Werror -pedantic -Iuniversity/include university/src/Mission.cpp university/src/ProgressStore.cpp university/src/Process.cpp university/src/RepositoryActions.cpp university/src/main.cpp -o build/university/pandata-university
build/university/pandata-university
```

Repository tests use temporary copies/checkouts only; they never edit your source/progress/history. run-check itself compiles/runs the actual 44 U1 engine tests. Each action saves separate machine evidence; it never advances a lesson automatically.


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
