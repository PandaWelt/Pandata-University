# Migration mission — portable build, prepared but not implemented

Status: waiting for Windows validation access; not the next learner mission.
Order: Mission 001, then debugging/assertions (002), then a focused header/source lesson before the full build migration. A build blocker may justify doing the minimal tooling part sooner without marking learner concepts complete.
Time: one 60–90 minute Windows build slice; Linux verification is a separate platform gate.
Value: build the real source from a moved checkout with Visual Studio still available.

## Boundary and recommendation

Add `CMakeLists.txt` and `CMakePresets.json`; retain the existing `.vcxproj`, `.filters`, `.slnx` and resource files throughout verification. No directory refactor, dependency update, renderer replacement, database, or dashboard. Read-only: `thirdparty/`, Mission 001 behavior, product specification. Allowed: build files and their documentation; any source incompatibility must be diagnosed and separately scoped before editing.

Use target-scoped includes/options, C++20 for the app, and two actual targets initially: `pandabank_imgui` (static library) and `pandabank_app` (console executable). No empty core/storage targets. CMake adds no production language or runtime dependency; it costs a build-tool installation and a compile/link lesson. Removing unadopted CMake files reverses the migration. Do not make CMake authoritative until the acceptance gate passes.

## Source mapping (explicit lists, no glob)

`pandabank_app` compiles:

- `src/main.cpp`
- `src/UI/AppUISetup.cpp`
- `src/UI/AppUIConfig.cpp`
- `src/Inputs/InputConfig.cpp`
- on Windows only, existing `visualstudio/Pandata.bank.rc` (enable RC and verify `winres.h`).

`pandabank_imgui` compiles the same vendored sources as the native project:

- `thirdparty/imgui/imgui.cpp`
- `thirdparty/imgui/imgui_draw.cpp`
- `thirdparty/imgui/imgui_tables.cpp`
- `thirdparty/imgui/imgui_widgets.cpp`
- `thirdparty/imgui/imgui_demo.cpp`
- `thirdparty/imgui/backends/imgui_impl_sdl2.cpp`
- `thirdparty/imgui/backends/imgui_impl_opengl3.cpp`

Expose ImGui root/backend include paths from its target, SDL headers through the SDL target, and `src/UI` privately on the app. Keep the unused placeholder translation units initially for parity; cleanup is a different change.

## Deliberate dependencies

1. On Windows/MSVC/x64, model the supplied SDL DLL with a SHARED IMPORTED target: `IMPORTED_IMPLIB` = repository's `thirdparty/SDL2/lib/x64/SDL2.lib`, `IMPORTED_LOCATION` = adjacent `SDL2.dll`, `INTERFACE_INCLUDE_DIRECTORIES` = `thirdparty/SDL2/include`. Same supplied import library for Debug and Release; validate both CRT/link configurations. Fail configuration for non-x64 or unsupported Windows compilers rather than guessing compatible binaries.
2. Link only SDL2, not SDL2main: `main.cpp` already defines `SDL_MAIN_HANDLED`. Audit SDL main-readiness behavior if runtime startup fails; do not add entry-point shims speculatively.
3. Use `find_package(OpenGL REQUIRED)` and `OpenGL::GL`. Link Windows `user32` deliberately for `SetProcessDPIAware`. Keep ImGui's included OpenGL loader; no replacement loader. Link `${CMAKE_DL_LIBS}` where the bundled loader needs it on Unix.
4. Copy `$<TARGET_FILE:SDL2::SDL2>` next to `$<TARGET_FILE:pandabank_app>` in a Windows post-build command, using `copy_if_different`. Quote paths / use `VERBATIM`. No SDL DLL copied into source.
5. On Linux, require an explicitly available compatible SDL2 development package and its CMake imported target; never link the Windows files or mix vendored Windows headers with unrelated system libraries. Prefer exact 2.32.2 to preserve the stack; absence gives a clear configure failure. No automatic FetchContent/download fallback. Record any proposed alternate version before implementation. macOS/iOS/Emscripten are not supported by this first migration.
6. Current `bin/fonts` is empty and no file font is loaded. Do not invent a font download or package a nonexistent asset.

## Incremental execution

1. Before edits, run `git status --short`, preserve unrelated changes, and checkpoint the current mission result. Run the native Debug build in `docs/BUILD_WINDOWS.md` to establish parity.
2. Add CMake 3.21+ configuration and version-3 presets with generator `Visual Studio 17 2022`, architecture `x64`, binary directory `${sourceDir}/build/vs2022`. Add `windows-debug` and `windows-release` build presets referencing a `windows-vs2022` configure preset. Test the actual installed CMake version. No CMake files have been supplied as if already verified.
3. From Developer PowerShell, after implementing the named presets:

```powershell
cmake --preset windows-vs2022
cmake --build --preset windows-debug
cmake --build --preset windows-release
```

4. Open repository root in Visual Studio's CMake workflow, or open the generated solution under `build/vs2022`. Set `pandabank_app` as startup target and launch both configurations. Record executable paths and prove DLL deployment without relying on PATH/global SDL installations.
5. Validate from a clean checkout in a path containing spaces, outside the original personal tree. Compare native and CMake startup, popup, and the completed Mission 001 behavior. If a CTest target exists by then, register and run it; currently none exists, so zero CTest tests is NOT a passing behavior test.
6. Only after this evidence, record adoption in `DECISION_LOG.md`. Decide when to retire manual native source-list maintenance in a later explicit change. Keep Visual Studio usable via CMake. Do not delete the native project as part of this mission.

## Predictions, checks and feedback

Predict which failure comes from a missing header, a missing compiled `.cpp`, a missing import library, and a missing runtime DLL. Use the first diagnostic: configure failure → package/path; compile failure → declarations; link failure → definitions/library; launch failure → runtime/context. Do not escalate warnings to errors on vendored code.

Acceptance evidence: both configurations compile/link, launch from moved path, input smoke passes, DLL is local to executable, no personal paths, no third-party diff, same source files in Visual Studio and CMake. Linux support remains unverified until separately configured/built/launched there.

Hints: first inspect the target's source list; then its transitive include/link dependencies; then runtime deployment. Rescue after two failed attempts without new evidence: fix only the build blocker, record supplied infrastructure and review debt. Transfer: explain why adding a header to Solution Explorer cannot substitute for compiling a `.cpp`.

Rollback: checkpoint first; use Git diffs and selectively revert only migration-owned files after preserving unrelated work. No `reset --hard` or folder deletion. Record build support code in `course/scaffold_manifest.json`; remove any temporary probes before adoption or assign replacement evidence.
