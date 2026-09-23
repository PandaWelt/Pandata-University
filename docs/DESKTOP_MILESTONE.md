# Pandata University Desktop 1 — PU-DESKTOP-1

Base: **c4f664009d45e8e2f7befd9457883362c009d4b5**. PU-OFFLINE-1.0 is accepted and
closed. This is one functional graphical milestone, not another foundation cycle.

## What you can use

The separate `pandata-university-desktop.exe` opens a dashboard with your existing
course catalog and saved position. Continue learning opens the active short step.
Why, expected result, more, syntax, professional alternatives and history/report
expand on demand. Short answers and hints use the accepted append-only progress
store. Errors leave the last saved state intact. Draft answers must be explicitly
saved; they are not saved on window close. Switching lessons/advancing with a
nonempty draft is refused until it is saved or cleared.

The workspace has read-only, selectable/copyable relevant-file text and a checks/
output tab. Check course validates actual definitions. Explicit University check,
Git status/diff and Open in Visual Studio buttons use the existing fixed U2 actions.
Processes run off the UI thread, one at a time, with the existing timeout/output
bounds and separate machine evidence. Closing waits for a running action's result.
No command textbox, file-save, proposal application or source-writing operation exists.

The AI mentor is visibly disabled: no provider code is linked into this executable,
no account is queried and no context is sent. Prepared help remains available offline.
The console executable and `Start-Pandata-University.cmd` are unchanged fallbacks.
Both views use the same definitions and real progress; do not study in both at once.
The existing optimistic generation check refuses stale-session saves.

## Graphics choice and boundary

Reuse the exact bundled SDL2 + OpenGL 3 + Dear ImGui source. SDL2 is zlib-licensed;
ImGui is MIT-licensed. No new dependency, Qt, service, subscription or asset download.
The default fonts are embedded in ImGui. Bundled font/stb/SDL/ImGui notices are
copied beside the executable; existing exact-revision/binary provenance audit is
still open for distribution. No third-party source is modified or upgraded.

`DeskController` depends only on the accepted StudyDesk/store/repository interfaces.
`DesktopView` owns ImGui widgets and transient drafts; `desktop_main.cpp` owns the
SDL/OpenGL window. No PandaBank source is compiled into University. The canonical
`university/desktop-sources.json` supplies the build list.

Tradeoffs: this toolkit is a small, already-compatible route to an interactive
desktop with keyboard navigation, resizable panels and adjustable text size.
It is not the final integrated editor, native screen-reader accessibility layer,
or phone UI. A future editor needs a separate document/undo/save controller and
explicit authorized-path/write gates; it must not grow out of the read-only viewer
by simply removing a flag. A future mentor supplies advisory events behind its
own capability boundary only after native isolation/auth/allowance gates pass.
Either future view can replace ImGui without changing mission/progress formats.

## Your complete four-step procedure

1. **Apply one package.** Close both University views. Extract the patch outside
   the repository. In PowerShell at the extracted package folder:

   ```powershell
   .\Apply-Update.ps1 -Repository (Read-Host 'Existing PandaBank repository root')
   ```

   Exact ancestry/preimage/manifest checks preserve unrelated work and refuse
   touched-file conflicts. No reset, stage, commit, push or progress replacement.

2. **Run one validator.** In **x64 Developer PowerShell for VS 2022**, at the root:

   ```powershell
   .\Validate-University-Desktop.cmd
   ```

   Expected: desktop executable and test executable compile; **47 new desktop
   controller/render checks** pass; actual graphics smoke renders 20 frames;
   deliberate `--apply` argument is rejected with exit **1**; fresh graphics smoke
   passes. Sequence **0 / 1 / 0**. Real source, history, definitions and progress
   preservation checks pass. This runs no U1–U4 foundation suites and no AI.
   A brief graphics window can appear and close during each automated smoke.

   Final line:

   ```text
   PASS: PU-DESKTOP-1 build, controller/render checks, graphics/refusal/fresh and preservation.
   ```

   The validator builds in a short physical `%TEMP%\pug-<id>` directory, outside
   the repository, and prints its location. It keeps logs and
   `desktop-acceptance.json` there. It copies only the checked executable, supplied
   SDL2.dll and notices into `build/university/desktop`. It does not copy real
   progress into tests. Preserve the first error/logs on failure; do not relax gates.
   C4996 warnings inherited from U2 are non-blocking unless compilation fails.
   Graphics unavailability is a failure, not a falsely passed or skipped UI test.
   Previous Windows symlink coverage remains SKIPPED; this milestone does not
   claim to close that separate coverage gap.

3. **Launch and visually inspect once.** Double-click
   `Start-Pandata-University-Desktop.cmd`. This opens the checked Windows desktop
   executable, not the console. The launcher initializes installed VS tools when
   available, so explicit University checks can find the compiler. A missing tool
   produces a useful action error; it does not prevent ordinary offline study.

   Verify the dashboard shows your existing U1/Mission 003 state. Choose Continue
   learning, expand Why/Syntax/Professional alternatives, resize the window and
   adjust text size. Select AppUISetup.cpp in Code/files. In Checks/output, choose
   Check course and confirm visible PASS. AI mentor must remain disabled. No dummy
   answer needs to enter your real progress: save/restart behavior was tested with
   disposable data. This is the visual part of the same milestone, not another
   foundation acceptance cycle. When you are ready to learn, Save answer and Next
   step record your real observations normally.

4. **Review, commit and push once.** Include this package's exact changed paths,
   preserving unrelated work. Return the validator final line, visual result and
   commit hash. No separate records-only or design-only package is required first.

The executable can also be launched directly from `build/university/desktop`; it
finds the repository above its build folder. Moving only the executable elsewhere
does not package the course; use the repository launcher. SDL2.dll stays beside it.

## Preservation and Windows staging lesson

The final offline foundation passed at a short physical copy after nested staging
exceeded Windows path limits. SUBST did not shorten the canonical physical path.
This validator avoids nested repository copies entirely, rejects overly long temp
roots and reparse paths, and keeps synthetic state shallow. TEMP must refer to a
short physical writable directory, not a SUBST workaround. The repository's existing
source paths must still be accessible to the Windows compiler. This is not a claim
of unlimited Windows long-path support.

All accepted domain/store/console/mentor files, old tests, course definitions,
snapshots, PandaBank and third-party code remain unchanged. Only desktop code,
build/validation tooling, package-stage support and current records are added or
updated. GUI-generated ImGui ini/log files are disabled.

## Validation provenance

Linux: C++20 warnings-as-errors builds of the complete desktop and focused tests;
47 controller/render checks; actual SDL/OpenGL smoke on Mesa offscreen; refusal/
fresh smoke; graphical layout review; package/static regressions and apply/conflict
checks. Native Windows/MSVC/PowerShell execution is pending your single procedure.
See `docs/validation/desktop-1.json` and the package's VALIDATION.json.

## Sources

- Existing bundled source and APIs: thirdparty/imgui and thirdparty/SDL2.
- ImGui MIT notice: https://github.com/ocornut/imgui/blob/master/LICENSE.txt
- SDL2 zlib license: https://www.libsdl.org/license.php
- MSVC Unicode filenames/response files: https://learn.microsoft.com/en-us/cpp/build/reference/unicode-support-in-the-compiler-and-linker
