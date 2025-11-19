---
title: "Current Project Status"
type: status
status: active
priority: critical
created: 2025-11-18
updated: 2025-11-18
aliases:
  - Status
  - Current State
  - Project State
---

# Current Project Status

**Last Updated:** November 18, 2025
**Build Status:** ✅ Compiling Successfully
**Executable:** `bin/ProjectAnu_x64d.exe` (Debug build)

---

## Quick Reference

**Current Phase:** Phase 2 (Input & Selection) - In Progress
**Last Completed Phase:** Phase 1 (Foundation Systems) - ✅ Complete
**Latest Commit Focus:** Selection system compilation fixes

---

## Project Overview

**ProjectAnu** - Turn-based tactics game using Pathfinder 2e Remastered rules, built with Unigine Engine (C++/CMake).

- **Engine:** Unigine (C++14, Double Precision Mode)
- **Platform:** Windows x64
- **Build System:** CMake + Visual Studio 2022
- **Scope:** 13 encounters across 4 Yugas (Hindu mythology setting)

---

## Build System Status

### How to Build

**Using MSBuild (Windows):**
```bash
"C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe" "D:/Unigine/ProjectAnu/source/build/ProjectAnu.vcxproj" -p:Configuration=Debug -nologo
```

**Using CMake:**
```bash
cd source/build
cmake ..
cmake --build . --config Debug
```

### Build Output
- **Debug:** `bin/ProjectAnu_x64d.exe`
- **Release:** `bin/ProjectAnu_x64.exe`

### Launch Scripts
- `launch_editor.bat` - Open Unigine Editor
- `launch_debug.bat` - Run debug build
- `launch_release.bat` - Run release build

---

## Implementation Status

### ✅ Phase 1: Foundation Systems (COMPLETE)

**Completion Date:** January 16, 2025

**Systems Implemented:**

1. **GameManager** (`GameManager.h/cpp`)
   - Central coordinator for all game systems
   - Owns GridSystem, TurnManager, GridRenderer, SelectionSystem
   - Initialized from AppWorldLogic
   - Handles lifecycle: init(), update(), handleInput(), shutdown()

2. **GridSystem** (`Grid/GridSystem.h/cpp`, `Grid/GridCell.h`)
   - 20×20 tactical grid (configurable via GridConfigComponent)
   - Discrete elevation support (z-levels)
   - Distance calculation (5-5-10 diagonal rule)
   - Cell occupancy tracking
   - Blocking/terrain support

3. **GridConfigComponent** (`Components/GridConfigComponent.h/cpp`)
   - **Data-driven configuration** - NO MAGIC NUMBERS!
   - Editor-exposed parameters:
     - Grid dimensions: `grid_width`, `grid_height` (default 20×20)
     - Cell size: `cell_size` (1.0m = PF2e 5ft square)
     - Elevation: `elevation_height` (1.0m per level)
     - Visual: tile coverage, colors, offsets
   - **REQUIRED SETUP:** Create dummy node named "GridConfig" in Editor with this component

4. **GridRenderer** (`UI/GridRenderer.h/cpp`)
   - Visual grid representation (400 box meshes for 20×20)
   - Uses `Primitives::createBox()` (reliable!)
   - Semi-transparent gray tiles with 10% gaps
   - Positioned 0.1m above floor (avoids z-fighting)
   - Highlight support for movement/targeting
   - Methods: createGridVisuals(), highlightCell(), clearHighlights(), show/hide()

5. **UnitComponent** (`Components/UnitComponent.h/cpp`)
   - Unigine Component with PF2e stats
   - **Registration:** Uses `REGISTER_COMPONENT()` (NOT old `COMPONENT_DEFINE`)
   - Editor-exposed properties via `PROP_PARAM`:
     - Identity: unit_id, unit_name, level
     - Combat: max_hp, current_hp, armor_class, speed
     - Abilities: STR, DEX, CON, INT, WIS, CHA
     - Turn tracking: initiative, actions_remaining, current_map, has_reaction
   - Methods: takeDamage(), heal(), isAlive(), ability modifiers

6. **TurnManager** (`Core/TurnManager.h/cpp`)
   - PF2e turn-based combat implementation
   - Initiative: Perception + d20 (C++11 `<random>`, NOT `rand()`)
   - 3-action economy per turn
   - Multiple Attack Penalty (MAP): -5/-10 standard, -4/-8 agile
   - Reaction tracking (1 per round)
   - Methods: rollInitiative(), startNextTurn(), spendActions(), getCurrentMAP()

