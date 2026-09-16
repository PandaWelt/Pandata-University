# Third-Party Dependency Status

Observed bundled dependencies:

| Dependency | Current role | Source form present | License status in this package | Action before release |
|---|---|---:|---|---|
| Dear ImGui | UI/widgets and OpenGL/SDL backend | Yes | No dedicated license file observed in uploaded project | Verify exact bundled version; add its license/notice and source reference. |
| SDL2 | windowing, events, OpenGL context | Headers, x64 library, DLL | `SDL_copying.h` exists, but no deliberate dependency inventory exists | Record exact version/source; preserve required license text; verify redistributable files. |
| OpenGL system library | rendering API | Windows system link | Platform-provided | Document platform requirement. |

This is an audit flag, not a legal conclusion. Astra must not invent versions from file dates or silently download newer dependencies. During the portable-build phase, establish:

- exact version or commit;
- authoritative source URL;
- license identifier and included notice;
- whether source or binary is vendored;
- supported platforms/configurations;
- update procedure;
- reason the dependency is necessary.

No new dependency may be added until its commercial-use and redistribution terms are checked against `AGENTS.md`.
