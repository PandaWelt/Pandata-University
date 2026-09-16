# Chronological Roadmap

This roadmap is deliberately staged. Later phases are direction, not permission to scaffold every system now.

## Phase 0 — Preserve and stabilize the prototype

Learning focus: frame loops, persistent state, object copies, vector elements, stable ImGui identity, debugging observations.

Product outcomes:

- baseline checkpoint;
- reproducible build instructions;
- Mission 001 repairs spawning/input;
- multiple independently editable objects;
- basic object inspection/debug display;
- obvious runtime assertions for duplicate IDs or runaway spawn requests.

Exit evidence:

- user can explain why the vector looked like it replaced an entry;
- one click creates exactly one object;
- each object retains independent text and identity;
- build/smoke evidence is recorded.

## Phase 1 — Portable build and test foundation

Learning focus: compiler versus linker, headers versus `.cpp`, libraries/executables, include paths, Debug/Release, tests.

Product outcomes:

- relative/portable build configuration;
- CMake presets for Windows Debug and Release;
- Visual Studio remains usable;
- initial test target;
- third-party dependency/license inventory;
- generated files excluded from source control.

Exit evidence:

- clean checkout/configure/build/test from a non-personal path;
- no `D:\CREATIVE...` dependency;
- user adds one header/source pair and connects it to a target.

## Phase 2 — Object model separated from rendering

Learning focus: responsibility, value types, ownership, IDs, interfaces only where useful.

Product outcomes:

- persistent component/object data independent from ImGui calls;
- object collection with create/select/edit/delete behaviors;
- rendering reads model data;
- minimal undoable or recoverable edits considered before destructive UI tooling.

Exit evidence:

- core tests run without creating a window;
- UI does not own object identity;
- user can trace one object from model through renderer.

## Phase 3 — Project model and safe local persistence

Learning focus: serialization, database schema, transactions, errors, migrations, resource lifetime.

Product outcomes:

- create/open project;
- self-contained project metadata location;
- atomic save/reopen behavior;
- schema versioning and backup/recovery path;
- visual project-card home skeleton.

Exit evidence:

- restart preserves a test project;
- interrupted/invalid save does not silently destroy the last valid state;
- moved project folder reopens through relative data.

## Phase 4 — Naming tree vertical slice

Learning focus: trees, parent/child filtering, validation, deterministic serial allocation.

Product outcomes:

- configurable naming fields/order;
- child-only option visibility;
- separate new-entry control;
- temporary versus permanent entries;
- path-scoped serials;
- register and search generated names.

Exit evidence:

- unit tests cover representative trees and serial paths;
- invalid combinations cannot silently register;
- user can explain traversal and persistence flow.

## Phase 5 — Asset registry vertical slice

Learning focus: relational modeling, optional fields, tags, many-to-many relationships, search.

Product outcomes:

- owner categories and tags;
- planned assets without files;
- category default fields plus per-asset overrides;
- related-asset links;
- expandable list/card view;
- one useful search workflow.

Exit evidence:

- a character/boss and a road mesh can both be represented without hard-coded special cases;
- related animations retain their own owner category;
- search finds assets by name, tag, category, and useful field data.

## Phase 6 — Managed file workflow

Learning focus: `std::filesystem`, failure handling, transactions, platform adapters, external process launching.

Product outcomes:

- link/create/open/reveal/rename/move/unlink;
- explicit delete safeguards;
- project-relative paths;
- create file from an asset record into configured category structure;
- reconciliation queue for external moves/renames.

Exit evidence:

- integration tests use temporary directories;
- failures leave registry and disk in a explainable/recoverable state;
- folders move with their contents normally.

## Phase 7 — Workspace tabs, pages, and templates

Learning focus: composition, layout data, commands, state transitions.

Product outcomes:

- tabs and pages persisted per project;
- workspace templates configure tabs/pages/layouts only;
- deliberate Layout/Work mode switch;
- component binding to approved built-in commands;
- responsive container foundation.

## Phase 8 — Course dashboard v1

This phase may begin with small tooling spikes earlier, but full dashboard investment waits until build/test commands and mission format are stable.

Product outcomes:

- relevant-file editor;
- terminal/check output;
- offline missions/hints;
- adaptive Rescue Mode integration;
- Open in Visual Studio;
- progress/scaffold visibility.

An isolated web implementation is allowed only after the provisional decision in `DECISION_LOG.md` is approved. Do not make Chris learn its internal language as part of the C++ curriculum.

## Phase 9 — Layout and compact vector/whiteboard foundation

Learning focus: geometry, transforms, selection, command history, serialization, rendering independence.

Order:

1. shapes, selection, transforms;
2. paths and control points;
3. text/style properties;
4. grouping and boolean/shape operations;
5. component binding;
6. hover/pressed/show/hide transitions;
7. advanced animation only after the model is stable.

## Phase 10 — Packaging, polish, and portfolio readiness

- clean Release build;
- installer or portable package;
- license notices;
- crash/data recovery behavior;
- sample project and screenshots;
- user-facing documentation;
- performance profiling and accessibility pass.

## Deferred expansion

- background owner-PC service;
- authenticated remote phone access;
- proposed external edits and owner approval;
- offline phone edits/conflict resolution;
- iPhone client;
- advanced shape morphing/path animation.
