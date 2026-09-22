# U4-F1 — bounded, in-memory proposal preview

Base: 6c6286d158337bc62c4bd2f10bffd212020fcffe, verified records-only descendant of accepted U3 checkpoint 330420d. See U-09a in UNIVERSITY_ADRS.md for the capability-specific prerequisite resolution, documented before code was added.

## What works

Inside University: **proposal-preview** displays one supplied in-memory replacement proposal. **proposal-stale-demo** changes a temporary snapshot, then refuses the stale proposal. A following preview succeeds again. Neither command opens a target file, queries Git/Codex, starts a child, calls a provider, records evidence, saves progress or applies anything.

The fixed logical path is fixture/review-note.txt. It is a label; no such file is created or opened. The supplied fixture-r1 revision is explicitly not a verified Git commit. Exact before-bytes and revision are compared on every call. Before/after data are capped at 4 KiB each, claims at 256 bytes and revisions at 64 characters. Only format 1, ID u4-f1 and class fixture are accepted. Out-of-scope identities/paths/classes refuse. Display uses a full replacement view with escaped quoted bytes; no diff parser or patch application exists. Control bytes, newlines, NUL and non-ASCII bytes are visibly escaped. Claims always remain labeled unverified.

This is the first runnable part of the proposal-review boundary, not the full earlier U4 design. File import, UTF-8 document parsing, SHA-256 bindings, real Git snapshot resolution and filesystem/reparse checks remain unimplemented. Direct byte comparison is sufficient for this bounded fixture and avoids inventing a hash or I/O subsystem. No mutable caller state or action callbacks enter the preview module.

## Why U-09 does not block this slice

The Codex/native isolation prerequisite protects against external execution and context retrieval. This trusted, bounded string comparator/renderer receives only supplied values and has no interfaces for those capabilities. Its tests and dependency boundary are appropriate for its actual operations; an OS sandbox would add no necessary capability protection to this slice.

That conclusion is scoped to this fixture, not a claim that the whole University process is sandboxed. Normal University startup still reads its definitions/progress and explicitly selected U2 commands retain their existing authority. The focused --proposal-fixture entry point runs before any mission, repository or progress initialization. The preview module links only standard string/memory/exception support in the observed object-symbol inspection.

Live mentoring, source writes, proposal application, automated Rescue/Maintenance, arbitrary commands, hot reload and executable activation remain blocked/unimplemented. Their native isolation and approval requirements have not changed. Any new file/provider/process/write interface requires a separate review before implementation; this fixture cannot be used as an approval token.

## Windows application and focused validation

1. Extract the package outside the repository. In PowerShell in the extracted U4_Fixture folder, run:

       .\Apply-U4-Fixture.ps1 -Repository (Read-Host 'Existing PandaBank repository root')

   The installer requires 6c6286d ancestry, checks touched bytes and rejects conflicts. It neither stages nor commits nor resets.
2. Open **Developer PowerShell for VS 2022** in the existing repository and run:

       .\Validate-Proposal-Preview.cmd

   This builds only the 33 new checks and the actual University executable, then exercises focused preview/stale/preview with expected exits 0/1/0. It does not rerun U1–U3 suites, start Codex or load learner state. Compiler outputs go to a fresh ignored build/university/preview-* directory; the runtime preview itself writes nothing.
3. Expected final line: PASS: preview / expected stale rejection / preview. Keep the first compiler/test failure if any. Native Windows/MSVC execution was not available to the reviewer.
4. The normal Start-Pandata-University.cmd now also compiles the preview and runs its checks while retaining every existing check. There is no need to rerun that full launcher for this focused validation. Once using that normal console, proposal-preview and proposal-stale-demo expose the same implementation.

## Observed validation and limits

C++20 warnings-as-errors builds passed for the focused tests and complete University. 33 focused checks passed. From an empty directory and empty PATH (no Git or Codex), the actual University's focused entry points returned 0/1/0. Console preview/stale/preview and quit/relaunch preserved every copied source/Git/progress/evidence byte; completed U1 remained completed. No persistent fixture record was written.

Syscall tracing was attempted but denied by host ptrace permissions; no syscall absence claim is made from that attempt. Object-symbol/source inspection found no file/process/network/provider dependency in the preview module. These are bounded client/fixture tests, not proof of a native sandbox. Reparse tests are not applicable to a module with no filesystem lookup; the prior Windows U2 junction skip remains skipped, not passed. U1–U3 suites and live mentoring were not rerun.

Machine-readable evidence: validation/u4-fixture.json. No application features or learner mission answers were supplied. Next gate is Chris's focused Windows validation of U4-F1; full U4 is not complete.
