# Verify the native Windows build

Status: path references checked; Windows compilation, linking and launch NOT RUN here.
Run from this repository root in **Developer PowerShell for VS 2022**, with the C++ desktop workload, MSVC v143 and a Windows 10/11 SDK installed. Use the same extracted repository for edits and Visual Studio; do not copy mission code into a tutorial project.

## Build Debug and deploy the DLL

```powershell
$repo = (Get-Location).Path
msbuild .\visualstudio\Pandata.bank.vcxproj /m /t:Build /p:Configuration=Debug /p:Platform=x64 "/p:OutDir=$repo\out\native\Debug\\" "/p:IntDir=$repo\build\native\Debug\\"
if ($LASTEXITCODE -ne 0) { throw 'Debug build failed; inspect the first compiler/linker error.' }
Copy-Item .\thirdparty\SDL2\lib\x64\SDL2.dll .\out\native\Debug\SDL2.dll
& .\out\native\Debug\Pandata.bank.exe
```

These commands are prepared, not executed in this Linux audit. Output directories are ignored. `Copy-Item` targets generated output, never your source DLL. Preserve the terminal build output in the mission evidence (or record its first error). No library upgrade or SDL2main linking is needed to try this existing configuration.

## Visual Studio debugger

Open `visualstudio/Pandata.bank.vcxproj` through File > Open > Project/Solution, or use the existing `.slnx` if supported. Select **Debug / x64**. Build with Ctrl+Shift+B. For an IDE build using default output paths, find the executable path in Build Output and copy `thirdparty/SDL2/lib/x64/SDL2.dll` into that exact directory. Start with F5. A DLL-not-found message is deployment failure, not evidence about your vector fix.

The checked-in paths now use `$(ProjectDir)..\`, so do not restore the old `D:\CREATIVE...` settings. If a personal `.user.props` overrides settings, inspect the effective property values before changing repository configuration. If `winres.h` is missing, inspect the C++ workload/resource include installation; report that separately rather than editing third-party code.

The popup may remain open after selecting a type. Click outside it to dismiss before testing canvas input. Avoid leaving the known runaway-spawn baseline running for long: inspect a few frames, then stop debugging.

## Release (after the Debug smoke test)

```powershell
$repo = (Get-Location).Path
msbuild .\visualstudio\Pandata.bank.vcxproj /m /t:Build /p:Configuration=Release /p:Platform=x64 "/p:OutDir=$repo\out\native\Release\\" "/p:IntDir=$repo\build\native\Release\\"
if ($LASTEXITCODE -ne 0) { throw 'Release build failed.' }
Copy-Item .\thirdparty\SDL2\lib\x64\SDL2.dll .\out\native\Release\SDL2.dll
& .\out\native\Release\Pandata.bank.exe
```

A successful Release build is not distribution approval: dependency notices/provenance, runtime behavior and ownership reviews remain pending.

## Error routing

| First symptom | Check next |
|---|---|
| `msbuild` not found / v143 missing | Use Developer PowerShell; inspect installed C++ desktop tools. |
| Cannot open `imgui.h` / `SDL.h` | Confirm x64 configuration and effective project-relative include paths. |
| Unresolved SDL/OpenGL symbols | Confirm x64 `SDL2.lib` path and `opengl32.lib`; do not add arbitrary backends. |
| SDL2.dll missing | Copy bundled DLL next to the executable actually being launched. |
| Window opens but objects/input fail | Record separately; this is Mission 001 territory. |
| Animation warnings / malformed `#undef` warning | Record existing warnings; do not expand Mission 001 into animation cleanup. |
