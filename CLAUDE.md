# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 🚀 Quick Reference (START HERE!)

**Current Status:** Phase 2 (Input & Selection) - In Progress | ✅ Build: Compiling
**Last Updated:** November 18, 2025
**Executable:** `bin/ProjectAnu_x64d.exe`

### Essential Documents
- **Current Status:** `documentation/GDD/Current_Status.md` ← **READ THIS FIRST**
- **Phase 1 (Complete):** `documentation/GDD/04-Implementation/Phase-1-Foundation.md`
- **Phase 2 (In Progress):** `documentation/GDD/04-Implementation/Phase-2-Input-Selection.md`
- **GDD Overview:** `documentation/GDD/00-Overview/Game-Vision.md`

### What's Implemented
✅ **Phase 1:** GameManager, GridSystem (20×20 grid), GridRenderer, UnitComponent, TurnManager
🔄 **Phase 2:** SelectionSystem (compiles, not tested) - mouse unit selection with yellow ring indicator

### Critical Setup
⚠️ **REQUIRED:** GridConfig node in Editor with GridConfigComponent (see Phase-1-Foundation.md)
⚠️ **Component Registration:** Uses `REGISTER_COMPONENT()` NOT old `COMPONENT_DEFINE`
⚠️ **Node Hierarchy:** Must parent nodes to world root (see "Unigine Node Hierarchy" below)

### Next Steps
1. Test SelectionSystem with unit nodes in Editor
2. Add camera controls (zoom, pan, rotate)
3. Implement pathfinding and movement range display

---

## Working Principles

### Code Generation Policy
**Do NOT write, edit, or generate code unless explicitly requested by the user.** Ask for permission before making any code changes. The user will tell you when they want code written.

### Critical Analysis Required
**Provide honest, critical feedback.** Challenge assumptions, point out flaws, and identify potential issues. Do not agree with ideas just to be agreeable. The user values critical thinking over validation.

### Communication Style
**Be concise and technical.** Avoid:
- Unnecessary praise or ego-boosting
- Verbose explanations with fluff
- Over-enthusiasm or cheerleading
- Get directly to the point

## Project Overview

**ProjectAnu** is a turn-based tactics game built with Unigine Engine (C++/CMake) using **Pathfinder 2e Remastered** rules. Set across the four Yugas of Hindu mythology in fictionalized ancient India, players discover they are the Asuras causing the very apocalypses they seek to prevent.

### Core Features
- **PF2e 3-action economy:** Turn-based tactical combat with flexible action system
- **Grid-based combat:** Square grid with discrete elevation, PF2e cover rules
- **4 classes:** Fighter, Rogue, Cleric, Wizard (levels 1-4)
- **48 spells:** 24 per caster (Wizard/Cleric), prepared casting with heightening
- **13 encounters:** 3 per Yuga + final battle vs alternate selves
- **Squad management:** 4-6 units, permadeath, recruitment at menu-based HQ
- **Data-driven design:** CSV/JSON for spells, feats, classes, conditions

### Setting & Narrative
- **Four Yugas:** Satya (golden age) → Treta → Dvapara → Kali (iron age)
- **Narrative twist:** Player squad are the Asuras causing yuga transitions
- **Menu-based investigation:** Skill checks between missions reveal story
- **Ancient India aesthetic:** Fictionalized Mahajanapadas period, generic asset packs per yuga

### Scope
- **In scope:** PF2e levels 1-4, 8 conditions, limited feats, 3 ancestries, 8 backgrounds
- **Out of scope:** Flying, environmental destruction, 3D base, domains, rank 3+ spells

## Building the Project

### CMake Build (Recommended)
```bash
cd source
mkdir build && cd build
cmake ..
cmake --build . --config Debug    # or Release
```

### Visual Studio
Open `source/CMakeLists.txt` in Visual Studio or CLion, which will automatically configure the project.

Executables are output to `bin/`:
- `bin/ProjectAnu_x64d.exe` - Debug build
- `bin/ProjectAnu_x64.exe` - Release build