**Key Files Added (Phase 1):**
```
source/
├── GameManager.h/cpp
├── Grid/
│   ├── GridSystem.h/cpp
│   └── GridCell.h
├── Components/
│   ├── UnitComponent.h/cpp
│   └── GridConfigComponent.h/cpp
├── Core/
│   └── TurnManager.h/cpp
└── UI/
    └── GridRenderer.h/cpp
```

---

### 🔄 Phase 2: Input & Selection (IN PROGRESS)

**Status:** SelectionSystem compiles and integrated, not yet tested in-game

**Systems Implemented:**

1. **SelectionSystem** (`Input/SelectionSystem.h/cpp`) - ✅ Added Nov 18, 2025
   - Mouse-based unit selection
   - Visual selection indicator (yellow cylinder ring)
   - Raycasting to detect units under cursor
   - One-shot click detection (prevents multi-clicks)
   - Methods: update(), raycastForUnit(), handleMouseInput(), clearSelection()
   - **Integration:** Called from GameManager::handleInput()

**Recent Fixes (Nov 18, 2025):**
- ✅ Added SelectionSystem files to CMakeLists.txt
- ✅ Fixed memory leak (GameManager::shutdown() now deletes selection)
- ✅ Wired up input loop (GameManager::handleInput() calls selection->update())
- ✅ Removed redundant forward declaration in GameManager.h
- ✅ Rewrote SelectionSystem.cpp with correct Unigine APIs (15 compilation errors → 0)
- ✅ **Fixed mouse cursor grab** - Cursor now remains free (AppSystemLogic::update() continuously disables grab)

**API Corrections Made:**
- `deleteLater()` → proper node cleanup with removeWorldChild()
- `Primitives::createTorus()` → `createCylinder()` (thin disc)
- `Materials::findMaterialByGUID(string)` → getMaterial(0)->inherit() pattern
- `Input::getMouseCoordX/Y()` → `Input::getMousePosition()` (returns ivec2)
- `Player::getDirectionFromScreen()` → `getDirectionFromMainWindow()`
- `Game::getIntersection()` → `World::getIntersection()`
- `UnitComponent::getUnitName()` → `unit_name.get()` (PROP_PARAM access)

**Key Files Added (Phase 2):**
```
source/
└── Input/
    ├── SelectionSystem.h
    └── SelectionSystem.cpp
```

