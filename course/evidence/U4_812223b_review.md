# U4-F4 acceptance at 812223b — 2026-09-22

Verified submitted Git HEAD 812223b0052dcbf5fa0833516a458a3d723aebd6, direct parent d35d5529a9541ba029f82a76ca86cae07e445c53. Local origin/main matches; remote push is Chris-reported, not independently fetched. No history was rewritten.

Contract review: ReviewControls accepts only four exact <=32-byte commands, selects only the compiled fixture, delegates revalidation/dismissal to unchanged F3, refuses apply and malformed input without changing selection, and exposes no file/Git/process/provider/store interface. Main routes the existing review prefix into this adapter; the normal console still loads its pre-existing mission/progress environment. Focused demos return before normal startup. The claim is no review-operation side effects, not a no-I/O executable.

Chris reports Windows 33/128/58/48 checks, demo/refusal/fresh exits 0/1/0, automated and manual routing, no saved review progress/evidence, and no U1–U3 rerun. C4310/C4996 were non-blocking. The uploaded ignored build artifacts corroborate the transport diagnosis: controls-1310-25436 and controls-19507-5631/console.txt have seven review refusals; controls-1843-7955/console.txt has the expected display/dismiss flow and one intentional apply refusal. Its console-input.txt contains exactly eight expected ASCII commands with CRLF and no trailing spaces. Failed raw stdin was not captured, so its exact host bytes remain unmeasured.

The delivered-F4 versus committed validator repair is recorded in docs/validation/u4-f4-validator-repair.diff (line endings normalized for presentation). The substantive change is eight separate prefix-redirection echo( lines and stdin file redirection, replacing the grouped pipeline. Committed repair lines used LF among older CRLF lines; the next validator normalizes its generated batch file to CRLF.

Reviewer reran 48 focused F4 checks successfully on Linux and reviewed the committed code/contract. Windows results remain attributed to Chris and submitted captures. F4 is accepted; this is not live mentor or full U4 acceptance. No learner mastery, runtime progress, prior evidence, product source or Git metadata changed.

Next directly testable slice: U4-F5 validator transport verification, authorized by U-09e. This closes the demonstrated native harness gap without adding runtime features. See docs/U4_F5_IMPLEMENTATION.md. Live isolation, provider, application, source writes and automation gates remain unsatisfied.