## Running the Project

### Launch Editor (for level design & testing)
```bash
./launch_editor.bat
```

### Launch Debug Build
```bash
./launch_debug.bat
```

### Launch Release Build
```bash
./launch_release.bat
```

## Architecture

### Unigine Logic System
The engine uses a three-layer logic architecture (see `source/main.cpp`):

1. **SystemLogic** (`AppSystemLogic.cpp/.h`)
   - Lifecycle: Entire application runtime
   - Purpose: Engine-level initialization, component system setup, global managers
   - Update rate: Every render frame
   - Initialize component system here: `ComponentSystem::get()->initialize()`

2. **WorldLogic** (`AppWorldLogic.cpp/.h`)
   - Lifecycle: While a world/level is loaded
   - Purpose: Game logic, entities, world state
   - Update methods:
     - `update()` - Graphics-related per render frame
     - `updatePhysics()` - Fixed rate (60 FPS default), use for game logic
     - `save()/restore()` - World serialization
   - **WARNING**: Don't modify node transforms in `updatePhysics()` - rendering is in progress

3. **EditorLogic** (`AppEditorLogic.cpp/.h`)
   - Lifecycle: Only when Unigine Editor is running
   - Purpose: Editor-specific tools, debug visualization, in-editor workflow
   - Only active during development, not in final builds

### Project Structure
```
ProjectAnu/
├── source/               # C++ source code and CMakeLists.txt
├── data/                 # Game assets, worlds (.world files), configs
│   ├── ProjectAnu.world  # Main world file
│   └── template_assets/  # Default Unigine assets
├── bin/                  # Compiled executables and Unigine runtime DLLs
├── lib/                  # Unigine engine libraries (.lib files)
├── include/              # Unigine engine headers (read-only)
└── utils/                # Build utilities
```

### Adding New Source Files
When adding `.cpp`/`.h` files to `source/`, update `source/CMakeLists.txt`:
```cmake
add_executable(${target}
    ${CMAKE_CURRENT_LIST_DIR}/AppEditorLogic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AppEditorLogic.h
    # Add new files here:
    ${CMAKE_CURRENT_LIST_DIR}/NewFile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NewFile.h
)
```

## Key Technical Details

- **C++ Standard**: C++14
- **Precision**: Double precision (`UNIGINE_DOUBLE 1`)
- **Platform**: Windows x64 (cross-platform CMake configuration)
- **Component System**: Unigine's native component system initialized in `AppSystemLogic::init()`

## Phase 1 Implementation (COMPLETED)

### Systems Implemented

**GameManager** (`GameManager.h/cpp`)
- Central coordinator for all game systems
- Owns GridSystem, TurnManager, GridRenderer, and future systems (Combat, Spells)
- Finds and reads GridConfigComponent from world at initialization
- AppWorldLogic delegates all work to GameManager

**GridSystem** (`Grid/GridSystem.h/cpp`, `Grid/GridCell.h`)
- 20×20 configurable tactical grid (dimensions from GridConfigComponent)
- Discrete elevation support (z-levels)
- GridCell struct: position (x, y, z), elevation, blocked flag, occupant
- Methods: getCell(), isValidPosition(), isBlocked(), setElevation(), setOccupant()
- Distance calculation using 5-5-10 diagonal approximation
- Flat array storage for performance

**GridConfigComponent** (`Components/GridConfigComponent.h/cpp`)
- **Data-driven configuration** - no magic numbers in code!
- Place on a dummy node named "GridConfig" in the Editor
- Configurable parameters:
  - Grid dimensions: `grid_width`, `grid_height` (default: 20×20)
  - Cell sizing: `cell_size` (default: 1.0m = PF2e 5ft square), `elevation_height` (1.0m)
  - Visual rendering: `tile_coverage` (0.9 = 90% with 10% gaps), `tile_height_offset` (0.1m above floor), `tile_thickness` (0.01m)
  - Colors: `grid_color` (semi-transparent gray), `highlight_color` (cyan)
- All parameters editable in Editor without recompiling

