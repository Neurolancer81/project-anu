---
title: "Prototype Scope"
type: development
section: 07-Development
status: active
tags:
  - prototype
  - mvp
  - phase-1
  - scope
created: 2025-11-16
updated: 2025-11-16
priority: critical
---

# Prototype Scope

Minimum viable prototype to validate core PF2e combat systems before full production.

---

## Prototype Goals

### Primary Goal
**Validate that PF2e combat translates to 3D video game format.**

### Success Criteria
1. ✅ Complete single encounter start to finish
2. ✅ PF2e combat math works correctly (attacks, damage, saves)
3. ✅ 3-action economy feels tactical with meaningful choices
4. ✅ Elevation and cover affect combat meaningfully
5. ✅ Spells work (targeting, damage, healing, conditions)
6. ✅ No game-breaking bugs
7. ✅ Performance acceptable (30+ FPS)

### What Prototype Validates
- Core PF2e systems are implementable in Unigine
- 3-action economy translates to video game well
- Grid + elevation creates tactical depth
- Combat feels like PF2e encounters

### What Prototype Does NOT Validate
- Campaign structure or narrative
- Full spell/feat content library
- Progression systems between missions
- AI sophistication
- Full content pipeline

---

## Prototype Features

### Core Systems (Must Have)

#### Grid & Movement
- ✅ Square grid (5ft per square)
- ✅ 5-5-10 diagonal movement
- ✅ Discrete elevation levels (grid cells have height)
- ✅ **Stride** action (1 action, move Speed horizontally + descend)
- ✅ **Climb** action (1 action, move Speed upward)
- ✅ Movement visualization (reachable squares, action cost)

#### Combat Resolution
- ✅ PF2e attack rolls: d20 + attack bonus vs AC
- ✅ Damage rolls: weapon dice + modifiers
- ✅ **Four degrees of success:** Crit fail, fail, success, crit success
- ✅ Critical hits: Double damage dice (not modifiers)
- ✅ Saving throws: d20 + save bonus vs spell DC
- ✅ **Multiple Attack Penalty (MAP):** -5/-10 (or -4/-8 agile)
- ✅ Proficiency system: Trained/Expert add to bonuses

#### Cover & Elevation
- ✅ PF2e corner-based cover (lesser/standard/greater)
- ✅ Cover bonuses: +1/+2/+4 AC and Reflex
- ✅ **Height advantage:** +1 per 10ft elevation (max +4)
- ✅ **Cover reduction:** -1 cover level per 10ft elevation
- ✅ Falling damage: 1d6 per 10ft

#### Turn System
- ✅ Initiative order (Perception rolls)
- ✅ 3-action economy (each unit gets 3 actions/turn)
- ✅ Turn phases: Start → Actions → End
- ✅ Condition updates (decrement at end of turn)

#### Spells (8 Total)
**Wizard (4 spells):**
1. **Electric Arc (Cantrip)** - Single target spell attack, electric damage
2. **Magic Missile (Rank 1)** - Auto-hit force missiles, tests heightening
3. **Grease (Rank 1)** - AOE, Reflex save, Prone condition
4. **Invisibility (Rank 2)** - Buff, tests Hidden condition

**Cleric (4 spells):**
1. **Divine Lance (Cantrip)** - Single target spell attack, alignment damage
2. **Heal (Rank 1)** - Healing, single target or AOE, tests heightening
3. **Bless (Rank 1)** - AOE buff, status bonus, duration tracking
4. **Spiritual Weapon (Rank 2)** - Sustained spell, multi-turn effect

**Why these 8:**
- Cover core mechanics (damage, healing, buffs, control)
- Test different targeting modes (single, AOE, self)
- Test conditions (Prone, Hidden, status bonuses)
- Test heightening (Magic Missile, Heal)
- Test sustained spells (Spiritual Weapon)
- Minimal VFX complexity

#### Conditions (4 Core)
- ✅ **Flat-footed:** -2 AC
- ✅ **Prone:** AC penalty vs melee, bonus vs ranged, costs action to stand
- ✅ **Hidden/Invisible:** Flat-footed to attacks, concealment
- ✅ **Status bonuses/penalties:** From Bless, etc.

*(Other 4 conditions deferred: Frightened, Stunned, Slowed, Grabbed)*

#### Classes (4 Units, Fixed Builds)
**Fighter (Level 2):**
- Front-line melee
- Attack of Opportunity reaction
- High AC and HP

