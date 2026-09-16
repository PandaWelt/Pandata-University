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
