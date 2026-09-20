# U3 handoff — 2026-09-19

U2 is accepted from the submitted code/history, saved action evidence, and Chris's native Windows report. Commit **7e4789a** is present and remains HEAD. Windows passed 44 engine and 42 repository-action checks, real success/failure/success checks, views, rejection cases, Git visibility, Visual Studio handoff, and persistence. **Windows symlink/junction automation was skipped because creation was denied**, not passed. Earlier U2-pending notes are historical.

U3 implements one optional mission-aware explanation using the installed Codex App Server. It does not implement a general chatbot, editor, Rescue, Maintenance, patch application, model picker, Deep Review, or the PandaBank refactor. All original source, native projects, third-party inputs, Git files, learner progress, evidence and unrelated files are preserved.

## Use

Launch `Start-Pandata-University.cmd`. In the University prompt, `mentor-status` performs local/schema/account/catalog checks without a model request. `mentor-preview` displays the exact bounded bundle for the current U1 step (or the final transfer step when orientation is complete). `mentor-send` authorizes one request using that unchanged preview. A response or step change requires a new preview. These are University commands, not PowerShell commands.

The fixed question asks for an explanation/hint; after completion it asks about the distinction between orientation and demonstrated mastery. No whole-source upload, Git diff, diagnostic log, other response history, learner profile, personal email, or credential is included. No source excerpts are needed for this narrow conceptual question. Replies are advisory, escaped for terminal display, and have no path into progress, compiler, Git, or file actions. Engine checks remain the authority for engine behavior; no automatic learning assessment is added.

Each invocation saves an append-only `course/university/evidence/u3/<unique>/evidence.json` with sanitized state, selected plan/model, exact proposed context for explicit asks, advisory response and ephemeral thread/turn references. It is ignored by Git alongside U2 evidence. A failed call saves no partial answer. Back it up locally with existing progress; it is not part of this patch. Codex itself owns its normal authentication/session storage; University never reads token files.

## Deliberate limits

Only the locked ChatGPT Plus plan and catalog-confirmed `gpt-5.6-terra` are accepted. A model being in the catalog alone is insufficient: both quota windows must be current and unexhausted, and credit data must explicitly show no purchased/unlimited credits. Missing fields, extra unmapped quota buckets, incompatible schema, unsupported configuration or model means offline. No key, custom OAuth, purchase, add-credit nudge, quota-reset consumption or API billing path exists. This conservative gate may refuse a legitimate account rather than guess inclusion. It does not control unrelated account usage.

Native `codex.exe` must be on an absolute PATH outside the repository on Windows. A `codex.cmd` npm shim alone is not enough; add the installed native binary directory to the launcher environment if applicable. University does not download Codex or alter PATH/configuration. Sign in through installed Codex's own `codex login` flow using ChatGPT, outside University. A failed login leaves University offline.

The adapter requires generated schemas for token-free `getAuthStatus` metadata and confirms `authMethod=chatgpt`; a generic ChatGPT account label is insufficient. Missing/removed legacy metadata support leaves this conservative slice offline. It generates the installed protocol schema locally, then runs metadata-only discovery and a fresh App Server with discovered features/skills disabled. It verifies effective configuration, empty MCP/plugin/custom-provider maps, disabled tools/hooks/notifications/search, disabled skill context, and a read-only ephemeral thread before sending any bundle. Restricted read roots contain only disposable scratch. Server requests for approvals, credentials or tools are denied and terminate the interaction. Unknown tool items are treated as failures. This is defense in depth around Codex's own trusted sandbox; the test doubles do not prove that a particular installed Codex version enforces its OS sandbox.

No persistent multi-turn memory is added. One fresh ephemeral thread is reconstructable from each explicit mission bundle; references are evidence only and are never resumed. This trades conversational continuity for smaller context and no hidden session state. Timeout bounds are 15 seconds for schema generation, 15 seconds for discovery and 45 seconds for the interaction. A stopped/failed turn returns offline. Ctrl+C closes the console; already saved U1/U2 progress survives and child job handles close. Relaunch to continue. No streaming partial answer is accepted.

## Evidence and remaining gate

Linux GCC builds, 44 original engine checks, 46 unchanged Linux U2 checks and 61 U3 checks passed. U3 combines protocol fixtures with real native process/argv/stdin/stderr/deadline tests. CLI checks verified missing Codex, preview gating, continued offline checks, a deliberate harmless failure, a following pass, and quit/relaunch persistence in a disposable copy. See `validation/u3-validation.json` and `U3_WINDOWS_VALIDATION.md`.

**Not run here:** Windows/MSVC U3, installed Codex schema compatibility, real ChatGPT authentication/entitlement, a live model answer, native Codex sandbox enforcement, or Windows junction automation. Codex is not installed on this host. U3 is implemented and locally tested, but live/native acceptance remains pending; do not label it completed or proceed to U4.

Next action after applying the merge-safe patch: launch `Start-Pandata-University.cmd` in the existing repository and type `mentor-status` in University. Keep the first failure or exact offline reason; do not weaken gates to get a response.

Protocol references checked for this implementation: [official App Server](https://learn.chatgpt.com/docs/app-server) for schema generation, stdio, account/limit/model and thread operations; [configuration reference](https://learn.chatgpt.com/docs/config-file/config-reference) for the explicit tool/auth settings. Runtime installed schema and echoed configuration remain the compatibility gate; documentation alone is not evidence that Chris's installation passed it.

The token-free legacy auth query fields were also checked against the [official Codex protocol definitions](https://github.com/openai/codex/blob/main/codex-rs/app-server-protocol/src/protocol/v1.rs). Both token inclusion and token refresh are explicitly false; the adapter never requests a credential value.