**GridRenderer** (`UI/GridRenderer.h/cpp`)
- Visual representation of tactical grid
- Uses `Primitives::createBox()` for reliable mesh creation
- Grid cells: thin tiles (0.01m thick) with gaps between them
- Positioned 0.1m above floor to avoid z-fighting
- Color and size driven by GridConfigComponent
- Methods:
  - `createGridVisuals()` - Generates 400 box meshes (20×20 grid)
  - `highlightCell()` / `highlightCells()` - Show movement range, targeting
  - `clearHighlights()` - Remove highlights
  - `show()` / `hide()` - Toggle grid visibility
  - `gridToWorld()` - Convert grid coordinates to world position

**UnitComponent** (`Components/UnitComponent.h/cpp`)
- Unigine Component for units with PF2e stats
- Uses modern `REGISTER_COMPONENT()` macro (NOT old `COMPONENT_DEFINE`)
- Editor-exposed properties via `PROP_PARAM`:
  - Identity: `unit_id`, `unit_name`, `level`
  - Combat: `max_hp`, `current_hp`, `armor_class`, `speed`
  - Ability Scores: `strength`, `dexterity`, `constitution`, `intelligence`, `wisdom`, `charisma`
  - Turn tracking: `initiative`, `actions_remaining`, `current_map`, `has_reaction`
- Methods: `takeDamage()`, `heal()`, `isAlive()`, ability modifiers (e.g., `getWisdomMod()`)

**TurnManager** (`Core/TurnManager.h/cpp`)
- PF2e turn-based combat management
- Initiative system: Perception + d20, player units win ties
- 3-action economy: Each unit gets 3 actions per turn
- Multiple Attack Penalty (MAP): -5/-10 standard, -4/-8 agile weapons
- Reaction tracking: 1 per round, refreshes at turn start
- Uses C++11 `<random>` (Mersenne Twister) for d20 rolls
- Methods:
  - `startCombat()` / `endCombat()` - Combat lifecycle
  - `rollInitiative()` - Roll for all units, sort by result
  - `startNextTurn()` / `endCurrentTurn()` - Turn progression
  - `canSpendActions()` / `spendActions()` - Action economy
  - `getCurrentMAP()` - Calculate current attack penalty
  - `hasReaction()` / `spendReaction()` - Reaction tracking

### Critical Technical Learnings

**Unigine Component System:**
- Use `REGISTER_COMPONENT(ComponentName);` in .cpp file (NOT `COMPONENT_DEFINE`)
- Component properties: `PROP_PARAM(Type, name, default_value)`
- Components auto-register when `ComponentSystem::get()->initialize()` is called
- Get component from node: `ComponentSystem::get()->getComponent<T>(node)`

**Unigine Node Hierarchy:**
- Nodes created with `NodeDummy::create()` are NOT automatically in world
- **Must parent to world:** Call `root_nodes[0]->addChild(node)` after creation
- Check if in world: `node->isWorld()` should return `1`
- Get world root nodes: `World::getRootNodes(vector)`

**ObjectMeshDynamic API:**
- `addVertex(const vec3 &xyz)` - Takes `vec3`, not `dvec3` (even in double precision mode)
- **CRITICAL:** Must call `flushVertex()` and `flushIndices()` to upload data to GPU
- Without flush calls, mesh exists in memory but never renders
- Use `Primitives::createBox(vec3 size)` for simple meshes - much more reliable

**Materials:**
- Finding materials: `Materials::findMaterialByPath("path/to/material.mat")`
- Unigine core materials may not be accessible via simple paths
- Fallback: Use existing project materials (e.g., `"DummyUnits/capsule_mesh_base_0.mat"`)
- Modifying materials: `material->inherit()` then `setParameterFloat4("albedo_color", vec4)`

**Double Precision Mode (`UNIGINE_DOUBLE 1`):**
- World positions use `dvec3` (double precision)
- Mesh vertex positions use `vec3` (single precision)
- Must cast: `vec3(dvec3_position)` when passing to mesh APIs
- `setWorldPosition()` expects `dvec3`

