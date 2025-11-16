---
title: "Implementation Guide"
type: technical
section: 06-Technical
status: active
tags:
  - implementation
  - coding
  - unigine
  - workflow
created: 2025-11-16
updated: 2025-11-16
priority: critical
---

# Implementation Guide

Phase-by-phase implementation guide for the ProjectAnu prototype. This document provides code patterns, Unigine integration details, and development workflow for each phase.

---

## Development Environment

### Prerequisites
- Unigine SDK installed
- CLion configured with CMake
- Git + Git LFS initialized
- ProjectAnu created via Unigine SDK Browser

### Build & Run Workflow
```bash
# Build in CLion (Ctrl+F9)
# Or via command line:
cd D:\Unigine\ProjectAnu\source
cmake --build build --config Debug

# Run from Unigine Editor
./launch_editor.bat

# Run standalone builds
./launch_debug.bat    # Debug build
./launch_release.bat  # Release build
```

### Adding New Source Files
When creating new `.cpp`/`.h` files, update `source/CMakeLists.txt`:

```cmake
add_executable(${target}
    ${CMAKE_CURRENT_LIST_DIR}/AppEditorLogic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AppEditorLogic.h
    ${CMAKE_CURRENT_LIST_DIR}/AppSystemLogic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AppSystemLogic.h
    ${CMAKE_CURRENT_LIST_DIR}/AppWorldLogic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AppWorldLogic.h
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp

    # NEW: Grid system files
    ${CMAKE_CURRENT_LIST_DIR}/Grid/GridSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Grid/GridSystem.h
    ${CMAKE_CURRENT_LIST_DIR}/Grid/GridCell.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Grid/GridCell.h
    # ... add more files as you create them
)
```

**Important:** Reload CMake project in CLion after modifying CMakeLists.txt (Tools → CMake → Reload CMake Project).

---

## Phase 1: Foundation (Weeks 1-2)

**Goal:** Grid data structures, basic unit representation, turn management, grid visualization.

### 1.1 Grid System

**Create:** `source/Grid/GridSystem.h` and `GridSystem.cpp`

```cpp
// GridSystem.h
#pragma once
#include <UnigineVector.h>
#include <UnigineMap.h>

struct GridPosition {
    int x;
    int y;
    int z; // elevation level

    bool operator==(const GridPosition& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct GridCell {
    GridPosition position;
    int elevation;        // Height in 5ft increments
    bool blocked;         // Is this cell passable?
    Unigine::NodePtr occupant; // Unit currently on this cell (nullptr if empty)
};

class GridSystem {
public:
    GridSystem(int width, int height);

    // Grid queries
    GridCell* getCell(int x, int y);
    GridCell* getCell(GridPosition pos);
    bool isValidPosition(int x, int y);
    bool isBlocked(GridPosition pos);

    // Grid state
    void setElevation(int x, int y, int elevation);
    void setOccupant(GridPosition pos, Unigine::NodePtr unit);
    void clearOccupant(GridPosition pos);

    // Distance calculation
    int getDistance(GridPosition a, GridPosition b); // Respects 5-5-10 diagonal
    int getElevationDifference(GridPosition a, GridPosition b);

private:
    int grid_width;
    int grid_height;
    Unigine::Vector<GridCell> cells; // Flat array: index = y * width + x
};
```

**Implementation notes:**
- Store grid as flat array for cache efficiency
- PF2e uses 5ft squares; 1 grid square = 5ft
- Elevation stored as integer (0 = ground level, 1 = 5ft, 2 = 10ft, etc.)
- Diagonal distance uses 5-5-10 rule (implemented in Phase 2)

### 1.2 Unit Component

**Create:** `source/Components/UnitComponent.h` and `UnitComponent.cpp`

