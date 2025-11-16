---
title: "Technical Architecture"
type: technical
section: 06-Technical
status: active
tags:
  - architecture
  - design
  - unigine
  - systems
created: 2025-11-16
updated: 2025-11-16
priority: critical
---

# Technical Architecture

High-level technical architecture for ProjectAnu, documenting system design, data flow, class relationships, and Unigine engine integration.

---

## System Overview

### Architecture Layers

```
┌─────────────────────────────────────────────────────┐
│              Unigine Editor / Runtime               │
│  (Node hierarchy, Component system, GUI, Input)     │
└────────────────┬────────────────────────────────────┘
                 │
┌────────────────┴────────────────────────────────────┐
│            Application Logic Layer                   │
│  ┌──────────────┐  ┌──────────────┐  ┌───────────┐ │
│  │ SystemLogic  │  │ WorldLogic   │  │EditorLogic│ │
│  │ (Engine init)│  │ (Game logic) │  │(Dev tools)│ │
│  └──────────────┘  └──────────────┘  └───────────┘ │
└────────────────┬────────────────────────────────────┘
                 │
┌────────────────┴────────────────────────────────────┐
│              Game Systems Layer                      │
│  ┌──────────┐  ┌──────────┐  ┌──────────────────┐  │
│  │  Grid    │  │  Turn    │  │  Combat          │  │
│  │  System  │  │  Manager │  │  Resolution      │  │
│  └──────────┘  └──────────┘  └──────────────────┘  │
│  ┌──────────┐  ┌──────────┐  ┌──────────────────┐  │
│  │  Spell   │  │   UI     │  │  Data            │  │
│  │  System  │  │  Layer   │  │  Management      │  │
│  └──────────┘  └──────────┘  └──────────────────┘  │
└────────────────┬────────────────────────────────────┘
                 │
┌────────────────┴────────────────────────────────────┐
│            Component Layer                           │
│  ┌────────────────┐  ┌──────────────────────────┐  │
│  │ UnitComponent  │  │ SpellcasterComponent     │  │
│  │ (HP, AC, stats)│  │ (Spells, spell slots)    │  │
│  └────────────────┘  └──────────────────────────┘  │
└─────────────────────────────────────────────────────┘
```

### Unigine Logic Hierarchy

**SystemLogic** (`AppSystemLogic`)
- **Lifecycle:** Entire application runtime
- **Purpose:** Engine-level initialization, component registration
- **Key responsibilities:**
  - Register custom components (`UnitComponent`, `SpellcasterComponent`)
  - Initialize component system
  - Global manager setup

**WorldLogic** (`AppWorldLogic`)
- **Lifecycle:** While a world/level is loaded
- **Purpose:** Game logic, entities, combat management
- **Key responsibilities:**
  - Own `GridSystem`, `TurnManager`, `CombatResolver`
  - Game state updates in `updatePhysics()` (fixed 60 FPS)
  - Input handling in `update()` (per render frame)
  - World save/load

**EditorLogic** (`AppEditorLogic`)
- **Lifecycle:** Only when Unigine Editor is running
- **Purpose:** Development tools, debug visualization
- **Not active in final builds**

---

## Core Systems

### Grid System

**Purpose:** Manage tactical grid, pathfinding, and movement validation.

**Key classes:**
- `GridSystem` - Grid data structure, cell queries, occupancy
- `GridCell` - Cell properties (elevation, blocked, occupant)
- `Pathfinder` - A* pathfinding with PF2e movement rules
- `MovementValidator` - Validates Stride/Climb actions

**Ownership:** Member of `AppWorldLogic`

**Data flow:**
```
Player clicks unit → Input handler → AppWorldLogic
                                          ↓
                        Query GridSystem for reachable cells
                                          ↓
                        Pathfinder calculates valid paths
                                          ↓
                        GridRenderer highlights cells
                                          ↓
                        Player clicks destination
                                          ↓
                        MovementValidator checks legality
                                          ↓
                        Update GridSystem occupancy
                                          ↓
                        Move unit node in world
```