### Setup Requirements

**GridConfig Node (CRITICAL):**
1. Create a dummy node named exactly **"GridConfig"** in the Editor
2. Add **GridConfigComponent** to it
3. Configure parameters as needed (or use defaults)
4. Save world
5. GameManager will find this node at startup and fail if missing

**World Structure:**
```
ProjectAnu.world
├── ground (existing world root)
├── GridConfig (dummy node with GridConfigComponent)
├── Player_Fighter_01 (dummy with UnitComponent)
├── Player_Wizard_01 (dummy with UnitComponent)
└── Enemy_Goblin_01 (dummy with UnitComponent)
```

### Current Grid Configuration

**Default Settings (from GridConfigComponent):**
- Grid: 20×20 cells (configurable)
- Cell size: 1.0m (represents PF2e 5ft square)
- Total area: 20m × 20m
- Tile coverage: 90% (0.2m gaps between tiles)
- Tile thickness: 0.01m (1cm - paper-thin)
- Height offset: 0.1m above floor
- Colors: Semi-transparent gray grid, cyan highlights

**Grid Coordinate System:**
- Origin: (0, 0, 0) in world space
- X-axis: Grid X (left-right)
- Y-axis: Grid Y (forward-back)
- Z-axis: Elevation (up-down)
- `gridToWorld(x, y, z)` converts to world coordinates

### Files Added/Modified

**New Files:**
- `GameManager.h/cpp` - Central game coordinator
- `Grid/GridSystem.h/cpp` - Tactical grid logic
- `Grid/GridCell.h` - Grid data structures
- `Components/UnitComponent.h/cpp` - PF2e character stats
- `Components/GridConfigComponent.h/cpp` - Data-driven grid config
- `Core/TurnManager.h/cpp` - Turn-based combat management
- `UI/GridRenderer.h/cpp` - Visual grid rendering

**Modified Files:**
- `AppWorldLogic.h/cpp` - Owns GameManager, delegates all work
- `AppSystemLogic.cpp` - Removed manual component registration (auto-registers now)
- `CMakeLists.txt` - Added all Phase 1 source files

## Phase 2 Implementation (IN PROGRESS)

**Status:** SelectionSystem implemented and compiling, not yet tested in-game
**Date:** November 18, 2025

### Systems Implemented

**SelectionSystem** (`Input/SelectionSystem.h/cpp`)
- Mouse-based unit selection via raycasting
- Visual selection indicator (yellow cylinder ring)
- One-shot click detection (prevents multi-trigger)
- Integration: Called from GameManager::handleInput() every frame
- Methods:
  - `update()` - Handles mouse input and updates indicator position
  - `raycastForUnit()` - Performs World::getIntersection() to detect units
  - `handleMouseInput()` - Detects left-click transitions
  - `clearSelection()` - Deselects unit and hides indicator
  - `getSelectedUnit()` - Returns currently selected UnitComponent*

**API Corrections Made:**
During implementation, SelectionSystem had 15 compilation errors due to incorrect Unigine API usage. All fixed:
- ❌ `deleteLater()` → ✅ `removeWorldChild()` for cleanup
- ❌ `Primitives::createTorus()` → ✅ `Primitives::createCylinder()` (thin disc)
- ❌ `Materials::findMaterialByGUID(string)` → ✅ `getMaterial(0)->inherit()` pattern
- ❌ `Input::getMouseCoordX/Y()` → ✅ `Input::getMousePosition()` returns ivec2
- ❌ `Player::getDirectionFromScreen()` → ✅ `getDirectionFromMainWindow()`
- ❌ `Game::getIntersection()` → ✅ `World::getIntersection()`
- ❌ `hitNode->getNode()` → ✅ Direct node usage
- ❌ `getUnitName()` → ✅ `unit_name.get()` (PROP_PARAM access)

