---
aliases: [Phase 1, Foundation Systems, Grid Implementation]
tags: [implementation, phase-1, grid, components, complete]
status: completed
created: 2025-01-16
updated: 2025-01-16
---

# Phase 1: Foundation Systems (COMPLETED)

**Status:** ✅ Fully Implemented and Tested
**Duration:** Week 1
**Completion Date:** January 16, 2025

## Overview

Phase 1 establishes the core foundation for ProjectAnu's tactical combat system. All systems are implemented in C++ using Unigine Engine's component-based architecture with a data-driven design philosophy (no magic numbers!).

## Systems Implemented

### 1. GameManager (Central Coordinator)

**Location:** `source/GameManager.h/cpp`

**Purpose:** Central hub that owns and coordinates all game systems. Keeps AppWorldLogic clean by delegating all game logic.

**Features:**
- Owns all game systems (GridSystem, TurnManager, GridRenderer, future Combat/Spells)
- Reads GridConfigComponent from world at startup
- Validates configuration before initialization
- Provides lifecycle management (init, update, shutdown)
- Future: Combat state machine, unit management

**Usage:**
```cpp
// AppWorldLogic::init()
game = new GameManager();
game->init();  // Finds GridConfig node, creates systems

// AppWorldLogic::update()
game->handleInput();  // Per-frame input

// AppWorldLogic::updatePhysics()
game->update(dt);  // Fixed 60 FPS game logic
```

**Error Handling:**
- Fails gracefully if GridConfig node not found
- Logs detailed error messages to console
- Returns early to prevent crashes

---

### 2. GridSystem (Tactical Grid Logic)

**Location:** `source/Grid/GridSystem.h/cpp`, `source/Grid/GridCell.h`

**Purpose:** Manages the 20×20 tactical grid with discrete elevation support.

**Data Structures:**

```cpp
struct GridPosition {
    int x, y, z;  // z = elevation level
};

struct GridCell {
    GridPosition position;
    int elevation;           // Height level (0 = ground)
    bool blocked;            // Terrain blocking
    Unigine::NodePtr occupant;  // Unit in this cell
};
```

**Features:**
- Configurable dimensions (default: 20×20 from GridConfigComponent)
- Flat array storage for cache-friendly performance
- Discrete elevation system (z-levels)
- Occupancy tracking (one unit per cell)
- Distance calculation using 5-5-10 diagonal approximation
- Validation methods (isValidPosition, isBlocked)

**Key Methods:**
```cpp
GridCell* getCell(int x, int y);
bool isValidPosition(int x, int y) const;
bool isBlocked(GridPosition pos) const;
void setElevation(int x, int y, int elevation);
void setOccupant(GridPosition pos, Unigine::NodePtr unit);
int getDistance(GridPosition a, GridPosition b) const;
```

**Grid Coordinate System:**
- Origin: (0, 0, 0) at world position (0, 0, 0)
- X-axis: Grid columns (left-right)
- Y-axis: Grid rows (forward-back)
- Z-axis: Elevation (up-down, discrete levels)

---

### 3. GridConfigComponent (Data-Driven Configuration)

**Location:** `source/Components/GridConfigComponent.h/cpp`

**Purpose:** **Eliminates magic numbers** by centralizing all grid configuration in a single Editor component.

**Configurable Parameters:**

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `grid_width` | Int | 20 | Grid cells in X direction |
| `grid_height` | Int | 20 | Grid cells in Y direction |
| `cell_size` | Float | 1.0 | Meters per cell (PF2e 5ft square) |
| `elevation_height` | Float | 1.0 | Meters per elevation level |
| `tile_coverage` | Float | 0.9 | Tile size % (0.9 = 10% gaps) |
| `tile_height_offset` | Float | 0.1 | Height above floor (meters) |
| `tile_thickness` | Float | 0.01 | Tile thickness (meters) |
| `grid_color` | Vec4 | (0.4, 0.4, 0.4, 0.7) | Semi-transparent gray |
| `highlight_color` | Vec4 | (0.0, 0.8, 1.0, 0.8) | Cyan for highlights |

**Setup (REQUIRED):**
1. In Unigine Editor, create a dummy node named **"GridConfig"**
2. Add component: **GridConfigComponent**
3. Adjust parameters as needed (or keep defaults)
4. Save world
5. GameManager finds this node at startup

