# Current Prototype State

Audit basis: all 139 files in `PandataBank_Astra_Ready.zip`, inspected on 2026-09-16. `reference/current-ui.png` is historical user evidence, not a run performed here. Baseline: Git tag `uploaded-baseline`; exact original archive retained in `recovery/original-upload.zip`. This document distinguishes source inspection, compiler checks, and unverified runtime behavior.

## What exists

- A prototype configured as a Visual Studio C++20 application (not built on Windows in this audit).
- SDL2 window/event handling.
- OpenGL 3 rendering backend.
- Dear ImGui integration.
- A full-window canvas plus a separate popup/tool window.
- Prototype object types generated from an X-macro list:
  - `BasicBox`
  - `EditableTextBox`
  - `EditableText`
  - `DefinedTextBox`
  - `DefinedText`
- `BaseObjects`, containing type, ID, name, rectangle properties, colors, padding, and a fixed 512-byte text buffer.
- Drawing functions for boxes and text widgets.
- A prototype `Animation` class with several easing options.
- An `ObjectSpawner` with a `std::vector<BaseObjects>`.
- A popup menu that requests object creation.

The screenshot shows a working application window, canvas, colored shapes/text, and an object-selection popup.

## Source layout

```text
src/
  main.cpp
  Inputs/
    InputConfig.cpp          (empty)
    inputsConfig.h           (empty)
  UI/
    AppUIConfig.cpp          (placeholder only)
    AppUIConfig.h            (pragma once only)
    AppUISetup.cpp           (nearly all prototype behavior)
    AppUISetup.h

visualstudio/
  Pandata.bank.slnx
  Pandata.bank.vcxproj
  resources...

thirdparty/
  imgui/
  SDL2/
```

There is no observed database, persistence layer, asset model, naming tree, automated test suite, CMake configuration, course dashboard, or file registry yet.

## Confirmed object-spawning defect

The vector does not replace element zero. Three interacting defects make it look that way:

1. `PopupEditor::spawnRequested` is set to `true` but never consumed/reset. After one click, `spawn()` runs every frame and the vector grows continuously.
2. `ObjectSpawner::drawAll()` iterates with `obj` but renders the member variable `object` instead. Every vector element therefore redraws the same latest template object.
3. `drawAll()` pushes `object.id`, not `obj.id`, producing duplicate ImGui identities for the repeated widgets.

Every spawned object also begins at the same default position, so correctly stored objects would still overlap until placement or temporary staggering is introduced.

## Source-supported spawned text-input diagnosis

The source contains an identity/buffer chain consistent with the reported focus failure; runtime confirmation remains pending:

- the latest spawner object receives a new ID every frame because spawning never stops;
- every rendered copy uses that one ID and one object's text buffer;
- ImGui cannot preserve a stable active widget across frames;
- the input loses focus or appears non-interactive.

Mission 001 intentionally leaves these target lines unfixed so Chris can diagnose and repair them with guided support.

## Additional code findings

- `PrintVector;` references a function without calling it. A real call needs arguments, such as `PrintVector(sceneObjects);`.
- `#undef X(name)` should be `#undef X`.
- `DefinedText` and `DefinedTextBox` currently call editable `InputTextMultiline`; their implementation does not yet match the name.
- `Animation` does not explicitly initialize `startTime`, and `interpValue()` calculates progress before checking `isRunning`. The current `R` and `G` instances have static storage and receive zero-initialization; an automatic instance would not have that protection. Do not claim the current static instances read an indeterminate start time.
- `Animation::interpolate()` declares enum cases that are not all handled and has no guaranteed fallback return.
- Animation completion does not currently clear `isRunning`.
- `char font;` is uninitialized and does not represent a usable ImGui font handle.
- `AppUISetup.cpp` combines domain data, rendering, spawning, animation, popup state, and application composition. This is acceptable evidence of exploration but not a scalable boundary.
- Most classes exist only inside one `.cpp`, preventing focused unit tests without refactoring.

These are backlog evidence, not authorization for Astra to repair everything in one pass.

## Build portability findings

The Visual Studio project targets toolset `v143`, Windows SDK `10.0`, and C++20. The upload had 14 personal path prefixes across the Debug/Release x64 include/library settings. Foundation work replaced only those prefixes with `$(ProjectDir)..\`; the source lists, compiler settings, renderer and libraries are unchanged. All 30 explicit file/path references resolve in static XML checks.

- Win32 has no equivalent dependency settings and only x64 SDL libraries are bundled. Use x64; Win32 is unsupported in this handoff.
- The project links `SDL2.lib` and `opengl32.lib`; it does not link `SDL2main.lib`. `main.cpp` defines `SDL_MAIN_HANDLED` and supplies ordinary `main`.
- No post-build rule deploys `SDL2.dll`. The Windows instructions explicitly copy the supplied DLL beside the executable.
- `.slnx` availability depends on the installed Visual Studio version. Open the `.vcxproj` directly if the IDE cannot open the solution format; compatibility has not been tested here.
- `bin/` and `bin/fonts/` are empty. No external font is loaded by the current app. Git does not retain empty directories; no runtime asset was omitted.
- Resource input `Pandata.bank.rc` is UTF-16, includes `winres.h`, and contains no active icon/dialog/accelerator resource definitions. It remains unchanged and Windows-resource compilation remains untested.
- `AppUISetup.cpp` lacked a direct `<cstdint>` include: GCC 13.3 rejected `std::uint64_t`. Adding that include is the sole application-source edit. All Mission 001 target code remains unchanged.
- GCC syntax checks now pass for all four application `.cpp` files, with existing warnings in `AppUISetup.cpp`. Syntax success is not a link, build, behavior test, or proof of MSVC compatibility.
- This Linux host has no `cl`, `msbuild`, Windows resource compiler/SDK, `cmake`, `ninja`, `pkg-config`, or MinGW compiler on PATH. `/usr/include/SDL2/SDL.h` and `/usr/include/GL/gl.h` are absent. The bundled headers let `main.cpp` pass syntax checking; the bundled Windows `.lib`/DLL files do not provide a Linux link path.
- No native Windows build, executable link, or GUI smoke test was run. No CMake implementation was added because it cannot be verified here. See `docs/BUILD_WINDOWS.md` and `course/migrations/portable_cmake.md`.
- Dependency identifiers: ImGui header reports `1.92.8 WIP`; SDL headers report `2.32.2`. Exact ImGui commit and SDL binary provenance remain unverified. See `THIRD_PARTY_STATUS.md`.

The original ignore rule `x64/` hid the supplied SDL directory. The revised ignore file retains precisely the supplied DLL and three import/static library files while ignoring generated outputs, PDBs and caches. No original file was deleted. The supplied PDB remains on disk and in the recovery archive, not Git source history.

## Current learning level evidenced by the code

The user has working exposure to:

- classes and member data;
- enums and switch dispatch;
- functions and headers;
- `std::vector` and `emplace_back`;
- references in range-based loops;
- ImGui ID scopes at an early/uncertain level;
- macros/X-macros through experimentation;
- SDL/ImGui initialization and Visual Studio linking;
- basic animation interpolation.

The immediate gaps are not “syntax from zero.” They are object identity versus copies, persistent frame state, ownership, separation of responsibilities, declarations/definitions across files, build portability, and debugging state over time.

## Baseline rule

Preserve this version before restructuring. The first migrations should be small, test-backed, and reversible. Do not use the product vision as justification for replacing the prototype wholesale.
