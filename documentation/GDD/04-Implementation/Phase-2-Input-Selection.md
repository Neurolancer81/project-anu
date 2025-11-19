---
aliases: [Phase 2, Input & Selection, Unit Selection]
tags: [implementation, phase-2, input, selection, in-progress]
status: in-progress
created: 2025-11-18
updated: 2025-11-18
---

# Phase 2: Input & Selection (IN PROGRESS)

**Status:** 🔄 SelectionSystem implemented and compiling, not yet tested in-game
**Duration:** Week 2 (started November 18, 2025)
**Build Status:** ✅ Compiling Successfully

## Overview

Phase 2 focuses on player input and unit selection, enabling mouse-based interaction with the tactical grid and units. This phase builds on Phase 1's foundation to create the first interactive gameplay experience.

## Systems Implemented

### 1. SelectionSystem (Unit Selection via Mouse)

**Location:** `source/Input/SelectionSystem.h/cpp`

**Purpose:** Enable players to select units by clicking on them, with visual feedback.

**Features:**
- Mouse-based raycasting to detect units
- Visual selection indicator (yellow cylinder ring)
- One-shot click detection (prevents multi-trigger from held mouse button)
- Automatic deselection when clicking empty space
- Console logging for debugging

**Key Methods:**

```cpp
void init();                           // Initialize selection indicator
void update();                         // Handle input and update position (called every frame)
void clearSelection();                 // Deselect current unit, hide indicator

UnitComponent* getSelectedUnit() const;  // Get currently selected unit
bool hasSelection() const;               // Check if any unit is selected

private:
void createSelectionIndicator();       // Create yellow cylinder ring
void updateIndicatorPosition();        // Follow selected unit
UnitComponent* raycastForUnit();       // Perform raycasting to find unit under mouse
void handleMouseInput();               // Detect mouse clicks and select/deselect
```

**Visual Indicator:**
- Shape: Thin cylinder (0.5m radius, 0.05m height) - disc-like ring
- Color: Yellow/gold (vec4(1.0, 0.8, 0.0, 1.0))
- Position: Unit's feet, 0.05m above ground (z = 0.05)
- Visibility: Hidden when no selection, visible when unit selected

**Raycasting Logic:**
```cpp
UnitComponent* SelectionSystem::raycastForUnit()
{
    // 1. Get active camera/player
    PlayerPtr player = Game::getPlayer();

    // 2. Get mouse position in screen space
    ivec2 mouse = Input::getMousePosition();

    // 3. Calculate ray from camera through mouse position
    dvec3 camera_pos = player->getWorldPosition();
    vec3 direction = player->getDirectionFromMainWindow(mouse.x, mouse.y);

    // 4. Perform physics intersection (1000 units max distance)
    ObjectPtr hit_object = World::getIntersection(
        camera_pos,
        camera_pos + dvec3(direction) * 1000.0,
        1  // Intersection mask
    );

    // 5. Check if hit object has UnitComponent
    if (hit_object) {
        NodePtr hitNode = hit_object;
        UnitComponent* unit = ComponentSystem::get()->getComponent<UnitComponent>(hitNode);
        return unit;
    }

    return nullptr;
}
```

**Click Detection:**
```cpp
void SelectionSystem::handleMouseInput()
{
    bool currentMouseButtonState = Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT);

    // One-shot detection: transition from up to down
    if (currentMouseButtonState && !previousMouseButtonState) {
        UnitComponent* clickedUnit = raycastForUnit();

        if (clickedUnit) {
            selectedUnit = clickedUnit;
            Log::message("Unit selected: %s\n", clickedUnit->unit_name.get());
        } else {
            clearSelection();
            Log::message("Selection cleared\n");
        }
    }

    // Store state for next frame
    previousMouseButtonState = currentMouseButtonState;
}
```

---

## Integration with GameManager

**GameManager Changes:**

