# ProjectAnu - Current Status

**Last Updated:** 2025-11-16
**Current Phase:** Pre-Production / Prototype Planning
**Next Milestone:** Prototype - Single Combat Encounter

---

## Project Overview Status

| Aspect | Status | Notes |
|--------|--------|-------|
| **Game Design** | 🟡 In Progress | Core systems defined, content pending |
| **Prototype Scope** | ✅ Defined | 8 spells, 1 encounter, core mechanics |
| **Technical Setup** | 🟡 In Progress | Unigine project created, CLion setup pending |
| **Implementation** | ⚪ Not Started | Awaiting CLion setup |
| **Assets** | ⚪ Not Started | Placeholder assets for prototype |

**Legend:** ✅ Complete | 🟡 In Progress | ⚪ Not Started | ❌ Blocked

---

## Game Design Document Status

### 00-Overview
- [x] **Game-Vision.md** - Complete (PF2e + Yugas setting defined)
- [x] **00-Overview.md** - Section index created
- [ ] **Competitive-Analysis.md** - Not started
- [ ] **Target-Audience-Personas.md** - Not started

### 01-Core-Mechanics
- [x] **Movement-Grid-System.md** - Complete (grid, elevation, cover, Climb/Stride)
- [x] **Turn-System.md** - Complete (3-action economy, MAP, initiative)
- [ ] **Combat-Resolution.md** - Partial (defined in Prototype-Scope, needs dedicated doc)
- [ ] **Cover-Flanking.md** - Partial (in Movement doc, needs expansion)
- [ ] **Action-Point-System.md** - Covered in Turn-System
- [ ] **Abilities-Framework.md** - Not started

### 02-Systems
- [x] **Character-Progression.md** - Complete (levels 1-4, classes, spells overview)
- [ ] **Grid-Pathfinding.md** - Not started (technical implementation details)
- [ ] **Line-of-Sight.md** - Not started
- [ ] **AI-Behavior.md** - Not started
- [ ] **Inventory-Loadout.md** - Deferred (not in prototype)
- [ ] **Mission-Objective-System.md** - Not started

### 03-Meta-Layer
- [x] **Campaign-Structure.md** - Complete (13 encounters, Yuga narrative)
- [ ] **Base-Management.md** - Deferred (scope TBD)
- [ ] **Research-Tree.md** - Deferred (scope TBD)
- [ ] **Engineering-Upgrades.md** - Deferred (scope TBD)
- [ ] **Resource-Economy.md** - Deferred (scope TBD)
- [ ] **Save-System.md** - Not started

### 04-Content
- [ ] **Mission-Design.md** - Not started
- [ ] **Enemy-Types.md** - Not started (2-3 basic enemies for prototype)
- [ ] **Character-Classes.md** - Partial (overview in Character-Progression)
- [ ] **Weapons-Catalog.md** - Not started
- [ ] **Equipment-Catalog.md** - Deferred
- [ ] **Abilities-Catalog.md** - Not started
- [ ] **Spells-Catalog.md** - Partial (8 prototype spells defined)
- [ ] **Maps-Environments.md** - Not started

### 05-UI-UX
- [ ] **HUD-Design.md** - Not started
- [ ] **Tactical-UI.md** - Not started
- [ ] **Menu-Systems.md** - Not started
- [ ] **Control-Scheme.md** - Not started
- [ ] **Camera-System.md** - Not started
- [ ] **Feedback-Polish.md** - Not started

### 06-Technical
- [ ] **System-Architecture.md** - Not started
- [ ] **Data-Design.md** - Partial (JSON schemas sketched in Prototype-Scope)
- [ ] **Performance-Requirements.md** - Not started
- [ ] **Asset-Pipeline.md** - Not started
- [ ] **Build-Deployment.md** - Not started

### 07-Development
- [x] **Prototype-Scope.md** - Complete (MVP definition, 8 spells, 1 encounter)
- [ ] **Project-Milestones.md** - Not started
- [ ] **Phase-Breakdown.md** - Partial (in Prototype-Scope)
- [ ] **Task-Dependencies.md** - Not started
- [ ] **Risk-Assessment.md** - Partial (in Prototype-Scope)
- [ ] **Testing-Plan.md** - Not started

