# Mission 001 — One Click, One Stable Object

Estimated working time: 45–90 minutes  
Product value: object spawning becomes usable and editable text retains focus.  
Primary file: `src/UI/AppUISetup.cpp`

## Starting behavior

After choosing an object type once:

- the vector continues growing every frame;
- rendered objects appear to replace or overlap each other;
- spawned text widgets cannot reliably retain focus or accept input.

## Concepts

- an event/request versus persistent state;
- a copied object stored in a vector;
- the range-loop variable as the current stored element;
- stable unique ImGui identity across frames;
- model state versus what is rendered;
- visual overlap versus data replacement.

## Prediction before editing

Trace one click through these locations and write a short prediction:

1. the object-type button in `PopupEditor`;
2. the `popup.spawnRequested` check in `AppUISetup`;
3. `ObjectSpawner::spawn()`;
4. `ObjectSpawner::drawAll()`.

Questions:

- How many frames will the request remain true?
- Which variable is copied into `sceneObjects`?
- Which variable is actually drawn inside the loop?
- Which ID is pushed for each iteration?
- If several correct objects have identical positions, what will the screen look like?

## Completion criteria

- One menu selection creates exactly one vector element.
- No additional element appears on later frames without another selection.
- `drawAll()` draws each stored vector element.
- Every interactive object receives a stable unique ImGui ID.
- Two spawned text objects retain independent text buffers.
- Temporary placement makes multiple objects visibly distinguishable, or an inspector/log proves distinct state until drag/placement is taught.
- Vector logging is an actual function call rather than an unused function reference.
- Debug and manual smoke checks are recorded in `course/PROGRESS.md`.

## Allowed edits

- the spawn-request consumption block;
- `ObjectSpawner::spawn()`;
- `ObjectSpawner::drawAll()`;
- temporary debug logging or temporary position staggering clearly marked for later removal.

Do not refactor the whole file, introduce inheritance, implement drag/drop, add persistence, or build a generic event system during this mission.

## Hint ladder

### Hint 1 — Observe rate

Log vector size once per spawn. Compare how fast it grows with the application's frame rate.

### Hint 2 — Request lifetime

The popup creates a request. Find the place that consumes it. What should happen to the flag immediately after one request is handled?

### Hint 3 — Loop identity

Read the loop aloud: “for each `obj` in `sceneObjects`...” Then compare that sentence with the variable used by `PushID` and `Draw`.

### Hint 4 — Visual overlap

Correct storage does not automatically create different coordinates. Temporarily offset a newly created object's position using the count before insertion, or prove the elements differ through an inspector/log.

### Hint 5 — Partial form

The drawing loop should operate only on its current loop element. The request-handling block should consume and clear a successful request in the same logical action.

### Rescue threshold

Escalate to a partial patch if Chris has made two attempts without a new observation, spends more than roughly 25 minutes on an ImGui-specific identity detail, or reports being fried. Do not take over merely because the first build fails; interpret the failure first.

## Validation

Until an automated UI harness exists:

1. add observable vector-size logging or a small debug panel;
2. choose one object and wait several seconds;
3. verify size increased by exactly one;
4. create two editable text objects;
5. make them visibly distinct;
6. enter different text in each;
7. click away and return; verify both values persist.

## Transfer task

Explain why unique object names alone would not fix the bug if the code still pushed the wrong ImGui ID. Then identify one other place in a future UI where stable identity will matter.

## Mastery record

Do not mark stable identity durable after this mission alone. Schedule a later mission involving selection or deletion where the concept must be applied again without copying this repair.

## Foundation handoff — 2026-09-16 (no solution patch)

Status: ready for prediction; not attempted. Windows build/smoke pending.
Build instructions: `docs/BUILD_WINDOWS.md`. Evidence form: `course/evidence/001.md`.

Read-only during this mission: `thirdparty/`, `src/main.cpp`, animation/widget implementations, and build files unless a separately identified build blocker requires help. Only the regions listed above are the learning target. The foundation pass added `<cstdint>` at the top of the file, so line numbers shifted; use the function names below as anchors.

Concrete map:

| Code location | What to observe |
|---|---|
| `PopupEditor::SpawnButton()` | A clicked type records a request; this is not itself vector insertion. |
| `AppUISetup()` | Called every frame from `main.cpp`; its static popup and spawner survive calls. |
| `ObjectSpawner::spawn()` | `emplace_back(object)` stores a value copy, including its fixed text buffer. |
| `ObjectSpawner::drawAll()` | A reference in the range loop denotes one stored element. Compare the loop's variable with each call's actual argument. |

`PushID` adds an identity scope and `PopID` removes it. A repeated hidden label such as `##object` can be valid under different stable scopes. The numeric cast chooses the integer ID overload; it does not create a new object or make an unstable identity stable. The current 64-bit-to-int narrowing is a future large-ID concern, not a request for a new ID system now. A display name and a widget identity have different jobs.

Before editing, write your prediction in `course/evidence/001.md`, then run the Debug build. A compiler/tooling failure is recorded as blocked validation, not a failed understanding check. The foundation syntax checks cannot pass this mission for you.

For input testing, dismiss the selection popup before clicking the canvas. Distinguish the pre-existing `box2` demo at (200,200) from objects spawned at (100,100). Use the existing logging route or Visual Studio Watch to distinguish overlap from missing storage. Keep any temporary staggering/logging inside the permitted regions and register retained temporary code in the scaffold manifest. Do not modify animation to make the test look cleaner.

Checkpoint before edits: inspect `git status --short`, then commit only intended files if there is existing work. For rollback, save the attempted diff and selectively restore only your own edits after reviewing it; never hard-reset the repository. After success, record prediction, build/smoke results, explanation, transfer, assistance level and commit ID. Update `course/state.json` and `course/PROGRESS.md` together. Mission 002 unlocks only when behavior has been observed; mastering the concept remains a separate later review.