**Still TODO for Phase 2:**
- [ ] Test SelectionSystem in-game with actual unit nodes
- [ ] Camera controls (zoom, pan, rotate)
- [ ] Movement range highlighting
- [ ] Pathfinding system (A* with elevation)
- [ ] Movement actions (Stride, Climb)
- [ ] Turn UI integration (show selected unit's stats)

---

## Critical Technical Knowledge

### Unigine Component System (Modern API)

**Component Registration (2024+ API):**
```cpp
// In .cpp file:
REGISTER_COMPONENT(ComponentName);  // Auto-registers on init

// NOT this (old API):
COMPONENT_DEFINE(ComponentName, ComponentBase);  // WRONG!
```

**Component Properties:**
```cpp
PROP_PARAM(Type, variable_name, default_value);
// Access: component->variable_name.get()
```

**Getting Component from Node:**
```cpp
auto* comp = ComponentSystem::get()->getComponent<UnitComponent>(node);
```

**Initialization:**
- Call `ComponentSystem::get()->initialize()` in `AppSystemLogic::init()`
- Components with `REGISTER_COMPONENT()` auto-register

### Unigine Node Hierarchy (CRITICAL!)

**Nodes are NOT automatically in world when created!**

```cpp
// Create node
NodeDummyPtr node = NodeDummy::create();

// Check if in world
node->isWorld();  // Returns 0 (NOT in world!)

// FIX: Parent to world hierarchy
Vector<NodePtr> roots;
World::getRootNodes(roots);
roots[0]->addChild(node);  // Now node->isWorld() returns 1
```

**Why This Matters:**
- Nodes not in world hierarchy are invisible
- They exist in memory but don't render
- No errors thrown - just silently invisible!

### ObjectMeshDynamic API

**Vertex Data (Double Precision Mode):**
```cpp
// IMPORTANT: addVertex() takes vec3, NOT dvec3!
mesh->addVertex(vec3(x, y, z));

// If using dvec3, must cast:
dvec3 world_pos = gridToWorld(x, y, z);
vec3 local_pos = vec3(world_pos);
mesh->addVertex(local_pos);
```

**CRITICAL: Must Flush to GPU!**
```cpp
mesh->addVertex(v0);
mesh->addVertex(v1);
// ... add all vertices

mesh->addIndex(0);
mesh->addIndex(1);
// ... add all indices

// REQUIRED: Upload to GPU
mesh->flushVertex();
mesh->flushIndices();

// Without flush calls, mesh never renders!
```

**Recommendation:** Use `Primitives::createBox/Cylinder/Sphere()` for simple shapes - much more reliable!

### Materials

**Finding Materials:**
```cpp
// Unigine core materials not accessible via simple names
MaterialPtr mat = Materials::findMaterialByPath("mesh_base");  // Returns null!

// Solution 1: Use primitives (includes default material)
auto mesh = Primitives::createBox(vec3(1, 1, 1));
MaterialPtr mat = mesh->getMaterial(0);  // Works!

// Solution 2: Inherit and modify
mat = mat->inherit();  // Create instance
mat->setParameterFloat4("albedo_color", vec4(r, g, b, a));
mesh->setMaterial(mat, 0);
```

### Double Precision Mode (`UNIGINE_DOUBLE 1`)

| API | Precision | Example |
|-----|-----------|---------|
| `setWorldPosition()` | dvec3 | `node->setWorldPosition(dvec3(x, y, z))` |
| `getWorldPosition()` | dvec3 | `dvec3 pos = node->getWorldPosition()` |
| `addVertex()` | **vec3** | `mesh->addVertex(vec3(x, y, z))` |
| `Primitives::createBox()` | vec3 | `createBox(vec3(w, h, d))` |

### Input System

**Mouse Input:**
```cpp
ivec2 mouse_pos = Input::getMousePosition();  // Screen coordinates
bool left_click = Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT);
```

**Raycasting:**
```cpp
PlayerPtr player = Game::getPlayer();
dvec3 camera_pos = player->getWorldPosition();
vec3 direction = player->getDirectionFromMainWindow(mouse.x, mouse.y);

ObjectPtr hit_object = World::getIntersection(
    camera_pos,
    camera_pos + dvec3(direction) * 1000.0,
    1  // Intersection mask
);
```

**Mouse Cursor Control (CRITICAL FOR TACTICAL GAMES):**
```cpp
// In AppSystemLogic::update() - run EVERY frame
if (Input::isMouseGrab())
{
    Input::setMouseGrab(false);              // Disable mouse grab/lock
    Gui::getCurrent()->setMouseEnabled(true); // Ensure GUI can receive mouse events
}
```

**Why Continuous Check Required:**
- Unigine's player/camera systems may re-grab the mouse internally
- First-person games typically want mouse grab, but tactical games need free cursor
- Checking every frame has minimal overhead (only acts if grabbed)
- This is the **correct solution** for turn-based tactical games

---

## World Setup Requirements

### GridConfig Node (CRITICAL - MUST EXIST!)

**Setup Steps:**
1. Open Unigine Editor (`launch_editor.bat`)
2. Create a dummy node named exactly **"GridConfig"**
3. Add component: **GridConfigComponent**
4. Configure parameters (or use defaults):
   - Grid: 20×20 cells
   - Cell size: 1.0m
   - Elevation: 1.0m per level
   - Tile coverage: 90% (0.9)
5. Save world
6. GameManager will find this node at startup and fail gracefully if missing

**Expected World Structure:**
```
ProjectAnu.world
├── ground (existing world root)
├── GridConfig (dummy node with GridConfigComponent) ← REQUIRED!
├── Player_Fighter_01 (dummy with UnitComponent)
├── Player_Wizard_01 (dummy with UnitComponent)
└── Enemy_Goblin_01 (dummy with UnitComponent)
```

### Component Registration

**In AppSystemLogic.cpp:**
```cpp
int AppSystemLogic::init()
{
    // Initialize component system (auto-registers all REGISTER_COMPONENT components)
    ComponentSystem::get()->initialize();
    return 1;
}
```

**Components auto-register:**
- UnitComponent
- GridConfigComponent
- (Future: SpellcasterComponent, etc.)

---

## Known Issues & Limitations

### Phase 1 Limitations
- ✅ Grid renders but no pathfinding yet
- ✅ No unit selection (fixed in Phase 2)
- ✅ No movement system
- ✅ TurnManager compiled but not tested in combat
- ✅ No turn UI (initiative order display)
- ✅ No combat actions (Strike, damage, etc.)

### Phase 2 Current State
- ✅ SelectionSystem compiles and integrated
- ⚠️ SelectionSystem NOT tested with actual unit nodes in scene
- ⚠️ No camera controls yet
- ⚠️ No movement range highlighting
- ⚠️ No pathfinding implementation

### Testing Gaps
- **Manual testing required:**
  1. Does SelectionSystem detect units correctly?
  2. Does selection indicator appear at unit position?
  3. Does deselection work (click empty space)?
  4. Does raycasting work with unit meshes?

---

## Architecture Overview

### System Ownership

```
AppWorldLogic (Unigine layer)
└── owns GameManager
    ├── owns GridSystem (20×20 grid, elevation)
    ├── owns TurnManager (initiative, 3-action economy)
    ├── owns GridRenderer (visual grid)
    ├── owns SelectionSystem (mouse picking)
    ├── owns CombatResolver (NULL - not implemented yet)
    └── owns SpellSystem (NULL - not implemented yet)
```

### Update Loop

**AppWorldLogic::update() (per render frame, ~60-144 FPS):**
```cpp
void AppWorldLogic::update()
{
    game->handleInput();  // Mouse, keyboard
}
```

**AppWorldLogic::updatePhysics() (fixed 60 FPS):**
```cpp
void AppWorldLogic::updatePhysics()
{
    game->update(ifps);  // Game logic, AI, turn management
}
```

### Data Flow: Unit Selection

```
1. User clicks mouse
      ↓
2. SelectionSystem::handleInput() (called from GameManager::handleInput())
      ↓
3. Detect click (previousMouseButtonState transition)
      ↓
4. raycastForUnit() - World::getIntersection()
      ↓
5. Check if hit object has UnitComponent
      ↓
6. Set selectedUnit, log "Unit selected: [name]"
      ↓
7. updateIndicatorPosition() moves yellow ring to unit position
      ↓
8. selectionIndicator->setEnabled(1) - ring visible
```

---

## File Structure (Current)

```
ProjectAnu/
├── source/                     # C++ source code
│   ├── main.cpp
│   ├── AppSystemLogic.cpp/h    # Engine initialization
│   ├── AppWorldLogic.cpp/h     # Owns GameManager
│   ├── AppEditorLogic.cpp/h    # Editor tools
│   ├── GameManager.cpp/h       # Central coordinator
│   ├── CMakeLists.txt          # Build configuration
│   │
│   ├── Grid/
│   │   ├── GridSystem.cpp/h    # Grid logic
│   │   └── GridCell.h          # Grid data structures
│   │
│   ├── Components/
│   │   ├── UnitComponent.cpp/h         # PF2e character stats
│   │   └── GridConfigComponent.cpp/h   # Data-driven grid config
│   │
│   ├── Core/
│   │   └── TurnManager.cpp/h   # Turn system, initiative, MAP
│   │
│   ├── UI/
│   │   └── GridRenderer.cpp/h  # Visual grid rendering
│   │
│   └── Input/
│       ├── SelectionSystem.cpp/h  # Unit selection (NEW - Phase 2)
│
├── data/                       # Game assets, worlds, configs
│   ├── ProjectAnu.world        # Main world file
│   ├── configs/                # Engine configs
│   ├── template_assets/        # Default Unigine assets
│   └── (future: spells/, units/)
│
├── bin/                        # Compiled executables
│   ├── ProjectAnu_x64d.exe     # Debug build
│   └── ProjectAnu_x64.exe      # Release build
│
├── lib/                        # Unigine engine libraries
├── include/                    # Unigine headers (read-only)
├── documentation/              # GDD (Obsidian vault)
│   └── GDD/
│       ├── Current_Status.md   # THIS FILE
│       ├── GDD-Structure.md
│       ├── 00-Overview/
│       ├── 01-Core-Mechanics/
│       ├── 02-Systems/
│       ├── 03-Meta-Layer/
│       ├── 04-Implementation/
│       │   └── Phase-1-Foundation.md
│       ├── 06-Technical/
│       └── 07-Development/
│
├── CLAUDE.md                   # Instructions for Claude Code
├── .gitattributes              # Git LFS configuration
└── launch_*.bat                # Launch scripts
```

---

## Next Steps (Priority Order)

### Immediate (Testing Phase 2)
1. **Test SelectionSystem in Editor:**
   - Create unit nodes with UnitComponent in Editor
   - Add meshes/collision to units (ObjectMeshStatic)
   - Run game, test mouse clicking on units
   - Verify selection indicator appears

2. **Debug SelectionSystem if issues:**
   - Check if raycasting hits unit meshes
   - Verify UnitComponent is found on clicked nodes
   - Test selection indicator visibility
   - Log all raycasting results

### Short-Term (Complete Phase 2)
3. **Camera Controls:**
   - Tactical camera (zoom, pan, rotate, pitch)
   - Mouse wheel zoom
   - Middle-mouse drag for pan
   - Right-mouse drag for rotate

4. **Movement Range Display:**
   - Calculate reachable cells based on Speed
   - Highlight cells in range (use GridRenderer::highlightCells())
   - Show different colors for 1/2/3 action ranges

5. **Turn UI:**
   - Initiative order display (vertical list)
   - Action counter (3 action pips)
   - End Turn button
   - Selected unit stat display (HP, AC, etc.)

### Medium-Term (Phase 3: Movement)
6. **Pathfinding System:**
   - A* implementation with discrete elevation
   - Respect 5-5-10 diagonal costs
   - Handle blocked cells
   - Return action cost for path

7. **Movement Actions:**
   - Stride action (1 action, horizontal + descend)
   - Climb action (1 action, vertical upward)
   - Preview path before committing
   - Animate unit movement

### Long-Term (Phase 4+)
8. **Combat System:** Attack rolls, damage, saves
9. **Spell System:** Prepared casting, heightening, targeting
10. **Conditions:** 8 core conditions (Frightened, Prone, etc.)
11. **AI System:** Enemy behavior trees
12. **Meta Layer:** HQ, recruitment, progression

---

## Common Commands

### Build
```bash
# Clean rebuild (Windows)
"C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe" "D:/Unigine/ProjectAnu/source/build/ProjectAnu.vcxproj" -p:Configuration=Debug -t:Rebuild -nologo

# Incremental build
"C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe" "D:/Unigine/ProjectAnu/source/build/ProjectAnu.vcxproj" -p:Configuration=Debug -nologo
```

### Launch
```bash
# Editor
./launch_editor.bat

# Debug build
./launch_debug.bat

# Release build
./launch_release.bat
```

### Git LFS
```bash
# Check LFS status
git lfs ls-files

# See what will be committed to LFS
git lfs status

# Add new file type to LFS
echo "*.gltf filter=lfs diff=lfs merge=lfs -text" >> .gitattributes
```

---

## Contact Points for Resuming Work

### If Build Fails
- Check: CMakeLists.txt has all .cpp/.h files listed
- Check: GridConfig node exists in world with GridConfigComponent
- Check: Component registration in AppSystemLogic::init()
- See: "Critical Technical Knowledge" section above

### If Rendering Issues
- Check: Nodes added to world hierarchy (isWorld() returns 1)
- Check: Materials inherited before modification
- Check: ObjectMeshDynamic flushed (flushVertex/flushIndices)
- See: GridRenderer.cpp for working examples

### If Component Issues
- Check: Using REGISTER_COMPONENT() not COMPONENT_DEFINE
- Check: ComponentSystem::get()->initialize() called
- Check: Accessing PROP_PARAM with .get() (e.g., unit_name.get())
- See: UnitComponent.h/cpp for examples

### If Selection Not Working
- Check: Unit nodes have collision (ObjectMeshStatic or physics)
- Check: World::getIntersection() mask matches object masks
- Check: UnitComponent attached to correct nodes
- See: SelectionSystem.cpp raycastForUnit() method

---

## Documentation Map

**Quick Start:**
- THIS FILE (`Current_Status.md`) - Current state, what's done, what's next
- `CLAUDE.md` - Instructions for AI assistants, working principles

**Game Design:**
- `GDD-Structure.md` - Main index
- `00-Overview/Game-Vision.md` - Core vision, scope, pillars
- `01-Core-Mechanics/` - PF2e rules (Turn System, Movement, Combat)
- `02-Systems/Character-Progression.md` - Classes, spells, leveling
- `03-Meta-Layer/Campaign-Structure.md` - 13 encounters, Yugas, narrative

**Implementation:**
- `04-Implementation/Phase-1-Foundation.md` - Phase 1 details, learnings
- `06-Technical/Technical-Architecture.md` - System design, data flow
- `07-Development/Prototype-Scope.md` - What's in/out of scope

---

**STATUS SUMMARY:**
✅ Phase 1 Complete | 🔄 Phase 2 In Progress | 🎯 Next: Test Selection System
**Build:** ✅ Compiling | **Executable:** `bin/ProjectAnu_x64d.exe`
