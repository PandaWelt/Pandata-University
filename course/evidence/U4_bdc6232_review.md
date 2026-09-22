# U4-F1 review — 2026-09-22

Disposition: **Accept the offline fixture only.** This is supplied University infrastructure, not a learner C++ mission or proof of mastery.

## Commit and evidence provenance

Submitted HEAD/main: bdc62321d2967fc90d292b451988d64f4ae57992, direct parent 6c6286d158337bc62c4bd2f10bffd212020fcffe, whose parent is accepted U3 checkpoint 330420d. Local origin/main matches HEAD. GitHub push is Chris-reported; no remote query was performed. All 17 committed paths match the described F1 implementation and records. No PandaBank source changed in this commit. Tracked working files match HEAD; two unrelated untracked areas were preserved.

Chris reports Windows PASS: 33 U4-F1 checks, preview displayed, deliberately stale proposal rejected, then fresh preview displayed; preview left files, Git state, processes, AI context, progress and evidence unchanged. U1–U3 were not rerun. No separate raw Windows console log or Windows machine result is present in the submitted validation records. docs/validation/u4-fixture.json is the earlier Linux evidence, not a Windows result. This review records the report separately and does not rewrite it into machine-observed evidence.

Reviewer observed on Linux: focused 33-check suite and full University C++20 warnings-as-errors build passed; isolated focused preview/stale/preview returned 0/1/0 with empty PATH and no files created in the empty working directory. U1–U3 suites were not run. Windows/MSVC was not run by reviewer. No runtime syscall tracing was performed; absence of child-process/file/provider calls is supported by source inspection, not a claimed native sandbox measurement.

## Contract verification

| U-09a / F1 requirement | Implementation and review |
|---|---|
| Supplied immutable values only | ProposalPreview.h exposes values and reports; no repository, store, provider, process or callback interface. |
| Fixed fixture scope and bounded content | inspect rejects unsupported format/id/class/path; 4096-byte before/after/current, 256-byte claim and 64-character revision limits. |
| Stale invalidation | Exact before-bytes and supplied revision must match; stale output does not display a replacement. Temporary stale snapshot cannot alter the next invocation. |
| Safe, honest display | quotedBytes escapes controls and non-ASCII bytes; replacement is literal. Claims are unverified, revision is not a Git check, displayed conveys no approval. |
| No I/O or execution route | Focused main branch returns before mission/progress/repository initialization. Console branch prints a report and continues without saving. No target path resolution or parsing/execution of body text. |
| Existing controls preserved | U2/U3 implementation and tests unchanged; old launcher checks retained. Normal console/U2 commands keep their prior capabilities; no whole-process sandbox claim. |

The compiler/validation launcher necessarily creates build outputs and runs executables. The no-write/no-child-process claim belongs to the runtime preview operation, not its compiler or the whole University launcher. Standard runtime/output behavior is not asserted absent.

U1 snapshots, prior runtime evidence, learner profile/concepts, missions, third-party and PandaBank source are unchanged by this review. The existing U2 junction automation skip is still skipped. The F1 scaffold stays review_scheduled for maintainer ownership; passing fixture tests does not confer learner mastery.

## Next boundary

Prepare U4-F2: decode a bounded serialized proposal supplied in memory, reject malformed/unsupported data, then use the existing F1 comparator/renderer. Design only; no implementation is included. See docs/U4_F2_PREPARATION.md. U-09a explicitly excluded a parser, so this preparation is not an automatic extension of its authorization. Live U3, file imports, application, writes, Rescue and Maintenance automation remain blocked by their applicable gates.
