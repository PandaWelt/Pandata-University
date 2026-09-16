# PandaBank — foundation handoff

The first foundation pass is complete. This archive contains the same prototype, Git checkpoints, the original upload for recovery, and a prepared learning loop. Mission 001 has not been solved or attempted for you.

1. Extract to a development folder; do not overwrite a folder with newer changes. This extracted repository becomes your working PandaBank source in both Visual Studio and the course.
2. Read `course/missions/001_stable_object_identity.md` and write its prediction in `course/evidence/001.md` before editing code.
3. Follow `docs/BUILD_WINDOWS.md` to build Debug/x64 and put SDL2.dll beside the executable. Windows build/launch is still awaiting local verification.
4. Attempt only the mission's allowed regions. Share your prediction, attempted diff, first error or observed result for feedback. Saved hints are available offline.

Current audit: `docs/CURRENT_STATE.md`. Session state: `course/PROGRESS.md` and `course/state.json`. Rules: `AGENTS.md` and `docs/DECISION_LOG.md`. Recovery: `docs/RECOVERY.md`. Validation and changed files: `docs/FOUNDATION_HANDOFF.md`.

Only application source change: direct `<cstdint>` include. The existing Visual Studio x64 paths are now repository-relative. The spawning/input target code and all dependencies are untouched. GCC syntax checks passed with warnings; this is not a successful Windows build or GUI test.

Next learner mission after 001: debugging/assertions (002). CMake migration is prepared separately and not implemented. Product vision remains in `docs/PANDABANK_PRODUCT_SPEC.md`; it is not a statement of existing features. No dashboard or export layer exists: you edit ordinary C++ files in this repository.
