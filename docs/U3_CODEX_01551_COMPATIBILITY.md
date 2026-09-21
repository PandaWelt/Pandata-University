Follow-up — 2026-09-21: U3_READ_ISOLATION.md supersedes the earlier field-only conclusion. Experimental named profiles exist; their native Windows enforcement does not establish U3's exclusive read boundary. This first-patch diagnosis and evidence are retained as history.

# U3 Codex 0.155.1 compatibility — 2026-09-20

This is a narrow correction to the U3 schema preflight. **Codex 0.155.1 still cannot satisfy U3's restricted-read-root requirement, so the mentor remains offline.** Do not treat this update as live mentor acceptance or proceed to U4.

## Diagnosis

The submitted repository is at commit `947bc85` (U3), with `7e4789a` (U2) preserved. The two submitted U3 evidence records show missing Codex first and the combined “schema missing or oversized” error later. Those records contain no generated schema inventory, so they alone cannot distinguish the cause. Chris reports the native Windows installer, ChatGPT sign-in, and `codex-cli 0.155.1`; no token files were inspected.

The official Linux release binary reports that exact version. Its local `app-server generate-json-schema` command reproduced the mismatch without an account or model request:

| Required export | Observed in 0.155.1 |
|---|---|
| GetAuthStatusParams.json | Not exported |
| GetAuthStatusResponse.json | Not exported |
| v2/AccountUpdatedNotification.json | Exported; distinguishes managed ChatGPT and external tokens |
| v2/ThreadStartParams.json | 10,180 bytes |
| v2/TurnStartParams.json | 20,847 bytes |

The largest generated JSON file was 697,192 bytes, below 1,048,576. The legacy files were **omitted**, not relocated or enlarged. The tagged exporter explicitly excludes v1 JSON types other than initialization. Tagged request dispatch and the account processor still support the deprecated token-free `getAuthStatus` operation. Thus the original adapter incorrectly required files that this version intentionally does not produce.

A second, independent incompatibility remains: the actual TurnStartParams schema's readOnly policy has no `readableRoots` or `includePlatformDefaults`. The experimental export also lacks them. The adapter must not silently substitute unrestricted reading or assume the fields are honored.

## Changes and boundaries

- Keep legacy schema validation when the original pair is present; partial/malformed pairs cannot fall back.
- Only for exact `codex-cli 0.155.1`, use a source-reviewed contract for the omitted legacy metadata RPC and check the generated v2 AuthMode/Plus declarations. Unknown versions remain blocked. This avoids pretending that `account.type=chatgpt` alone proves managed authentication.
- Keep the actual `getAuthStatus` calls, explicit `includeToken=false` and `refreshToken=false`, returned managed-auth check, all account/Plus/quota checks, model checks, preview gate, sandbox/tool restrictions and response handling unchanged.
- Keep the **1 MiB** per-file/parser limit. Bound reads even if a file grows after measuring it. Missing, non-regular, unreadable, oversized and malformed schema errors identify the file; oversized errors include its size.
- Add a bounded five-second `--version` check. Existing schema-generation (15 s), discovery (15 s) and interaction (45 s) deadlines remain. No App Server session starts when schema checking fails.

This update provides accurate compatibility handling and preserves the remaining safety block. Enabling live mentoring on a provider without restricted roots would need a separately reviewed equivalent isolation design; that is outside this patch. No automatic installation, upgrade, authentication change, API-key fallback, or altered Codex configuration is included.

## Windows application and validation

1. Extract the patch archive outside the existing repository. In PowerShell opened in the extracted folder, run:

   ```powershell
   .\Apply-U3-Compatibility.ps1 -Repository (Read-Host 'Existing PandaBank repository root')
   ```

   It verifies commit ancestry and touched-file hashes before applying. It refuses conflicts and never stages, commits, resets, or replaces your repository.
2. Launch `Start-Pandata-University.cmd`. Expected: existing engine/U2 checks plus **74 U3 checks**. Windows compilation was not available here; keep the first failure if it differs. Existing Windows junction automation remains skipped, not passed.
3. Inside University, type `mentor-status`. For 0.155.1, expect:

   `Unsupported App Server schema: v2/TurnStartParams.json lacks readableRoots; U3 requires restricted read roots. Mentor remains offline; read-only policy was not relaxed.`

   This confirms the specific preserved safety block, **not** live/authentication/quota success. A different error should be reported with the new U3 evidence record.
4. Confirm `mentor-send` without a preview requests `mentor-preview`; `files`, `run-check`, quit/relaunch and completed U1 still work. No need to send any model request or change account settings. Preserve the new U3 evidence alongside your old records.

## Validation and source basis

Observed here: warnings-as-errors C++20 test and full University builds; 74 U3 checks (all original 61 retained plus 13 compatibility/boundary cases); real 0.155.1 local schema/version generation; expected nonzero restricted-root rejection; console preview gate, offline file list, fresh error evidence and U1 completion after restart in a disposable copy. No existing source/progress/evidence bytes changed during that console test. Windows/MSVC, live sign-in/allowance, model inference and native sandbox acceptance are not claimed.

Official sources: [0.155.1 release](https://github.com/openai/codex/releases/tag/rust-v0.155.1), [tagged exporter](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/app-server-protocol/src/export.rs), [legacy request definitions](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/app-server-protocol/src/protocol/v1.rs), [request dispatch](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/app-server/src/message_processor.rs), [account processor](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/app-server/src/request_processors/account_processor.rs). Downloads/schema generation were temporary diagnostics; no Codex binary or generated schema is shipped as project source.
