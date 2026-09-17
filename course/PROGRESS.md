# Course Progress

## Current position — Mission 001 reviewed, 2026-09-16

- Phase 0: stabilize prototype.
- Mission 001: **completed with guided assistance**, accepted against submitted source and Chris's runtime evidence. Review: `course/evidence/001_review.md`.
- Active mission: `002_debugging_invariants`, **not attempted**, ready for the first debugger observation.
- Windows Debug/x64 build/link and two-text-object smoke: **passed, user-reported** in `course/evidence/001.md`. Reviewer did not run Windows/GUI. Release: not reported.
- Reviewer GCC syntax check on submitted AppUISetup.cpp: passed, existing unrelated warnings remain. See `docs/validation/mission001-review.txt`.
- Chris reports commit message `Complete Mission 001 stable object identity`. ZIP omits .git and commit hash, so history verification is unavailable. Keep Chris's existing local Git repository; do not replace it with old foundation history.
- Foundation refs remain historical: uploaded-baseline / foundation-ready. They are not present in this source-only ZIP. See `docs/RECOVERY.md`.
- Temporary scaffold count: **2**, M001-SPACING and M001-LOGGING. Both already existed in the submitted repair; now tracked.
- CMake remains a prepared migration. No dashboard, persistence, new runtime features, or Mission 002 assertion code was implemented.

## Concept confidence

| Concept | State | Next evidence |
|---|---|---|
| Vector values | practiced_with_help | Observe insertion before/after in the debugger. |
| Loop references | practiced_with_help | Apply again without copying a repair in a later model mission. |
| Frame/request lifetime | practiced_with_help | Distinguish one-call assertions from across-frame observation. |
| Stable ImGui IDs | practiced_with_help | Another UI example in 002; later selection/deletion review before durable. |
| Header/source/build relationships | introduced | Focused later lesson. |
| Debugger observation / assertions | introduced / not yet practiced | Mission 002. |

## Exact next action

Open `src/UI/AppUISetup.cpp` in Visual Studio, select Debug/x64, and place a breakpoint on `sceneObjects.emplace_back(object);` inside `ObjectSpawner::spawn()`. Start with F5 and choose EditableTextBox once. Record `sceneObjects.size()` before insertion, press F10 once, then record it again in `course/evidence/002.md`. This is observation only; do not add assertions yet. The mission gives the remaining steps and saved hints.

## Resume and checkpoint

Read AGENTS.md, state.json, this progress file and Mission 002. Inspect local Git status before edits. Use existing working build instructions; no new setup is needed if Debug already runs. Keep the completed Mission 001 repair. After each attempt record observations and assistance in the evidence form. Mission 002 completion requires the learner's Debug checks, deliberate-fault demonstration and Release evidence; it is not unlocked by a syntax check alone.

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


### 2026-09-16 — Mission 001 review and Mission 002 preparation

- Chris's attempt: persistent flag reset; per-element ID/draw; actual logging call; temporary horizontal placement. Scoped to mission regions.
- Evidence: prediction and repeated breakpoint before fix; reported Debug build/link, one-click count, idle stability, independent editable text/focus after fix; correct object/ID explanation.
- Decision: accept Mission 001 complete; advance guided confidence without claiming durable mastery. Carry the missing future-UI transfer example into 002 without reopening 001.
- Reviewer validation: source diff versus foundation; GCC C++20 syntax pass; dependency/native build bytes unchanged. Commit hash unavailable because Git metadata is omitted.
- Scaffold: registered two existing diagnostic regions, added no code.
- Assistance: Chris reported guided / duration unknown / fine. This session only reviewed and prepared instructions.
- Updated course state, evidence review, current-state audit, recovery guidance and next mission. Application/third-party/build bytes preserved.
- Next: the breakpoint-and-F10 observation above, then write the Debug-only checks yourself using Mission 002.
