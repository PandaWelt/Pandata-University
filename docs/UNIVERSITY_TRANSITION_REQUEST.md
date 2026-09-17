# Pandata University — Astra Transition and Foundation Prompt

You are continuing a real early-stage C++ project named PandaBank and transitioning its temporary document-based learning pilot into a modular local learning application officially named **Pandata University**.

Read `AGENTS.md` first. Then read every file under `docs/` and `course/`, inspect the actual source, build files, Git-visible state, Mission 001 review, Mission 002 implementation, and Mission 002 evidence before proposing or making changes. Treat the submitted repository as authoritative. Do not infer implementation state from the product vision.

## Immediate first responsibility

Mission 002 has been completed and committed by Chris.

First:

1. Inspect the submitted implementation and evidence.
2. Verify Mission 002 against its actual requirements.
3. Confirm the deliberate duplicate-ID fault was removed.
4. Confirm the Debug-only insertion-count check, duplicate-ID scan, assertions, and logging boundaries are correct.
5. Review the Debug and Release evidence honestly; do not claim runtime tests you did not personally run.
6. Update persistent progress, confidence, scaffold, current-state, decision, and recovery records.
7. Preserve Chris’s existing Git history and source changes.
8. Do not redo Mission 001 or Mission 002.
9. Do not prepare another ordinary Markdown-only mission as the primary next experience.

Mission 002 concludes the temporary document-interface pilot. The next phase is an explicitly authorized early foundation spike for the actual course application.

## Official product identity

The learning application is officially named:

# Pandata University

Use **PandaBank** for the productivity/database/vector-layout application being built.

Use **Pandata University** for the private project-based learning application that teaches Chris C++ while advancing PandaBank.

They operate on the same repository and source files, but Pandata University must remain a separate executable/tool boundary. Course-only code, reference answers, mocks, or runtime dependencies must never leak into the distributable PandaBank application.

Update course-facing specifications and future UI terminology to use the official Pandata University name. Preserve useful historical references when renaming them would obscure past evidence.

## Primary product purpose

Pandata University is a private learning accelerator first, not initially a polished public educational product.

It should:

- teach Chris C++ through real chronological PandaBank development;
- let him make meaningful project progress without becoming an AI spectator;
- reduce burnout and unbounded side quests;
- allow normal source editing in the dashboard or Visual Studio;
- use the same repository, files, build commands, tests, and Git history as ordinary development;
- remain useful without internet access or AI credits;
- eventually provide an editor, terminal, diagnostics, tests, Git visibility, hints, progress, and an Open in Visual Studio action;
- avoid becoming a full custom IDE before the learning loop is proven in the application;
- keep PandaBank’s application code C++ and avoid requiring Chris to learn another language for the course.

Windows and Visual Studio 2022 are the immediate environment. PandaBank remains C++20, no Qt, no royalties, and should prefer permissive/open-source dependencies suitable for eventual commercial distribution. Future iPhone support must not be blocked by unnecessary core coupling, but phone/server work is deferred.

## Scope discipline for this run

Do not attempt to build the complete university, full IDE, complete PandaBank product, vector editor, database, remote service, or phone application in one pass.

This run should:

1. Review Mission 002.
2. Reconcile the existing documentation with all requirements below.
3. Create the durable Pandata University specification and architecture decisions.
4. Establish the smallest reusable course-engine foundation.
5. Implement only one small, verifiable vertical slice when the environment permits it.
6. Leave a precise next learner action.
7. Preserve working PandaBank behavior.

Do not create empty directory architecture merely to appear complete. Add a boundary only when the run gives it real code, data, tests, or an immediately necessary documented contract.

## Existing PandaBank architecture: retain, adapt, or replace

Before extending the current prototype, inspect its relevant implementation and create a concise **RETAIN / ADAPT / REPLACE** classification with technical reasons and explicit migration timing.

Do not preserve ineffective code merely because it already exists. Do not perform a wholesale rewrite merely because the product vision is large. Prefer small, reversible, test-backed migrations.

At minimum, assess:

