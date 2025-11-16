# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

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
