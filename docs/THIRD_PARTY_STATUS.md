# Third-party inventory — source audit, 2026-09-16

No dependency was added, upgraded, downloaded, or modified. Identifiers below come from bundled files, not dates or assumptions. `BASELINE_INVENTORY.json` fingerprints every original file; identical hashes preserve the exact WIP snapshot until provenance is established.

| Component | Observed identifier / use | Bundled evidence | Remaining release work |
|---|---|---|---|
| Dear ImGui core + SDL2/OpenGL3 backends | `IMGUI_VERSION "1.92.8 WIP"` in `imgui.h`; exact commit unknown | `imgui.cpp` says standard MIT and points to missing `LICENSE.txt`; seven `.cpp` files including demo/backends are in the native project | Identify exact source commit/archive, obtain matching full notice without replacing implementation; verify any local modifications. |
| ImGui's `imstb_*` headers | rectpack, textedit, truetype bundled internally | Each contains MIT/public-domain alternatives at its end | Preserve full notices in distribution; these are part of this ImGui snapshot, not separate upgrades. |
| SDL2 | Header macros report 2.32.2 | `SDL_copying.h` contains zlib-style terms; `SDL_revision.h` has empty default revision; supplied x64 DLL and three `.lib` files | Verify DLL version/provenance against headers; retain required notices and record original package/source. |
| OpenGL | Desktop GL context, default 3.0 / GLSL 130 on Windows | Bundled SDL GL headers, ImGui OpenGL3 loader; native link is `opengl32.lib` | Verify Windows system-library link and actual driver/context behavior. Do not add GLAD/GLEW during stabilization. |

`SDL2main.lib` and `SDL2test.lib` are present but not linked by the native project. Keep them as supplied inputs; do not add them simply because they exist. SDL2.dll must accompany the app executable. SDL2.pdb was in the original foundation upload but is absent from the Mission 002 source-only ZIP; no deletion from Chris's machine is inferred.

For later provenance work, use the source locations referenced in the bundled files (Dear ImGui upstream and SDL upstream). No upstream commit or binary match was verified in this run. Do not call the WIP identifier an exact release pin and do not silently substitute a current release. Record source URL, commit/package checksum, local changes, license notice and tested platforms before closing the release block. No commercial licensing conclusion is inferred from a missing notice.

Pandata University U1 uses only the C++20 standard library. No third-party package or license was added; the PandaBank dependency audit remains open.
