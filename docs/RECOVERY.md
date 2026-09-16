# Recovery and checkpoints

The delivered directory is the one PandaBank working repository. `.git` contains local history; there is no external remote. Do not overlay this archive onto a folder containing newer edits. Preserve any such work and compare diffs first.

- `uploaded-baseline`: `3b9a6b7fac5dd9f53c0876dd90922197e39fc910`, source plus supplied dependency inputs before foundation edits.
- `foundation-ready`: foundation changes and evidence.
- `recovery/original-upload.zip`: byte-for-byte original `PandataBank_Astra_Ready.zip`, including SDL2.pdb and empty directories; ignored by Git but included in the delivered archive.
- Original ZIP SHA-256: `2899048eeb00fd44b202ffdf7ed590710559cdc01837796eccea42aeaac08def`.
- `docs/BASELINE_INVENTORY.json`: all 139 original file paths, lengths and SHA-256 hashes.

Git intentionally excludes the supplied debug-symbol PDB (and future generated outputs). Therefore the exact upload is recovered from the ZIP, not solely from a Git source checkout. No original bytes were deleted from the delivered working tree.

Inspect safely from the repository root:

```powershell
git status --short
git log --oneline --decorate
git diff uploaded-baseline foundation-ready -- src visualstudio
git fsck --full
Get-FileHash .ecovery\original-upload.zip -Algorithm SHA256
```

If a future edit must be undone, first preserve its diff and unrelated work, then selectively restore only the intended changes. Never use `git reset --hard` as a routine course instruction. The original archive can be extracted to a separate recovery location for comparison; it is a backup, not a second active tutorial implementation.
