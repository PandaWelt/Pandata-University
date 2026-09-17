# Current Prototype State

Latest basis: submitted `PandataBank_Mission001.zip`, reviewed against the foundation archive on 2026-09-16. Mission 001 is complete by source review plus Chris's reported Windows Debug/smoke evidence. Reviewer GCC syntax check passed; Windows/GUI was not re-run here. `reference/current-ui.png` remains historical, not a new screenshot.

The submission omits `.git`, `recovery/`, `bin/` and SDL2.pdb; it is a source-only transfer, not proof those were deleted locally. Historical baseline identifiers remain documented in RECOVERY.md; no commit hash can be verified from this ZIP.

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

## Mission 001 — repaired by Chris

- AppUISetup consumes the spawn request and clears it after the call. Idle frames no longer take that branch until another selection.
- ObjectSpawner::spawn copies one BaseObjects value into sceneObjects and calls PrintVector(sceneObjects).
- drawAll uses each stored obj both for its pushed ID and for Draw; each stored object's text array is used.
- Temporary placement offsets new objects horizontally by 120 pixels per existing object. This is diagnostic spacing, not a layout/drag system; sufficiently many objects go offscreen.
- Chris reports one-click/one-object, no idle growth, and two independently editable text buffers that survive focus changes. See course/evidence/001.md and 001_review.md. Source inspection supports these results; live Windows behavior was not independently rerun.
- M001-SPACING and M001-LOGGING track retained temporary positioning and console diagnostics. No assertion checks have been implemented yet; those belong to Mission 002.
- Current integer ID cast is sufficient for the small mission test; narrowing large 64-bit IDs and broader identity requirements remain deferred, not certified solved for all scales.

The old runaway-request/template-rendering diagnosis is historical and explained in Mission 001's assignment/review. Do not reintroduce or redo the repair.

## Additional code findings

- PrintVector is now called. It prints the whole collection on each insertion, is not Debug-only, and has rough formatting; track as diagnostic scaffolding rather than permanent logging.
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
- The foundation had empty bin/fonts directories; this ZIP omits them. No current file font is loaded. No runtime font asset is required.
- Resource input `Pandata.bank.rc` is UTF-16, includes `winres.h`, and contains no active icon/dialog/accelerator resource definitions. It remains unchanged and Windows-resource compilation remains untested.
- `AppUISetup.cpp` lacked a direct `<cstdint>` include: GCC 13.3 rejected `std::uint64_t`. The foundation added that include. Chris subsequently repaired the Mission 001 regions; this review changes no application source.
- GCC syntax checks now pass for all four application `.cpp` files, with existing warnings in `AppUISetup.cpp`. Syntax success is not a link, build, behavior test, or proof of MSVC compatibility.
- This Linux host has no `cl`, `msbuild`, Windows resource compiler/SDK, `cmake`, `ninja`, `pkg-config`, or MinGW compiler on PATH. `/usr/include/SDL2/SDL.h` and `/usr/include/GL/gl.h` are absent. The bundled headers let `main.cpp` pass syntax checking; the bundled Windows `.lib`/DLL files do not provide a Linux link path.
- The reviewer has not run a native Windows build/link/GUI test. Chris reports Debug/x64 build/link and Mission 001 smoke passed; Release remains unreported. No CMake implementation was added. See `docs/BUILD_WINDOWS.md` and `course/migrations/portable_cmake.md`.
- Dependency identifiers: ImGui header reports `1.92.8 WIP`; SDL headers report `2.32.2`. Exact ImGui commit and SDL binary provenance remain unverified. See `THIRD_PARTY_STATUS.md`.

The original ignore rule `x64/` hid the supplied SDL directory. The revised ignore file retains precisely the supplied DLL and three import/static library files while ignoring generated outputs, PDBs and caches. No original file was deleted. The foundation retained PDB/recovery files outside Git source history; they are not included in this later submission.

## Learning exposure and next evidence

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

Mission 001 provides guided practice with values, frame state, references and IDs. Next is repeatable debugger observation and assertions; ownership, file boundaries and build portability remain later topics. Do not label guided practice durable mastery.

## Baseline rule

Preserve this version before restructuring. The first migrations should be small, test-backed, and reversible. Do not use the product vision as justification for replacing the prototype wholesale.
