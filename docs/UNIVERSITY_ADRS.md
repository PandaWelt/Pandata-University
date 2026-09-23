## U-15 — close the offline baseline in one final integration, 2026-09-23

Accept c2d3a75, direct child of 114c272, with Chris's Windows run-1 report and
submitted workflow artifacts. Symlink coverage was skipped, not passed. Run 1
loaded real progress read-only; final sessions must instead use disposable state.

Name the offline baseline PU-OFFLINE-1.0. Add only a strict --no-pause launcher
switch (same real builder/schema/session path), a visible version label, and final
workflow/receipt tooling. No runtime store/policy/command relaxation. Built-in
PowerShell is maintainer validation glue, already used for patch application;
University and PandaBank remain C++ with no new learner dependency.

All final test processes target the fresh copy; synthetic Git commits/backups and
deliberate corruption exist only there. Hash real source/history/progress before
and after. Deterministic BOM-free CRLF input files replace shell echo pipelines.
Retained U4 checks run once inside the final validator, unchanged U1–U3 suites do
not. Real Start launcher gets two clean builds and learner sessions; accepted
Open-in-VS/run-check behavior is not redundantly retested or used to launch the IDE.

The final passing receipt plus review of Chris's returned result establishes
completion without another records-only package/commit. Native Windows result is
pending at delivery; no manufactured PASS or learner mastery. Only a genuine
failure justifies another run. After completion, move to Mission 003/content.
GUI/editor, CMake, live mentor, apply/source-writing and automation remain separately
deferred with existing gates intact; none freezes the useful offline baseline.

# Pandata University architecture decisions — 2026-09-17

These scoped decisions implement the authorized early spike. Each preserves a reconsideration trigger rather than declaring a permanent universal solution.

| ID / context | Selected approach and alternatives | Consequences, scope and reconsideration trigger |
|---|---|---|
| U-01: one repository, two programs | C++20 standard-library University domain + separate console executable in university/. Keep PandaBank native source/build unchanged. Compared extending AppUISetup and a web dashboard. | Avoids coupling teaching progress/auth to PandaBank and adds no dependency/language. Console is an interim view. Reconsider UI after U2 command/evidence contracts and a verified Windows build; ImGui panels remain a candidate. |
| U-02: mission data/progress | Typed model + strict versioned quoted-record wire format; separate append-only progress snapshots bound to definition bytes. Considered JSON via added parser and hardcoded UI lesson branches. | Small audited parser, no package installation; limited schema, not a general configuration language. Human-readable/exportable. Replace wire adapter with approved JSON parser when editors/nesting justify it, retaining typed domain and explicit migrations. |
| U-03: pilot compatibility | Explicit reviewed-completion index references original Markdown evidence; U1 orientation depends on 001/002. | Old lessons remain accessible without replay. Index is reviewer-authored evidence metadata, not runtime authentication. Any completion import changes require review; no silent arbitrary Markdown parsing into passes. |
| U-04: alternative learner routes | Contract/evidence-based review, five dispositions from spec; preserve scoped X-macro ObjectType route. | No expected-diff grading. Current malformed #undef is a local warning, not reason to discard X-macros. Reconsider runtime types/plugins/external schemas/serialization or demonstrated tooling/maintenance costs. |
| U-05: renderer/model | Retain renderer now; migrate BaseObjects data/drawing boundary before persistence or advanced vector work. | std::vector is an owned collection, not vector graphics. Core must exclude ImGui/SDL/SQL/Windows headers. No vector-renderer selection in this run. See PROTOTYPE_ASSESSMENT.md. |
| U-06: provider/cost | U3 adapter to locally installed Codex App Server via stdio; Codex-managed ChatGPT auth, Plus allowance only, no paid fallback. Compared direct billed API and custom OAuth storage; rejected both. | Must discover provider version/schema/account/model capabilities. Unknown quota/auth mode/model availability means offline. No guarantee of account entitlements; no token handling. Provider details never enter mission core. |
| U-07: advisory AI | Read-only Ask Mentor/Review/Hint/Explain/Compare; Astra behind explicit Deep Review; deterministic evidence remains authority. | Prefer Terra/Luna only when actually offered by account. No exact provider IDs inferred from UI names. Any modification goes through a distinct approval capability. |
| U-08: offline/failure | No network code in U1; save short responses/hints locally. Invalid definitions stop; corrupt latest snapshot falls back visibly to prior valid one; mismatch blocks migration. | Never silently reset progress. Optimistic generation check plus save lock rejects concurrent writers. OS power-loss durability, stale-lock recovery and migration UI are debt; do not claim a database-grade store. |
| U-09: maintenance | Explicit mode, four change classes, isolated diff/validation/approval; no hidden policy edits. | Meaningful merge/commit needs explicit approval. Staged executables activate only after restart; old version retained. U4 implementation waits for U2 actions and tested isolation; these rules do not make U1 self-modifying. |
| U-10: build scope | Direct standard C++ compiler commands + Windows launcher for University only. | Current environment lacks CMake/MSVC. No unverified root CMake or added third-party libraries. Reconsider a University CMake target with the real CMake migration; do not duplicate PandaBank source lists in this tool. |
| U-11: records/profile | Preferences in course/learner_profile.json; technical choices here/DECISION_LOG; confidence and evidence in course records. | No reliance on chat memory. U1 displays mission-provided wording, does not implement adaptive profile logic yet. Profile changes may not rewrite assessment gates. |

