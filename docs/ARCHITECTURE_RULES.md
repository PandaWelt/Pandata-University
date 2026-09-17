Current extension: University is a separate C++20 executable in university/, with course definitions and progress separate from source. No University source, tests, references or auth may link into PandaBank. The diagrams/targets below are future architecture, not an inventory. U1 uses direct compile commands; CMake is still a planned migration. See UNIVERSITY_ADRS.md and PROTOTYPE_ASSESSMENT.md.

# Architecture and Build Rules

This describes the intended direction, not an order to perform an immediate rewrite.

## One repository, two interaction surfaces

```text
Course dashboard/editor ─┐
                         ├── edits the same C++ source and project files
Visual Studio 2022 ──────┘
                                  │
                                  ▼
                         CMake configure/build/test
                                  │
                    ┌─────────────┴─────────────┐
                    ▼                           ▼
              Debug/course build          Release package
```

The course dashboard is an alternate learning interface, not a fork of PandaBank. Visual Studio remains available for advanced debugging, profiling, project inspection, and release polish.

## Target source boundaries

Evolve toward this structure only as real features require it:

```text
src/
  app/                 composition and program startup
  core/
    assets/            asset identity, categories, fields, relations
    naming/            naming tree, path rules, serial allocation
    projects/          project metadata and workspace definitions
  storage/             database repositories and migrations
  filesystem/          managed-file operations and reconciliation
  ui/                  ImGui views/controllers and layout runtime
  platform/            SDL/OS-specific services
tests/
course/
docs/
thirdparty/
```

Do not create every directory before it has code. Empty architecture is camouflage, not progress.

## Target build graph

As modules become real, prefer focused CMake targets:

- `pandabank_core`: platform-independent domain behavior.
- `pandabank_storage`: local persistence implementation.
- `pandabank_filesystem`: managed file operations/platform adapters.
- `pandabank_ui`: Dear ImGui views and layout runtime.
- `pandabank_app`: executable and dependency composition.
- `pandabank_tests`: automated behavior tests.

Use `target_include_directories`, `target_link_libraries`, and target-scoped compile options rather than global path soup. Keep Debug and Release presets predictable. Do not manually maintain separate source lists in several build systems indefinitely.

## Header/source teaching contract

PandaBank missions should explicitly teach how multiple files cooperate:

```text
Asset.h                       declares the Asset type
Asset.cpp                     defines Asset behavior
AssetRepository.h             declares a storage contract
SqliteAssetRepository.cpp     implements that contract
AssetBrowserView.cpp          renders UI using application/core services
```

The learner should practice this cycle:

1. decide the responsibility;
2. declare the usable interface in a header;
3. define behavior in a `.cpp`;
4. include only the declarations needed;
5. add the implementation to the correct CMake target;
6. link targets at explicit boundaries;
7. build, interpret compiler versus linker failures, and run tests.

Do not call C++ translation units “scripts” in course material without clarifying the distinction.

## Dependency direction

Allowed direction:

```text
app → ui/application services → core contracts
app → storage/filesystem implementations → core contracts
platform/ui/storage → approved external libraries
```

Forbidden direction examples:

- core asset objects including `imgui.h`;
- UI views issuing raw SQL;
- database code deciding screen layout;
- naming rules calling Windows APIs;
- tests depending on personal absolute paths.

## Identity and data model

- Use stable opaque IDs for projects, assets, categories, files, relations, pages, and UI components.
- A display name is not identity.
- Renaming a file or asset must not break relationships.
- Store normalized project-relative paths where possible; resolve absolute paths at runtime.
- Treat file operations as transactions with validation, result reporting, and recoverable failure where practical.
- Before destructive file operations, resolve exact targets and distinguish unlinking from deleting.

## Persistence direction

SQLite is the leading candidate for structured local data, accompanied by human-readable project configuration for information that benefits from manual inspection. Before adoption, write an architecture decision covering:

- schema migrations;
- backups and recovery;
- transactions;
- foreign keys and relationship integrity;
- relative paths;
- audit/event history;
- database location inside the project;
- behavior when the project folder moves;
- commercial-license notice requirements.

Do not store arbitrary external files inside the database by default. Keep links and metadata unless a feature has a clear reason to embed content.

## File reconciliation

Filesystem monitoring must be separated from registry decisions:

1. watcher reports a possible create/move/rename/delete;
2. reconciliation compares stable metadata and known paths;
3. unambiguous changes update the registry;
4. ambiguous or destructive changes enter a review queue;
5. UI explains the proposed correction.

This prevents “automatic update” from becoming silent data corruption.

## UI and layout direction

- Immediate-mode rendering does not excuse mixing persistent model state with per-frame request flags.
- Every interactive widget requires stable identity across frames.
- Layout/component data is persistent project data; ImGui calls are a renderer for that data.
- Responsive layouts require container rules and constraints, not only absolute positions.
- Work Mode must not mutate layout accidentally.
- Whiteboard/vector geometry belongs in a renderer-independent model before advanced tools are added.

## Testing layers

1. **Core unit tests:** naming paths, serial allocation, asset relations, field schemas.
2. **Storage tests:** temporary project folders/databases, migrations, rollback, reopen.
3. **Filesystem integration tests:** create/link/rename/move/unlink with explicit temporary directories.
4. **UI behavior tests where feasible:** controller/state logic separated from raw drawing.
5. **Manual smoke tests:** window startup, input, rendering, external program launch.

The course dashboard must run deterministic checks without AI. AI explains results; it is not the source of pass/fail truth.

## Release gate

A distributable Release build must fail or remain blocked when:

- any `TEMPORARY_SCAFFOLD` entry remains in an unresolved release-blocking state;
- course-only mocks/stubs are linked into the app;
- temporary feature flags are enabled;
- tests/build checks fail;
- required third-party license notices are absent;
- the package depends on personal absolute paths;
- runtime data can only be found through the developer's machine layout.

Course documents, tests, and reference solutions may remain in the repository, but should not be packaged as runtime dependencies.
