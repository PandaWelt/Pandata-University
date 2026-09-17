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