- `AppUISetup.cpp` currently combines application composition, persistent state, rendering, spawning, animation, popup state, and diagnostics.
- `BaseObjects` mixes stored object data with ImGui drawing behavior.
- `ObjectSpawner` reuses a mutable template object, which could carry stale properties between object types.
- `std::vector<BaseObjects>` is currently a value collection. The collection concept is valid, but future pointer/reference invalidation and possible object-slicing risks must be understood.
- `nextId` is runtime-only and resets between launches; it is not a final persistent project identity allocator.
- application `std::uint64_t` IDs are cast to `int` for ImGui IDs, which can truncate future values.
- fixed ImGui-oriented text buffers are not the final persistent document representation.
- application state stored as function-local `static` values is difficult to test, save, reset, or use across multiple projects.
- absolute object coordinates are not a responsive layout system.
- animation state and rendering are currently coupled.
- console vector printing is temporary diagnostic behavior.
- Visual Studio project configuration is not the final portable build definition.
- bundled SDL/ImGui provenance and license information require a release audit.
- direct SDL/OpenGL/ImGui assumptions must not enter the platform-independent core.

## `std::vector` versus vector graphics

Do not confuse `std::vector`, which is a C++ collection container, with PandaBank’s future vector-graphics system.

Preserve useful concepts including:

- stable application-level object identity;
- explicit object types;
- owned object collections;
- predictable creation flow;
- Debug invariants;
- separation between display names and identity.

Dear ImGui may remain useful for:

- Pandata University panels and controls;
- PandaBank menus, property inspectors, diagnostics, and editor chrome;
- temporary canvas presentation;
- developer tools.

Dear ImGui must not become the authoritative persistent storage system for:

- vector paths or geometry;
- layout/component definitions;
- transforms and styles;
- text content;
- hierarchy and grouping;
- selection;
- animation data;
- command history;
- project state.

The current `BaseObjects::Draw()` coupling is prototype architecture. Evolve toward a renderer-independent document/object model and a renderer boundary before advanced whiteboard/vector tooling.

Do not choose a final vector-rendering library during this foundation unless an architecture decision compares:

- license and commercial suitability;
- Windows and future iOS support;
- maintained status;
- paths and control points;
- text shaping and font handling;
- gradients and clipping;
- hit testing;
- boolean operations;
- serialization implications;
- animation requirements;
- GPU/CPU behavior;
- replacement cost.

## Header/source and modular C++ learning

The course must explicitly teach normal C++ organization:

1. identify one responsibility;
2. declare the usable interface in a header;
3. define behavior in a `.cpp`;
4. include only the declarations required;
5. add the implementation to the appropriate build target;
6. link targets through explicit dependency boundaries;
7. diagnose compiler errors separately from linker errors;
8. run focused tests;
9. trace behavior from model through application and renderer.

Do not describe C++ translation units as “scripts” without explaining the distinction.

Chronology and prerequisites outrank rushing toward flashy features.

## Educational experience

Pandata University must be genuinely educational, not a list of unexplained instructions and not an answer-copying machine.

The learner experience should follow this loop:

1. **Resume:** load repository, mission, concept, assistance, scaffold, and progress state.
2. **Why:** briefly explain why PandaBank needs the active concept.
3. **Mental model:** explain how the relevant C++ or system behavior works in concrete plain English.
4. **Predict:** ask for a short prediction or choice before execution.
5. **Attempt:** let Chris write or complete the logic.
6. **Run:** invoke the same real build/test commands used outside the course.
7. **Feedback:** explain the smallest relevant failure.
8. **Hint:** escalate only as needed.
9. **Prove:** use a small variation, code result, debugger observation, or isolated exercise.
10. **Checkpoint:** record evidence and Git state automatically.
11. **Continue:** unlock the next chronological prerequisite.

## Progressive disclosure

Do not display an entire long mission document as the default experience.

Show one active action at a time, with:

- the immediate purpose;
- the exact current action;
- the expected observable result;
- Run, Check, Hint, Explain More, Professional Approaches, Ask Mentor, and Rescue controls when applicable.

Default explanations should normally be approximately 80–150 words.

Additional information should be optional:

- plain-English hover definitions for syntax and terminology;
- an expandable syntax breakdown;
- an expandable deeper explanation;
- an optional diagram where relationships genuinely benefit;
- an optional Professional Approaches section;
- optional alternative implementation comparisons.

Professional Approaches should describe common real-software solutions, why they are used, alternatives, limitations, and tradeoffs. Do not present one implementation as universally correct merely because it is common.

## Questions and evidence

Questions should usually require:

- a selection;
- a number;
- a code blank;
- a short phrase;
- one sentence;
- a tiny ordering or matching exercise;
- a code edit whose result answers the question.

Do not require two-to-four-paragraph written explanations unless a critical architectural decision cannot be demonstrated more efficiently.

Treat working code, tests, debugger evidence, small variations, and transfer tasks as the primary proof of understanding.

Pandata University should automatically collect evidence from:

