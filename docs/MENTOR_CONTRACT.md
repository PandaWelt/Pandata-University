# Mentor and Maintenance contracts — U3/U4, not implemented

U1 has no provider process, credentials, network client or patch execution. All online/maintenance operations are unavailable; progress survives independently.

## Verified documentation, not runtime evidence

Official App Server documentation describes default stdio JSONL, initialize/initialized, account/read, managed ChatGPT login, account/rateLimits/read and model/list. It distinguishes Codex-managed ChatGPT authentication from API-key and externally managed tokens. Recheck the installed version's generated schema before U3 implementation. [Official App Server documentation](https://learn.chatgpt.com/docs/app-server).

Official authentication documentation distinguishes ChatGPT subscription sign-in from usage-based API-key access. This supports choosing subscription authentication, not a promise that every model/amount of usage is included for this account. [Official authentication documentation](https://learn.chatgpt.com/docs/auth).

## Provider boundary

A later platform adapter exposes availability/account summary, discoverModels, explain/review/hint/compare, and cancellation/events to the application controller. The mission domain receives only advisory text and explicit request/evidence metadata. Provider requests cannot mark a step passed, modify a file, alter a gate or commit. Context bundle: mission/step, allowed relevant excerpts, current relevant diff, latest diagnostics, attempts/hints, learner preferences, accepted decisions/confidence/rules; omit unrelated source and personal data.

Launch installed `codex app-server` as a local child process, using default stdio. Keep stderr out of JSONL parsing. Initialize protocol before starting mission threads/turns. Version-check generated schema and supported fields. One reconstructable thread per mission; save references independently. Reject unsupported transports; no experimental WebSocket transport.

Codex alone performs browser/device-code ChatGPT authentication and stores/refreshes tokens. University never opens, copies or displays token files and never implements external-token refresh. Do not accept an API key field or inherit a paid-provider fallback as a convenience. Read account state through protocol; permit only the intended managed ChatGPT mode and eligible plan. API key/external tokens/unknown mode → offline.

Read rate limits and model catalog before requests; subscribe to account/limit changes. Preferences Terra (ordinary), Luna (narrow), Astra (explicit Deep Review) are user-facing choices, not evidence of available model IDs. Show actual selected provider model and availability/reset info when returned. Unknown quota/cost boundary or unavailable requested model must stop the online action without substituting a paid path. Never call purchase, add-credit nudge or reset-consumption operations automatically. This application cannot promise account-wide zero charges from unrelated external usage; its own adapter must have no purchase/API billing path and must remain offline if included allowance cannot be established.

Read-only mentor is a capability boundary, not merely a prompt. U3 must verify supported sandbox/approval settings with installed schema; deny write/command approvals, disable unrelated tools/connectors and test attempted writes. Preserve relevant source/diffs before calls. If the environment cannot enforce read-only behavior, Ask Mentor remains unavailable.

## Maintenance request contract

Request: user outcome + explicit mode + change class (content/engine/PandaBank/protected policy). Proposal: base commit and dirty-state inventory + owned paths + exact diff + model attribution + validation results + rollback + approval state. Pending is never applied. Explicit user approval binds to the exact diff/base; if files/base change, invalidate approval and regenerate review. Tests/gates/policy may not be relaxed to hide failures. Protected changes require separate explicit elevated acknowledgement identifying changed safeguards.

Use isolated branch/worktree when practical; never overwrite unrelated dirty files. Show readable status/diff/checkpoint history. No meaningful automatic merge or commit. After approval, apply only owned changes and make the explicitly approved checkpoint. Log what/why/model/evidence/recovery. Live content reload requires schema validation and approval; invalid reload keeps last-valid content. Engine updates build into a staging location, keep current executable/last-known-good untouched, activate only after restart, and restore on failed launch validation. U1 does not implement this workflow.

## U3/U4 acceptance gates

Test missing Codex, unauthenticated/wrong auth mode, unknown plan, exhausted/unknown quotas, unsupported models, truncated JSONL, process death, cancellation, failed login, read-only write attempts and offline resumption. U4 additionally tests stale diff/base, rejected approvals, protected changes, dirty worktree preservation, failed tests, invalid content reload and failed staged executable startup. None of these runtime/authentication tests ran during U1.