---

## Prototype Development Checklist

### Pre-Production (Current Phase)
- [x] Define prototype scope
- [x] Select 8 prototype spells
- [x] Document PF2e combat math
- [x] Define prototype encounter
- [ ] **Setup CLion IDE** ⬅️ CURRENT TASK
- [ ] Test Unigine build in CLion
- [ ] Initialize Git repository
- [ ] Create .gitignore for Unigine project

### Phase 1: Foundation (Weeks 1-2)
- [ ] Grid system data structures
- [ ] Unit representation (stats, position)
- [ ] Turn manager (initiative, action tracking)
- [ ] Basic rendering (grid visualization)
- [ ] Unit selection system

### Phase 2: Movement (Weeks 3-4)
- [ ] Pathfinding algorithm (A*)
- [ ] Stride action implementation
- [ ] Climb action implementation
- [ ] Elevation system (height per cell)
- [ ] Movement visualization (reachable squares)
- [ ] Diagonal movement (5-5-10 rule)

### Phase 3: Basic Combat (Weeks 5-6)
- [ ] Attack roll system (d20 + mods)
- [ ] Damage calculation (dice + mods)
- [ ] Critical hit logic (double dice)
- [ ] Health tracking
- [ ] Death detection
- [ ] Basic UI (HP display, action buttons)

### Phase 4: Advanced Combat (Weeks 7-8)
- [ ] Cover system (corner-based raycasting)
- [ ] Height advantage calculation
- [ ] Multiple Attack Penalty (MAP)
- [ ] Degrees of success (4 outcomes)
- [ ] Saving throw system
- [ ] Cover visualization (UI indicators)

### Phase 5: Spells (Weeks 9-10)
- [ ] Spell data loading (JSON parser)
- [ ] Spell targeting system (single target)
- [ ] AOE targeting (ground, burst)
- [ ] Spell effects (damage, healing, conditions)
- [ ] Heightening system
- [ ] Implement 8 prototype spells:
  - [ ] Electric Arc (Wizard cantrip)
  - [ ] Magic Missile (Wizard rank 1)
  - [ ] Grease (Wizard rank 1)
  - [ ] Invisibility (Wizard rank 2)
  - [ ] Divine Lance (Cleric cantrip)
  - [ ] Heal (Cleric rank 1)
  - [ ] Bless (Cleric rank 1)
  - [ ] Spiritual Weapon (Cleric rank 2)

### Phase 6: Polish & AI (Weeks 11-12)
- [ ] Basic enemy AI (melee aggression)
- [ ] Ranged enemy AI (cover seeking)
- [ ] Caster enemy AI (spell selection)
- [ ] Encounter start/end flow
- [ ] Win/loss detection
- [ ] Restart encounter functionality
- [ ] UI polish (initiative order, action economy display)
- [ ] Performance optimization
- [ ] Bug fixing
- [ ] Internal playtesting

---

## Technical Setup Status

### Development Environment
- [x] Unigine SDK installed
- [x] ProjectAnu created via SDK browser (CMake project)
- [ ] **CLion IDE setup** ⬅️ CURRENT TASK
- [ ] CMake configuration tested in CLion
- [ ] Debug build successful
- [ ] Release build successful

### Version Control
- [ ] Git initialized
- [ ] .gitignore configured (exclude bin/, lib/, build/, .obsidian/)
- [ ] Initial commit
- [ ] Remote repository (GitHub/GitLab?) - Optional

### Project Structure
- [x] `source/` - C++ source files (Unigine generated)
- [x] `data/` - Game assets and worlds
- [x] `bin/` - Executables and DLLs (Unigine managed)
- [x] `lib/` - Unigine libraries (Unigine managed)
- [x] `include/` - Unigine headers (Unigine managed)
- [x] `documentation/` - GDD (Obsidian vault)
- [ ] `source/Core/` - Game state, turn system (to be created)
- [ ] `source/Grid/` - Grid and movement (to be created)
- [ ] `source/Combat/` - Combat resolution (to be created)
- [ ] `source/Spells/` - Spell system (to be created)
- [ ] `source/UI/` - User interface (to be created)
- [ ] `source/Data/` - Data loading (to be created)