```cpp
// GameManager.h
class GameManager {
public:
    // ... existing systems
    Unigine::SelectionSystem* selection;
};

// GameManager.cpp::init()
void GameManager::init() {
    // ... existing initialization

    // Create selection system
    selection = new Unigine::SelectionSystem();
    selection->init();
}

// GameManager.cpp::handleInput() - called every frame
void GameManager::handleInput() {
    // Update selection system (handles mouse picking)
    if (selection) {
        selection->update();
    }

    // TODO: Keyboard input
    // TODO: UI button callbacks
}

// GameManager.cpp::shutdown()
void GameManager::shutdown() {
    // Delete systems in reverse order
    delete selection;
    delete grid_renderer;
    delete turn_manager;
    delete grid;

    // Reset pointers
    selection = nullptr;
    // ... existing cleanup
}
```

**Update Loop Integration:**

```
AppWorldLogic::update() [Per render frame, ~60-144 FPS]
    ↓
GameManager::handleInput()
    ↓
SelectionSystem::update()
    ↓
handleMouseInput() - Detect clicks, raycast for units
    ↓
updateIndicatorPosition() - Move yellow ring to selected unit
```

---

## Mouse Cursor Fix (CRITICAL)

**Problem:** Mouse cursor was getting captured/grabbed on first click, making it unusable for tactical gameplay.

**Root Cause:** Unigine's player/camera systems internally grab the mouse cursor (default behavior for first-person games).

**Solution:** Continuously check and disable mouse grab every frame in `AppSystemLogic::update()`.

**Implementation:**

```cpp
// AppSystemLogic.cpp
#include <UnigineGui.h>  // Added for Gui::getCurrent()

int AppSystemLogic::update()
{
    // Tactical game mode: continuously ensure mouse cursor remains free
    // NOTE: Unigine's player/camera systems may re-grab the mouse, so we must check every frame
    if (Input::isMouseGrab())
    {
        Input::setMouseGrab(false);              // Disable mouse grab/lock
        Gui::getCurrent()->setMouseEnabled(true); // Ensure GUI can receive mouse events
    }
    return 1;
}
```

**Why This Works:**
- `Input::isMouseGrab()` - Minimal overhead check (only acts if grabbed)
- `Input::setMouseGrab(false)` - Immediately release mouse grab
- `Gui::getCurrent()->setMouseEnabled(true)` - Ensure GUI system processes mouse
- Runs every frame but only takes action when needed

**Why Continuous Check Required:**
- Initial `setMouseGrab(false)` in `init()` gets overridden by player/camera systems
- Player nodes or camera controllers re-grab mouse internally
- This is the **correct solution** for tactical/strategy games in Unigine
- First-person games want mouse grab; tactical games need free cursor

---

## API Corrections & Learnings

### Original Issues (15 Compilation Errors)

SelectionSystem was initially written with incorrect/outdated Unigine API calls. All issues fixed:

**1. Node Cleanup:**
```cpp
// ❌ WRONG:
selectionIndicator->deleteLater();  // Method doesn't exist

// ✅ CORRECT:
selectionIndicator->removeWorldChild(selectionIndicator);
```

**2. Primitive Creation:**
```cpp
// ❌ WRONG:
selectionIndicator = Primitives::createTorus(0.6f, 0.5f, 32, 16);  // Method doesn't exist

// ✅ CORRECT:
selectionIndicator = Primitives::createCylinder(0.5f, 0.05f);  // Thin disc
```

**3. Material Handling:**
```cpp
// ❌ WRONG:
MaterialPtr material = Materials::findMaterialByGUID("7f3da24663f8db1f24fd996983f903dd");  // String not UGUID

// ✅ CORRECT (GridRenderer pattern):
MaterialPtr material = selectionIndicator->getMaterial(0);  // Get default material
MaterialPtr instanceMaterial = material->inherit();         // Create instance
instanceMaterial->setParameterFloat4("albedo_color", vec4(1.0f, 0.8f, 0.0f, 1.0f));
selectionIndicator->setMaterial(instanceMaterial, 0);
```