- build results;
- test results;
- assertions;
- mission checks;
- Git diffs and commits;
- debugger observations entered through focused fields;
- hints used;
- Rescue Mode use;
- short learner answers.

Do not force Chris to manually interpret and maintain large Markdown evidence worksheets during normal use. Keep durable evidence internally and export it to human-readable records when useful.

## Hint and answer-reveal policy

Use an adaptive hint ladder:

1. reproduce and observe;
2. identify the relevant state or region;
3. ask a concrete tracing/comparison question;
4. provide pseudocode;
5. provide a partial line or constrained fill-in;
6. provide the blocked syntax or minimal blocked portion;
7. Rescue Mode may propose or apply the narrow repair.

Do not force every rung when the learner is clearly blocked, fatigued, or dealing with irrelevant tooling.

Do not immediately provide finished copyable code when logic is the learning target.

When syntax is new but the underlying reasoning is the target:

- provide the necessary syntax signature;
- provide a tiny unrelated example if useful;
- leave PandaBank-specific integration and reasoning to Chris.

Reference solutions must remain outside ordinary runtime/source visibility and appear only through deliberate hint/rescue escalation.

## Isolated sandbox exercises

Support small, disposable exercises that compile separately from PandaBank when a concept is easier or safer to practice on a fresh canvas.

Sandbox exercises must:

- directly support the active concept;
- remain small;
- avoid unrelated side quests;
- never contaminate PandaBank source;
- have deterministic checks;
- allow quick reset;
- transfer immediately back into the real project.

## Fatigue adaptation

Use observable signals:

- repeated attempts without new information;
- rapid error cycling;
- time far beyond the estimate;
- explicit tiredness/frustration;
- recurring setup failures unrelated to the concept;
- difficulty explaining a previously demonstrated concept.

An optional `Good / Tired / Fried` control is acceptable, but do not require emotional self-reporting.

Adjust by shortening transfer tasks, providing a more direct hint, moving a concept into a sandbox, or postponing explanation review. Do not falsely mark copied code as independently mastered.

## Alternative implementation routes

Evaluate required behavior, invariants, architecture boundaries, safety, and explainability rather than requiring an exact expected source diff.

Chris may discover or prefer an implementation different from the reference.

For an alternative solution:

1. Compile and run the same deterministic checks.
2. Compare it with the mission contract.
3. Identify concrete correctness, readability, maintenance, portability, extensibility, performance, licensing, and roadmap consequences.
4. Do not claim something is more optimized without measurement or a specific complexity/resource argument.
5. Classify it as:
   - Accept;
   - Accept With Tradeoff;
   - Adapt;
   - Reject With Evidence;
   - Compare in Sandbox.
6. Preserve a valid learner-selected implementation.
7. Explain concrete downstream problems before requiring replacement.
8. Offer a small adaptation or reversible comparison when appropriate.

Save teaching preferences separately from technical decisions.

Use a local learner-profile record for:

- explanation style;
- preferred level of abstraction;
- fatigue behavior;
- accepted assistance patterns;
- preferred exercise types;
- concepts needing review.

Use the project decision log for:

- context;
- selected technical approach;
- alternatives;
- consequences;
- scope;
- explicit reconsideration triggers.

Do not depend on conversational AI memory.

The existing X-macro-generated `ObjectType` list is an accepted learner-selected route unless review finds a concrete current defect. Record it as a scoped decision rather than automatically replacing it with duplicated enum/name lists.

Possible reconsideration triggers include:

- runtime or user-defined object types;
- plugin registration;
- external schemas;
- serialization/versioning requirements;
- recurring macro-diagnostic or tooling problems;
- demonstrated maintenance cost.

## Data-driven course engine

Mission behavior must not be individually hard-coded into the dashboard UI.

Define reusable step types such as:

- Explain;
- Predict;
- Inspect;
- Short Answer;
- Edit;
- Run;
- Debug;
- Check;
- Hint;
- Professional Comparison;
- Sandbox;
- Transfer;
- Review;
- Checkpoint.

Mission content should define:

- prerequisites;
- active concept;
- allowed/read-only files;
- steps;
- optional explanations;
- syntax tooltips;
- professional comparisons;
- hints;
- Rescue behavior;
- checks;
- evidence;
- completion gates;
- transfer;
- rollback;
- related scaffolds.

Keep course definition, learner progress, project decisions, and application source logically separate.

Missions and progress must remain readable and recoverable without Pandata University.

## Offline operation

Every prepared mission must work without AI or internet through:

- local instructions;
- stored explanations;
- stored syntax definitions;
- stored professional notes;
- stored hints;
- build commands;
- deterministic checks;
- known-error explanations;
- sandbox exercises;
- locally stored progress.