U1 source is supplied infrastructure, not learner-completed C++ coursework. Tests can accept the engine's behavior without claiming Chris understands its internals. No license for Chris's entire project is chosen by this spike.

## U-12 — U2 repository actions, 2026-09-18

Context: completed U1 needs actual local checks and visibility without becoming an IDE. Selected: fixed C++ action IDs and numbered relevant-file list, direct native process adapter, separate machine evidence; preserve U1 schema/fingerprint/state bytes. Alternatives rejected: general shell/command strings, editable arbitrary tool paths, changing old lesson to force replay, automated product refactor.

Consequences: useful bounded actions now; installed tools and repository C++ remain trusted. Git helpers/config filters are refused, not guessed safe. Windows paths use native process quoting; POSIX uses argv. Neither is a hostile-code sandbox. Open VS is an explicit handoff with ordinary IDE side effects. Real check compiles current U1 engine tests; failure probe uses disposable data. No autonomous command choice.

Reconsideration: add new actions only with reviewed scope/tests; native Windows verification before U3, progress/evidence migration and compaction before scale, stronger OS sandbox only if untrusted workloads become a real requirement. This session explicitly authorizes implementation, so no additional Maintenance confirmation is required for these reviewable local changes. No Git commit/merge was authorized or performed in Chris's tree.


## U-13 — U3 minimal advisory mentor, 2026-09-19

Use a separate standard-library C++ JSONL/platform adapter, not a new web/Python runtime or direct paid API. Codex owns managed ChatGPT login and token storage. Keep the locked Plus-only boundary; require actual catalog Terra ID, current known allowance and no enabled credit balance. Other plans/models/unknown metadata remain offline without fallback. No learner-state pointer or repository-action dispatcher enters the controller.

Use one ephemeral thread per explicit, previewed mission interaction and reconstruct from the bounded bundle; record references in separate ignored advisory evidence, never resume hidden provider history. This deliberately postpones multi-turn continuity. Only the active/final U1 conceptual explanation is included; no source/diff/diagnostic upload is necessary. Installed schemas, effective settings, feature/skill catalogs and echoed thread permissions gate the request; request tools/approval callbacks are denied. Metadata discovery uses a separate short-lived process so discovered optional features/skills can be disabled and rechecked before a thread.

Tradeoffs: conservative compatibility (native executable only on Windows; unknown fields/config/allowance blocks), an audited small JSON codec rather than an added dependency, and up to 75 seconds bounded startup/request time. Existing Codex binary, OS, and user configuration are trusted local installations, not hostile executables. Fixture tests establish client decisions; only local installed-version validation can establish actual sandbox/entitlement behavior. Revisit at a protocol upgrade or before wider mentor/editor scope. No U4 capability or approval bypass is authorized.


### U-13a — exact-version schema export compatibility, 2026-09-20