**Rogue (Level 2):**
- Flanking specialist
- Sneak Attack damage
- High mobility

**Cleric (Level 2):**
- Healer/support
- 4 divine spells
- Prepared caster

**Wizard (Level 2):**
- Arcane damage/control
- 4 arcane spells
- Prepared caster

**Fixed builds:** Pre-selected stats, feats, spells (no character creation in prototype)

#### UI (Minimal)
- ✅ Grid visualization (squares, elevation indicators)
- ✅ Unit selection
- ✅ Action buttons (Stride, Climb, Strike, Cast Spell)
- ✅ Movement range indicators
- ✅ Attack range visualization
- ✅ Initiative order display
- ✅ HP/condition display per unit
- ✅ Action counter (3 actions remaining)
- ✅ End turn button

---

## Prototype Encounter: "Proving Grounds"

### Map Design
- **Size:** 20x20 grid
- **Elevation:** 2-3 height levels (ground, elevated platform, high ground)
- **Cover:** Multiple cover objects (walls, crates, pillars)
- **Assets:** Simple placeholder 3D blocks, ramps, cubes
- **Purpose:** Test all movement, elevation, cover mechanics

### Player Squad (4 Units)
- 1 Fighter (level 2, longsword + shield)
- 1 Rogue (level 2, rapier)
- 1 Cleric (level 2, mace, 4 prepared spells)
- 1 Wizard (level 2, staff, 4 prepared spells)

**Starting position:** South edge, spread formation

### Enemy Squad (4 Units)
- 2 Melee enemies ("Goblin Warriors" - simple melee attackers)
- 1 Ranged enemy ("Goblin Archer" - bow attacks from distance)
- 1 Caster enemy ("Goblin Shaman" - uses 1-2 simple spells)

**Starting position:** North edge, defensive formation near cover

**Enemy AI (basic):**
- Melee: Move toward nearest player, Strike
- Ranged: Seek cover, attack lowest AC target
- Caster: Cast damaging spell or buff allies

### Victory Condition
All enemy units defeated

### Defeat Condition
All player units defeated

### What This Tests
- Movement (Stride, Climb, diagonal movement)
- Height advantage mechanics
- Cover system (seeking cover, cover bonuses)
- Melee combat (MAP, crits, damage)
- Ranged combat (range penalties, cover)
- Spellcasting (targeting, AOE, saves, healing)
- Conditions (Prone from Grease, Flat-footed from flanking)
- Turn order and action economy
- Win/loss detection

---

## Explicitly Cut from Prototype

### Meta-Layer
- ❌ Character creation
- ❌ Recruitment system
- ❌ Permadeath (can restart encounter freely)
- ❌ HQ menus (just "Start Battle" button)
- ❌ Progression between missions
- ❌ Research tree
- ❌ Engineering/upgrades
- ❌ Resource economy
- ❌ Multiple encounters

### Campaign/Narrative
- ❌ Yuga setting (just generic test map)
- ❌ Story/narrative
- ❌ Investigation sequences
- ❌ Mission variety
- ❌ All 13 encounters

### Content
- ❌ Full 48 spell list (only 8 spells)
- ❌ Feat selection (fixed builds)
- ❌ Ancestries/backgrounds (preset Human)
- ❌ Multiple enemy types (only 2-3 basic enemies)
- ❌ Equipment/inventory system

### Advanced Mechanics
- ❌ Reactions beyond Attack of Opportunity
- ❌ Difficult terrain
- ❌ Flying units
- ❌ Environmental interaction
- ❌ Destructible cover
- ❌ Full condition set (only 4 conditions)
- ❌ Stealth/detection
- ❌ Fog of war

### Polish
- ❌ Final VFX/animations (placeholder only)
- ❌ Sound effects/music
- ❌ Polished UI (functional > beautiful)
- ❌ Tutorials/tooltips
- ❌ Settings/options menu

---

## Technical Implementation Priorities

### Phase 1: Foundation (Weeks 1-2)
- Grid system implementation (square grid, cells, coordinates)
- Unit representation (stats, position, state)
- Turn manager (initiative, turn order, action tracking)
- Basic UI framework (grid rendering, unit display)

### Phase 2: Movement (Weeks 3-4)
- Stride action (pathfinding, movement along grid)
- Climb action (vertical movement)
- Elevation system (height per cell, visualization)
- Movement validation (blocked cells, action cost)

