# Course Progress

## Current position

- Phase: 0 — Preserve and stabilize prototype
- Active mission: `001_stable_object_identity`
- Repository baseline: not yet checkpointed in this cleaned workspace
- Build verification: previously runs on the user's Windows machine; not reverified by this launch-pack preparation environment
- Temporary scaffold entries: 0 registered
- Release blockers: baseline/build portability, known spawn/input defect, no tests, no persistence

## Known working behavior

- SDL2/OpenGL window launches on the user's current setup.
- Dear ImGui renders the full canvas and popup/tool window.
- Basic boxes and prototype text widgets render.
- Popup exposes the object-type choices.

## Active defect

One selection sets `spawnRequested` permanently, the spawner adds an object every frame, `drawAll()` renders the reusable member instead of each vector element, and all repeated widgets use the wrong ID. Objects also share default coordinates.

## Concepts currently active

| Concept | State | Evidence needed next |
|---|---|---|
| `std::vector` stores separate values | introduced | explain vector growth versus visual overlap |
| Range-based loop references | introduced | use current `obj` rather than the spawner template |
| Immediate-mode frame state | introduced | consume a one-frame request |
| Stable ImGui IDs | practiced_with_help | preserve focus across frames and objects |
| Header/source/build relationships | introduced | later portable-build mission |

## Next chronological actions

1. Astra performs the restricted first-run audit/foundation work.
2. Chris completes Mission 001 with adaptive assistance.
3. Verify one click/one object, independent IDs, and independent text.
4. Create the next focused debugging/assertion mission.

## Session log

Append dated entries with:

- mission and duration;
- what Chris attempted before assistance;
- tests/build/smoke evidence;
- assistance level reached;
- concepts advanced or review due;
- scaffold created/reviewed/replaced;
- next exact action.