**Integration with GameManager:**
```cpp
// GameManager.h
Unigine::SelectionSystem* selection;

// GameManager.cpp::init()
selection = new Unigine::SelectionSystem();
selection->init();

// GameManager.cpp::handleInput() - called every frame
if (selection) {
    selection->update();
}

// GameManager.cpp::shutdown()
delete selection;
selection = nullptr;
```

**Files Added:**
- `source/Input/SelectionSystem.h`
- `source/Input/SelectionSystem.cpp`

**Modified Files:**
- `CMakeLists.txt` - Added SelectionSystem files to build
- `GameManager.h/cpp` - Added selection system ownership and lifecycle
- `AppSystemLogic.cpp` - Added mouse cursor control (tactical game mode)

**Mouse Cursor Fix (Critical for Tactical Games):**
```cpp
// In AppSystemLogic::update() - runs every frame
if (Input::isMouseGrab())
{
    Input::setMouseGrab(false);              // Disable mouse grab/lock
    Gui::getCurrent()->setMouseEnabled(true); // Ensure GUI can receive mouse events
}
```
**Why:** Unigine's player/camera systems internally grab the mouse (first-person game behavior). Tactical games need free cursor for unit selection and UI. Must check continuously as systems re-grab the mouse after initialization.

### Current Limitations (Phase 2)

- ⚠️ **Untested:** SelectionSystem compiles but not tested with actual unit nodes in scene
- ❌ **No camera controls:** Cannot zoom, pan, or rotate view
- ❌ **No movement range:** Cannot visualize reachable cells
- ❌ **No pathfinding:** No A* implementation yet
- ❌ **No movement actions:** Stride/Climb not implemented
- ❌ **No turn UI:** No initiative order, action pips, or stat display

### Testing Requirements (Phase 2)

To test SelectionSystem:
1. Create unit nodes in Editor (ObjectMeshStatic with collision)
2. Add UnitComponent to each unit node
3. Set unit_name property (e.g., "Fighter", "Goblin")
4. Run game, click on units
5. Verify: Yellow ring appears at unit feet, console logs "Unit selected: [name]"
6. Click empty space to deselect

### Next Steps (Phase 2 Completion)

- **Movement System:** Pathfinding, Stride/Climb actions, terrain costs
- **Unit Selection:** Mouse picking, selected unit indicator
- **Movement Range Display:** Highlight reachable cells based on Speed
- **Combat Actions:** Strike action, attack rolls, damage, MAP application
- **Camera Controls:** Tactical camera (zoom, pan, rotate)
- **Turn UI:** Initiative order display, action counter, end turn button

## Planned Game Architecture

The PF2e tactics game will be organized into these systems (to be added to `source/`):

- `Core/` - Game state manager, PF2e turn system (3 actions), initiative, round tracking
- `Grid/` - Square grid with discrete elevation, pathfinding, Climb/Stride actions
- `Combat/` - PF2e attack rolls, saves, damage, Multiple Attack Penalty (MAP)
- `Spells/` - Prepared casting, spell slots, heightening, targeting (single/AOE)
- `Conditions/` - 8 core conditions (Flat-footed, Frightened, Prone, Stunned, Slowed, Grabbed, Sickened, Clumsy/Enfeebled)
- `Cover/` - PF2e corner-based cover (lesser/standard/greater), height advantage
- `Units/` - Character stats, PF2e progression (levels 1-4), class features, feats
- `AI/` - Enemy behavior trees, PF2e action selection
- `UI/` - 3-action economy HUD, spell preparation menu, initiative order, cover indicators
- `Meta/` - Menu-based HQ, recruitment, investigation skill checks, mission select
- `Data/` - CSV/JSON for spells, feats, class features, conditions, enemies

### Key PF2e Systems to Implement
- **3-Action Economy:** Each unit gets 3 actions/turn (Stride, Strike, Climb, Cast, etc.)
- **Multiple Attack Penalty:** 2nd attack -5, 3rd attack -10 (or -4/-8 for agile weapons)
- **Prepared Casting:** Daily spell selection for Wizard/Cleric, spell slots per rank
- **Spell Heightening:** Cast lower-rank spells in higher slots for enhanced effects
- **Discrete Elevation:** Grid cells have height, Climb action (1 action = move Speed upward)
- **Height Advantage:** +1 per 10ft elevation (max +4), reduces cover by 1 level per 10ft
- **Corner-Based Cover:** Ray-trace from attacker corner to target corner through terrain
- **Conditions:** Value-based (Frightened 2), decrement end-of-turn, affect checks/AC