**Benefits:**
- Designer-friendly: Tweak grid in Editor without recompiling
- Consistent values across all systems
- Easy A/B testing of grid configurations
- Can have different configs per level/encounter

---

### 4. GridRenderer (Visual Grid Display)

**Location:** `source/UI/GridRenderer.h/cpp`

**Purpose:** Renders the tactical grid as thin, semi-transparent tiles with gaps.

**Features:**
- Uses `Primitives::createBox()` for reliable mesh creation
- Configurable appearance via GridConfigComponent
- Grid cells: 1cm thick tiles with 10% gaps
- Positioned 0.1m above floor (avoids z-fighting)
- Parent to world hierarchy (visible in scene)
- Highlight support for movement/targeting ranges

**Key Methods:**

```cpp
void createGridVisuals();  // Generate 400 box meshes
void destroyGridVisuals(); // Clean up all meshes
void highlightCell(GridPosition pos, vec4 color);
void highlightCells(const Vector<GridPosition>& cells, vec4 color);
void clearHighlights();    // Remove all highlights
void show() / hide();      // Toggle visibility
dvec3 gridToWorld(int x, int y, int elevation);  // Coordinate conversion
```

**Visual Specifications:**
- **Tile Size:** 90% of cell (0.9m × 0.9m for 1m cells)
- **Gaps:** 10% (0.1m gaps between tiles)
- **Thickness:** 0.01m (paper-thin)
- **Height:** 0.1m above floor
- **Color:** Semi-transparent gray (configurable)
- **Highlights:** Slightly above grid tiles (+0.02m)

**Rendering Pipeline:**
1. Create grid root node (dummy)
2. Parent grid root to world hierarchy (**CRITICAL**)
3. For each cell: Create box primitive, set material, parent to grid root
4. Total: 400 meshes for 20×20 grid

---

### 5. UnitComponent (PF2e Character Stats)

**Location:** `source/Components/UnitComponent.h/cpp`

**Purpose:** Unigine component holding all PF2e character statistics for units.

**Properties (Editor-Exposed via `PROP_PARAM`):**

**Identity:**
- `unit_id` (String) - Unique identifier
- `unit_name` (String) - Display name
- `level` (Int, default: 1) - Character level (1-4)

**Combat Stats:**
- `max_hp` (Int, default: 10) - Maximum hit points
- `current_hp` (Int, default: 10) - Current hit points
- `armor_class` (Int, default: 10) - AC (10 + bonuses)
- `speed` (Int, default: 25) - Movement speed (feet, 5ft squares)

**Ability Scores (10 = +0 modifier):**
- `strength`, `dexterity`, `constitution`
- `intelligence`, `wisdom`, `charisma`

**Turn Tracking:**
- `initiative` (Int) - Initiative roll result
- `actions_remaining` (Int, default: 3) - Actions left this turn
- `current_map` (Int, default: 0) - Multiple Attack Penalty
- `has_reaction` (Int, default: 1) - Boolean (1 = yes)

**Other:**
- `grid_position` (GridPosition) - Current grid cell
- `is_player_unit` (Bool) - Player vs enemy

**Methods:**
```cpp
void takeDamage(int amount);  // Reduce HP, log to console
void heal(int amount);        // Restore HP (max capped)
bool isAlive() const;         // HP > 0
int getWisdomMod() const;     // (wisdom - 10) / 2
// ... other ability modifiers
```

**Component Registration:**
```cpp
// In .cpp file:
REGISTER_COMPONENT(UnitComponent);  // Modern Unigine API
```

---

### 6. TurnManager (PF2e Combat Management)

**Location:** `source/Core/TurnManager.h/cpp`

**Purpose:** Implements PF2e turn-based combat rules: initiative, 3-action economy, MAP.

**Data Structures:**

```cpp
struct InitiativeEntry {
    Unigine::NodePtr unit_node;
    UnitComponent* unit_component;  // Cached for performance
    int initiative_value;           // Perception + d20
    bool is_player_unit;            // Ties: player wins
};

enum class ActionType {
    NONE,         // Not an attack
    STRIKE,       // Standard Strike (triggers MAP)
    SPELL_ATTACK, // Spell with attack roll (triggers MAP)
    SPELL_SAVE,   // Spell with save (does NOT trigger MAP)
    ABILITY       // Class ability (varies)
};
```

**Features:**

**Initiative System:**
- Roll: d20 + Wisdom modifier (Perception)
- Uses C++11 `<random>` (Mersenne Twister, NOT `rand()`)
- Sorted highest to lowest
- Ties: Player units act before enemies