Codex 0.155.1 excludes deprecated v1 auth schemas from JSON export while retaining the token-free RPC. Admit this metadata schema omission only for that exact reported executable version, after tagged source review and generated v2 managed/external AuthMode/Plus checks. Keep actual managed-auth RPC/result validation unchanged. Unknown versions and partial legacy exports still fail closed. Retain the 1 MiB bound and all restricted-root requirements. Same-version real schemas lack restricted roots; live mentor remains blocked. Do not infer a new isolation policy or advance to U4. See U3_CODEX_01551_COMPATIBILITY.md for reproducible evidence and source links.

### U-13b — named profiles are not native isolation proof, 2026-09-21

Recognize exact Codex 0.155.1 experimental thread/turn permissions fields and profile provenance through bounded schema generation. Record diagnostics independently of authorization. The native Windows unelevated backend refuses restricted reads; the elevated backend's ordinary Users membership and read-grant ACL handling do not establish an exclusive readable set. Keep mentoring offline before App Server startup; do not silently enable elevated setup, apply broad deny ACLs, assume read-only equals read-isolated, or turn standalone host fs/readFile into a fake sandbox probe. A separately verified context-only backend remains a U3 prerequisite. No policy/cost/auth scope expansion. See U3_READ_ISOLATION.md and validation/u3-read-isolation.json.

### U-13c — accept validation without claiming live capability, 2026-09-21

Source archive HEAD and independently verified complete Git bundle both identify 330420d36035c71777ff2c1cfeb8f2dbbf714be9. Accept the cumulative Windows safety validation from Chris's 86-check report and the submitted machine evidence of the expected block. Record live mentor, managed-auth/allowance and native isolation as unaccepted. No code or safety-policy change is needed to close this validation task. A GitHub remote and matching origin/main are present; backup success is user-reported, not independently queried.

The next chronological stage is U4, but U-09 explicitly requires tested isolation before implementation. Prepare only its offline proposal-review contract now. Preparation is not a waiver, implementation, approval token or completion claim. Do not keep asking Chris to repeat the unchanged U3 diagnostic. Resolving native isolation remains an open engineering prerequisite, not a repeated learner assignment. See U4_PREPARATION.md.

### U-09a — capability-specific prerequisite for the fixture preview, 2026-09-21

Authorized by Chris's request to resolve U-09 for an offline, no-AI, no-process, no-write proposal-preview fixture. Verified 6c6286d is a records-only descendant of 330420d. This clarification precedes implementation and supersedes the blanket U4 implementation block only for the exact slice below.

Native sandbox isolation is required when an untrusted provider/executable can retrieve context, execute commands or propose automated effects. It does not apply to a trusted in-process comparison/rendering function operating only on bounded caller-supplied values. The fixture receives immutable data, has no filesystem/process/network/approval/store interface, and returns display text/status. It cannot acquire more files or apply a proposal. Its appropriate assurance is input bounds, fixed fixture identity, exact snapshot comparison, escaped display, no capability dependencies, and preservation tests—not pretending that Windows Codex isolation passed.

Permit U4-F1 only: one compiled, disposable-in-memory fixture; supplied base revision and exact before/after bytes; a fixed logical path; preview or stale rejection. No import parser, repository reader, hash library, mutable progress pointer, output/evidence persistence, callback, provider or action dispatcher. Existing University startup and explicitly selected U2 actions retain their existing behavior; the new preview route never invokes them. The focused fixture entry point bypasses mission/progress loading entirely. Build/validation tools naturally compile executables; the no-process/no-write guarantee concerns the runtime preview route.

The broader U4_PREPARATION contract remains a future design. For this smaller fixture, exact byte equality replaces proposed hash binding, and a plainly labeled fixture revision replaces Git resolution. Neither is represented as proof about the real worktree. This is an explicit reversible scope reduction, not approval to import real proposals or to skip future path/reparse/Git checks.

Live mentoring and automated proposal generation remain blocked by exclusive read isolation. Proposal application, source writes, Rescue/Maintenance automation, arbitrary command execution, hot reload, staged activation and Git mutation remain unimplemented and gated by their existing isolation and exact-diff approval rules. U3 status, code, tests and acceptance do not change. Any new external capability invalidates this narrow exemption and requires a separate reviewed boundary before implementation.