AI may interpret novel failures and generate improvements, but ordinary progress must not require an AI call.

## Embedded AI mentor

The preferred online mentor provider is a local Codex App Server using ChatGPT-managed authentication and Chris’s existing ChatGPT Plus allowance.

Do not use:

- an OpenAI API key;
- usage-based API billing;
- automatic paid fallback;
- hidden credit purchasing;
- a provider that can create charges beyond Chris’s existing subscription.

When ChatGPT-managed usage is unavailable, switch to offline operation.

Preferred integration:

- Pandata University launches `codex app-server` as a local child process.
- Use the default stdio JSONL transport.
- Do not use the experimental WebSocket transport.
- Codex owns ChatGPT OAuth or device-code authentication.
- Never directly read, copy, display, transmit, or manage Codex’s stored authentication tokens.
- Use App Server account methods to detect authentication state, plan type, usage, and rate limits.
- Display mentor availability, selected model, and reset/unavailable status.
- Preserve all progress independently from the Codex process.
- Maintain or reconstruct a mentor thread for each mission.

Model policy:

- use a lower-cost capable model such as Terra for normal mentor assistance;
- use Luna for narrow definitions or simple prepared transformations when appropriate;
- invoke Astra only through an explicit Deep Review action or a clearly defined difficult-problem escalation;
- never invoke Astra automatically for every course step;
- never fall back to separately billed API-key usage.

Mentor actions:

- **Ask Mentor:** read-only explanation.
- **Review My Attempt:** inspect relevant source/diff and explain.
- **Give Hint:** follow the mission hint ladder.
- **Explain Differently:** adapt the mental model without changing mission requirements.
- **Compare Approaches:** evaluate alternatives and tradeoffs.
- **Deep Review:** explicitly use Astra for difficult architecture or debugging.
- **Rescue Mode:** propose a narrow repair and apply it only after explicit confirmation.

AI output is advisory. Compiler, tests, assertions, Git state, and deterministic mission checks remain the normal pass/fail authority.

Each mentor request should receive a minimal relevant context bundle containing:

- active mission and step;
- relevant source excerpts;
- current diff when applicable;
- latest compiler/test output;
- recent attempts;
- hints already used;
- learner preferences;
- accepted project decisions;
- concept-confidence state;
- relevant architecture rules.

Do not resend the entire repository unnecessarily.

## Pandata University self-improvement

Pandata University should eventually allow Chris to request changes to the course from inside Pandata University rather than repeatedly exporting ZIP files to ChatGPT.

This capability must be explicit, controlled, recoverable, and separated from ordinary Study Mode.

Provide a distinct **University Maintenance Mode**.

Study Mode must not silently modify:

- course definitions;
- progress rules;
- assessment criteria;
- reference answers;
- tests;
- Pandata University source;
- PandaBank source;
- architecture rules.

University Maintenance Mode may support requests such as:

- rewrite an explanation;
- add or change a tooltip;
- add a Professional Approaches section;
- shorten questions;
- change hint timing;
- add a sandbox exercise;
- revise a mission;
- add a new reusable step type;
- improve the dashboard;
- generate the next chronological mission;
- update the learner profile;
- revise accepted technical decisions;
- fix a Pandata University bug.

Classify proposed changes into:

1. **Content-only changes:** mission data, explanations, hints, tooltips, professional notes, and exercises. These may be validated and hot-reloaded without rebuilding when their schema allows.
2. **Course-engine changes:** step types, validation, state handling, checks, and UI behavior. These require source review, tests, a staged build, and restart.
3. **PandaBank application changes:** product source changes. These remain governed by the active learning mission or a separately approved maintenance request.
4. **Protected-policy changes:** completion gates, assessment logic, safety boundaries, tests, reference-solution visibility, AI permissions, billing/authentication behavior, and release gates. These require elevated explicit approval and must never be weakened merely to make a mission pass.

Self-modification workflow:

1. Chris writes a change request inside University Maintenance Mode.
2. The AI restates the requested outcome and scope.
3. It inspects current relevant files and Git state.
4. It proposes a small plan.
5. It creates changes on an isolated Git branch or worktree when technically practical.
6. It shows a readable summary and exact diff.
7. It runs schema validation, course checks, tests, and builds appropriate to the changed layer.
8. It must not alter tests or requirements merely to conceal a failure.
9. Chris approves, rejects, or requests revision.
10. Only after approval are the changes applied/merged.
11. The system creates a clear Git checkpoint.
12. The system records what changed, why, which AI/model assisted, validation performed, and how to roll back.

