# Mission 002 — Catch a Broken Spawn Contract

Status: locked until Mission 001 behavior is verified. Estimate: 45–60 minutes.
Value: detect an incorrect object collection before a confusing screen hides it.
Prerequisites: one click/one object, stable stored IDs, references, Debug build.
Allowed: focused debug-only checks in `ObjectSpawner` and the request handling in `src/UI/AppUISetup.cpp`; mission evidence and scaffold records.
Read-only: dependencies, renderer, build architecture, animation, product features.

## Starting behavior and prediction

Start from Chris's verified Mission 001 commit. Re-run its two-text-object smoke check. Break in `spawn()` and watch `sceneObjects.size()`, `nextId`, and the inserted object's ID. Predict the size change for one click and several idle frames. Explain why a second click may create another object but idle frames may not.

## Work

Use Visual Studio breakpoints/Watch to observe a single request entering and leaving the creation path. Add a small Debug-only invariant check at the insertion boundary: the count changes by one and the live stored IDs remain unique. Keep it local; no new class hierarchy or test framework. Guard potentially expensive scans from Release builds. Teach the difference between an assertion reporting an internal programming error and user-facing input validation.

## Evidence

- Debug builds and the Mission 001 smoke remains working.
- One click increases count once; idle frames leave it unchanged (manual frame observation is still necessary).
- In a disposable, uncommitted test edit, deliberately duplicate an ID and demonstrate the assertion stops at the relevant boundary. Preserve unrelated edits; restore only this probe and prove normal spawning works again.
- Release builds without executing the debug-only scan; this does not prove all release behavior correct.
- Explain the failing assertion and the state you inspected. Transfer: explain how unique IDs and unique display names differ, without implementing rename/delete.

The in-app assertion is a deterministic check of collection invariants, not an automated GUI test. Do not claim it proves the request is consumed: count-per-click is a separate observation. No automated check exists for this mission yet; Chris implements it.

## Hints and rescue

1. Watch before/after one insertion before writing code.
2. State the condition that must hold as a sentence.
3. Compare the inserted ID with existing stored IDs.
4. If stuck after two attempts without new observations or about 25 minutes, ask for one partial assertion and explain its variables. Log assistance and review due; do not finish a whole debug framework.

## Checkpoint

Before editing, inspect Git status and commit only intentional Mission 001 work. When done, record results in `course/state.json` and `course/PROGRESS.md`, and create a focused Git commit. Do not use hard reset for rollback. Permanent debug checks can be reviewed production infrastructure; temporary fault injections must be removed, and any retained probe needs a scaffold ID/replacement plan. Related scaffold: none initially. Do not start this mission during the foundation run.
