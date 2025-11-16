---
title: "Game Vision"
type: overview
section: 00-Overview
status: in-progress
tags:
  - vision
  - design-pillars
  - scope
  - target-audience
  - pf2e
  - yugas
created: 2025-11-15
updated: 2025-11-16
priority: high
aliases:
  - Vision
  - Core Vision
---

# Game Vision

> **Elevator Pitch:** A PF2e-based turn-based tactics game set across the four Yugas of ancient India, where players command a squad through pivotal moments in history, only to discover they are the Asuras causing the very apocalypses they seek to prevent.

---

## Core Vision

ProjectAnu is a turn-based tactical combat game built on Pathfinder 2e Remastered rules that emphasizes:
- **Tactical depth** through PF2e's 3-action economy and positioning
- **Meaningful choices** in combat through class abilities, spells, and terrain
- **High stakes** through permadeath and limited squad size
- **Narrative revelation** through a compact, focused 13-encounter campaign

The game uses **Pathfinder 2e Remastered** as its ruleset foundation, providing proven tactical depth while limiting scope to core classes and mechanics for solo development feasibility.

---

## Core Pillars

### 1. PF2e Tactical Combat
Every encounter leverages PF2e's 3-action economy for deep tactical gameplay.

**Key Elements:**
- **3-action economy:** Each character gets 3 actions per turn
- **Grid-based movement:** Square grid with discrete elevation
- **Cover system:** PF2e corner-based cover (lesser/standard/greater)
- **Height advantage:** +1 per 10ft elevation (max +4), reduces cover
- **Class abilities:** Fighter maneuvers, Rogue tactics, Cleric spells, Wizard magic
- **Spell system:** Prepared casting with heightening

### 2. Squad Management & Permadeath
Build attachment to soldiers through progression and the threat of permanent loss.

**Key Elements:**
- **Squad size:** 4-6 units per mission
- **Permadeath:** XCOM-style permanent death
- **Character progression:** Levels 1-4, unlock class feats and spells
- **Recruitment:** Replace fallen soldiers at HQ
- **4 classes:** Fighter, Rogue, Cleric, Wizard
- **3 ancestries, 8 backgrounds**

### 3. Yuga-Spanning Narrative
A compact but impactful story across the four Yugas (cosmic cycles) of Hindu mythology.

**Key Elements:**
- **13 encounters:** 3 per Yuga + final battle
- **Slow revelation:** Evil force → Asuras → You are the Asuras
- **Pivotal moments:** Deploy to critical points in each Yuga
- **Menu-based investigation:** Skill checks reveal clues between missions
- **Final twist:** Face alternate version of your squad

### 4. Scope Discipline
Focused execution over feature bloat. Solo dev constraints drive design.

**Key Elements:**
- **Data-driven design:** CSV/JSON for spells, feats, classes
- **Limited content:** 48 spells, 4 classes, 8 conditions
- **Generic assets:** Use premade asset packs per Yuga
- **Menu-based HQ:** No 3D base building
- **13 encounters:** Tight, curated campaign

---

## Setting & Tone

### Fictionalized Ancient India (Yugas)
The game is set across the four Yugas of Hindu cosmology, reinterpreted for tactical gameplay:

**Satya Yuga (Golden Age):**
- Era of truth and righteousness
- Visual: Bright, pristine temples and nature
- Enemies: Corrupted beings, early threats

**Treta Yuga (Silver Age):**
- Decline begins, conflict emerges
- Visual: Grand kingdoms, fortifications
- Enemies: Organized forces, tactical units

**Dvapara Yuga (Bronze Age):**
- Moral degradation accelerates
- Visual: Darker tones, war-torn landscapes
- Enemies: Advanced threats, magic users

**Kali Yuga (Iron Age):**
- Age of strife and corruption
- Visual: Apocalyptic, ruined civilization
- Enemies: Most dangerous foes, desperation

**Yugantara Battles:**
- End-of-Yuga cataclysmic encounters
- Reveal Asuras appear at each transition
- Progressive difficulty spike

### Cultural Grounding
- **No Golarion:** Not using Pathfinder's official setting
- **Mythological inspiration:** Yugas, Asuras, cosmic cycles
- **Fictionalized:** Not historically accurate, mythology-inspired fantasy
- **Respectful adaptation:** Ancient India aesthetic without stereotypes

---

## Target Audience

### Primary Audience
- **TTRPG players** familiar with Pathfinder 2e or D&D
- **Tactics enthusiasts** who want deep combat without 50+ hour campaigns
- **PC gamers** comfortable with complex rulesets