```cpp
// UnitComponent.h
#pragma once
#include <UnigineComponentSystem.h>
#include "Grid/GridSystem.h"

class UnitComponent : public Unigine::ComponentBase
{
public:
    COMPONENT_DEFINE(UnitComponent, ComponentBase);
    COMPONENT_INIT(init);
    COMPONENT_UPDATE(update);
    COMPONENT_SHUTDOWN(shutdown);

    // PF2e stats (exposed to editor)
    PROP_PARAM(String, unit_id);
    PROP_PARAM(String, unit_name);
    PROP_PARAM(Int, level, 1);

    // Combat stats
    PROP_PARAM(Int, max_hp, 10);
    PROP_PARAM(Int, current_hp, 10);
    PROP_PARAM(Int, armor_class, 10);
    PROP_PARAM(Int, speed, 25); // PF2e Speed (ft per action)

    // Ability scores
    PROP_PARAM(Int, strength, 10);
    PROP_PARAM(Int, dexterity, 10);
    PROP_PARAM(Int, constitution, 10);

    // Saves
    PROP_PARAM(Int, fortitude_save, 0);
    PROP_PARAM(Int, reflex_save, 0);
    PROP_PARAM(Int, will_save, 0);

    // Grid position
    GridPosition grid_position;

    // Methods
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive() const { return current_hp > 0; }

protected:
    void init();
    void update();
    void shutdown();
};

// UnitComponent.cpp
#include "UnitComponent.h"

COMPONENT_DEFINE(UnitComponent, ComponentBase);

void UnitComponent::init() {
    // Initialize unit on grid
    // Get world position, convert to grid coordinates
}

void UnitComponent::update() {
    // Per-frame update (visualization, animations)
}

void UnitComponent::shutdown() {
    // Cleanup
}

void UnitComponent::takeDamage(int amount) {
    current_hp -= amount;
    if (current_hp < 0) current_hp = 0;

    Unigine::Log::message("Unit %s took %d damage (HP: %d/%d)\n",
        unit_name.get(), amount, current_hp, max_hp);
}

void UnitComponent::heal(int amount) {
    current_hp += amount;
    if (current_hp > max_hp) current_hp = max_hp;

    Unigine::Log::message("Unit %s healed %d HP (HP: %d/%d)\n",
        unit_name.get(), amount, current_hp, max_hp);
}
```

**Register component in AppSystemLogic:**

```cpp
// AppSystemLogic.cpp
#include "Components/UnitComponent.h"

int AppSystemLogic::init() {
    // Initialize component system
    Unigine::ComponentSystem::get()->initialize();

    // Register custom components
    UnitComponent::registerComponent();

    return 1;
}
```

### 1.3 Turn Manager

**Create:** `source/Core/TurnManager.h` and `TurnManager.cpp`

```cpp
// TurnManager.h
#pragma once
#include <UnigineVector.h>
#include "Components/UnitComponent.h"

struct InitiativeEntry {
    UnitComponent* unit;
    int initiative_roll;
};

class TurnManager {
public:
    TurnManager();

    // Initiative
    void rollInitiative(Unigine::Vector<UnitComponent*> units);
    void sortInitiativeOrder();

    // Turn flow
    void startCombat();
    void nextTurn();
    void endTurn();
    UnitComponent* getCurrentUnit();

    // Action tracking
    int getActionsRemaining() const { return actions_remaining; }
    bool canTakeAction() const { return actions_remaining > 0; }
    void spendAction(int cost = 1);
    void resetActions();

    // Combat state
    bool isCombatActive() const { return combat_active; }
    int getCurrentRound() const { return current_round; }

private:
    Unigine::Vector<InitiativeEntry> initiative_order;
    int current_turn_index;
    int current_round;
    int actions_remaining;
    bool combat_active;

    int rollPerception(UnitComponent* unit); // d20 + Perception modifier
};
```

**Key PF2e rules:**
- Initiative = d20 + Perception modifier
- Each unit gets 3 actions per turn
- Turn order doesn't change between rounds

### 1.4 Basic Grid Visualization

**Create:** `source/UI/GridRenderer.h` and `GridRenderer.cpp`

```cpp
// GridRenderer.h (simplified)
#pragma once
#include "Grid/GridSystem.h"
#include <UnigineWorld.h>

class GridRenderer {
public:
    GridRenderer(GridSystem* grid);

    void createGridVisuals(); // Create node per cell
    void updateGridVisuals(); // Refresh colors, highlights
    void highlightCell(GridPosition pos, Unigine::Math::vec4 color);
    void clearHighlights();

private:
    GridSystem* grid;
    Unigine::Vector<Unigine::NodePtr> cell_nodes;
};
```