**4. Mouse Input:**
```cpp
// ❌ WRONG:
float mouseX = Input::getMouseCoordX();  // Method doesn't exist
float mouseY = Input::getMouseCoordY();  // Method doesn't exist

// ✅ CORRECT:
ivec2 mouse = Input::getMousePosition();  // Returns ivec2 with x, y
```

**5. Ray Direction Calculation:**
```cpp
// ❌ WRONG:
Vec3 dir = player->getDirectionFromScreen(mouseX, mouseY, 0, 0);  // Wrong signature

// ✅ CORRECT:
vec3 direction = player->getDirectionFromMainWindow(mouse.x, mouse.y);
```

**6. Physics Intersection:**
```cpp
// ❌ WRONG:
ObjectPtr hitObject = Game::getIntersection(from, from + dir * 1000.0f, 1);  // Wrong class

// ✅ CORRECT:
ObjectPtr hit_object = World::getIntersection(camera_pos, camera_pos + dvec3(direction) * 1000.0, 1);
```

**7. Node Access:**
```cpp
// ❌ WRONG:
NodePtr hitNode = hitObject->getNode();  // Method doesn't exist

// ✅ CORRECT:
NodePtr hitNode = hit_object;  // Direct cast (ObjectPtr is NodePtr)
```

**8. Component Property Access:**
```cpp
// ❌ WRONG:
Log::message("Unit selected: %s\n", clickedUnit->getUnitName().get());  // Method doesn't exist

// ✅ CORRECT:
Log::message("Unit selected: %s\n", clickedUnit->unit_name.get());  // PROP_PARAM access
```

---

## Critical Technical Learnings

### Input System (Unigine)

**Mouse Position:**
```cpp
ivec2 Input::getMousePosition();  // Returns screen coordinates (pixels)
```

**Mouse Buttons:**
```cpp
bool Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT);   // Current state
bool Input::isMouseButtonPressed(Input::MOUSE_BUTTON_LEFT); // One-shot (use with caution)
```

**Best Practice:** Manually track previous state for reliable one-shot detection:
```cpp
bool currentState = Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT);
if (currentState && !previousState) {
    // Button just pressed
}
previousState = currentState;
```

### Raycasting & Intersection

**World::getIntersection():**
```cpp
ObjectPtr World::getIntersection(
    const Math::dvec3 &p0,     // Ray start position
    const Math::dvec3 &p1,     // Ray end position
    int mask                   // Intersection mask (1 = default)
);
```

**Returns:**
- `ObjectPtr` if hit
- `nullptr` if no hit

**Converting to Node:**
```cpp
ObjectPtr hit_object = World::getIntersection(...);
NodePtr hitNode = hit_object;  // Direct cast works
```

**Getting Component from Hit Node:**
```cpp
UnitComponent* unit = ComponentSystem::get()->getComponent<UnitComponent>(hitNode);
```

### Player & Camera

**Getting Active Camera:**
```cpp
PlayerPtr player = Game::getPlayer();  // Returns current player/camera
```

**Ray Direction from Mouse:**
```cpp
ivec2 mouse = Input::getMousePosition();
vec3 direction = player->getDirectionFromMainWindow(mouse.x, mouse.y);
```

**Camera Position:**
```cpp
dvec3 camera_pos = player->getWorldPosition();
```

---

## File Structure Changes

**New Files:**
```
source/
└── Input/
    ├── SelectionSystem.h       # SelectionSystem class definition
    └── SelectionSystem.cpp     # Implementation with correct Unigine APIs
```

**Modified Files:**
```
source/
├── CMakeLists.txt              # Added Input/SelectionSystem.cpp/h to build
├── GameManager.h               # Added SelectionSystem* selection member
└── GameManager.cpp             # Added selection init/update/shutdown
```

