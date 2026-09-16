# Course Progress

## Current position — 2026-09-16

- Phase 0: preserve and stabilize prototype.
- Active mission: `001_stable_object_identity`, **not attempted**; prediction ready, Windows validation pending.
- Baseline: `uploaded-baseline` = `3b9a6b7fac5dd9f53c0876dd90922197e39fc910`.
- Foundation checkpoint: `foundation-ready`. Recovery: `docs/RECOVERY.md`.
- Bookkeeping: `course/state.json` is the compact state record; this file records human context. Update both at a session checkpoint; resolve disagreements from actual evidence, never infer mastery.
- Build: GCC application syntax passes with existing warnings; native project path checks pass. Windows Debug/Release, link and GUI smoke have NOT run. CMake is a prepared migration only.
- Temporary scaffolds: 0. Two supplied infrastructure edits await learner/platform review; two dependency inventories have unresolved release evidence.
- Release is not ready: known mission bug, unverified native build/runtime, license/provenance gaps, no behavior test suite or persistence. Missing future features are not requirements to implement now.

## Observed versus reported

Previously reported/screenshot: Windows window, canvas, boxes/text, popup. Source supports those paths. No application was launched here. The retained defects and detailed audit are in `docs/CURRENT_STATE.md`.

## Concept confidence

| Concept | State | Next evidence |
|---|---|---|
| Vector values | introduced | Explain growth versus overlap. |
| Loop references | introduced | Trace current stored element through drawing. |
| Frame/request lifetime | introduced | One click, one insertion across idle frames. |
| Stable ImGui IDs | practiced_with_help | Independent text/focus, then later transfer. |
| Header/source/build relationship | introduced | Review direct cstdint include; focused later lesson. |

Foundation work does not advance Chris's concept confidence. Do not mark stable IDs durable from one repair.

## Resume / offline use

1. Read `AGENTS.md`, this file, decisions, `course/state.json` and active mission.
2. Inspect `git status --short`; preserve unrelated work.
3. Chris writes prediction in `course/evidence/001.md`; then build per `docs/BUILD_WINDOWS.md` and work on the allowed regions.
4. Use mission's saved hints offline. Record attempts and observed results. Build failures and behavior failures are separate.
5. Review the diff and manual evidence; update state/progress/scaffold entries, then commit only intended files.
6. Unlock `002_debugging_invariants` only after Mission 001 passes. Full CMake work stays in `course/migrations/portable_cmake.md`; do not treat it as the next feature or build a dashboard.

## Session log

### 2026-09-16 — Foundation pass (audit, narrow implementation, teaching preparation)

- Chris attempts: none in this run; elapsed learner time unknown.
- Read all supplied docs/course files; audited complete inventory, source, build configuration and bundled dependency identifiers.
- Preserved baseline Git tag and byte-exact original ZIP; refined ignores without deleting files.
- Added only direct `<cstdint>` application include; replaced 14 native project personal-path prefixes. Mission 001 target logic untouched; third-party files unchanged.
- Validation: before-fix GCC rejected std::uint64_t; after-fix syntax checks pass. Native XML/path checks pass. Stored evidence under `docs/validation/`. No Windows build or runtime pass claimed.
- Assistance: foundation infrastructure only; no user mission solution or patch. Concept confidence unchanged; review still due.
- CMake not implemented: no CMake/MSVC/Windows SDK available; prepared migration with explicit gates.
- Next exact action: write Mission 001's prediction tracing the click through `SpawnButton`, `AppUISetup`, `spawn`, and `drawAll` before changing code.

Append future entries with date, mission/time, attempted changes, evidence, assistance level, confidence changes, scaffold changes and next action.