**3-Action Economy:**
- Each unit: 3 actions per turn
- Actions cost 1-3 actions (e.g., Strike = 1, Sudden Charge = 2)
- `canSpendActions(cost)` checks availability
- `spendActions(cost, type)` deducts and tracks MAP

**Multiple Attack Penalty (MAP):**
- **Standard weapons:** -5 (2nd attack), -10 (3rd+ attack)
- **Agile weapons:** -4 (2nd attack), -8 (3rd+ attack)
- Tracked per turn, resets at turn end
- Only attack actions trigger MAP (not spell saves)

**Reaction System:**
- 1 reaction per round
- Refreshes at turn start (not round start)
- `hasReaction()` / `spendReaction()`

**Key Methods:**

```cpp
// Combat lifecycle
void startCombat(player_units, enemy_units);
void endCombat();

// Initiative
void rollInitiative(player_units, enemy_units);
void sortInitiativeOrder();  // Highest to lowest

// Turn progression
void startNextTurn();  // Advance to next unit, reset actions
void endCurrentTurn(); // Apply end-of-turn effects

// Action economy
bool canSpendActions(int cost) const;
void spendActions(int cost, ActionType type);
int getActionsRemaining() const;
int getCurrentMAP() const;  // Returns -5/-10 or -4/-8

// Reactions
bool hasReaction() const;
void spendReaction();

// Queries
UnitComponent* getCurrentUnit() const;
bool isPlayerTurn() const;
int getCurrentRound() const;
```

**Implementation Notes:**
- Uses `std::random_device` + `std::mt19937` for RNG
- Caches UnitComponent pointers for performance
- Logs all initiative rolls to console (for debugging)

---

## Critical Technical Learnings

### Unigine Component System

**Modern API (2024+):**
```cpp
// In Component.cpp:
REGISTER_COMPONENT(ComponentName);  // Auto-registers!

// NOT this (old API):
COMPONENT_DEFINE(ComponentName, ComponentBase);  // WRONG!
```

**Component Properties:**
```cpp
PROP_PARAM(Type, variable_name, default_value);
```

**Getting Component from Node:**
```cpp
auto* comp = ComponentSystem::get()->getComponent<UnitComponent>(node);
```

**Initialization:**
- `ComponentSystem::get()->initialize()` in `AppSystemLogic::init()`
- Components with `REGISTER_COMPONENT()` auto-register

---

### Unigine Node Hierarchy

**CRITICAL:** Nodes are NOT automatically in world when created!

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
- No errors - just silently invisible!

---

### ObjectMeshDynamic API

**Vertex Data:**
```cpp
// Takes vec3, NOT dvec3 (even in UNIGINE_DOUBLE mode!)
mesh->addVertex(vec3(x, y, z));

// If using dvec3, must cast:
dvec3 world_pos = gridToWorld(x, y, z);
vec3 local_pos = vec3(world_pos);
mesh->addVertex(local_pos);
```

**CRITICAL: Must Flush Data to GPU!**
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

**Recommendation:**
- Use `Primitives::createBox(vec3 size)` for simple meshes
- Much more reliable than manual ObjectMeshDynamic setup
- Includes normals, UVs, materials automatically

---

### Materials

**Finding Materials:**
```cpp
// Unigine core materials not accessible via simple names
MaterialPtr mat = Materials::findMaterialByPath("mesh_base");  // Returns null!

// Solution 1: Use project materials
mat = Materials::findMaterialByPath("DummyUnits/capsule_mesh_base_0.mat");

// Solution 2: Use Primitives (includes default material)
auto mesh = Primitives::createBox(vec3(1, 1, 1));
MaterialPtr mat = mesh->getMaterial(0);  // Works!
```

**Modifying Materials:**
```cpp
MaterialPtr mat = mesh->getMaterial(0);
mat = mat->inherit();  // Create instance
mat->setParameterFloat4("albedo_color", vec4(r, g, b, a));
mesh->setMaterial(mat, 0);
```

---

### Double Precision Mode

**When `UNIGINE_DOUBLE 1` is set:**

| API | Precision | Example |
|-----|-----------|---------|
| `setWorldPosition()` | dvec3 | `node->setWorldPosition(dvec3(x, y, z))` |
| `getWorldPosition()` | dvec3 | `dvec3 pos = node->getWorldPosition()` |
| `addVertex()` | **vec3** | `mesh->addVertex(vec3(x, y, z))` |
| `Primitives::createBox()` | vec3 | `createBox(vec3(w, h, d))` |