**CMakeLists.txt Addition:**
```cmake
# Input Systems (Phase 2 - Unit Selection)
${CMAKE_CURRENT_LIST_DIR}/Input/SelectionSystem.cpp
${CMAKE_CURRENT_LIST_DIR}/Input/SelectionSystem.h
```

---

## Testing & Validation

### Phase 2 Test Checklist

- [ ] **Unit nodes created in Editor** - ObjectMeshStatic with collision
- [ ] **UnitComponent attached** - Set unit_name property for each unit
- [ ] **Selection indicator visible** - Yellow ring appears when unit clicked
- [ ] **Selection indicator position** - Ring follows unit at feet (z = 0.05)
- [ ] **Console logging works** - "Unit selected: [name]" appears
- [ ] **Deselection works** - Clicking empty space clears selection
- [ ] **Raycasting accurate** - Clicks on unit mesh trigger selection
- [ ] **No multi-trigger** - Holding mouse doesn't repeatedly select

### Known Issues & Gaps

**Untested:**
- ⚠️ SelectionSystem has NOT been tested with actual unit nodes in scene
- ⚠️ Raycasting may need intersection mask tuning
- ⚠️ Selection indicator may not appear if units lack collision meshes
- ⚠️ Camera/player setup in world may affect raycasting

**Missing Features:**
- ❌ No camera controls (zoom, pan, rotate)
- ❌ No visual feedback on hover (before click)
- ❌ No multi-select (intentional - tactical game uses single selection)
- ❌ No keyboard shortcuts (e.g., Tab to cycle units)
- ❌ No selected unit stat display in UI

---

## Setup Requirements

### World Setup (Testing Phase 2)

**Required Elements:**
1. **GridConfig node** - (from Phase 1, already exists)
2. **Player/Camera node** - Must exist for raycasting
3. **Unit nodes** - At least 2 units for testing

**Creating Test Units:**

1. In Unigine Editor:
   ```
   Create → Node → Dummy
   Name: "Player_Fighter_01"
   ```

2. Add mesh (for clicking):
   ```
   Add Component → ObjectMeshStatic
   Mesh: capsule.mesh (or any visible mesh)
   ```

3. Add collision (for raycasting):
   ```
   In ObjectMeshStatic:
   - Enable "Intersection" checkbox
   - Intersection Mask: 1 (default)
   ```

4. Add UnitComponent:
   ```
   Add Component → UnitComponent
   Set properties:
   - unit_name: "Fighter"
   - level: 1
   - max_hp: 20
   - current_hp: 20
   - armor_class: 18
   ```

5. Repeat for additional units (Wizard, Goblin, etc.)

**Expected World Structure:**
```
ProjectAnu.world
├── ground
├── GridConfig (with GridConfigComponent)
├── Player_Camera (PlayerPerspective or PlayerSpectator)
├── Player_Fighter_01 (ObjectMeshStatic + UnitComponent)
├── Player_Wizard_01 (ObjectMeshStatic + UnitComponent)
└── Enemy_Goblin_01 (ObjectMeshStatic + UnitComponent)
```

---

## Design Rationale

### Why Cylinder for Selection Indicator?

**Alternatives Considered:**
- ❌ Torus (ring) - `Primitives::createTorus()` doesn't exist in Unigine
- ❌ Custom ObjectMeshDynamic ring - Overly complex, manual vertex/index management
- ✅ **Thin cylinder** - Simple, reliable, looks ring-like when flattened

**Cylinder as Ring:**
- Radius: 0.5m (larger than typical unit base)
- Height: 0.05m (very thin, disc-like appearance)
- Result: Appears as flat ring around unit's feet

### Why One-Shot Click Detection?

**Problem with `isMouseButtonPressed()`:**
- May not be reliable across all input systems
- Frame-rate dependent behavior