### U-09b — authorized U4-F2 pure-data adapter, 2026-09-22

Chris explicitly accepted the committed U4_F2_PREPARATION brief at 6da8b60 and requested the complete implementation/test package. This is the separate scoped authorization anticipated by U-09a, not a silent extension of F1. The accepted brief's capability analysis applies: trusted bounded parsing of supplied values has no context retrieval/execution/write interface, so native Codex isolation is not a prerequisite for this exact adapter. That analysis does not establish native isolation or authorize live mentoring, imports, Git, application, writes or automation.

Reuse mentor/Json.cpp unchanged as a pure codec. ProposalParser alone enforces the 64 KiB envelope, exact eight-field schema, types and mathematically exact format value 1 before handing values to unchanged F1 policy. An exact decimal-value guard prevents the double-based codec from accepting numbers that merely round to 1. No provider/transport is linked into the focused parser checks. Namespace/header coupling is retained to avoid unrelated U3 migration. Fixed CLI demos return before repository/progress setup; extra payload/path arguments reject there.

Ship implementation, focused tests, Windows validation, records, manifest and guarded apply together. After Chris returns clean results, combine acceptance records with the next safe directly testable slice under his continuation instruction. Do not force a separate preparation-only delivery; do not infer permission to cross read/write/automation or paid-provider gates. Native read isolation remains open for live mentoring.

### U-09c — U4-F3 transient review lifecycle, 2026-09-22

F2 at 6b8081d is accepted against its bounded contract and Chris's Windows report. Chris authorized the next safe directly testable slice with acceptance records in the same package. Before adding code, select the pending/displayed/stale/rejected/dismiss flow already anticipated by U4_PREPARATION. F2 validates a single call but owns no pending review; a future view must not keep displaying an obsolete proposal after replacement, rejection or dismissal.

Implement only a one-proposal in-memory session using unchanged F2/F1. Selecting validates against a supplied snapshot, clears any prior selection first, and yields proposed (not displayed). Preview always revalidates against the current supplied snapshot. Rejected/stale checks discard the selection; reverting data cannot resurrect it without explicit reselection. Dismiss clears the selection. Displayed means the last checked view only; every new preview needs a current snapshot. No approval token, snapshot lookup, change listener, import, command dispatcher, callback, persistence or apply method exists. Fixed demos enter before repository/progress initialization.

This extends pure-data lifecycle operations only. Local string/session memory changes are not source writes or saved progress. U-09a/b's capability reasoning still applies: there is no external retrieval/execution authority requiring a Codex sandbox. Live mentoring remains blocked by exclusive read isolation; proposal application/source writes/Rescue/Maintenance automation remain gated and unimplemented. This is neither real-worktree freshness verification nor a whole-process sandbox. One complete implementation/test/validation package is authorized; no preparatory-only handoff is needed.

### U-09d — U4-F4 fixed review controls, 2026-09-22

F3's transient lifecycle is accepted at d35d552 from source review, Linux checks and Chris's Windows report. Under Chris's instruction to deliver the next safe testable slice with records, connect that lifecycle to the existing University console using exactly review-select, review-preview, review-status and review-dismiss. The user can now drive the sequence instead of only watching a compiled demo.

A pure string-to-fixed-action adapter owns one F3 session; it has no stream, file, process, Git, provider, progress or approval interface. It selects only the compiled fixture and uses only its compiled snapshot. No arbitrary proposal/document/path input is accepted. Unknown/extra/oversized commands return a fixed refusal, never echo input or invoke another action. Status describes the last review operation and confers no approval. Existing console input and startup continue to load existing missions/progress as before; individual review commands do not access them or save them. Focused compiled demos bypass startup entirely. This is a scoped presentation integration, not a new external-data import or general command dispatcher.

