# U4-F2 — bounded in-memory proposal parser

Base: 6da8b60f51587ccbd491264d6b52186064f1261b, verified direct child of accepted F1 implementation bdc6232. Chris explicitly authorized implementation of the committed U4_F2_PREPARATION contract. U-09b records that scoped boundary. This package contains working code and focused validation, not another preparation assignment.

## Implemented

ProposalParser accepts a const JSON string and a separate const Snapshot, returning a Report. At most 65536 encoded bytes reach the unchanged JSON codec. Exactly eight required fields are accepted: numeric format and string id/path/baseRevision/changeClass/before/after/claim. Missing, unknown, duplicate, wrong-type, invalid UTF-8/escape, trailing and excessive-nesting inputs refuse. The codec's existing recursion bound is retained. The adapter adds exact decimal-value checking so floating-point rounding cannot admit an unsupported format.

Decoded proposals pass through unchanged F1 inspect: format 1, ID u4-f1, class fixture, logical label fixture/review-note.txt, bounded strings/revisions, exact before/revision match, escaped replacement display and unverified claims. The logical path is never opened. Proposal input cannot set the authoritative Snapshot. Parser errors return a fixed message without echoing raw input or provider diagnostics. Rejection/staleness leaves later valid calls unaffected.

Only compiled fixed demos are exposed, before repository/mission/progress initialization. Extra payload/path arguments and unknown --proposal-json flags reject immediately. There is no stdin/clipboard/file import, Git lookup, external process, AI, network, persistence, approval or apply route. Existing normal University/U2 behavior is separate and unchanged. Linking the full executable does not make it an OS sandbox. The parsed fixture retains the F1 display label deliberately; F2 adds decoding, not a new permission class.

The unchanged mentor/Json codec's namespace/header coupling is a documented tradeoff, not a provider call. No U3 source or tests, F1 policy/tests, PandaBank source, third-party files or learner snapshots were changed. Existing build scripts only gain the new source dependency; normal startup additionally checks F2. The standalone F2 validator does not run U1–U3 suites or require F1 reacceptance.

## Apply and immediately validate on Windows

1. Extract the delivered archive outside your repository. In PowerShell in its U4_F2 folder run:

       .\Apply-U4-F2.ps1 -Repository (Read-Host 'Existing PandaBank repository root')

   Touched-byte hashes, required 6da8b60 ancestry and Git preflight protect existing work. Unrelated dirty files are allowed. Conflicts refuse without reset/staging/commit/push. Keep affected files closed during application. The package manifest lists every changed path and its before/after SHA-256. Do not copy a replacement repository over your working tree.
2. In **Developer PowerShell for VS 2022**, at the existing repository root, run:

       .\Validate-Proposal-Parser.cmd

   No additional launch, install, API account or configuration is required. Missing cl.exe reports that the Developer shell is required. Builds use a fresh ignored build/university/parser-* directory; collision refuses replacement. Validation compiles the focused tests and the actual University executable. Compiler activity creates outputs/processes; the runtime parser/preview performs no file writes or child launches.

## Exact expected validation sequence

Compiler output/warnings may appear between these steps. A failure stops at its first failed command.

| Step | Expected result | Exit |
|---|---|---|
| Preserved F1 dependency regression | PASS: 33 U4-F1 checks (in-memory preview only) | 0 |
| New parser suite | PASS: 128 U4-F2 checks (in-memory parser only) | 0 |
| Full University compilation | Build succeeds | 0 |
| --proposal-json-fixture | DISPLAYED: U4-F1 in-memory fixture only; exact before/after | 0 |
| --proposal-json-malformed | REJECTED: invalid or unsupported serialized fixture proposal | 1, expected |
| --proposal-json-stale | STALE: supplied revision or before-bytes changed | 1, expected |
| --proposal-json-fixture again | DISPLAYED again; previous refusals did not change the fixture | 0 |

Final line of sequence:

    PASS: U4-F2 valid / rejected / stale / valid; exits 0 / 1 / 1 / 0.

The two exit-1 demos are deliberate successful rejection tests, not a reason to repair progress or rerun old stages. Return both PASS counts and the final sequence result, or the first compiler/test failure. There is no need to repeat U1–U3 or the completed F1 Windows assignment. Runtime demos intentionally save no evidence/progress; report the console result.

## Validation observed here

Linux: 128 parser checks, 33 unchanged F1 dependency checks, complete University C++20 -Wall -Wextra -Werror build; actual focused flags returned 0/1/1/0 in an empty directory with empty PATH, followed by safe rejection of extra/unknown arguments. Original source/history/progress/evidence bytes outside the declared edits were preserved. Object-symbol/source inspection shows pure codec/F1/standard-library dependencies for the adapter, no filesystem/process/provider/store calls. No syscall-trace or native Windows sandbox proof is claimed.

AddressSanitizer/UndefinedBehaviorSanitizer checks pass with leak detection disabled. LeakSanitizer could not operate under this host's tracing restrictions; leak checking is not claimed passed. Native Windows/MSVC remains pending your local run. Machine-readable reviewer evidence: validation/u4-f2.json. The PowerShell apply wrapper is statically reviewed; native execution is not claimed. Patch application is tested on a disposable input copy.

## After your result

Review and record returned Windows results. If clean, combine that acceptance with the next safe directly testable implementation in one delivery, rather than a separate design-only patch cycle. Do not broaden into file access, live mentoring, writes, apply, Rescue or Maintenance automation without their applicable reviewed safety gates. No next-stage implementation or full-U4 completion is claimed by this F2 delivery.
