# Decision Log

Statuses: **Locked**, **Provisional**, **Deferred**, or **Open only if blocking**.

| Decision | Status | Rule |
|---|---|---|
| Product name | Provisional | Use PandaBank in documentation; preserve existing `Pandata.Bank` filenames until a dedicated rename. |
| Primary purpose | Locked | Private accelerator first; polished portfolio product later. |
| Learning language | Locked | C++ is the user's learning and PandaBank production language. |
| Desktop platform | Locked | Windows first, Visual Studio 2022 Community supported. |
| Existing UI stack | Locked for stabilization | Dear ImGui + SDL2 + OpenGL. Do not replace during foundation work. |
| Qt | Locked | Excluded. |
| Licensing | Locked | Commercially usable without royalties or mandatory paid licenses; licenses recorded. |
| Mission order | Locked | Chronological prerequisites, not fastest feature progress. |
| Mission size | Locked | One meaningful 45–90 minute feature/fix by default. |
| Offline behavior | Locked | Prepared missions and automated checks continue without AI/internet. |
| Real codebase | Locked | Dashboard and Visual Studio edit the same repository; no later export/merge step. |
| User evidence | Provisional | Prediction + working result + debugging explanation + transfer task, adapted to fatigue/performance. |
| Rescue Mode | Locked in behavior | Adapt among hint, guided steps, partial solution, and intervention using recent fatigue/performance. |
| Home screen | Locked | Visual project cards dominate. |
| Default project storage | Locked | One configurable default PandaBank projects folder. |
| “Discipline presets” | Clarified/Locked | Remembered shortcut/access destinations, not project-content presets. |
| Workspace templates | Locked | Configure tabs, pages, and layouts only. |
| Tabs | Locked | Non-negotiable. |
| Asset organization | Locked | One owner category plus searchable tags and relationships. |
| Planned assets | Locked | Asset records may exist before physical files. |
| Project portability | Locked | Database/configuration required for the project lives inside its folder. |
| External file moves | Locked intent | Detect and safely update registry; ambiguity requires review. |
| Layout editor | Locked direction | Component-based layouts with built-in command bindings and compact vector tooling. |
| Responsive UI | Locked direction | Horizontal, vertical, resized, and eventual phone-sized layouts. |
| Phone/server | Deferred | Preserve boundaries; do not implement now. |
| Dashboard implementation technology | Provisional | PandaBank remains C++. Isolated web technology may be used for the private course dashboard only if a feasibility comparison shows it materially avoids building an editor/terminal from scratch. Obtain user approval before adding a second-language toolchain. |
| Database | Provisional | SQLite is the leading local-first candidate; verify architecture and licensing before adoption. |
| Build system | Provisional target | CMake becomes portable source of truth after baseline preservation and verified migration. |
| SDL2 to SDL3 upgrade | Deferred | No upgrade during stabilization. |
| AI inside final PandaBank | Deferred/Open | Routine PandaBank use must not depend on AI credits. |

## Question policy

Do not reopen locked decisions. For provisional decisions, first produce a concrete recommendation with cost, learning burden, licensing, reversibility, and effect on the product. Ask only immediately before implementation if approval is required. Defer anything that does not block the current chronological mission.

## Foundation decisions — 2026-09-16

- **Adopted, narrow stabilization:** keep all filenames and source layout. Add direct `<cstdint>` include and normalize the existing x64 project paths. No behavior rewrite. These changes are reversible by selective Git reversion; native Windows validation is pending.
- **Provisional build execution:** defer CMake implementation because this environment lacks CMake and Windows tools. Prepared migration lives at `course/migrations/portable_cmake.md`; the native VS project remains the current build entry point. No dependency or second-language dashboard toolchain was added.
- **Course state:** use Markdown plus `course/state.json` and an evidence form now. No dashboard prerequisite. Mission 001 remains unattempted; next learner mission is local debugging/assertions (002).
- **Dependency preservation:** pin supplied bytes by inventory, not an invented WIP commit. Resolve notices/provenance before distribution. Supplied SDL libraries/DLL are dependency inputs; PDB remains recoverable outside Git source history.
- **Approval boundary unchanged:** no new cost, account, publication, dependency license change, destructive step or second-language dashboard implementation was authorized or performed.