**Why not Unigine NavMesh?**
- Unigine's `NavigationMesh` is for continuous 3D pathfinding (smooth character movement)
- Our game requires **discrete grid-based movement** with tactical rules:
  - 5-5-10 diagonal costs (alternating 1/2 squares)
  - Climb vs Stride distinction (upward requires Climb action)
  - Elevation-based movement costs
  - Action point tracking (1-3 actions per turn)
- Custom A* respects PF2e rules and returns action costs, not just distance

### Turn Management System

**Purpose:** Initiative order, turn flow, action tracking.

**Key classes:**
- `TurnManager` - Initiative, turn order, action economy
- `InitiativeEntry` - Unit + initiative roll pairing

**PF2e rules implemented:**
- Initiative = d20 + Perception modifier
- Turn order sorted highest to lowest (doesn't change between rounds)
- Each unit gets 3 actions per turn
- Multiple Attack Penalty (MAP) tracked per turn

**Data flow:**
```
Combat start → Roll initiative for all units
                      ↓
           Sort initiative order (highest first)
                      ↓
           Loop: For each unit's turn
                      ↓
           Reset actions to 3, reset MAP
                      ↓
           Player/AI takes actions (costs 1-3 actions each)
                      ↓
           Update MAP on attacks
                      ↓
           End turn when actions = 0 or "End Turn" clicked
                      ↓
           Decrement conditions (Frightened, etc.)
                      ↓
           Next unit's turn
                      ↓
           Round ends when all units acted → Increment round
```

**Integration with UI:**
- `TurnManager::getCurrentUnit()` → UI displays active unit
- `TurnManager::getActionsRemaining()` → UI shows action pips (3/2/1/0)
- `TurnManager::getCurrentMAPPenalty()` → UI shows attack penalties

### Combat Resolution System

**Purpose:** Attack rolls, damage calculation, saving throws.

**Key classes:**
- `AttackResolver` - d20 + mods vs AC, degrees of success
- `DamageCalculator` - Dice rolling, critical damage
- `SavingThrowResolver` - d20 + save vs DC
- `CoverSystem` - Cover detection, AC/Reflex bonuses

**PF2e rules implemented:**
- Four degrees of success (crit fail / fail / success / crit success)
- Degree based on margin: ≥10 = crit success, 0-9 = success, -10 to -1 = fail, ≤-10 = crit fail
- Natural 20: Improve degree by 1 step
- Natural 1: Worsen degree by 1 step
- Critical damage: Double dice, NOT modifiers
- MAP: -5/-10 (or -4/-8 agile)

**Cover detection algorithm:**
```
For attacker at position A, target at position T:
    1. Get 4 corners of attacker's cell
    2. Get 4 corners of target's cell
    3. For each attacker corner → target corner pair:
        Raycast from corner to corner
        If ray blocked by terrain:
            Increment blocked_rays
    4. Cover level = blocked_rays / total_rays
        0-25%: No cover
        26-50%: Lesser cover (+1 AC)
        51-75%: Standard cover (+2 AC)
        76-100%: Greater cover (+4 AC)
```

**Height advantage:**
- +1 to hit per 10ft elevation difference (max +4)
- Reduces cover by 1 level per 10ft (e.g., standard → lesser at 10ft elevation)

### Spell System

**Purpose:** Spell casting, targeting, effects, prepared casting.

**Key classes:**
- `SpellSystem` - Casting validation, spell slot management
- `SpellData` - Spell definition (loaded from JSON)
- `SpellDatabase` - Runtime spell catalog
- `SpellEffect` - Apply damage/healing/conditions
- `TargetingSystem` - Single target, AOE, line, cone

**Prepared casting:**
- Wizard/Cleric prepare spells daily (menu before mission)
- Spell slots per rank: Rank 0 (cantrips) = unlimited, Rank 1/2 = limited
- Heightening: Cast lower-rank spells in higher slots for enhanced effects

**Data flow:**
```
Player selects "Cast Spell" action
         ↓
UI shows prepared spells for current unit
         ↓
Player selects spell (e.g., "Magic Missile")
         ↓
SpellSystem checks: Spell slot available? Enough actions?
         ↓
TargetingSystem shows valid targets (based on range, AOE)
         ↓
Player selects target
         ↓
SpellEffect applies effect:
    - Damage: Roll dice, target saves (if applicable)
    - Healing: Add HP
    - Condition: Apply to target (e.g., Prone from Grease)
         ↓
Spend action(s) and spell slot
```

**Spell data (JSON schema):**
```json
{
  "id": "magic_missile",
  "name": "Magic Missile",
  "rank": 1,
  "actions": 2,
  "traits": ["evocation", "force"],
  "range": 120,
  "targets": 1,
  "damage": {"dice": "1d4+1", "type": "force"},
  "heightening": {"type": "per_rank", "effect": "+1_missile"}
}
```

### UI System

**Purpose:** HUD, action buttons, grid visualization, feedback.

**Key components:**
- `GridRenderer` - Visual grid, cell highlighting
- `TacticalHUD` - Action buttons, HP bars, initiative order
- `TargetingUI` - Spell/attack targeting overlays
- `CombatLog` - Text feed of combat events

**Unigine GUI integration:**
- Use `WidgetButton` for action buttons
- Use `WidgetLabel` for HP/stats display
- Use `WidgetCanvas` for custom drawing (spell AOE preview)

**HUD layout (placeholder):**
```
┌────────────────────────────────────────────────────┐
│ Initiative Order: [Fighter] → Wizard → Goblin     │
└────────────────────────────────────────────────────┘
┌──────────────────────┐  ┌───────────────────────┐
│  Selected Unit       │  │  Combat Log           │
│  Fighter (HP: 18/20) │  │  > Fighter hits       │
│  AC: 18  Speed: 25   │  │    Goblin for 8 dmg   │
│  Actions: ●●●        │  │  > Wizard casts       │
│                      │  │    Magic Missile      │
│  [Stride] [Climb]    │  └───────────────────────┘
│  [Strike] [Cast]     │
│  [End Turn]          │
└──────────────────────┘
```

---

## Component Architecture

### Component System Usage

**Unigine Components:**
- Components attach to nodes (scene graph objects)
- `COMPONENT_DEFINE` macro registers component with engine
- `PROP_PARAM` exposes properties to Editor
- Components are data + behavior, serialized with world

**Custom Components:**

**UnitComponent** (base unit stats)
- Attached to: Every unit node (player units, enemies)
- Properties: HP, AC, stats (STR/DEX/etc.), saves, speed
- Methods: `takeDamage()`, `heal()`, `isAlive()`
- Editor-visible: ✅ (all PROP_PARAM properties)

**SpellcasterComponent** (optional, for casters only)
- Attached to: Wizard, Cleric units
- Properties: Spell DC, spellcasting class
- Data: Prepared spells, spell slots
- Methods: `castSpell()`, `prepareSpells()`, `hasSlot()`

**Why Components for Units?**
- Editor integration (modify stats without recompiling)
- Serialization (save/load unit state with world)
- Modularity (SpellcasterComponent optional)
- Matches Unigine workflow

**Why NOT Components for Grid?**
- Grid is global, not per-entity
- Doesn't benefit from serialization (regenerated each combat)
- Simpler as `AppWorldLogic` member

### Class Relationships

```
AppWorldLogic
├── owns GridSystem
│   ├── contains Vector<GridCell>
│   └── used by Pathfinder
├── owns TurnManager
│   ├── contains Vector<InitiativeEntry>
│   └── tracks UnitComponent pointers
├── owns CombatResolver
│   └── uses AttackResolver, DamageCalculator
└── owns SpellSystem
    ├── owns SpellDatabase
    └── uses SpellEffect

UnitComponent (ComponentBase)
├── has GridPosition
├── references TurnManager (for actions)
└── optional SpellcasterComponent sibling

GridRenderer
├── references GridSystem
└── creates ObjectMeshDynamic nodes (visual grid)
```

---

## Data Flow

### Turn Start → Action → Turn End

```
1. TurnManager::nextTurn()
      ↓
2. Get current unit from initiative order
      ↓
3. Reset actions to 3, reset MAP, reset attack count
      ↓
4. UI updates: Highlight active unit, show action buttons
      ↓
5. Player selects action (e.g., "Strike")
      ↓
6. Input handler → AppWorldLogic::onActionClicked()
      ↓
7. Validate action: TurnManager::canTakeAction()?
      ↓
8. Show targeting UI (valid targets highlighted)
      ↓
9. Player selects target
      ↓
10. Execute action:
    - AttackResolver::resolveAttack()
    - DamageCalculator::rollDamage()
    - Target UnitComponent::takeDamage()
      ↓
11. Spend action: TurnManager::spendAction()
      ↓
12. Update MAP: TurnManager::incrementAttackCount()
      ↓
13. UI updates: Action pips, HP bars, combat log
      ↓
14. Repeat 5-13 until actions = 0 or "End Turn" clicked
      ↓
15. TurnManager::endTurn()
      ↓
16. Decrement conditions (Frightened value--, etc.)
      ↓
17. Check win/loss conditions
      ↓
18. Go to step 1 (next turn)
```

### Spell Casting Flow

```
1. Player clicks "Cast Spell" button
      ↓
2. UI shows prepared spells (from SpellcasterComponent)
      ↓
3. Player selects spell (e.g., "Grease")
      ↓
4. SpellSystem validates:
    - Has spell slot for rank?
    - Enough actions? (Grease = 2 actions)
    - Target in range?
      ↓
5. TargetingUI shows valid targets (AOE preview if applicable)
      ↓
6. Player selects target cell/unit
      ↓
7. SpellSystem::castSpell() executes:
    - Spend spell slot
    - Spend actions (2 for Grease)
    - Apply spell effect
      ↓
8. SpellEffect::apply() for Grease:
    - Get all units in AOE (4 cells)
    - For each unit: Roll Reflex save vs spell DC
    - Degree of success → apply Prone condition (crit fail/fail)
      ↓
9. UI updates: Unit markers show Prone icon
      ↓
10. Combat log: "Wizard casts Grease. Goblin fails save, falls Prone!"
```

---

## Code Organization

### File Structure

```
source/
├── main.cpp                      # Entry point
├── AppSystemLogic.cpp/.h         # Engine initialization
├── AppWorldLogic.cpp/.h          # Game logic layer
├── AppEditorLogic.cpp/.h         # Editor tools
│
├── Core/
│   ├── GameState.h/cpp           # Game state manager
│   ├── TurnManager.h/cpp         # Turn system, initiative
│   └── ActionSystem.h/cpp        # Action validation
│
├── Grid/
│   ├── GridSystem.h/cpp          # Grid data structure
│   ├── GridCell.h/cpp            # Cell properties
│   ├── Pathfinding.h/cpp         # A* implementation
│   └── MovementValidator.h/cpp   # Stride/Climb validation
│
├── Components/
│   ├── UnitComponent.h/cpp       # Unit stats, HP, AC
│   ├── SpellcasterComponent.h/cpp # Spell slots, prepared spells
│   └── CombatantComponent.h/cpp  # Initiative, actions (optional)
│
├── Combat/
│   ├── AttackResolver.h/cpp      # Attack rolls, degrees
│   ├── DamageCalculator.h/cpp    # Damage, crits
│   ├── SavingThrowResolver.h/cpp # Saving throws
│   └── CoverSystem.h/cpp         # Cover detection
│
├── Spells/
│   ├── SpellSystem.h/cpp         # Casting, slot management
│   ├── SpellEffect.h/cpp         # Apply effects
│   ├── SpellData.h/cpp           # Spell definition struct
│   └── SpellDatabase.h/cpp       # Runtime catalog
│
├── UI/
│   ├── GridRenderer.h/cpp        # Visual grid
│   ├── TacticalHUD.h/cpp         # Action buttons, HUD
│   ├── TargetingUI.h/cpp         # Targeting overlays
│   └── CombatLog.h/cpp           # Text log
│
└── Data/
    ├── DataLoader.h/cpp          # JSON loading
    └── UnitDatabase.h/cpp        # Runtime unit catalog
```

**NO Public/Private separation:**
- Single-module project (one executable)
- Solo developer (no external API consumers)
- Standard C++ organization sufficient

### CMakeLists.txt Management

**When adding new files:**
1. Create `.h` and `.cpp` in appropriate folder
2. Update `source/CMakeLists.txt`:
```cmake
add_executable(${target}
    # Existing files...
    ${CMAKE_CURRENT_LIST_DIR}/AppWorldLogic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AppWorldLogic.h

    # NEW: Grid system
    ${CMAKE_CURRENT_LIST_DIR}/Grid/GridSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Grid/GridSystem.h
    ${CMAKE_CURRENT_LIST_DIR}/Grid/Pathfinding.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Grid/Pathfinding.h
    # ... etc
)
```
3. Reload CMake in CLion: `Tools → CMake → Reload CMake Project`

---

## Data Management

### Spell Data (JSON)

**Location:** `data/spells/`

**Files:**
- `wizard_spells.json` - All Wizard spells (24 total, 8 for prototype)
- `cleric_spells.json` - All Cleric spells (24 total, 8 for prototype)

**Schema:**
```json
{
  "spells": [
    {
      "id": "electric_arc",
      "name": "Electric Arc",
      "rank": 0,
      "actions": 2,
      "traits": ["evocation", "electricity"],
      "range": 30,
      "targets": 2,
      "damage": {
        "dice": "1d4+0",
        "type": "electricity"
      },
      "save": {
        "type": "reflex",
        "dc": "spell_dc"
      },
      "heightening": {
        "type": "interval",
        "interval": 1,
        "effect": "damage_increase_1d4"
      }
    }
  ]
}
```

**Loading:**
```cpp
// In AppWorldLogic::init()
SpellDatabase spell_db;
spell_db.loadSpells("data/spells/wizard_spells.json");
spell_db.loadSpells("data/spells/cleric_spells.json");
```

### Unit Data (JSON)

**Location:** `data/units/`

**Files:**
- `player_units.json` - Fixed prototype units (Fighter, Rogue, Cleric, Wizard level 2)
- `enemy_units.json` - Prototype enemies (Goblin Warrior, Archer, Shaman)

**Schema:**
```json
{
  "units": [
    {
      "id": "fighter_l2",
      "name": "Fighter",
      "level": 2,
      "class": "fighter",
      "stats": {"str": 16, "dex": 12, "con": 14, "int": 10, "wis": 10, "cha": 10},
      "hp_max": 20,
      "ac": 18,
      "saves": {"fort": 6, "reflex": 4, "will": 3},
      "speed": 25,
      "attack_bonus": 6,
      "weapon_damage": "1d8+3"
    }
  ]
}
```

---

## Performance Considerations

### Update Frequency

**`update()` (per render frame, ~60-144 FPS):**
- Input handling (mouse clicks, keyboard)
- UI updates (button highlights, tooltips)
- Animations (unit movement interpolation)
- Camera controls

**`updatePhysics()` (fixed 60 FPS):**
- Game logic (turn management, combat resolution)
- Pathfinding (when requested)
- AI decisions
- State updates

**Avoid in `updatePhysics()`:**
- Node transform modifications (rendering in progress)
- Heavy computations (defer to separate thread if needed)

### Optimization Strategies

**Grid pathfinding:**
- Cache reachable cells per unit (invalidate on movement)
- A* early exit if goal unreachable
- Pre-allocate open/closed lists

**Cover detection:**
- Raycasts are expensive; cache results per attacker-target pair
- Invalidate cache on terrain/unit movement
- Limit raycasts to 4 corner pairs (not every pixel)

**Spell AOE:**
- Pre-calculate AOE cells when spell selected
- Reuse for all targets in AOE (don't recalculate per unit)

**Memory:**
- Use `Vector::reserve()` for known sizes
- Avoid allocations in hot loops (pre-allocate buffers)
- Reuse temporary containers (e.g., pathfinding open list)

---

## Testing Strategy

### Unit Testing (Manual)

**Grid System:**
- ✅ Create 20x20 grid, query cells
- ✅ Set elevation, verify retrieval
- ✅ Mark cells blocked, verify pathfinding respects
- ✅ Calculate distance (5-5-10 diagonal rule)

**Pathfinding:**
- ✅ A* finds optimal path on flat grid
- ✅ A* respects elevation (no upward moves for Stride)
- ✅ A* respects blocked cells
- ✅ Reachable cells correct for action budget

**Combat:**
- ✅ Attack roll math (d20 + bonus vs AC)
- ✅ Degrees of success correct
- ✅ Critical damage doubles dice, not modifiers
- ✅ MAP applies correctly (-5/-10 per attack)

**Spells:**
- ✅ Spell data loads from JSON
- ✅ Targeting validates range
- ✅ AOE calculates affected units correctly
- ✅ Heightening increases effects as expected

### Integration Testing

**Full combat encounter:**
1. Start encounter, roll initiative
2. Move units with Stride/Climb
3. Attack enemies, verify damage/HP
4. Cast spells (damage, healing, conditions)
5. Enemy AI takes actions
6. Complete encounter (all enemies defeated)

**Test cases:**
- Diagonal movement costs 1/2/1/2 (5-5-10 rule)
- Climbing costs actions, Stride doesn't climb
- Height advantage grants +1 per 10ft (max +4)
- Cover bonuses apply (+1/+2/+4 AC)
- Conditions affect gameplay (Prone = AC penalty)

---

## Debugging Tools

### Unigine Console

**Access:** Press `~` key in Editor or game

**Useful commands:**
```
world_reload       # Reload current world
show_fps 1         # Show FPS counter
show_profiler 1    # Show performance profiler
render_show_grid 1 # Show world grid
```

### Custom Debug Visualization

**Draw grid in AppEditorLogic:**
```cpp
// AppEditorLogic.cpp (Editor only)
void AppEditorLogic::update() {
    // Draw debug grid overlay
    for (int x = 0; x < grid.getWidth(); x++) {
        for (int y = 0; y < grid.getHeight(); y++) {
            GridCell* cell = grid.getCell(x, y);
            vec3 pos = gridToWorld(x, y, cell->elevation);

            // Draw cell outline
            Visualizer::renderBoundBox(pos, vec3(2.5f), vec4(1, 1, 0, 1));

            // Draw elevation number
            Visualizer::renderMessage3D(pos, vec4(1, 1, 1, 1),
                String::format("%d", cell->elevation));
        }
    }
}
```

### Logging

```cpp
#include <UnigineLog.h>

// Log movement
Log::message("Unit %s moved to (%d, %d, %d)\n",
    unit->unit_name.get(), pos.x, pos.y, pos.z);

// Log combat
Log::message("Attack: %d + %d = %d vs AC %d → %s\n",
    d20, attack_bonus, total, target_ac, degree_name);

// Log errors
Log::error("Failed to load spell data: %s\n", filepath);
```

---

## Future Expansion (Post-Prototype)

**Phase 7: Meta-Layer** (if in scope)
- HQ menu system (WidgetDialog, WidgetVBox)
- Recruitment (generate new units from unit templates)
- Progression (level-up UI, feat selection)
- Save/load system (serialize AppWorldLogic state)

**Phase 8: Campaign**
- 13 encounters across 4 Yugas
- Encounter data (JSON with map layout, enemy spawn, objectives)
- Narrative system (investigation sequences, skill checks)
- Yuga-specific assets (replace placeholders)

**Phase 9: Full Content**
- Expand to 48 spells (24 Wizard, 24 Cleric)
- All 8 conditions (Frightened, Stunned, Slowed, Grabbed)
- Feat selection (character creation)
- Ancestries (3) and backgrounds (8)
- Enemy variety (3-4 types per Yuga)

**Phase 10: Polish**
- VFX (spell effects, hit impacts)
- Audio (SFX, music)
- Animations (unit attacks, spell casting)
- UI polish (animated HP bars, better tooltips)
- Performance optimization (LOD, culling)

---

*See also: [[Prototype Scope]], [[Implementation Guide]], [[System Architecture]]*
