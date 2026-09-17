# Mission 002 — Catch a Broken Spawn Contract

**Ready. Not attempted.** Mission 001 is complete. Estimate: 45–60 minutes.
Goal: make incorrect insertion/count or duplicate IDs stop in the debugger near the mistake.
An invariant is a condition that should always hold here, such as “all stored IDs differ.” An assertion checks such a condition; it is for programming mistakes, not a substitute for handling normal user errors.

## Start here — no code edits yet

1. Open your existing project in Visual Studio and select **Debug / x64**.
2. In `src/UI/AppUISetup.cpp`, find `ObjectSpawner::spawn()`. Click on `sceneObjects.emplace_back(object);` and press **F9** to set a breakpoint. Remove/disable old breakpoints that would interrupt this observation first.
3. Before running, predict the collection size immediately before and after this line. Write the prediction in `course/evidence/002.md`.
4. Press **F5**, open Spawn Object, and select EditableTextBox once.
5. The app pauses **before** that line executes. Its window will stop responding while paused; that is expected. Return to Visual Studio.
6. Open **Debug > Windows > Watch > Watch 1**. Add `sceneObjects.size()`, `object.id`, and `nextId`. Record their values. If size() cannot be evaluated, expand `sceneObjects` in Locals and read its size through the vector visualizer. `object.id` belongs to the object about to be copied; `nextId` is the next ID available for a later creation.
7. Press **F10 once** to execute the insertion without stepping into the vector's internals. Record the new size. The next source line is `PrintVector(sceneObjects);`.
8. Press **F5** to resume. Without selecting another object, does the insertion breakpoint fire again? Record the result. Press **Shift+F5** when done.

Stop after this first observation if you want feedback. Send the before/after sizes and the ID values; do not guess at an assertion implementation while unsure what those values mean.

## Then write the checks yourself

Allowed edits: focused diagnostic checks inside ObjectSpawner::spawn(), a standard assertion header if needed, Debug-only handling of existing logging, and course records. Do not refactor the renderer, change Mission 001's working request/draw repair, change ID allocation for normal operation, or add dependencies/classes/UI panels. All other source, thirdparty and build files are read-only unless a distinct build blocker is agreed.

- Capture enough information to check that a single call inserts exactly one stored element.
- Check that no two live stored objects have the same ID. Choose a simple comparison you can explain; this tiny prototype needs no elaborate container architecture.
- Keep diagnostic captures/scans and assertions Debug-only. Standard assert is disabled when NDEBUG is defined; a loop around assert does not automatically disappear. Ensure Release does not execute the diagnostic scan or the existing full-vector console dump.
- Preserve temporary placement through this mission so text objects remain visible. M001-SPACING remains tracked. Resolve M001-LOGGING by guarding/removing the Release dump, then record whether the remaining Debug helper is accepted or temporary.

No implementation is supplied here. These are the conditions your code must test.

## Prove the checks catch a mistake

Build Debug and repeat one normal insertion. Then make a small **temporary, uncommitted** fault that gives a second inserted object a duplicate stored ID. Predict which check should fire. Run it and record the assertion location and conflicting values. Stop the app and remove only your fault-injection edit. Rebuild and prove normal creation works again. Do not commit or leave the duplicate-ID fault in the app.

A count-per-call check alone cannot catch two separate calls caused by one click. Retain the idle-frame/breakpoint observation separately. Never claim the assertions are an automated UI test.

## Completion evidence

Use `course/evidence/002.md`:

- Prediction plus debugger before/after observations.
- Normal Debug insertion passes; waiting does not insert more objects.
- Deliberate duplicate triggers the intended check; fault is removed and normal run passes again.
- Two spawned text objects still retain separate text after changing focus (regression smoke, not a repeat of the Mission 001 lesson).
- Release compiles and launches; source review confirms NDEBUG excludes diagnostic scans and logging. Use `docs/BUILD_WINDOWS.md` if you need the command.
- Explain which condition failed and how the debugger showed it.
- Transfer: name one other future UI action that needs stable identity, and explain why a per-call count check cannot establish one-call-per-click.

## Hints and feedback

1. State each required condition in plain English before writing C++.
2. Compare the collection count immediately before and after insertion.
3. For identity, ask which pairs of stored objects need comparison; do not compare a stored ID with itself.
4. Inspect the preprocessor condition used for Debug/Release before assuming assert removes all surrounding work.

After two attempts without new observations, about 25 minutes stuck on one detail, or clear fatigue, request a partial hint with the first error/Watch values and your attempted diff. Rescue assistance must be recorded, not silently credited as independent mastery.

## Save progress safely

Your Mission 001 commit already exists according to your report; do not recommit or redo it. Run `git status --short` to identify pending review-document updates and unrelated edits. Checkpoint only intended changes before experimental code. Use selective edits/diffs for rollback; no hard reset. Commit the finished Mission 002 changes only after removing fault injection, and record the resulting commit hash. Keep its status not_attempted/in_progress until the actual evidence exists.

Related scaffolds: M001-SPACING (retain); M001-LOGGING (review/guard/remove). Any extra temporary diagnostic code retained needs an exact manifest location and removal/review condition. The next chronological topic after this is declarations/definitions across files; do not implement that lesson now.
