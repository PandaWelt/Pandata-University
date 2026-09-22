Implemented subset — 2026-09-21: U4-F1 now provides only the bounded in-memory fixture described in U4_FIXTURE.md under U-09a. The remaining contract below is a future design, not implemented capability. Focused Windows validation is next.

Current resolution — 2026-09-21: U-09a in UNIVERSITY_ADRS.md permits only U4-F1, an in-memory comparison/display fixture with no I/O or provider capability. The larger contract below is retained as a future design; its external input, real Git binding, hashes and filesystem checks are not claimed implemented. All live/execute/apply/write automation gates remain blocked. See U4_FIXTURE.md for this implementation's exact scope and validation.

# U4 preparation — one offline proposal-review contract

Status: **design prepared, not implemented or unlocked**. Base: accepted U3 safety checkpoint 330420d. This is a University engineering brief, not a new learner worksheet or a PandaBank mission.

## Why this is next

U4 follows U3 in the foundation sequence and covers approved Rescue/Maintenance. Its first useful boundary is the ability to inspect exactly what a proposed change would touch and to invalidate stale proposals. That can be designed without a live mentor. This review prepares that boundary only; it does not treat an offline fallback as a live U3 pass.

U-09 explicitly requires tested isolation before U4 implementation. That prerequisite is still open. Separate implementation authorization is also needed. No exception or relaxed read policy is introduced here. The already completed U3 diagnostic validation is not assigned again.

## Smallest proposed slice

An offline, read-only proposal preview for one manually supplied, bounded text proposal, with a base Git commit, exact current-file byte hashes, one owned relative path, literal proposed diff, change class, origin, validation provenance and rollback description. Initially use only a disposable University fixture; never a PandaBank implementation or real learner-state edit.

The proposal is data. Its body cannot choose a command, process, tool, extra path or URL. Reuse the principles of U2's numbered allowlist and direct fixed actions; do not turn its build runner into arbitrary proposal execution. There is no AI call or dependency on a working mentor.

Suggested single flow: load proposal -> validate schema/path/base -> display exact before/after and provenance -> dismiss or report stale. A later file/base change invalidates the review. A displayed status is not approval to write. The first slice has **no Apply or Approve-to-execute control**, executable callbacks, automatic Git actions, source writes or progress writes. No implementation is provided in this delivery.

## Contract to review before implementation

| Data | Required meaning |
|---|---|
| proposal_id / format_version | Immutable identity and supported strict format |
| base_commit | Full commit ID; resolve locally, never fetch automatically |
| owned_path | One reviewed relative fixture path; no traversal, absolute path, links or reparse points |
| before_sha256 / after_sha256 | Bind exact byte versions; mismatches are stale, never auto-merged |
| exact_diff | Bounded literal display; terminal controls escaped; no interpretation/execution |
| change_class / origin | Fixture/content/engine/product/protected-policy attribution; unknown classes refuse |
| validation | Each claim marked machine-observed, user-reported or unverified; a proposal cannot certify its own tests |
| rollback_description | Informational only; no executable script |
| review_state | proposed / displayed / stale / rejected; none is write authorization |

Provisional bounds for later review: one UTF-8 text fixture, <=64 KiB before/after, <=128 KiB proposal, one selected proposal at a time. Reuse an existing parser only if it can enforce the contract without widening its current limits. Do not design a generic editor, patch engine or new framework.

## Concrete scenarios for the later implementation gate

1. A valid one-line fixture proposal displays exact before/after while fixture bytes and Git state stay unchanged.
2. Edit the fixture after preview: rechecking rejects the stale hash. A changed base commit also invalidates it.
3. Missing/malformed/oversized input, traversal, absolute paths, reparse escape and unsupported class are refused before any external process.
4. A body containing shell syntax or fake “tests passed” text is displayed as untrusted data, never executed or accepted as machine evidence.
5. Dismiss/reject, missing Git and restart leave existing U1/U2 progress, U3 evidence and all source unchanged.
6. With Codex absent, every proposal-review action remains usable; no authentication/model call occurs.
7. Windows reparse cases denied by the host are recorded skipped, never passed.

These are planned acceptance cases, not tests run in this review. Any subsequent change-application, Rescue, hot reload, staging/activation or rollback implementation is outside this slice and requires its own exact-diff approval and applicable isolation proof.

## Entry and completion gates

Preparation is complete when the contract, one-path scope, no-write behavior and rejection cases are explicit; this document satisfies that preparation only. Implementation remains blocked until the existing U-09 isolation condition is satisfied and scoped implementation is authorized. A future request to proceed must address that prerequisite explicitly rather than silently interpreting this design as a waiver.

Do not reopen locked C++/Windows/ChatGPT-only/no-billing decisions or ask Chris to redo U1/U2/U3. No additional platform, dependency, sandbox installation or account change is selected here. Resolving the isolation prerequisite is an engineering task before U4 implementation, not a product refactor for Chris.

## Chris's next action

Apply the records-only review patch, then open this document. No coding exercise or repeat mentor-status run is required now. The next engineering handoff should use this contract and the unresolved U-09 entry gate; it must not ask for full U4/Rescue implementation on the assumption that live U3 passed.
