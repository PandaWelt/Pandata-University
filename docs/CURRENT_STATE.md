# Current Prototype State

Audit basis: uploaded `Pandata.Bank.zip` and the current UI screenshot supplied on 2026-09-15. This document distinguishes observed code from future intent.

## What exists

- A Visual Studio C++20 application.
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

## Confirmed spawned text-input defect

The same unstable/duplicate identity chain prevents reliable text focus:

- the latest spawner object receives a new ID every frame because spawning never stops;
- every rendered copy uses that one ID and one object's text buffer;
- ImGui cannot preserve a stable active widget across frames;
- the input loses focus or appears non-interactive.

Mission 001 intentionally leaves these target lines unfixed so Chris can diagnose and repair them with guided support.

## Additional code findings

- `PrintVector;` references a function without calling it. A real call needs arguments, such as `PrintVector(sceneObjects);`.
- `#undef X(name)` should be `#undef X`.
- `DefinedText` and `DefinedTextBox` currently call editable `InputTextMultiline`; their implementation does not yet match the name.
- `Animation::startTime` is not initialized before `getValue()` can calculate progress.
- `Animation::interpolate()` declares enum cases that are not all handled and has no guaranteed fallback return.
- Animation completion does not currently clear `isRunning`.
- `char font;` is uninitialized and does not represent a usable ImGui font handle.
- `AppUISetup.cpp` combines domain data, rendering, spawning, animation, popup state, and application composition. This is acceptable evidence of exploration but not a scalable boundary.
- Most classes exist only inside one `.cpp`, preventing focused unit tests without refactoring.

These are backlog evidence, not authorization for Astra to repair everything in one pass.

## Build portability findings

The Visual Studio project targets toolset `v143` and C++20. Debug and Release x64 configurations contain absolute include/library paths rooted at:

```text
D:\CREATIVE\ART\PROJECTS\Pandata.Bank\...
```

This ties the project to one directory on one computer. Win32 configurations do not contain equivalent SDL/ImGui setup and are not known to work. The repository bundles SDL2 and ImGui source/binaries, but does not contain CMake or dependency metadata.

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
