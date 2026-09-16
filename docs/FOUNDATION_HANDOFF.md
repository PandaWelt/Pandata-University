# Foundation handoff — 2026-09-16

Completed: preserved original upload and Git source baseline; inspected all 139 uploaded files by inventory, read all application/build/course documents, and checked dependency headers/notices. Corrected CURRENT_STATE, refined ignore rules, and established mission/evidence/decision/scaffold records. Prepared the next learner mission (002) and a separate incremental CMake migration.

Only executable-source edit is a direct <cstdint> include. Native project edits are 14 repository-relative path-prefix replacements. No Mission 001 target code or third-party file changed. No future product feature implemented.

## Validation and limitations

| Check | Evidence / result |
|---|---|
| Original checkpoint | uploaded-baseline tag; exact ZIP hash and 139-file inventory; recovery instructions. |
| GCC application syntax | Four application translation units pass after include fix; existing warnings remain. docs/validation/ui-syntax.txt and main-syntax.txt. |
| Native project paths | XML parses; all 30 source/header/resource/include/library references resolve; only path prefixes changed. |
| Protected source | Mission target logic unchanged; every third-party file byte-identical to ZIP. |
| Bookkeeping / ignores | JSON parses; 12 representative ignore cases pass; CRLF-aware diff check passes. |
| Windows build/link/GUI | NOT RUN. Linux host lacks MSBuild/MSVC/Windows SDK/resource compiler. |
| CMake | NOT IMPLEMENTED. CMake absent; prepared migration includes explicit source/target/dependency mappings and Windows gates. |

Source inspection does not establish working text focus. Windows commands in docs/BUILD_WINDOWS.md are the pending verification path. Current ImGui WIP commit/core license notice and SDL binary provenance remain unresolved before release. No automated GUI suite or course dashboard exists, and release-policy flags are not yet an enforced build gate.

## First learner action

Open course/missions/001_stable_object_identity.md and write the prediction in course/evidence/001.md: trace a type-button click through SpawnButton → AppUISetup → spawn → drawAll. Predict vector size after several idle frames. Do this before editing the target code. Then build Debug/x64 using docs/BUILD_WINDOWS.md and attempt the mission. Share the prediction, attempted diff and observed result for focused feedback.

Mission 001 is not_attempted, not completed by infrastructure checks. Mission 002 (debugging/assertions) unlocks after its behavior is verified. No giant new backlog was generated.

## Files changed from uploaded baseline

- `.gitignore`
- `README_FIRST.md`
- `course/PROGRESS.md`
- `course/missions/001_stable_object_identity.md`
- `course/scaffold_manifest.json`
- `docs/CURRENT_STATE.md`
- `docs/DECISION_LOG.md`
- `docs/THIRD_PARTY_STATUS.md`
- `src/UI/AppUISetup.cpp`
- `visualstudio/Pandata.bank.vcxproj`

## Added files

- `course/evidence/001.md`
- `course/migrations/portable_cmake.md`
- `course/missions/002_debugging_invariants.md`
- `course/state.json`
- `docs/BASELINE_INVENTORY.json`
- `docs/BUILD_WINDOWS.md`
- `docs/FOUNDATION_HANDOFF.md`
- `docs/RECOVERY.md`
- `docs/validation/baseline-ui-failure.txt`
- `docs/validation/integrity.txt`
- `docs/validation/main-syntax.txt`
- `docs/validation/ui-syntax.txt`

Also delivered: local Git history and ignored recovery/original-upload.zip. The original SDL2.pdb remains present and excluded from Git.