**Implementation approach:**
- Create `ObjectMeshDynamic` for each grid cell (simple quad)
- Use `setMaterial()` to color cells (reachable = green, blocked = red)
- Parent all grid nodes to a single root node for easy toggling

**In AppWorldLogic:**

```cpp
// AppWorldLogic.h
class AppWorldLogic : public Unigine::WorldLogic {
public:
    GridSystem grid;
    TurnManager turn_manager;
    GridRenderer* grid_renderer;

    int init() override;
    int updatePhysics() override; // Game logic goes here
};

// AppWorldLogic.cpp
int AppWorldLogic::init() {
    // Create 20x20 grid for prototype
    grid = GridSystem(20, 20);

    // Create visual grid
    grid_renderer = new GridRenderer(&grid);
    grid_renderer->createGridVisuals();

    return 1;
}

int AppWorldLogic::updatePhysics() {
    // Fixed 60 FPS update
    // Game logic updates here (turn management, etc.)
    return 1;
}
```

### 1.5 Phase 1 Testing

**Test checklist:**
- ✅ Grid data structure created (20x20)
- ✅ Can query cells, set elevation, mark blocked
- ✅ Visual grid rendered in world
- ✅ Can create units with UnitComponent
- ✅ Units appear in Unigine Editor hierarchy
- ✅ Can modify unit stats in Editor Properties panel
- ✅ TurnManager rolls initiative correctly
- ✅ Turn order sorted by initiative (highest first)
- ✅ Can advance turns (3 actions → next unit)

**Debugging tips:**
- Use `Unigine::Log::message()` extensively
- Check Console window in Unigine Editor (~ key)
- Verify components registered in AppSystemLogic::init()
- Use Editor's Node hierarchy to inspect grid nodes

---

## Phase 2: Movement (Weeks 3-4)

**Goal:** Pathfinding (A*), Stride/Climb actions, elevation system, movement visualization.

### 2.1 Pathfinding (A*)

**Create:** `source/Grid/Pathfinding.h` and `Pathfinding.cpp`

```cpp
// Pathfinding.h
#pragma once
#include "GridSystem.h"
#include <UnigineVector.h>

struct PathNode {
    GridPosition position;
    int g_cost; // Cost from start
    int h_cost; // Heuristic to goal
    int f_cost() const { return g_cost + h_cost; }
    PathNode* parent;
};

class Pathfinder {
public:
    Pathfinder(GridSystem* grid);

    // Find path with action costs
    Unigine::Vector<GridPosition> findPath(
        GridPosition start,
        GridPosition goal,
        bool allow_climb = true
    );

    // Calculate action cost for path
    int calculateActionCost(const Unigine::Vector<GridPosition>& path);

    // Reachable cells within action budget
    Unigine::Vector<GridPosition> getReachableCells(
        GridPosition start,
        int actions_available
    );

private:
    GridSystem* grid;

    int getMovementCost(GridPosition from, GridPosition to);
    int heuristic(GridPosition a, GridPosition b); // Manhattan distance
    Unigine::Vector<GridPosition> getNeighbors(GridPosition pos, bool allow_climb);
};
```

**Movement cost rules:**
- Horizontal/vertical: 1 square = 1 cost
- Diagonal: Alternating 1/2 costs (5-5-10 rule)
- Upward: Requires Climb action (separate from Stride)
- Downward: Free with Stride

**5-5-10 diagonal implementation:**

```cpp
int Pathfinder::getMovementCost(GridPosition from, GridPosition to) {
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);
    int dz = to.z - from.z;

    // Vertical movement handled separately (Climb action)
    if (dz > 0) return -1; // Invalid for Stride

    // Horizontal movement
    if (dx == 0 || dy == 0) {
        return 1; // Straight move
    } else {
        // Diagonal: 5-5-10 rule
        // First diagonal = 1, second diagonal = 2
        // Track diagonal count in path context
        return 1; // Simplified; full implementation tracks alternating costs
    }
}
```

### 2.2 Movement Validator

**Create:** `source/Grid/MovementValidator.h`