---

## Content Definition Status

### Prototype Spells (8 Total)
**Wizard (4):**
- [x] Electric Arc - Defined
- [x] Magic Missile - Defined
- [x] Grease - Defined
- [x] Invisibility - Defined

**Cleric (4):**
- [x] Divine Lance - Defined
- [x] Heal - Defined
- [x] Bless - Defined
- [x] Spiritual Weapon - Defined

### Full Spell Lists (48 Total) - Deferred
**Wizard (24):** Not yet selected (need 20 more spells)
**Cleric (24):** Not yet selected (need 20 more spells)

### Classes
- [x] Fighter - Stats defined for level 2
- [x] Rogue - Stats defined for level 2
- [x] Cleric - Stats defined for level 2
- [x] Wizard - Stats defined for level 2
- [ ] Feats per class - Not curated yet

### Ancestries & Backgrounds
- [ ] 3 Ancestries - Not selected
- [ ] 8 Backgrounds - Not selected

### Enemies
- [ ] Melee enemy ("Goblin Warrior") - Stats needed
- [ ] Ranged enemy ("Goblin Archer") - Stats needed
- [ ] Caster enemy ("Goblin Shaman") - Stats needed

---

## Data Schema Status

### Defined (Sketched)
- [x] Spell JSON structure (example in Prototype-Scope.md)
- [x] Unit JSON structure (example in Prototype-Scope.md)
- [x] Enemy JSON structure (example in Prototype-Scope.md)

### To Be Finalized
- [ ] Class feature data structure
- [ ] Feat data structure
- [ ] Condition data structure
- [ ] Encounter/map data structure
- [ ] AI behavior data structure

---

## Open Design Questions

### High Priority (Blocking Prototype)
- [ ] Exact stat blocks for 4 prototype player units (Fighter, Rogue, Cleric, Wizard level 2)
- [ ] Exact stat blocks for 3 prototype enemy units
- [ ] Spell data details (damage dice, save DCs, AOE radius, etc.)
- [ ] Class features for level 2 (which feats, abilities?)

### Medium Priority (Post-Prototype)
- [ ] Meta-layer scope decision (research/engineering in or out?)
- [ ] Recruitment mechanics (new recruits level 1 or scale?)
- [ ] Which 3 ancestries?
- [ ] Which 8 backgrounds?
- [ ] Full 48 spell selection

### Low Priority (Later)
- [ ] Mission objective variety beyond "defeat all"
- [ ] Line of sight / fog of war implementation
- [ ] Stealth/detection mechanics
- [ ] Map size range (20x20, 25x25, 30x30?)

---

## Next Immediate Tasks

### This Session
1. ✅ Document prototype scope in GDD
2. ✅ Create Current_Status.md
3. ⬅️ **Setup CLion for Unigine development** (IN PROGRESS)
4. Test build in CLion

### Next Session
1. Finalize prototype unit stat blocks
2. Define spell data details (for 8 spells)
3. Initialize Git repository
4. Begin Phase 1 implementation (grid system)

---

## Risk Tracking

| Risk | Severity | Status | Mitigation |
|------|----------|--------|------------|
| PF2e too complex for video game | High | Monitoring | Prototype validates this |
| Unigine learning curve too steep | Medium | Monitoring | Focus on gameplay over graphics |
| 12-week prototype timeline too optimistic | Medium | Monitoring | Cut features aggressively if needed |
| Performance issues with grid/pathfinding | Medium | Unmitigated | Optimize early, consider async pathfinding |
| Spell variety too hard to implement | Medium | Monitoring | 8-spell prototype tests this |

---

*For detailed prototype specification, see: `documentation/GDD/07-Development/Prototype-Scope.md`*
*For full game design, see: `documentation/GDD/GDD-Structure.md`*