Never auto-merge or auto-commit a meaningful self-modification without Chris’s explicit approval.

Content changes may reload live after approval.

Pandata University executable changes must:

- build to a staging output;
- leave the currently running executable untouched;
- preserve the last known working version;
- present validation results;
- activate only after restart;
- support rollback if startup or validation fails.

The self-improvement system must not require Astra for every change. Routine content changes may use a lower-cost mentor model. Astra should be reserved for difficult architecture, engine changes, or deep review.

The current ZIP/ChatGPT handoff remains a recovery path, not the normal long-term workflow.

## Git and recovery

Before modifications:

- inspect status;
- preserve unrelated work;
- avoid `git add .`;
- never use destructive reset/checkout operations;
- distinguish generated files and source;
- keep a recoverable baseline.

Pandata University should eventually make Git understandable through:

- current branch and cleanliness;
- intended changed files;
- readable diff;
- checkpoint description;
- rollback point;
- commit history related to missions and maintenance;
- clear separation between learner work and AI-applied rescue or maintenance work.

Do not make Git completely invisible. Teach enough of the real workflow for Chris to use the repository outside the dashboard.

## First foundation sequence

Prepare a staged roadmap similar to:

### U0 — Pilot review and durable decisions

- accept/reject Mission 002 with evidence;
- apply the Pandata University name;
- reconcile course documentation;
- record the architecture boundaries;
- record AI-provider and self-modification decisions;
- record alternative-solution policy.

### U1 — Offline mission engine foundation

- reusable mission schema;
- reusable step types;
- mission loader and validation;
- progress state separate from mission definitions;
- one-step-at-a-time presentation;
- expandable explanations;
- syntax tooltip data;
- Professional Approaches data;
- short learner responses;
- deterministic completion state;
- recovery from malformed mission/state data.

### U2 — Real repository actions

- restricted relevant-file view;
- build/check actions;
- captured output;
- evidence generation;
- Git status/diff visibility;
- Open in Visual Studio;
- no custom full IDE yet.

### U3 — Read-only AI mentor

- provider interface;
- Codex installation detection;
- App Server lifecycle;
- ChatGPT-managed login;
- plan/rate-limit status;
- one mission-aware read-only mentor thread;
- offline fallback;
- no code modification.

### U4 — Controlled Rescue and University Maintenance

- narrow patch proposals;
- approval flow;
- isolated Git branch/worktree;
- validation;
- content hot reload;
- staged executable update;
- rollback;
- protected-policy boundaries.

Do not implement every stage in this run.

## Smallest acceptable implementation for this run

After review and architectural reconciliation, implement the smallest foundation that creates real reusable value.

Prefer a platform-independent mission-domain foundation with tests if the Windows GUI cannot be built or validated in the current environment.

A reasonable first vertical slice is:

- a formal mission schema or strongly typed mission model;
- one or more reusable step types;
- validation with useful error messages;
- progress stored separately from immutable mission definition;
- conversion or compatibility loading for the prepared missions;
- tests proving valid content loads and invalid content fails safely;
- one minimal Pandata University presentation surface only if it can be validated honestly.

Do not pretend an unvalidated GUI is complete.

Do not implement autonomous AI editing in the first slice. Establish its interfaces and decisions so the later App Server and Maintenance Mode work do not require redesigning the mission engine.

## Final deliverables from this Astra run

Return:

1. Mission 002 review result.
2. Complete changed-file list.
3. Validation performed and what remains unverified.
4. Updated persistent course and project state.
5. Pandata University product/course specification.
6. Architecture decisions covering:
   - course/PandaBank separation;
   - mission data and progress;
   - alternative learner routes;
   - renderer/model separation;
   - Codex App Server provider;
   - ChatGPT Plus-only cost boundary;
   - offline fallback;
   - University Maintenance Mode and self-update safety.
7. RETAIN / ADAPT / REPLACE assessment of relevant prototype systems.
8. The smallest verified foundation implementation.
9. Scaffold/debt entries with exact owners and removal conditions.
10. A clean recoverable archive that does not overwrite Chris’s newer source or Git history.
11. One exact next action for Chris.

Do not bury the next action in a large explanation.

Do not claim compilation, runtime behavior, authentication, rate-limit behavior, or GUI behavior that was not actually verified.

Do not turn this run into another long questionnaire. Ask only an absolutely blocking question whose answer would materially change the safe first foundation. Otherwise, use the established requirements and make the smallest reversible decision.