```cpp
// MovementValidator.h
#pragma once
#include "GridSystem.h"
#include "Pathfinding.h"

enum class MovementType {
    STRIDE,  // Horizontal + descend
    CLIMB    // Upward only
};

class MovementValidator {
public:
    MovementValidator(GridSystem* grid, Pathfinder* pathfinder);

    // Stride validation (1 action, Speed ft horizontal/descend)
    bool canStrideTo(GridPosition from, GridPosition to, int speed);
    Unigine::Vector<GridPosition> getStrideReachable(GridPosition from, int speed);

    // Climb validation (1 action, Speed ft upward only)
    bool canClimbTo(GridPosition from, GridPosition to, int speed);
    Unigine::Vector<GridPosition> getClimbReachable(GridPosition from, int speed);

private:
    GridSystem* grid;
    Pathfinder* pathfinder;

    int gridDistanceToFeet(int squares) { return squares * 5; }
};
```

**Stride logic:**
```cpp
bool MovementValidator::canStrideTo(GridPosition from, GridPosition to, int speed) {
    // Cannot Stride upward
    if (to.z > from.z) return false;

    // Find path (horizontal + descend only)
    auto path = pathfinder->findPath(from, to, false); // allow_climb = false
    if (path.size() == 0) return false;

    // Calculate distance in feet
    int distance_ft = (path.size() - 1) * 5; // Each square = 5ft

    // Can reach if within Speed
    return distance_ft <= speed;
}
```

**Climb logic:**
```cpp
bool MovementValidator::canClimbTo(GridPosition from, GridPosition to, int speed) {
    // Climb only moves upward (same x/y)
    if (to.x != from.x || to.y != from.y) return false;
    if (to.z <= from.z) return false; // Not climbing up

    int elevation_gain_ft = (to.z - from.z) * 5;
    return elevation_gain_ft <= speed;
}
```

### 2.3 Movement Actions

**Add to UnitComponent:**

```cpp
// UnitComponent.h additions
class UnitComponent : public Unigine::ComponentBase {
public:
    // Movement
    bool stride(GridPosition target, TurnManager* turn_mgr);
    bool climb(int levels_up, TurnManager* turn_mgr);

    Unigine::Vector<GridPosition> getStrideRange();
    Unigine::Vector<GridPosition> getClimbRange();
};

// UnitComponent.cpp
bool UnitComponent::stride(GridPosition target, TurnManager* turn_mgr) {
    if (!turn_mgr->canTakeAction()) return false;

    // Validate movement (use MovementValidator)
    // Move unit to target
    // Update grid occupancy
    // Spend action
    turn_mgr->spendAction(1);

    return true;
}

bool UnitComponent::climb(int levels_up, TurnManager* turn_mgr) {
    if (!turn_mgr->canTakeAction()) return false;

    int climb_distance_ft = levels_up * 5;
    if (climb_distance_ft > speed) return false;

    // Update grid position
    grid_position.z += levels_up;

    // Spend action
    turn_mgr->spendAction(1);

    return true;
}
```

### 2.4 Movement Visualization

**Update GridRenderer:**

```cpp
void GridRenderer::showReachableCells(
    Unigine::Vector<GridPosition> reachable,
    Unigine::Math::vec4 color
) {
    clearHighlights();
    for (auto& pos : reachable) {
        highlightCell(pos, color);
    }
}
```

**Use in game logic:**

```cpp
// When unit selected, show where they can move
UnitComponent* selected_unit = getSelectedUnit();
auto stride_range = selected_unit->getStrideRange();
grid_renderer->showReachableCells(stride_range, vec4(0, 1, 0, 0.5)); // Green
```

### 2.5 Phase 2 Testing

**Test checklist:**
- ✅ A* finds paths correctly (respects blocked cells)
- ✅ 5-5-10 diagonal costs calculated properly
- ✅ Stride action moves horizontally + descends
- ✅ Climb action moves upward only
- ✅ Movement range visualization shows correct cells
- ✅ Units cannot move through occupied cells
- ✅ Elevation affects pathfinding correctly

---

## Phase 3: Basic Combat (Weeks 5-6)

**Goal:** Attack rolls, damage, critical hits, health tracking, death detection.

### 3.1 Attack Resolver

**Create:** `source/Combat/AttackResolver.h`