**Key Takeaway:** World-level coordinates use `dvec3`, but mesh vertices use `vec3`. Must cast when needed.

---

## File Structure

```
source/
├── GameManager.h/cpp              # Central coordinator
├── AppWorldLogic.h/cpp            # Owns GameManager
├── AppSystemLogic.cpp             # Initializes ComponentSystem
│
├── Grid/
│   ├── GridSystem.h/cpp           # Grid logic
│   └── GridCell.h                 # Grid data structures
│
├── Components/
│   ├── UnitComponent.h/cpp        # PF2e character stats
│   └── GridConfigComponent.h/cpp  # Data-driven config
│
├── Core/
│   └── TurnManager.h/cpp          # Turn-based combat
│
└── UI/
    └── GridRenderer.h/cpp         # Visual grid rendering
```

---

## Testing & Validation

### Phase 1 Test Checklist

- [x] **Grid renders correctly:** 20×20 tiles with gaps, semi-transparent
- [x] **GridConfig found:** GameManager finds GridConfig node at startup
- [x] **No magic numbers:** All values driven by GridConfigComponent
- [x] **Grid in world hierarchy:** `grid_root->isWorld()` returns 1
- [x] **Materials applied:** Grid tiles have correct color
- [x] **Coordinate conversion:** `gridToWorld()` matches expected positions
- [x] **Component registration:** UnitComponent accessible via ComponentSystem
- [x] **TurnManager compiles:** Initiative, MAP, 3-action economy logic complete
- [x] **Build succeeds:** No compilation errors or warnings

### Known Limitations

- **No pathfinding yet:** Movement system not implemented (Phase 2)
- **No unit selection:** Mouse picking not implemented (Phase 2)
- **No turn UI:** Initiative order display not implemented (Phase 2)
- **No combat actions:** Strike/damage not implemented (Phase 3)
- **TurnManager untested:** Combat system not wired up yet (Phase 2)

---

## Next Steps (Phase 2)

### Movement System
- Pathfinding (A* with discrete elevation)
- Stride action (1 action, move up to Speed)
- Climb action (1 action, move up 1 elevation + horizontal)
- Terrain costs (difficult terrain = 2x cost)
- Movement range highlighting (show reachable cells)

### Input & Camera
- Mouse picking (select units, select cells)
- Tactical camera (zoom, pan, rotate, pitch)
- Selected unit indicator
- Hover highlights

### Turn Flow
- Start combat UI (initiative order display)
- Action counter UI (3 actions remaining)
- End turn button
- Auto-advance turns when no actions left

### Integration
- Wire up TurnManager to actual unit nodes
- Test combat initiation
- Test initiative rolls
- Test action economy

---

## Lessons Learned

### What Went Well
✅ **Data-driven design:** GridConfigComponent eliminates magic numbers, very designer-friendly
✅ **Component architecture:** Clean separation of concerns, easy to extend
✅ **Primitives API:** Much more reliable than manual mesh creation
✅ **Comprehensive logging:** Made debugging node hierarchy issues trivial

### What Was Challenging
⚠️ **Node hierarchy:** Nodes not auto-added to world (silent failures)
⚠️ **Material loading:** Unigine core materials not accessible via simple paths
⚠️ **API documentation:** Had to reverse-engineer ObjectMeshDynamic from headers
⚠️ **Double precision:** vec3 vs dvec3 confusion in mesh APIs

### Recommendations for Phase 2+
1. **Use Primitives** for all simple meshes (boxes, spheres, cylinders)
2. **Validate world hierarchy** early (check `isWorld()` immediately)
3. **Log extensively** during development (remove in production)
4. **Test in Editor first** before writing code (verify node setup works)
5. **Keep GridConfigComponent pattern** for other systems (MovementConfig, CombatConfig, etc.)

---

## References

**Code Files:** See `source/` directory for full implementation
**Unigine Docs:** https://developer.unigine.com/en/docs/latest/
**PF2e Rules:** Pathfinder 2e Remastered Core Rulebook
**GDD:** `documentation/GDD/01-Core-Mechanics/Turn-System.md`

---

**Phase 1 Status: ✅ COMPLETE**
**Next Phase:** Movement & Input (Week 2)
