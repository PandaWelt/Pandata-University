# U4-F2 preparation — serialized proposal validation in memory

Status: **design only; not implemented or authorized by this records review**. Prerequisite: accepted U4-F1 at bdc6232. This is one University engineering step, not a PandaBank refactor or learner worksheet. U4-F1 need not be repeated.

## Why this is the next smallest step

F1 starts with an already-constructed Proposal. It cannot yet reject a malformed serialized document at the boundary where proposal data will eventually arrive. Add only that missing conversion: supplied string -> strict validation -> existing F1 inspect -> display or refusal. Keep both the input and the authoritative fixture snapshot in memory. This makes parsing failures reviewable before any file-import or application capability exists.

## U-09 isolation analysis, before any implementation

U-09a explicitly authorized F1 only and excluded an import parser. It must not be silently read as permission for F2. A future scoped implementation request should explicitly adopt this pure-data boundary before adding code.

A bounded deterministic parser with no I/O, callbacks, provider or action interface does not require native Codex read isolation: it cannot retrieve context or acquire file/process/write authority. Its required safeguards are strict parsing, bounded allocation/recursion, fixed fixture policy, safe diagnostics, escaped display and preservation tests. Malicious strings remain data; they may cause rejection but never select an operation. This is a capability-specific engineering recommendation, not proof that native isolation works or that all of U4 is unlocked.

Any file reader, stdin import, clipboard integration, live-generated proposal, Git resolution, command, persistent state or application path is outside F2. Stop for a separately reviewed capability boundary before adding one. External execution/live mentoring still requires the unresolved exclusive context-only read isolation; writes/automation also retain their exact-diff approval and other safety gates. Preparation does not satisfy those gates.

## Proposed contract

- One pure adapter takes a caller-owned UTF-8 JSON string and a supplied Snapshot by const reference; returns a rejected/stale/displayed Report. No executable callback, repository pointer, stream, path resolver, provider or progress access.
- Maximum encoded input: 64 KiB, checked before parsing. One flat object with exactly eight keys: format, id, path, baseRevision, changeClass, before, after, claim. No missing, extra or duplicate keys, trailing input or type coercion. format is JSON numeric value exactly 1; all seven remaining values are strings. Equivalent JSON number spellings for 1 are acceptable; no conversion by truncation.
- Reuse the existing pure mentor/Json.cpp codec unchanged if dependency inspection confirms it remains data-only. It already rejects duplicate keys, invalid UTF-8/escapes, trailing data and excessive nesting. Translate its provider-specific exception into a fixed proposal rejection; never echo raw malformed input or call Mentor/Channel. Do not refactor or change U3 to share it during this slice. Tradeoff: temporary namespace/header coupling avoids a second JSON implementation or new dependency.
- On decoding success call the unchanged F1 inspect function. Retain format 1, ID u4-f1, class fixture and logical path fixture/review-note.txt. F2 names the engineering increment, not a new authorization class. Retain all decoded F1 size and revision bounds. A JSON document cannot supply or overwrite the separately supplied current Snapshot.
- No permission, approval, command, source-origin or machine-validation fields are accepted. A claim stays unverified. No actual Git commit/hash verification is implied. Before/after strings use valid UTF-8 JSON encoding; decoded control bytes must still be escaped by F1 display.
- Expose at most focused compiled serialized-fixture demos (valid, malformed, stale, then valid). They must run before mission/progress initialization, with no arbitrary command-line payload/file argument. No editor, import chooser, approve/apply control or progress/evidence writer.

## Acceptance for the later implementation

1. Valid serialized fixture displays the same exact replacement as F1. Changed supplied snapshot produces stale; fresh input afterward succeeds.
2. Reject missing/extra/duplicate keys, wrong types/format, invalid UTF-8/escapes, trailing data, excessive nesting and encoded input above 64 KiB. Check exact encoded/decoded bounds, including escape expansion and embedded NUL.
3. Existing F1 policy rejects changed path/id/class/revision and oversized decoded fields. Strings containing shell syntax or fake test-pass claims stay escaped/literal/unverified; no execution or approval results.
4. Invalid input leaves caller data unchanged, does not leak a partial proposal into the next request, and emits fixed safe diagnostics. The independent Snapshot cannot be forged through proposal keys.
5. Focused F1 regression plus new parser checks and full University build; empty working directory/PATH and source/progress/evidence byte preservation. Record actual platform results. Windows focused validation must avoid U1–U3 reruns and exercise valid/rejected/stale/valid.
6. Review source/link dependencies: no filesystem/process/network/provider/store capability in the new adapter. Compiler writes are outside the runtime guarantee. Do not claim syscall observation or OS isolation without evidence.

Likely owned code: one small proposal parsing adapter and its focused tests; additive focused main/validation integration only if necessary. Existing ProposalPreview policy, U2/U3 code/tests and all PandaBank source stay unchanged. If unchanged codec reuse cannot meet this contract, document the concrete gap and stop rather than expand the task into a parser/framework migration.

## Exact next action

Apply the records-review package. Then read this brief; no source edits or stage reruns are assigned. A subsequent explicit request may authorize **U4-F2 in-memory parsing only** under the boundary above. It must not authorize file import, live mentoring, proposal application, source writes, Rescue or Maintenance automation.