## Game Design Document

**Location:** `documentation/GDD/` (Obsidian vault)

### Key GDD Documents
- **GDD-Structure.md** - Main index and navigation
- **00-Overview/Game-Vision.md** - Core vision, pillars, scope, setting
- **01-Core-Mechanics/Movement-Grid-System.md** - Grid, elevation, cover, Climb/Stride
- **01-Core-Mechanics/Turn-System.md** - 3-action economy, initiative, MAP
- **02-Systems/Character-Progression.md** - Levels 1-4, classes, spells, feats
- **03-Meta-Layer/Campaign-Structure.md** - 13 encounters, Yuga narrative, investigation

### Design Decisions Summary
- **Combat system:** PF2e Remastered rules (not XCOM-style)
- **Movement:** Square grid, 5-5-10 diagonals, Climb (1 action) for upward movement
- **Elevation:** Discrete levels, +1 to hit per 10ft (max +4), reduces cover
- **Spells:** 48 total (24 Wizard, 24 Cleric), prepared casting, heightening
- **Campaign:** 13 encounters across 4 Yugas + final battle
- **Setting:** Fictionalized ancient India (Yugas), not Golarion
- **Meta-layer:** Menu-based HQ, no 3D base building (scope TBD for research/engineering)

## Development Workflow

1. **Design:** Update GDD in `documentation/GDD/` (Obsidian)
2. **Implement:** Write game logic in `source/` directory
3. **Assets:** Create/import 3D assets in Unigine Editor (`launch_editor.bat`)
4. **Build:** Compile with CMake
5. **Test:** Run debug build (`launch_debug.bat`)
6. **Iterate:** Balance, polish, playtest

## Version Control & Assets

### Git LFS Configuration
**This project uses Git LFS for large binary files.**

**Tracked by LFS (see `.gitattributes`):**
- Unigine packages: `*.ung`, `*.upackage`
- Cache files: `*.cache`
- 3D models: `*.fbx`, `*.blend`, `*.obj`
- Textures: `*.psd`, `*.tga`, `*.exr`
- Audio: `*.wav`, `*.ogg`, `*.mp3`
- Video: `*.mp4`, `*.mov`
- Worlds: `*.world`

### Adding New Assets - IMPORTANT

**When adding large asset files:**

1. **Verify file type is in `.gitattributes`**
   - If new file type (e.g., `.gltf`), add to `.gitattributes` first
   - Format: `*.gltf filter=lfs diff=lfs merge=lfs -text`

2. **Check before committing:**
   ```bash
   git lfs ls-files  # See what's tracked
   git lfs status    # See what will be committed to LFS
   ```

3. **If file is large but NOT in LFS:**
   - Add extension to `.gitattributes`
   - Run: `git add .gitattributes`
   - Then: `git add <your-large-file>`

4. **Verify LFS is handling it:**
   ```bash
   git lfs ls-files  # Your file should appear here
   ```

**Rule of thumb:**
- Files > 1MB → Should be in LFS
- Source code, text, configs → Regular Git
- Binary assets → LFS

### What NOT to Commit
Even with LFS, avoid committing:
- Temporary/cache files (already in `.gitignore`)
- Build artifacts (`bin/`, `lib/`, `cmake-build-*`)
- IDE files (`.idea/`, `.vs/`)
- Personal notes/scratch files

## Important Notes

- All file paths in CMakeLists.txt should be relative to `source/` directory
- Unigine headers in `include/` are read-only (linked from SDK installation)
- Don't modify files in `bin/`, `lib/`, or `include/` - these are managed by Unigine SDK
- Return `1` from logic methods to indicate success, `0` for failure
