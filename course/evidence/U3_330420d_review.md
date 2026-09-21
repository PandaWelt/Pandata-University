# U3 cumulative Windows safety validation review — 2026-09-21

Disposition: **accept the completed cumulative compatibility/safety validation; live mentoring remains blocked**. This is not a full live-U3 acceptance or an authorization to implement U4.

- Source archive HEAD: 330420d36035c71777ff2c1cfeb8f2dbbf714be9; tracked worktree bytes match that commit.
- Ancestors 1cbff0c, 947bc85 and 7e4789a are present. Main, origin/main and the U3 tag in the supplied bundle identify the same checkpoint.
- Bundle verification reports complete history; independent bare import and git fsck --full pass. Its main tree equals source HEAD tree ee2f445d46ddca4bf80a41bd16da047092beeeff.
- Six local U3 evidence records are supplied. The newest, course/university/evidence/u3/17900172827024590-0/evidence.json, records Codex 0.155.1, experimental named profiles, contextOnlyIsolationAccepted=false, modelRequestSent=false, an offline result, and empty context/answer/thread/turn/model/plan.
- Source inspection confirms checkSchema rejects this profile before spawn/discovery and before interact. The saved record is application-generated evidence, not an independent packet trace. Together with Chris's observation it supports the no-session/no-context result; it is not proof that native isolation works.
- **86 U3 checks passed on Windows: user-reported.** The archive has no separate raw Windows console test log. Existing checked-in Linux evidence is not relabeled as Windows evidence. No native build, login or model call was performed by this reviewer.
- All nine U1 snapshots remain present; 9.pup records completed orientation. No learner response, concept confidence, mission definition or completion flag is changed by this review. Chris reports source/progress unchanged by mentor-status; the current record/source support that behavior, but no independent before/after Windows runtime hash trace is supplied.
- Git comparison from U2 to this commit shows no changes to PandaBank src/, thirdparty/ or its native project file. Two pre-existing untracked areas remain unrelated and untouched.
- GitHub backup succeeded by Chris's report. A GitHub origin URL and matching local origin/main exist; the remote was not contacted, so remote durability is not independently certified.
- Windows U2 junction automation remains skipped because creation was denied; not passed.

The exclusive disposable-context read requirement remains unchanged. Authentication, Plus allowance and a live answer have not passed this blocked run. Do not reinstall Codex, purchase credits, weaken permissions, or repeat the completed diagnostic to close this review.

Next chronological stage: U4. Only the offline proposal-review design in docs/U4_PREPARATION.md is prepared. U-09's tested-isolation prerequisite still blocks implementation. No Rescue, apply/merge, hot reload, autonomous edits, paid fallback or PandaBank feature is supplied.

Machine-readable review and evidence hashes: docs/validation/u3-330420d-review.json.
