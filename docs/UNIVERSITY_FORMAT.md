# University U1 data contract

Version 1 is a deliberately small quoted-record format, not JSON, an executable language or a general-purpose parser. Typed C++ definitions are in university/include/Mission.h; parsing and pure transitions in Mission.cpp; disk persistence in ProgressStore.cpp. The console depends on both; neither depends on PandaBank, SDL/ImGui, Git or an AI provider. It contains no reference solution or source patch.

## Mission definition (.pum)

One record per line. Double-quote all string fields. Spaces separate fields. `std::quoted` escaping applies: escape a quote or backslash with a backslash; `\n` is not a JSON newline escape. No multiline strings. UTF-8 bytes are accepted but not Unicode-normalized. LF and CRLF are supported; CRLF is normalized for the definition fingerprint. Blank lines and whole-line # comments are allowed. Tabs/control bytes are rejected.

```
mission 1 "mission-id" "revision" "Title"
requires "reviewed-prerequisite-id"
scope "read" "src/Existing.h"
step "unique-step-id" "Explain" "ack" "Step title"
why "Immediate purpose and mental model"
action "Exact action"
expect "Observable result"
more "Optional deeper explanation"
syntax "term" "Optional meaning"
professional "Optional approaches and tradeoffs"
hint "Optional prepared hint"
end
finish
```

Requires/scope records are optional and precede steps. Scope values are read/edit, safe repository-relative slash paths, unique across both lists. This is descriptive metadata in U1: the program has no file editing or execution capability. It is not a security sandbox for an eventual command runner.

Supported step types: Explain, Predict, Inspect, ShortAnswer, Review, Checkpoint. Explain/Checkpoint require ack. Predict/ShortAnswer require response. Inspect/Review accept either gate. ack completes on explicit next; response requires a nonblank recorded short answer before next. It does not grade correctness. why/action/expect are required once per step; more/professional once; syntax terms unique; at most seven hints. IDs/revision use lowercase ASCII letters, digits, hyphen or underscore, at most 80 bytes. Max 64 steps, 8192 bytes per line, 1 MiB per file. Unknown fields/types/versions, duplicate fields/IDs, missing terminators and trailing records fail with a line diagnostic.

The U1 wire schema does not yet carry executable commands, automated correctness evaluators, rescue patches, sandbox definitions, concept mappings or rollback actions. These remain requirements for a reviewed schema extension, not ignored fields. Header/source concept, read-only scope and no-edit rollback are explicit in the prepared lesson and course records. Unsupported Run/Edit/Debug/Sandbox steps are rejected. The console does not contain branches for individual lesson IDs.

## Pilot import (.puc)

```
pilot 1
completed "001_stable_object_identity" "course/evidence/001_review.md" "review provenance"
```

The explicit reviewer-authored index imports accepted pilot completions. The engine checks required IDs are present; it does not independently verify Markdown evidence, Git commits or authorship. U1 currently resolves prerequisites against this index only; intermission progression through runtime snapshots awaits a tested extension. Do not fabricate a completed entry to bypass a prerequisite.

## Progress (.pup)

```
progress 1 "mission-id" "definition-fingerprint"
record "step-id" 0 1
attempt "A short saved response"
finish
```

There is one record per definition step, in order. Values after the ID mean complete (0/1) and hints revealed. Zero or more attempt records follow response steps. Full snapshots retain all short attempts (max 100 per step, 240 printable bytes each) and hint counts. Complete steps must be a prefix; future steps cannot contain attempts/hints. ack steps cannot contain answers. A completed response step must contain evidence. The first incomplete step is the resume position.

The fingerprint is FNV-1a 64 over CRLF-normalized definition text, including comments/wording. It is a change detector, not a cryptographic integrity check. Changed definitions/revisions block reuse until explicit migration; the implementation provides no migration command. Evidence files remain readable in any text editor. A snapshot completion means prepared steps recorded, never C++ correctness, independent mastery, a Git commit or a completed product refactor.

## Save and recovery

The only normal runtime writes are `course/university/progress/<mission-id>/N.pup` snapshots and save staging/lock entries there. Answers, revealed hints and advancement save immediately. Read-only validate/display do not create progress. No timestamps, Git/build logs or dynamic confidence inference are collected in U1. Tests use temporary synthetic state, never Chris's real progress.

Writes acquire a directory lock, reject a stale generation, write/flush/close a new numbered .tmp file, then rename to a new snapshot in the same directory. Existing snapshots are never intentionally overwritten. Failed save leaves the displayed active step unchanged. Two cooperative instances cannot overwrite each other's saved generation; restart after a stale-session error. This is not an adversarial filesystem/TOCTOU security boundary or a power-loss durability guarantee; platform fsync and crash tests remain debt.

On load, an invalid newest snapshot is preserved and reported; the latest valid older one resumes. This may lose only the latest unvalidated progress, and the warning must remain visible. No valid existing snapshot means stop, not reset. Definition mismatch stops even if older snapshots exist. An interrupted .tmp is preserved, ignored and its sequence reserved. The loader rejects symlink traversal in the snapshot location and nonregular snapshot entries. A crash can leave a stale .save-lock; close all University instances, copy the progress directory to a backup, then remove only that empty lock directory. Do not delete snapshots as a repair shortcut.

For corrupted state, preserve the complete directory and restore a known-good snapshot in a separate copy for inspection; request a narrow recovery/migration if all copies are invalid. Do not change assessment content merely to make progress load. The directory has a 10,000-entry bound; archival/compaction needs an explicit future workflow before long sessions reach it. Back up course/university/progress and its matching mission definitions together; it is Git-ignored personal state and not automatically synced by this console.
