# Pandata University specification

Authority: Chris's 2026-09-17 transition brief, preserved as UNIVERSITY_TRANSITION_REQUEST.md. This specification replaces the Markdown-only pilot as the intended normal experience. The pilot records remain evidence, not assignments to repeat.

## Identity and boundaries

PandaBank is the productivity/database/layout application. Pandata University is the private C++ learning accelerator. They share one repository and actual PandaBank source files; University is a separate executable. No course source, tests, reference answers, mocks, network/auth dependencies or learner state may link into a distributable PandaBank binary. Windows/VS2022 is first. C++20, no Qt, no royalties, no additional required language/toolchain for Chris. Phone/server and final vector-renderer selection remain deferred.

## Learning experience

Resume → Why → concrete mental model → short prediction → learner attempt → real run/check → focused feedback → adaptive hint → small proof/transfer → evidence checkpoint → next prerequisite. Show one active step. Default explanations are about 80–150 words; optional More, syntax definitions, deeper explanation, Professional Approaches and useful diagrams provide detail. Questions normally require a choice, number, code blank, short phrase or one sentence. Working behavior and small variations matter more than essays.

Use evidence from builds/tests/assertions, approved Git actions, learner-entered debugger observations, responses, hint use and rescue. Mark the origin of each item (machine-observed, learner-reported, reviewer-assessed). Prepared checks are deterministic; AI cannot grant a pass merely by saying one occurred. A response-required gate checks presence only unless a separately reviewed evaluator exists. Completion of an orientation is not mastery or completion of an application change.

Allowed tools grow by stage. Applicable controls eventually include Run, Check, Hint, Explain More, Professional Approaches, Ask Mentor and Rescue. An unavailable capability must be clearly unavailable, not a decorative working-looking button. Progressive disclosure must not depend on AI.

## Teaching policy

Hint ladder: observe → locate state/region → trace/compare → pseudocode → partial line → blocked syntax/portion → explicitly approved narrow Rescue. Escalate after repeated attempts without new evidence, irrelevant setup failure, time far beyond estimate, or explicit fatigue. Optional Good/Tired/Fried, never compulsory. Supply unfamiliar syntax signatures or unrelated tiny examples while leaving target reasoning to Chris. Maintain confidence independently: introduced, practiced_with_help, practiced_independently, review_due, durable. Never infer independent work from compilation alone.

Accept valid alternatives against behavior, invariants, architecture, readability, maintenance and concrete resource costs. Classify Accept / Accept With Tradeoff / Adapt / Reject With Evidence / Compare in Sandbox. Preserve valid learner routes. Sandboxes are small, disposable, separately compiled, deterministic and immediately transferable to PandaBank; no duplicate tutorial app or contamination of production source.

Explicitly teach responsibility → header declaration → .cpp definition → includes → compiling the .cpp → linking → compiler versus linker diagnostics → tests → tracing model/application/renderer. A C++ translation unit is not a script. Next curricular topic after the accepted pilots is this boundary, presented through University.

## Data and continuity

Separate immutable mission definitions, local learner profile, learner progress/evidence, technical decisions, and PandaBank source. Definitions carry prerequisites, concepts, scope, step types, why/action/expected result, optional syntax/professional notes, hints, checks/gates, rescue, evidence and rollback/scaffold contracts. Definitions/progress must remain human-readable without the executable. Version changes must not silently preserve completion under changed assessment rules.

The U1 schema deliberately implements only Explain, Predict, Inspect, ShortAnswer, Review and Checkpoint presentation with ack/response gates. Future Edit, Run, Debug, Check, Sandbox, Transfer and Professional Comparison may be executable step types only after handlers and tests exist. Unknown types fail closed. Hints/professional notes already work as optional step data. See UNIVERSITY_FORMAT.md.

Pilot compatibility uses an explicit reviewed-completion index pointing to original 001/002 evidence. It neither scrapes arbitrary Markdown as proof nor asks Chris to redo completed work. New U1 progress is collected automatically in append-only snapshots. The original course/state.json remains historical mission/reviewer state; U1 does not rewrite it or invent completed PandaBank missions.

## AI and maintenance — specified, not implemented in U1

Preferred U3 mentor: local codex app-server, default stdio JSONL, Codex-managed ChatGPT sign-in using existing Plus allowance. No API key, API billing, paid fallback or purchase mechanism. Availability/model/rate limits require actual provider checks; no entitlement promises. Missing, unknown or depleted allowance → offline study. Terra for ordinary mentoring and Luna for narrow definitions are preferences contingent on provider availability, not hard-coded promises; Astra only explicit Deep Review/difficult-problem escalation. See UNIVERSITY_ADRS.md and MENTOR_CONTRACT.md.

Study Mode may record learning progress but must not silently change definitions, gates, tests, references, policy, University source or PandaBank source. Later source editing is an explicit learner action in allowed files. Mentor explanation/review is read-only. Rescue proposes a narrow diff and applies only after confirmation.

University Maintenance Mode is distinct and explicit. Classes: content-only; course-engine; PandaBank application; protected-policy. Protected gates/tests/reference visibility/auth/billing/permissions/release rules need elevated explicit approval, never weakening to hide failure. Inspect → bounded plan → isolated branch/worktree → exact diff → layer-appropriate validation → approve/reject/revise → apply/merge → approved Git checkpoint + attribution + rollback. No meaningful auto-merge/commit. Content can reload after schema validation and approval. Executable changes stage away from the running program, retain last-known-good, activate after restart and support rollback.

## Foundation staging and honest status

| Stage | Deliverable / acceptance | This run |
|---|---|---|
| U0 | Accept pilots, name/spec, architecture and learner policy | Implemented documentation/review. |
| U1 | Mission loader, validation, separate recoverable progress, one-step presentation, tests | Implemented offline C++ console slice; validation reported in UNIVERSITY_HANDOFF.md. |
| U2 | Restricted file view; allowlisted real build/check execution and captured output; visible Git status/diff; Open in VS | Implemented minimal console actions; Linux verified, Windows U2 pending. No full editor/IDE. |
| U3 | Read-only mission-aware mentor, account/limits, offline fallback | Contract only; no child process/auth/model run. |
| U4 | Approved Rescue/Maintenance, hot reload, staged updates, rollback | Contract only; no autonomous editing. |

The console is a real minimal presentation for testing the engine, not the promised finished GUI. Windows compilation/interaction requires a local check; the implementation is compiled/tested on the available Linux host. Build/test commands for University are separate from the unchanged PandaBank project. CMake migration of PandaBank is not claimed.

U2 status/evidence and limitations: U2_HANDOFF.md and U2_ACTION_CONTRACT.md. U1 definition/progress contract remains unchanged. New repository evidence never auto-promotes lesson completion or mastery.
