# PandaBank prototype — RETAIN / ADAPT / REPLACE

Scope: submitted Mission 002 snapshot; no product-source migration performed in the University spike.

| Element | Decision | Technical reason and migration timing |
|---|---|---|
| C++20 + SDL2/OpenGL/ImGui startup | RETAIN | Working according to user Debug/Release evidence. Keep stack through stabilization; platform adapter later, before multi-platform integration. |
| AppUISetup.cpp composition/state/drawing/animation/diagnostics | ADAPT | One large translation unit obscures ownership and testing. Extract one responsibility in the next actual product header/source lesson; no wholesale rewrite. |
| BaseObjects stored data + Draw methods | ADAPT | Renderer-coupled types/buffers cannot be final document model. Separate data from renderer before serialization or advanced vector tools. |
| Mutable ObjectSpawner::object template | ADAPT | Today assignments/copy support pilot; future creation may carry stale properties between types. Introduce explicit per-creation initialization when object defaults become editable; regression tests before removal. |
| std::vector<BaseObjects> values | RETAIN | Owned value collection is valid. Reallocation invalidates addresses/references; later selections should store stable IDs. Current no inheritance means no slicing now; storing future derived instances as BaseObjects would slice. Revisit only with polymorphism/scale evidence. |
| Explicit ObjectType + X-macro name mapping | RETAIN | Learner-selected single list fits compile-time prototype types. Malformed #undef X(name) remains a narrow warning to fix separately, not evidence against whole route. Reconsider dynamic types/plugins/schema versioning. |
| Creation flow + stable application ID/name distinction | RETAIN | Missions 001/002 establish predictable insertions and checks. Keep behavioral invariants through later refactors. |
| nextId runtime counter | ADAPT | Resets per process; not a persistent or cross-project allocator. Design persistent identity before saving/merging projects; no UUID library added now. |
| uint64_t to int ImGui scope | REPLACE later | Narrows high bits; pairwise application-ID check cannot detect all widget collisions. Add a full-width adapter/encoding and tests when extracting UI identity boundary. Do not churn small current mission. |
| Fixed char[512] + ImU32 in object | ADAPT | Fine for temporary widgets; not durable document text/style representation. Use renderer-independent text/style before persistence; UI buffer conversion at renderer boundary. |
| Function-local static popup/spawner/animation | ADAPT | Lifetime fits one exploratory scene but hinders reset/testing/multiple projects. Move into explicit owned application/document state with header/source lesson sequence. |
| Absolute positions/count-based horizontal spacing | REPLACE later | Diagnostic spacing is not anchors, containers or responsive layout; offscreen growth. Keep until real placement/layout rules are tested. Track M001-SPACING. |
| Animation reads ImGui time and shares rendering file | ADAPT | Explicit time input/state and initialization can be tested without GUI. Static current instances are zero-initialized; automatic instances lack that protection. Migrate after object boundary, before richer animation. |
| Debug insertion-count + ID invariants | RETAIN | Correctly guard check work with NDEBUG. Pairwise scan is O(n²) per insertion, cumulative O(n³) for n inserts; acceptable small Debug prototype, review before large scenes. |
| Debug PrintVector | RETAIN scoped | User guards the call and improves formatting. Accept as Debug diagnostic; helper definition remains compiled but unused in Release. Remove/rework if output/scan scale costs matter. |
| Native Visual Studio project | ADAPT | Relative x64 paths now work by user report, but not final portable truth. Keep usable until CMake Debug/Release parity and relocated-checkout tests pass. |
| SDL/ImGui provenance/notices | ADAPT records | Bundled bytes preserved; WIP commit and missing complete ImGui notice need release audit. No silent replacement/download. |
| SDL/OpenGL/ImGui in domain core | REJECT future coupling | Current prototype has no independent core. Keep these out of new core from its first extraction. |

ImGui remains useful for inspectors, menus, University panels, editor chrome and temporary drawing. Geometry, text, hierarchy, transforms/styles, selection, animation, command history and project state belong to a renderer-independent document. No final vector-rendering library selected: later ADR must compare license/commercial use, Windows/iOS, maintenance, paths/control points, shaping/fonts, gradients/clipping, hit testing, booleans, serialization, animation, GPU/CPU and replacement cost.