```cpp
// AttackResolver.h
#pragma once
#include "Components/UnitComponent.h"

enum class DegreeOfSuccess {
    CRITICAL_FAILURE,
    FAILURE,
    SUCCESS,
    CRITICAL_SUCCESS
};

struct AttackResult {
    DegreeOfSuccess degree;
    int damage;
    bool hit;
    int attack_roll; // Actual d20 result
    int total_bonus; // Attack bonus + modifiers
};

class AttackResolver {
public:
    // PF2e attack roll: d20 + attack_bonus vs AC
    AttackResult resolveAttack(
        UnitComponent* attacker,
        UnitComponent* target,
        int attack_bonus,
        int map_penalty = 0 // Multiple Attack Penalty
    );

private:
    int rollD20();
    DegreeOfSuccess getDegree(int total, int dc);
};
```

**PF2e degree of success:**

```cpp
DegreeOfSuccess AttackResolver::getDegree(int total, int dc) {
    int margin = total - dc;

    if (margin >= 10) return DegreeOfSuccess::CRITICAL_SUCCESS;
    if (margin >= 0) return DegreeOfSuccess::SUCCESS;
    if (margin >= -10) return DegreeOfSuccess::FAILURE;
    return DegreeOfSuccess::CRITICAL_FAILURE;
}
```

**Natural 20/1 rule:**
```cpp
AttackResult AttackResolver::resolveAttack(...) {
    int d20 = rollD20();
    int total = d20 + attack_bonus - map_penalty;

    DegreeOfSuccess degree = getDegree(total, target->armor_class);

    // Natural 20: Improve degree by 1 step
    if (d20 == 20) {
        if (degree == DegreeOfSuccess::SUCCESS) degree = DegreeOfSuccess::CRITICAL_SUCCESS;
        else if (degree == DegreeOfSuccess::FAILURE) degree = DegreeOfSuccess::SUCCESS;
    }

    // Natural 1: Worsen degree by 1 step
    if (d20 == 1) {
        if (degree == DegreeOfSuccess::FAILURE) degree = DegreeOfSuccess::CRITICAL_FAILURE;
        else if (degree == DegreeOfSuccess::SUCCESS) degree = DegreeOfSuccess::FAILURE;
    }

    // Calculate damage
    // ...
}
```

### 3.2 Damage Calculator

**Create:** `source/Combat/DamageCalculator.h`

```cpp
// DamageCalculator.h
#pragma once

struct DamageRoll {
    int num_dice;
    int die_size;
    int modifier;
};

class DamageCalculator {
public:
    // Roll damage: XdY + modifier
    int rollDamage(DamageRoll damage);

    // Critical damage: Double dice, NOT modifier
    int rollCriticalDamage(DamageRoll damage);

private:
    int rollDice(int num, int size);
};

// Example usage
int DamageCalculator::rollCriticalDamage(DamageRoll damage) {
    int dice_total = rollDice(damage.num_dice * 2, damage.die_size); // Double dice
    return dice_total + damage.modifier; // Add modifier once
}
```

### 3.3 Strike Action

**Add to UnitComponent:**

```cpp
// UnitComponent.h
class UnitComponent {
public:
    PROP_PARAM(Int, attack_bonus, 0);
    PROP_PARAM(String, weapon_damage, "1d6+2"); // Format: XdY+Z

    AttackResult strike(UnitComponent* target, TurnManager* turn_mgr);

    int getAttackBonus();
    DamageRoll parseDamageString(const char* damage_str);
};

// UnitComponent.cpp
AttackResult UnitComponent::strike(UnitComponent* target, TurnManager* turn_mgr) {
    if (!turn_mgr->canTakeAction()) return {}; // Invalid

    // Get Multiple Attack Penalty
    int map = turn_mgr->getCurrentMAPPenalty();

    // Resolve attack
    AttackResolver resolver;
    AttackResult result = resolver.resolveAttack(this, target, attack_bonus, map);

    // Apply damage
    if (result.hit) {
        target->takeDamage(result.damage);
    }

    // Spend action and increment MAP
    turn_mgr->spendAction(1);
    turn_mgr->incrementAttackCount();

    return result;
}
```

### 3.4 Multiple Attack Penalty

**Add to TurnManager:**