### Secondary Audience
- **Cultural representation seekers** interested in underrepresented settings
- **Indie game fans** who appreciate focused, well-executed concepts

---

## Scope & Constraints

### In Scope (v1.0)
- ✅ 13 tactical encounters (3 per Yuga + final)
- ✅ 4 classes (Fighter, Rogue, Cleric, Wizard)
- ✅ 3 ancestries, 8 backgrounds
- ✅ 48 spells (24 Wizard, 24 Cleric): 8 cantrips, 8 rank 1, 8 rank 2 per class
- ✅ Levels 1-4 progression
- ✅ 8 core conditions (Flat-footed, Frightened, Prone, Stunned, Slowed, Grabbed, Sickened, Clumsy/Enfeebled)
- ✅ Prepared casting with heightening
- ✅ Squad size 4-6, permadeath, recruitment
- ✅ Menu-based HQ and investigation
- ✅ 3D tactical maps with discrete elevation
- ✅ PF2e corner-based cover system
- ✅ Generic asset packs per Yuga

### Out of Scope (v1.0)
- ❌ Levels beyond 4 (no rank 3+ spells)
- ❌ Additional classes (Ranger, Bard, etc.)
- ❌ Cleric domains
- ❌ Flying units
- ❌ Environmental interaction (destructible terrain)
- ❌ 3D base building
- ❌ Multiplayer
- ❌ Procedural generation
- ❌ Full PF2e feat trees (limited, curated feats)
- ❌ All PF2e conditions (only top 8)

### Maybe Later (Post-Launch)
- 🔄 Additional Yuga mission pack
- 🔄 New classes (Ranger, Barbarian)
- 🔄 More spells per class
- 🔄 Challenge modes
- 🔄 Flying units and advanced movement

---

## Success Criteria

### Minimum Viable Product (MVP)
- Complete 13-encounter campaign
- All 4 classes playable with distinct tactical roles
- 48 spells implemented with VFX
- PF2e combat rules accurately translated
- Menu-based HQ and investigation functional
- Narrative twist lands effectively

### Success Metrics
- **Playtime:** 10-15 hours for first completion
- **Replayability:** Players experiment with different class compositions
- **Difficulty:** Challenging but fair (PF2e balance maintained)
- **Polish:** Responsive controls, clear UI feedback, minimal bugs
- **Narrative impact:** Asura twist is surprising but earned

---

## Design Philosophy

### Guiding Principles

**1. PF2e Fidelity Over Simplification**
- Use PF2e rules as designed where feasible
- Simplify only where video game constraints require it
- Leverage existing game design (PF2e is playtested)

**2. Clarity Over Complexity**
- Show hit chances, save DCs, modifiers clearly
- Visualize cover, elevation, positioning
- Clear action economy feedback

**3. Meaningful Tactical Choices**
- 3-action economy creates interesting decisions
- Elevation, cover, positioning matter
- Class roles are distinct

**4. Respect Player Time**
- 13 encounters, no padding
- Menu-based investigation (no fake exploration)
- Skip/speed-up options for animations

**5. Solo Dev Realism**
- Data-driven architecture for maintainability
- Generic assets avoid art bottleneck
- Limited scope executed well > ambitious scope executed poorly

---

## Technical Approach

### PF2e Implementation
- **Data-driven:** Spells, feats, class features in CSV/JSON
- **Modular:** Easy to add new content post-launch
- **Validation:** Data files validate against PF2e rules
- **Scripting:** Complex spell effects may need custom logic

### 3D in Unigine
- **Grid-based:** Square grid with discrete elevation
- **Generic assets:** Different asset packs per Yuga
- **Visual fidelity:** Leverage Unigine's rendering for polish
- **Camera:** Tactical camera angle, manual rotation

### Menu-Based Meta
- **No 3D base:** All HQ in menus/UI
- **Investigation:** Static backgrounds + skill check UI
- **Resource management:** (To be determined - research/engineering scope TBD)

---

## Open Questions

🔄 **Meta-layer scope:**
- Research tree in v1.0 or cut?
- Engineering/upgrades or fixed equipment?
- Resource economy or simplified progression?

🔄 **Map size range:**
- 20x20? 25x25? 30x30? Variable per encounter?

🔄 **Recruitment mechanics:**
- New recruits start at level 1 or scale?
- Hiring cost and roster limits?

🔄 **Setting finalization:**
- Commit to fictionalized Mahajanapadas or keep generic fantasy option?

---

*Last Updated: 2025-11-16*