### Phase 3: Basic Combat (Weeks 5-6)
- Attack rolls (d20 + mods vs AC)
- Damage calculation (dice rolls, crits)
- Health tracking and damage application
- Death detection

### Phase 4: Advanced Combat (Weeks 7-8)
- Cover system (corner-based detection)
- Height advantage calculation
- Multiple Attack Penalty
- Degrees of success (crit success/fail)

### Phase 5: Spells (Weeks 9-10)
- Spell data loading (JSON)
- Targeting system (single, AOE)
- Saving throws
- Spell effects (damage, healing, conditions)
- Heightening

### Phase 6: Polish & AI (Weeks 11-12)
- Basic enemy AI
- UI improvements
- Bug fixes
- Performance optimization
- Playtest iteration

**Total estimate:** 12 weeks (3 months) for functional prototype

---

## Data-Driven Architecture

### Spell Data (JSON)
```json
{
  "id": "magic_missile",
  "name": "Magic Missile",
  "rank": 1,
  "actions": 2,
  "traits": ["evocation", "force"],
  "range": 120,
  "targets": 1,
  "damage": {
    "dice": "1d4+1",
    "type": "force"
  },
  "heightening": {
    "type": "per_rank",
    "effect": "+1_missile"
  }
}
```

### Unit Data (JSON)
```json
{
  "id": "fighter_l2",
  "name": "Fighter",
  "level": 2,
  "class": "fighter",
  "stats": {
    "str": 16, "dex": 12, "con": 14,
    "int": 10, "wis": 10, "cha": 10
  },
  "hp_max": 20,
  "ac": 18,
  "saves": {"fort": 6, "reflex": 4, "will": 3},
  "speed": 25
}
```

### Enemy Data (JSON)
```json
{
  "id": "goblin_warrior",
  "name": "Goblin Warrior",
  "level": 1,
  "type": "melee",
  "hp": 16,
  "ac": 16,
  "attack_bonus": 5,
  "damage": "1d6+2",
  "ai_behavior": "aggressive_melee"
}
```

---

## Success Metrics

### Functional Completeness
- ✅ Can start encounter
- ✅ Can move units (Stride, Climb)
- ✅ Can attack (melee, ranged)
- ✅ Can cast all 8 spells
- ✅ Can complete encounter (win or lose)
- ✅ Can restart encounter

### Correctness
- ✅ Attack rolls follow PF2e math
- ✅ Damage calculation is accurate
- ✅ Cover bonuses apply correctly
- ✅ Height advantage calculates properly
- ✅ MAP applies to 2nd/3rd attacks
- ✅ Conditions affect gameplay as expected

### Feel
- ✅ 3-action economy feels tactical
- ✅ Positioning matters (cover, elevation, flanking)
- ✅ Choices are meaningful (which action to take)
- ✅ Combat feels like PF2e tabletop

### Performance
- ✅ 30+ FPS on development machine
- ✅ No stuttering during combat
- ✅ Grid and pathfinding responsive

---

## Post-Prototype Expansion

Once prototype validates core systems:

1. **Expand spell list** to full 48 spells
2. **Add remaining conditions** (Frightened, Stunned, Slowed, Grabbed)
3. **Implement character creation** (ancestries, backgrounds, feats)
4. **Build HQ system** (recruitment, progression)
5. **Create 13 campaign encounters** (Yuga settings, narrative)
6. **Add enemy variety** (3-4 enemy types per Yuga)
7. **Implement meta-layer** (research/engineering if in scope)
8. **Polish pass** (VFX, audio, UI, animations)
9. **Balancing and playtesting**
10. **Yuga-specific assets** (replace placeholders)

---

## Risks & Mitigation

### Risk: PF2e Too Complex for Video Game
**Mitigation:** Prototype tests this explicitly. If too complex, simplify rules while keeping 3-action core.

### Risk: Performance Issues with Grid/Pathfinding
**Mitigation:** Start with 20x20 grid. Optimize early. Consider async pathfinding.

### Risk: Unigine Learning Curve
**Mitigation:** Keep prototype visually simple. Focus on gameplay over graphics.

### Risk: 12 Weeks Too Optimistic
**Mitigation:** Cut features aggressively. Prototype is for validation, not polish.

### Risk: Spells Too Varied to Implement
**Mitigation:** 8 spell prototype tests variety. If too hard, reduce spell count in full game.

---

*See also: [[Game Vision]], [[Movement & Grid System]], [[Turn System]], [[Current Status]]*