```cpp
// TurnManager.h
class TurnManager {
public:
    int getCurrentMAPPenalty();
    void incrementAttackCount();
    void resetAttackCount(); // Called at start of turn

private:
    int attacks_this_turn; // 0, 1, 2, ...
    bool current_weapon_agile; // TODO: Get from weapon data
};

// TurnManager.cpp
int TurnManager::getCurrentMAPPenalty() {
    if (attacks_this_turn == 0) return 0;
    if (attacks_this_turn == 1) return current_weapon_agile ? -4 : -5;
    if (attacks_this_turn >= 2) return current_weapon_agile ? -8 : -10;
    return -10; // Safety
}
```

### 3.5 Phase 3 Testing

**Test checklist:**
- ✅ Attack rolls calculated correctly (d20 + bonus vs AC)
- ✅ Four degrees of success work (crit fail/fail/success/crit)
- ✅ Natural 20 improves degree by 1 step
- ✅ Damage rolls correct (XdY + modifier)
- ✅ Critical hits double dice, not modifier
- ✅ MAP applies correctly (-5/-10 or -4/-8 agile)
- ✅ HP reduces on hit, units die at 0 HP
- ✅ Death detection works (unit removed from turn order)

---

## Phase 4-6 Implementation

*(Abbreviated for brevity; follow same pattern)*

**Phase 4: Advanced Combat**
- Cover system (raycasting from corners)
- Height advantage calculation
- Saving throws

**Phase 5: Spells**
- Spell data loading from JSON
- Targeting UI
- Spell effects (damage, healing, conditions)
- Heightening system

**Phase 6: Polish & AI**
- Basic enemy AI (behavior trees or simple state machine)
- UI improvements
- Bug fixing and optimization

---

## Unigine Integration Patterns

### Input Handling

```cpp
// In AppWorldLogic::update() - per frame
int AppWorldLogic::update() {
    // Mouse click to select unit
    if (Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT)) {
        // Raycast from camera to world
        // Check if hit a unit node
        // Set as selected unit
    }

    return 1;
}
```

### GUI for Actions

```cpp
// Create WidgetButton for actions
WidgetButtonPtr stride_button = WidgetButton::create("Stride (1 action)");
stride_button->setCallback(Gui::CLICKED, MakeCallback(this, &AppWorldLogic::onStrideClicked));

void AppWorldLogic::onStrideClicked() {
    if (selected_unit && turn_manager.canTakeAction()) {
        // Show movement range
        // Wait for player to click destination
    }
}
```

### Data Loading (JSON)

```cpp
// Use Unigine's Json class
#include <UnigineJson.h>

void SpellDatabase::loadSpells(const char* filepath) {
    Unigine::Json json;
    if (!json.load(filepath)) {
        Log::error("Failed to load %s\n", filepath);
        return;
    }

    for (int i = 0; i < json.getNumChildren(); i++) {
        Unigine::Json spell_json = json.getChild(i);

        SpellData spell;
        spell.id = spell_json.getString("id");
        spell.name = spell_json.getString("name");
        spell.rank = spell_json.getInt("rank");
        // ... parse rest

        spells.append(spell);
    }
}
```

---

## Development Best Practices

### Logging
```cpp
#include <UnigineLog.h>

Unigine::Log::message("Unit %s moved to (%d, %d)\n", unit->unit_name.get(), x, y);
Unigine::Log::warning("Invalid grid position: (%d, %d)\n", x, y);
Unigine::Log::error("Failed to load spell data\n");
```

### Assertions
```cpp
assert(grid.isValidPosition(x, y) && "Invalid grid position");
```

### Git Workflow
```bash
# Create feature branch
git checkout -b feature/grid-system

# Commit frequently
git add source/Grid/
git commit -m "Implement GridSystem data structure"

# Merge to main when complete
git checkout main
git merge feature/grid-system
```

### Performance Tips
- Use `updatePhysics()` for game logic (fixed 60 FPS)
- Avoid allocations in hot loops (pre-allocate Vectors)
- Use `Vector::reserve()` for known sizes
- Profile with Unigine Profiler (Editor → Tools → Profiler)

---

*See also: [[Prototype Scope]], [[Technical Architecture]], [[System Architecture]]*