No native Codex isolation prerequisite is waived: the adapter acquires no external retrieval/execution/write authority. Live mentoring remains blocked, and application, source writes, Rescue/Maintenance automation, Git mutation and real-file proposal imports remain unimplemented/gated. F1/F2/F3 policies/tests remain unchanged. The next stage must not keep expanding fixture demonstrations without identifying a concrete remaining capability gate.

Package repair: the F3 generator globally replaced a validator filename after inserting the allowlist, duplicating Review and omitting Parser. Generate the manifest and literal allowlist from one canonical path list; interpolate fixed template tokens once, never global stage-name replacements after insertion. Before packaging, verify uniqueness and exact equality of manifest, allowlist, and patch paths. Retain a regression for the reported same-count duplicate/omission. Native apply independently verifies that equality before touching repository files. Maintainer packaging tools are not a new runtime dependency for Chris.

### U-09e — U4-F5 validator transport verification, 2026-09-22

Accept F4 at 812223b including Chris's committed deterministic input-file repair. No runtime trim/case/whitespace relaxation is warranted. The next directly testable slice is the missing validation boundary exposed by the native failure: verify exact command bytes before exercising routing, and retain diagnostics capable of measuring cmd.exe's grouped-pipeline reconstruction on Windows. Do not invent another review feature while this concrete native validation gap remains.

Supply maintainer deterministic input-block generation, a bounded standalone C++ stdin byte probe, adversarial exact-command tests, and a Windows validator that checks the staging input before University receives it. Probe reads only its stdin and writes stdout; shell harness creates disposable ignored build artifacts. It is validation infrastructure, not a new University file-import/process/provider/approval capability. Runtime implementation and existing exact-command rules remain unchanged.

Production validators must not pipe grouped echo commands. Optional diagnostic script may reproduce the two reported bad pipelines only into the byte probe, never University or a shell command consumer; capture raw hex/length evidence without requiring a specific OS-specific failure to call validation passed. Native execution is unverified on the Linux reviewer host; the generated Windows validator performs the actual cmd.exe check when Chris runs it.

Preserve U-09d package path uniqueness/equality regression. Generate console input from an explicit fixed token list using separate prefix-redirection echo( lines, no trailing spaces, then stdin file redirection. Before packaging, statically reject pipelines/noncanonical console-input blocks in production root validators. Python packaging tools stay maintainer-only; native validation requires only existing VS C++ tools and Windows cmd.exe.

Live mentoring, real-file proposal import, application/source writes and Rescue/Maintenance automation remain blocked. Validation harness writes are explicitly not permission for runtime source writes. F4 acceptance is not full U4 completion. Further feature work must identify and satisfy the applicable external-input/isolation/approval gate rather than multiplying fixture stages.

## U-14 — consolidated offline foundation, 2026-09-22

Chris authorizes two remaining implementation/Windows-validation cycles, not more fixture stages. Accept F5 at 114c272. The combined package supplies a practical offline workspace: fixed lesson catalog, validated snapshot prerequisites, resume/review/report, prepared help, next real PandaBank learner mission and normal launch without suite reruns. One comprehensive validator runs retained U4 regressions and new checks; U1–U3 suites stay closed. A second integration package closes the baseline; further Windows tests require an actual failure.

Capability: fixed local course definitions plus existing append-only study progress. This trusted offline infrastructure has no provider, general shell, source-writing, proposal-application or Git-mutation interface. Recorded completion means evidence collected, not correctness, acceptance or mastery. U1 fingerprints stay unchanged. Missing, corrupt/recovered or mismatched prerequisites do not silently unlock lessons. Arbitrary mission paths cannot grant capabilities; no runtime course authoring or migration.

Mission 003 asks Chris to extract the existing ObjectType/name boundary with the accepted X-macro route in Visual Studio. No refactor, reference solution or PandaBank edit is supplied. Only two fixed learner-file read slots are added. Existing study-store authority permits responses, not source/definition writes.

Keep the C++20 console as the practical offline baseline. GUI/editor, CMake and automation are separate deferrals. Native context-only isolation remains unsatisfied. Normal launcher builds into fresh ignored staging and launches explicitly, not a self-update/activation service. No dependency/runtime language added. After the second passing run, accept the offline baseline and move to useful coursework without a third acceptance-only commit/package.
