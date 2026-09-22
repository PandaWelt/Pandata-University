Accepted 2026-09-22 at d35d552. Windows results are Chris-reported; Linux contract checks corroborate them. The original package apply allowlist had a generator defect corrected by Chris outside the repository. That installer was not validated by native execution here; prior patch-only checks did not catch its scope mismatch. See U4_F4_IMPLEMENTATION.md for the permanent packaging correction and next validation. Earlier F3 pending/apply instructions below are historical.

# U4-F3 — one transient proposal-review session

Base: verified 6b8081da77d0413abd1bd5516db9b9c0c44eebf7. F2 is accepted from committed-source verification, focused Linux checks and Chris's Windows report. This complete package combines that acceptance with the next directly testable implementation. U-09c documents the boundary before code was added.

## What this adds

F2 parses/previews a single call. F3 owns the pending-review lifecycle anticipated in U4_PREPARATION: select one proposal, explicitly preview it, dismiss it, or invalidate it when the supplied snapshot/replacement is no longer valid. It prevents a view from silently retaining a previous displayed proposal after failure. This is a real review controller, not another parser or an approval engine.

| Action | State/result |
|---|---|
| New session | empty; preview reports NO ACTIVE PROPOSAL |
| select(valid document, supplied snapshot) | proposed; previous selection removed, no replacement view yet |
| preview(current supplied snapshot) | F2 revalidates; displayed only when it still matches |
| changed revision/bytes | stale; selection discarded |
| invalid snapshot or malformed replacement | rejected; old selection discarded |
| restore old bytes after invalidation | still no active proposal; explicit reselection required |
| dismiss | dismissed; selection discarded; repeated dismiss is harmless |
| explicitly select again | proposed; preview required again |

One private owned document, at most 64 KiB after F2 validation, survives only for the lifetime of the session. There is no history list or saved report cache. Replacing even with another valid proposal resets displayed to proposed. F2/F1 code and tests are unchanged. Caller data is const; ownership copies prevent subsequent caller edits from changing the selected proposal behind the view.

The state getter describes the last operation, not current freshness: there is no file watcher or current snapshot lookup. Every preview requires supplied snapshot data. Returned text is advisory historical output, never an approval token. Clearing selection prevents reuse through this API; it is not a claim of secure memory erasure or erasing text already printed.

## Boundaries

Fixed compiled demos only, before any mission/progress/repository initialization. Reject extra payload/path arguments and unknown --proposal-review flags. No arbitrary stdin input/import, file or Git access, process launches, provider/AI, persistence, approval/apply API or callback is introduced. Transient memory state changes do not modify source or learner progress. Existing normal University/U2 commands retain their existing capabilities; this is not a whole-process sandbox.

The U-09a/b reasoning applies to this pure-data controller: it acquires no external retrieval/execution authority, so it does not need the unavailable Codex isolation backend. That is not a waiver for a future file import, live mentor, write, application, Rescue or Maintenance path. Those gates remain unsatisfied. No PandaBank feature or learner mission answer is implemented.

## Apply and validate on Windows

1. Extract outside the repository. In PowerShell in the extracted U4_F3 folder:

       .\Apply-U4-F3.ps1 -Repository (Read-Host 'Existing PandaBank repository root')

   The script requires 6b8081d ancestry, checks exact touched bytes and reparse paths, then preflights the patch. It allows unrelated dirty files, never stages/commits/resets/pushes, and refuses conflicts. Keep touched files closed while applying. manifest.json and CHANGED_FILES.md enumerate the exact edits.
2. In **Developer PowerShell for VS 2022**, at the existing repository root:

       .\Validate-Proposal-Review.cmd

   This single command compiles/tests dependencies plus F3 and the actual University, then runs the demos. No extra launch or previous-stage replay is assigned. Missing cl.exe gives a Developer-shell error. Build outputs go to a fresh ignored build/university/review-* directory. Compiler/installer disk writes and process launches are outside the runtime review guarantee.

## Exact expected results

| Step | Expected result | Exit |
|---|---|---|
| Unchanged F1 dependency | PASS: 33 U4-F1 checks (in-memory preview only) | 0 |
| Unchanged F2 dependency | PASS: 128 U4-F2 checks (in-memory parser only) | 0 |
| F3 tests | PASS: 58 U4-F3 review-session checks | 0 |
| University build | Compilation succeeds | 0 |
| --proposal-review-demo | PROPOSED -> DISPLAYED -> DISMISSED -> NO ACTIVE PROPOSAL -> PROPOSED -> DISPLAYED | 0 |
| --proposal-review-stale | PROPOSED -> DISPLAYED -> STALE -> NO ACTIVE PROPOSAL, even after restoring original snapshot | 1, expected |
| --proposal-review-rejected | PROPOSED -> DISPLAYED -> REJECTED -> NO ACTIVE PROPOSAL | 1, expected |
| --proposal-review-demo again | Full fresh lifecycle succeeds | 0 |

The initial DISPLAYED inside each failure demo is intentionally shown as chronological output before invalidation; it is not a still-active proposal. No repeated replacement view appears after STALE/REJECTED. The successful preview retains the F1 label because the underlying fixture policy is unchanged.

Final sequence line:

    PASS: U4-F3 lifecycle / stale / rejected replacement / fresh lifecycle; exits 0 / 1 / 1 / 0.

Send all three PASS counts and that final result, or the first failure. The two exit-1 cases are deliberate. U1–U3 suites are not run; F1/F2 dependency checks do not reopen accepted assignments. Runtime demos intentionally save no progress or evidence.

## Observed validation and limits

Linux: 58 F3 checks, unchanged 33/128 dependencies, complete University C++20 warnings-as-errors build, expected 0/1/1/0 demo outputs plus extra/unknown argument refusal in an empty working directory/PATH. F2 entry sequence remains 0/1/1/0. Source and object imports show only F2/F1/string/memory/exception dependencies for the controller; no file/process/provider/store calls. No syscall trace or native sandbox proof is claimed. ASan/UBSan pass with leak checking disabled due the known host limitation; leak detection is not claimed passed. See validation/u4-f3.json.

Windows/MSVC and the PowerShell wrapper are not executed by the reviewer. The wrapper is statically reviewed; patch preflight/application, delivered-source tests, exact postimage hashes and preservation of all untouched original files (including Git/progress/evidence) are tested on a disposable input copy. Old build scripts receive only needed source/test wiring; U2/U3/F1/F2 policies/tests are not weakened.

No full U4 completion or live mentoring is claimed. On clean Windows results, combine acceptance with the next safe directly testable slice where authorized; do not create another records-only round trip unless a real gate or decision prevents implementation.
