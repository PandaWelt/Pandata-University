# PandaBank Astra-Ready Workspace

This is a cleaned copy of the current PandaBank prototype plus the documents Astra needs to continue it safely. The original C++ source has not been repaired or reorganized in this package. The known vector and text-input defects remain intentionally available as the first guided learning mission.

## Start here

1. Keep your original PandaBank folder as a backup.
2. Extract this archive to a normal development path on the Windows PC.
3. Open the extracted folder as the Work/Codex local workspace, or attach the archive if a local workspace is unavailable.
4. Select GPT-6 Astra.
5. Paste the complete contents of `ASTRA_START_PROMPT.md` into the new session.
6. Let Astra inspect the repository and present its first-run plan.
7. Reject any plan that proposes rebuilding all of PandaBank or completing the first learning mission for you.

The existing Visual Studio solution is under `visualstudio/`. Its project file currently contains absolute paths from the original machine. Astra is instructed to preserve the baseline, then introduce a portable build without pretending the current build has been verified when its environment cannot run Visual Studio.

## Important documents

- `AGENTS.md`: persistent rules for every coding session.
- `docs/PANDABANK_PRODUCT_SPEC.md`: product vision and boundaries.
- `docs/COURSE_DESIGN.md`: learning loop and teaching behavior.
- `docs/CURRENT_STATE.md`: evidence-based audit of the uploaded prototype.
- `docs/ARCHITECTURE_RULES.md`: target structure and migration constraints.
- `docs/THIRD_PARTY_STATUS.md`: dependency and license gaps to resolve.
- `docs/ROADMAP.md`: chronological implementation and learning sequence.
- `docs/DECISION_LOG.md`: locked, provisional, deferred, and open decisions.
- `course/scaffold_manifest.json`: registry for temporary and accepted code.
- `course/PROGRESS.md`: durable cross-session status.
- `course/missions/001_stable_object_identity.md`: first guided repair mission.

## Core principle

There is one PandaBank codebase. The course dashboard, Visual Studio, automated tests, and release build must all operate on this same repository. There is no tutorial copy that must later be exported or merged into the real application.
