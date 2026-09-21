# U3 read isolation review — Codex 0.155.1 — 2026-09-21

**This cumulative update diagnoses the supported profile API, but does not enable live mentoring.** There is no verified Windows implementation of U3's exclusive disposable-context read boundary in this adapter. U1/U2 remain usable. U4 is not started.

## What the actual schemas say

Reproduced with the official Linux release executable reporting exactly `codex-cli 0.155.1`. The submitted native Windows evidence agrees with the missing legacy field; its generated schemas were not included in the archive. University now generates the installed Windows executable's experimental schema locally and records its own measurements on the next status check.

| Surface | Stable export | Experimental export |
|---|---|---|
| ThreadStartParams.permissions | absent | nullable string: named profile ID |
| TurnStartParams.permissions | absent | nullable string: named profile ID |
| ThreadStartResponse.activePermissionProfile | absent | profile provenance |
| readOnly SandboxPolicy.readableRoots | absent | absent |
| readOnly SandboxPolicy.includePlatformDefaults | absent | absent |

Restricted reads did not move to another TurnStartParams field with a new spelling. The supported mechanism is a **named permissions profile**, defined in config under `permissions.<id>.filesystem`, selected through `permissions`. The schema says not to combine thread `permissions` with `sandbox`, or turn `permissions` with `sandboxPolicy`. Experimental protocol negotiation is required for a live profile request. Generating an experimental schema does not enable that live negotiation.

The tagged profile compiler begins with a restricted empty filesystem policy, then compiles explicit entries. A context-only candidate would grant only an exact disposable directory read access, with no inheritance, `:root`, `:minimal`, workspace/temp grants, write entries, or network enablement. That is a policy description, not sufficient proof of native enforcement. The legacy `readOnly` echo cannot prove an exclusive readable set.

The old auth-export fix remains intact: exact-version omission of deprecated v1 exports is permitted only under the reviewed contract; actual managed ChatGPT auth checks are unchanged. No schema/parser limit was increased. The relevant experimental schemas are 12,971, 23,746 and 77,846 bytes, each below 1 MiB. See `validation/u3-read-isolation.json` for hashes and commands.

## Why Windows is still blocked

The 0.155.1 tagged Windows implementation establishes these facts:

- The unelevated backend explicitly refuses restricted read access.
- The elevated backend resolves read roots and grants read ACLs. Its token creation uses `WRITE_RESTRICTED`, which does not make the listed capability SIDs an exclusive read allowlist.
- Its sandbox account is added to the ordinary Windows `Users` group. The read-ACL helper recognizes existing Users/Authenticated Users/Everyone read permissions, and adds sandbox-group grants when required.
- Explicit deny-read paths are handled separately. An omitted path is not equivalent to an explicit deny ACL. Existing broad/group grants therefore cannot be assumed to disappear when a narrower profile is selected.
- Launching that backend can provision/repair accounts and refresh ACLs. U3 must not perform that machine configuration as a hidden compatibility step.

**Conclusion from source review:** selecting a profile that grants only the context directory does not, by itself, establish that every other file is unreadable to the native Windows sandbox. This is a failure to establish U3's stronger boundary, not a claim of a reproduced Windows exploit or a claim that all named profiles are broken. Windows execution and adversarial native sandbox tests were not available here.

The App Server's standalone `fs/readFile` request is documented by its generated schema as host filesystem access; it has no thread/profile field. It would not be a valid sandbox test. A prompt, a read-only echo, disabled tool requests, or rejection of a tool notification after execution also cannot replace the boundary.

No profile is activated, no native sandbox setup/probe is run, no broad deny ACLs are applied, and no account/configuration file is changed. There is no API-key or paid fallback. A future U3 isolation fix must first demonstrate a supported, closed read set (including already group-readable files, previous sandbox grants, reparse escapes, and denied writes), without changing arbitrary computer ACLs or exposing credentials. That is still U3 work, not permission for U4. Do not troubleshoot login or purchase credits for this isolation blocker.

## What this update changes

- Exact 0.155.1 performs bounded `--version` (5 seconds), then local `app-server generate-json-schema --experimental` (15 seconds).
- New diagnostics inspect the exact top-level `permissions` types and active-profile provenance, rather than treating a nested name match as support.
- Status/send evidence gains an additive `compatibility` object: version, export, recognized mechanism, selected schema sizes, unchanged 1 MiB limit, and explicit false isolation/model-request flags.
- Recognized profiles produce a specific isolation error **before starting any App Server session**. No model context or account request is sent.
- Existing preview, token-free managed-auth, Plus/allowance, no-credit, tool/approval, timeout, advisory-only and offline rules remain unchanged.
- All 74 earlier U3 tests remain; 12 diagnostic/boundary checks are added. Test fixtures never count as native enforcement evidence.

## Windows application and validation

1. Extract the delivery ZIP outside the repository. In PowerShell opened in its `U3_Read_Isolation` folder:
   ```powershell
   .\Apply-U3-Read-Isolation.ps1 -Repository (Read-Host 'Existing PandaBank repository root')
   ```
   The installer selects a reviewed patch for original U3 or Patch 1, checks touched-file hashes and required history, and refuses conflicts. It does not reset, stage, commit, replace the repository, or write progress. Keep unrelated edits.
2. Launch `Start-Pandata-University.cmd`. Expect 44 engine checks, the existing U2 suite, and **86 U3 checks**. The prior Windows U2 result was 42; junction automation was skipped due to denied creation and remains **not passed**. Keep the first compile/test failure if any. PowerShell installer/MSVC execution were not available here.
3. Inside University, type `mentor-status`. Expected: offline, with “uses experimental named permission profiles” and “no verified context-only native backend.” In the newly printed evidence file, expect `mechanism=experimental_named_permission_profiles`, `contextOnlyIsolationAccepted=false`, and `modelRequestSent=false`. This is successful diagnosis, **not live mentor acceptance**. Missing/malformed/changed schemas must still block.
4. Confirm `mentor-send` before a preview requests a preview; `files` and `run-check` still work; quit/relaunch preserves completed U1. An optional preview then send must give the same isolation block, no answer, and a separate record. Do not weaken the gate or start U4.

## Source basis

All code links below are pinned to `rust-v0.155.1`, not moving main:
- [Release](https://github.com/openai/codex/releases/tag/rust-v0.155.1)
- [Profile compiler](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/core/src/config/permissions.rs), [config schema](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/core/config.schema.json)
- [Unelevated rejection](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/windows-sandbox-rs/src/lib.rs)
- [Token flags](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/windows-sandbox-rs/src/token.rs)
- [Read roots](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/windows-sandbox-rs/src/setup.rs), [ACL grants](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/windows-sandbox-rs/src/bin/setup_main/win.rs)
- [Users membership](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/windows-sandbox-rs/src/bin/setup_main/win/sandbox_users.rs), [setup/refresh behavior](https://github.com/openai/codex/blob/rust-v0.155.1/codex-rs/windows-sandbox-rs/src/identity.rs)
- [App Server documentation](https://learn.chatgpt.com/docs/app-server)

No downloaded provider code/binaries or generated schemas are added to project source.
