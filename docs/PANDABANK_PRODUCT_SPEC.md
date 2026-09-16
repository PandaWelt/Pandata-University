# PandaBank Product Specification

## Product definition

PandaBank is a local-first creative project operating system: a visual database, documentation workspace, naming engine, asset registry, project launcher, and eventually a lightweight interface/whiteboard builder. It is intended to manage a creative project from initial concepts through production without forcing the user to hunt through File Explorer or maintain disconnected documents.

The first user is Chris. The first objective is a private productivity and learning accelerator. Portfolio polish and broader commercial distribution come after the core workflow is dependable.

## Product principles

1. **Data integrity first.** Losing, duplicating, silently unlinking, or corrupting project information is worse than an ugly interface.
2. **Picture-book readability.** Project information should be visually natural to scan, not a thin reskin of directory trees.
3. **Minimal setup.** Creating a project, category, asset, or linked file should require few decisions and little directory surfing.
4. **Scope control.** Build chronological foundations before advanced editing, animation, phone access, or collaboration.
5. **One source of truth.** PandaBank records identity and relationships; physical files remain ordinary accessible files rather than proprietary blobs.
6. **Portable projects.** Project database/configuration and required metadata live inside the project folder so the folder can move between computers.

## Primary desktop experience

### Home

- The home screen is dominated by visual project cards.
- New projects normally use one configurable default PandaBank projects folder.
- A project may also create a shortcut/link in a remembered access location. Earlier “discipline presets” referred to choosing these shortcut destinations without repeatedly browsing for them.
- Opening a card enters that project's workspace.

### Workspace templates

After project creation, a workspace template configures only:

- tabs;
- pages within tabs;
- their layouts.

Templates do not dictate the real storage location, asset schema, or creative discipline. A project can customize its workspace after creation.

### Tabs and pages

- Tabs are non-negotiable.
- Pages and tabs may represent assets, characters, story, roadmap, references, naming conventions, or user-defined concerns.
- Edits remain scoped to the relevant page/tab while all content belongs to the same project.

### Asset Browser

The Asset Browser is the core production interface. It should feel like navigable pages, expandable rows/cards, tables, and wiki-style records—not like a conventional file tree.

- Assets may begin as planned concepts before any physical file exists.
- Every asset has one owner category and any number of searchable tags.
- Categories can define default optional fields for newly created assets.
- Fields can be added or removed on an individual asset.
- An asset may contain any useful related information: description, biography, image, notes, variables, instructions, disk path, engine path, source files, derived files, URLs, or links to other PandaBank pages/projects.
- Complex assets relate to smaller assets without stealing ownership. Example: a boss character links to animation assets whose owner category remains Animation.
- Rows/cards can expand for detail and support useful list/card display modes.
- Create New Asset is available from the Asset Browser globally and within category contexts.
- Creating a physical file starts from the desired asset record so the new file is immediately associated and placed correctly.

### Managed files

From PandaBank, the user must eventually be able to:

- create;
- open in the associated external program;
- rename;
- move;
- delete with explicit safeguards;
- link existing files/folders;
- unlink without necessarily deleting;
- reveal the physical location.

Moving a managed folder moves its contained files normally. PandaBank must not conceptually “unparent” the contents. If managed files are renamed or moved outside PandaBank, the application should detect the change and update its registry when identity can be established safely. Ambiguous matches require confirmation rather than guessing.

### Naming convention engine

The naming engine generates consistent names from a configurable hierarchical path.

- Candidate fields include Project, Category, Type, Subtype, Variant, and Serial, but conventions choose which fields exist and their order.
- Each dropdown shows only children of the previous selection.
- A separate New Entry control sits above/outside the dropdown to prevent accidental selection.
- Entries may be permanent or temporary. Temporary entries should not appear in future choices after registration.
- Serial numbers increment within the relevant path/kind, not blindly across the entire project.
- Pressing Enter/confirm creates the registered item and places/logs it according to project rules.
- Search-first navigation is at least as important as generating names.

## Layout Mode and Work Mode

Names remain provisional, but the application requires two clearly distinct modes:

- **Layout Mode:** edit PandaBank's workspace interface, place components, modify visual properties, and bind supported built-in commands.
- **Work Mode:** use the resulting interface to document and manage the project.

Switching modes must be deliberate enough to prevent accidental layout edits, but should not require a tedious multi-step process.

### Responsive layouts

Layouts should support horizontal monitors, vertical monitors, window resizing, and eventual phone-sized presentation. The system will need containers, anchors/constraints, min/max sizes, wrapping, visibility rules, and potentially separate breakpoint layouts. Arbitrary absolute coordinates alone are insufficient.

## Whiteboard and vector/component editor

Long-term, PandaBank should include a compact Illustrator-like editor used for quick storyboards, diagrams, project visuals, whiteboards, and building interface components.

Desired tool direction:

- selection and direct selection;
- pen/path editing;
- rectangle and ellipse;
- text with fonts and basic typography controls;
- fill, stroke, color, and gradients;
- brush/freehand input;
- shape-building/boolean operations;
- component creation and built-in command bindings.

Animation direction:

- priority: hover, pressed, show, and hide transitions;
- transition duration and interpolation/easing;
- later: keyframes for position, scale, rotation, color, and opacity;
- possible future: path animation and shape morphing.

The architecture should leave room for these capabilities, but none belongs in the first foundation releases.

## Remote/phone direction — deferred

The eventual phone experience should primarily browse, search, present, and edit notes. The phone connects over the internet to a lightweight PandaBank service running on the always-on owner PC. External edits are proposed changes: the owner/admin computer previews them and has final authority to accept or reject them.

Offline phone editing, server deployment, authentication, conflict resolution, and iOS implementation are explicitly deferred. Do not let them distort the local desktop foundation.

## Technical and commercial constraints

- PandaBank production code is C++.
- Windows 11 desktop is first.
- Visual Studio 2022 Community is the user's main full IDE.
- Existing rendering/input stack is Dear ImGui + SDL2 + OpenGL.
- Qt is excluded.
- Dependencies must be open-source or otherwise permit commercial distribution without royalties or mandatory licensing fees. Record exact licenses and notices.
- iPhone support is a future target, not a current milestone.
- User data remains accessible through ordinary files and a documented local database format.

## Explicit non-goals for early versions

- competing with Adobe Illustrator feature-for-feature;
- real-time multi-user collaboration;
- cloud accounts or paid hosting;
- phone implementation;
- marketplace/plugin ecosystem;
- AI required for routine use;
- hiding project files inside an opaque proprietary package;
- Stats + Script Sync between documents and game code (postponed/scrapped for now).