**Manual State Tracking:**
```cpp
bool currentMouseButtonState = Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT);
if (currentMouseButtonState && !previousMouseButtonState) {
    // Reliable one-shot detection
}
previousMouseButtonState = currentMouseButtonState;
```

**Benefits:**
- Frame-rate independent
- Predictable behavior
- Easy to debug

### Why No Multi-Select?

**Design Decision:**
- Tactical turn-based game uses single unit per turn
- Multi-select would conflict with turn-based flow
- Simplifies input handling and UI
- Matches PF2e tabletop rules (one character acts at a time)

---

## Next Steps (Phase 2 Completion)

### Immediate (Testing)
1. **Test SelectionSystem:**
   - Create unit nodes in Editor with meshes + collision
   - Add UnitComponent to units
   - Run game, test clicking on units
   - Verify selection indicator appears and follows unit

2. **Debug & Iterate:**
   - Tune intersection masks if raycasting fails
   - Adjust indicator size/color if visibility issues
   - Fix positioning if indicator not at unit feet

### Short-Term (Complete Phase 2)
3. **Camera Controls:**
   - Tactical camera component (zoom, pan, rotate)
   - Mouse wheel zoom
   - Middle-mouse drag for pan
   - Right-mouse drag for rotate
   - WASD/arrow key movement (optional)

4. **Movement Range Display:**
   - Calculate reachable cells based on unit Speed
   - Highlight cells using GridRenderer::highlightCells()
   - Show different colors for 1/2/3 action ranges
   - Display elevation changes (Climb required)

5. **Turn UI Integration:**
   - Show selected unit stats (HP, AC, actions remaining)
   - Highlight current turn's unit automatically
   - Action buttons (Stride, Climb, Strike, End Turn)
   - Visual feedback for available actions

---

## Medium-Term (Phase 3: Movement)

6. **Pathfinding System:**
   - A* implementation with discrete elevation
   - Respect 5-5-10 diagonal costs
   - Handle blocked cells and unit occupancy
   - Return action cost for path

7. **Movement Actions:**
   - Stride action (1 action, horizontal + descend)
   - Climb action (1 action, vertical upward)
   - Preview path before committing
   - Animate unit movement along path

8. **Movement Validation:**
   - Check action budget (can afford movement?)
   - Validate destination not blocked
   - Prevent moving through occupied cells
   - Respect turn order (only current unit can move)

---

## Lessons Learned

### What Went Well
✅ **GridRenderer as reference:** Using working code as template prevented API errors
✅ **Incremental fixes:** Fixing 15 errors one-by-one was manageable
✅ **Component integration:** GameManager pattern made adding SelectionSystem clean
✅ **Build system:** CMakeLists.txt update was straightforward

### What Was Challenging
⚠️ **API documentation gaps:** Many Unigine methods not documented, required header inspection
⚠️ **Primitive limitations:** No torus/ring primitive, had to use cylinder workaround
⚠️ **Intersection API:** World::getIntersection() signature not intuitive
⚠️ **Input method names:** getMousePosition() vs getMouseCoord() inconsistency

### Recommendations for Phase 3+
1. **Always check GridRenderer/existing code** for API usage patterns before writing new code
2. **Test early with actual scene setup** - don't wait for full implementation
3. **Use Primitives for all simple shapes** - avoid manual ObjectMeshDynamic unless necessary
4. **Log extensively during development** - helped debug raycasting issues
5. **Keep CLAUDE.md updated** with API corrections as discovered

---

## References

**Code Files:** See `source/Input/` directory for full implementation
**Unigine Docs:** https://developer.unigine.com/en/docs/latest/
**Phase 1 Foundation:** `04-Implementation/Phase-1-Foundation.md`
**Current Status:** `Current_Status.md`
**GDD:** `01-Core-Mechanics/` for movement/combat design

---

**Phase 2 Status: 🔄 IN PROGRESS**
**Next Milestone:** Test SelectionSystem in-game with unit nodes
**Build Status:** ✅ Compiling Successfully
