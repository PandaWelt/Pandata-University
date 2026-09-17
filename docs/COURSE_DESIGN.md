Current authority: PANDATA_UNIVERSITY_SPEC.md and UNIVERSITY_ADRS.md refine this pilot design. Missions 001/002 are accepted historical evidence. The next experience is the separate offline University console; a full dashboard is not implemented.

# Pandata University Learning System

## Purpose

The course is a private accelerator that teaches C++ by advancing the real PandaBank repository. It must reduce burnout without converting the user into a spectator.

The learner likes trial-by-fire experimentation but burns out when every obstacle becomes an unbounded research side quest. The system should preserve discovery while controlling its duration and scope.

## Practical game loop

1. **Resume:** dashboard reads repository state, active mission, last results, and unresolved scaffold.
2. **Brief:** one feature/fix, why PandaBank needs it, completion evidence, estimated 45–90 minutes.
3. **Predict:** learner explains what selected code will do or predicts the failure before running.
4. **Work:** edit the real C++ files in the dashboard or Visual Studio.
5. **Run:** dashboard invokes the same build/test commands used outside the course.
6. **Feedback:** deterministic checks show evidence; the teacher explains the smallest relevant failure.
7. **Rescue:** hints escalate only as needed based on attempts, performance, time, and reported fatigue.
8. **Prove:** learner explains the bug/design and performs a small transfer or variation.
9. **Checkpoint:** record progress and evidence; show Git state when implemented. Meaningful commits/merges require explicit approval. U1 saves local snapshots only.

## Mission contract

Every mission contains:

- user-visible product value;
- prerequisite concepts;
- files allowed to change;
- files that are read-only;
- starting behavior and reproduction steps;
- completion criteria;
- automated checks where technically possible;
- manual smoke check when UI is involved;
- predicted failure/behavior question;
- hint ladder;
- Rescue Mode threshold guidance;
- transfer task;
- rollback/checkpoint instructions;
- related scaffold entries.

One mission should normally change one conceptual boundary. A mission may include several edits when they form one coherent feature.

## Evidence of understanding

Default evidence is:

1. **Prediction:** what should happen and why.
2. **Working behavior:** build/test/smoke evidence.
3. **Debug explanation:** identify the relevant state, control flow, identity, ownership, compiler, or linker relationship.
4. **Transfer:** make or explain a small variation without copying the exact repair.

This is not an exam ritual. When fatigue is high, the system may accept a smaller transfer task or postpone explanation review. It should not mark a concept mastered solely because supplied code compiled.

Track concept confidence separately from mission completion:

- `introduced`
- `practiced_with_help`
- `practiced_independently`
- `review_due`
- `durable`

## Adaptive assistance

### Hint ladder

1. Reproduce and observe.
2. Point to the relevant region/state without naming the correction.
3. Ask a concrete comparison or tracing question.
4. Show pseudocode or one partial line.
5. Provide a minimal patch for the blocked portion and explain it.
6. Rescue Mode implements the blocker, records scaffold/ownership debt, and schedules a later review.

Do not force the learner through every rung. Escalate faster after repeated similar failures, clear fatigue, or when tooling/configuration—not the target concept—is blocking progress.

### Fatigue/performance signals

Use recent evidence, not personality guesses:

- repeated attempts with no new observation;
- rapid error cycling;
- time substantially beyond mission estimate;
- inability to explain a previously demonstrated concept;
- explicit fatigue/frustration report;
- recurring setup/linker failure unrelated to the mission.

The dashboard may offer a quick check-in such as `Good / Tired / Fried`, but must remain usable without emotional surveillance or elaborate self-reporting.

## Offline operation

Every prepared mission must work without AI/internet through:

- local instructions;
- stored hints;
- build commands;
- automated tests/checks;
- known-error explanations;
- progress stored locally.

When AI is available, it may interpret novel failures and adapt teaching. Core progression and pass/fail status must not consume AI credits.

## Dashboard scope

Minimum useful dashboard:

- active mission brief;
- repository/file tree limited to relevant files;
- text editor with diagnostics where feasible;
- terminal/build/test output;
- Run/Check/Hint/Rescue controls;
- concept and mission progress;
- scaffold remaining count;
- Open in Visual Studio action;
- Git checkpoint/restore visibility.

Do not build a full IDE before proving the learning loop. Reuse mature editor/terminal components if an isolated dashboard technology is approved. PandaBank itself remains C++.

## Visual Studio relationship

- Both tools open the same repository.
- Dashboard saves normal source files.
- Visual Studio is used for breakpoints, watch windows, call stacks, profiling, and detailed project inspection.
- Missions should sometimes require Visual Studio debugging so the learner does not become dependent on dashboard abstractions.
- No export or synchronization layer is needed beyond ordinary file changes and build configuration.

## Curriculum ordering

Mission order follows prerequisites:

1. frame state and stable object identity;
2. repeatable debugging and assertions;
3. separating declarations/definitions and compile/link reasoning;
4. portable build and test execution;
5. object model and ownership;
6. serialization/persistence fundamentals;
7. naming tree logic;
8. asset registry and relationships;
9. file management safety;
10. application UI and workflow features;
11. layout/vector systems;
12. distribution and later remote boundaries.

Product progress matters, but chronology outranks rushing to the most visible feature.

## Reference solutions

Reference solutions must not be compiled into the application or placed where they are accidentally read during normal work. Prefer Git history, protected course data, or patches revealed only by Rescue Mode. A release gate verifies they are not runtime dependencies.

## Transition rules

Show Why/mental model and one action with expected observation; use short answers rather than essays. Offer More, syntax and Professional Approaches optionally. Seven hint rungs and explicit Rescue approval are specified in PANDATA_UNIVERSITY_SPEC.md. U1 supports prepared hints only; adaptive mentor/Rescue is not implemented.

Definitions: course/university/missions. Runtime progress: course/university/progress. Learner preferences: course/learner_profile.json. Technical decisions: docs/UNIVERSITY_ADRS.md and DECISION_LOG.md. Legacy mission/reviewer state: course/state.json and evidence/. Do not use chat memory as the database.

Judge alternate routes against behavior and real tradeoffs, not an exact expected diff. Preserve accepted X-macro ObjectType. Normal learning and recoverable progress require no AI. U2 adds same-repository build/test/Git visibility; U3 read-only mentor and U4 approved Maintenance remain future. See MENTOR_CONTRACT.md for the strict subscription-only/offline boundary.
