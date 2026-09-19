# Current recovery — Mission 002 / University transition

The byte-exact submitted Mission 002 ZIP is retained in `recovery/mission002-submission.zip`, SHA-256 `6306638d8c0ff6131ebe894c2dcd19d4b81e6c3b01093613208b4aea8ed8b911`. `UNIVERSITY_BASELINE.json` records every input file hash. Chris reports commit `21ba2af`; no .git was included, so that commit and the working branch cannot be independently verified. No replacement history or commit was fabricated.

The returned archive is a source/document transfer, not a new repository to replace Chris's checkout. Extract to a separate temporary folder. In the existing repository inspect `git status --short` and `git log -1 --oneline`; keep all newer work. Apply only added/modified paths listed in UNIVERSITY_CHANGED_FILES.md. Compare and merge any locally edited file instead of overwriting it. Do not copy the bundled src/, thirdparty/ or visualstudio/ over newer files; they are unchanged reference bytes. Never replace .git. Recovery archives are not source commits.

The changes are docs/course/reviewer records, AGENTS.md, .gitignore, README_FIRST.md, University source/tests and its new launcher. They contain no product patch. U1 writes only local course/university/progress snapshots during study. Copy that directory with its mission definitions into normal backups; it is ignored by Git. See UNIVERSITY_FORMAT.md for corruption, locking and migration handling.

Rollback of this handoff means selectively reverting its reviewed files in your own repository, preserving later edits and all learner progress. Stop University before removing its executable/source. Keep the submitted ZIP and local Git history; do not use hard reset or overwrite from the old foundation. Content changes cannot silently reuse previous completion fingerprints.

## Historical Mission 001 provenance (superseded transfer instructions)

The older text below records the earlier review only; its changed-path list and next action do not apply to this University delivery.

# Recovery and submission provenance

Chris reports Mission 001 is committed locally with message `Complete Mission 001 stable object identity`. The submitted ZIP contains no .git or hash, so the reviewer cannot independently identify that commit. Do not substitute a reviewer-made snapshot or an old foundation history for Chris's actual Git history.

Submission SHA-256: `d9b7fd8ed2f3428c60901548a4ad831bb6524fd5d29546fd6e3f884244244217`.
Application source was kept byte-for-byte during review. The original submission remains the review baseline; no source rollback or implementation was performed.

This archive, like the submission, is a source/document transfer. To bring the review back into the real repository, copy the updated course/ and docs/ folders and README_FIRST.md into the matching locations in Chris's existing working folder. Those are the only changed areas. Keep src/, thirdparty/, visualstudio/ and .git as they are. If documentation has newer local edits, compare and merge those instead of overwriting them.

Historical foundation references (not included in this ZIP):

- uploaded-baseline: 3b9a6b7fac5dd9f53c0876dd90922197e39fc910.
- foundation-ready: c6eeaf03507fdc50086fddba3c5009c8ee4b24fe.
- Original foundation recovery ZIP SHA-256: 2899048eeb00fd44b202ffdf7ed590710559cdc01837796eccea42aeaac08def.

The earlier foundation package contained .git and recovery/original-upload.zip; this later transfer does not. Do not infer loss of local history from their omission. Work in the existing repository and use git status / git log there. If desired, record `git rev-parse HEAD` before committing the review-document changes; no need to resend or redo Mission 001 merely to supply a hash. Never use hard reset as a normal course step.

## U2 checkpoint and merge safety — 2026-09-18

Authoritative U1 submission SHA-256: 9ba26175c2a021f1da0f9f1890e0accd539ff995c1646fa3f83e02690eab3f24; Git HEAD 74aef5e present. Original uploaded ZIP is unchanged. All U1 snapshots and Git files are retained byte-for-byte. The U2 delivery contains only changed/new files and a Git patch; never overwrite the whole checkout. Follow its APPLY_U2.md, which checks before applying and stops on conflicts. Do not reset, clean, replace .git or delete progress. Back up new action evidence with progress. The old source-only submission limitation above no longer applies to this U1-complete upload.
