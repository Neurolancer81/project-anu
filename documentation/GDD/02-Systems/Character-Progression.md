---
title: "Character Progression"
type: system
section: 02-Systems
status: in-progress
tags:
  - progression
  - levels
  - classes
  - spells
  - feats
  - pf2e
created: 2025-11-16
updated: 2025-11-16
priority: high
---

# Character Progression

Leveling, class advancement, and character growth systems.

---

## Level Range

### Levels 1-4
- **Starting level:** 1
- **Maximum level:** 4
- **Progression:** Milestone leveling (level up after specific encounters)

### Level-Up Schedule
- **Level 1:** Encounters 1-3 (Satya Yuga)
- **Level 2:** Encounters 4-6 (Treta Yuga)
- **Level 3:** Encounters 7-9 (Dvapara Yuga)
- **Level 4:** Encounters 10-13 (Kali Yuga + Final)

Approximately 1 level per 3 encounters.

---

## Classes

### Four Core Classes

**Fighter (Martial Striker)**
- **Role:** Front-line damage dealer, durability
- **Key features:** Attack of Opportunity, high AC, weapon mastery
- **Progression:** Gain combat feats, improved strikes

**Rogue (Martial Striker/Support)**
- **Role:** High single-target damage, mobility, skills
- **Key features:** Sneak Attack, high skills, evasion
- **Progression:** Gain skill feats, advanced tactics

**Cleric (Divine Caster/Support)**
- **Role:** Healing, buffs, divine magic
- **Key features:** Heal spells, Channel Energy, divine font
- **Progression:** Gain spell ranks, more spell slots

**Wizard (Arcane Caster/Control)**
- **Role:** Area damage, battlefield control, utility
- **Key features:** Arcane spells, spell school focus (TBD)
- **Progression:** Gain spell ranks, more spell slots

### Class Balance
- Each class fills distinct tactical role
- No overlap (Fighter ≠ Rogue, Cleric ≠ Wizard)
- Squad composition matters (need frontline + support + damage)

---

## Ancestries & Backgrounds

### 3 Ancestries
- **TBD:** To be selected based on setting
- Provide stat boosts, ancestry feat at level 1
- **Examples (if using PF2e standard):** Human, Elf, Dwarf

### 8 Backgrounds
- **TBD:** To be defined based on narrative
- Provide stat boosts, skill training, background feat
- **Examples:** Soldier, Scholar, Acolyte, Criminal, etc.

---

## Spellcasting Progression

### Spell Ranks
- **Cantrips:** Unlimited use, scale with level
- **Rank 1 spells:** Unlock at level 1
- **Rank 2 spells:** Unlock at level 3
- **No rank 3+ spells:** Level cap at 4

### Spell Slots Per Day
PF2e standard progression for Cleric/Wizard:

| Level | Rank 1 Slots | Rank 2 Slots |
|-------|--------------|--------------|
| 1     | 2            | -            |
| 2     | 3            | -            |
| 3     | 3            | 2            |
| 4     | 3            | 3            |

Plus cantrips (unlimited).

### Prepared Casting
- **Daily preparation:** Select spells to prepare before mission
- **Cannot change mid-mission:** Prepared spells locked during encounter
- **Spell slots:** Can cast any prepared spell using appropriate slot

**Example:**
- Wizard prepares: Magic Missile, Grease, Mage Armor (rank 1)
- Has 3 rank 1 slots at level 2
- Can cast Magic Missile 3 times, or Grease + Magic Missile + Mage Armor

### Spell List Size
- **24 spells per caster class** (total available)
  - 8 cantrips
  - 8 rank 1 spells
  - 8 rank 2 spells
- **Spell selection at level-up:** Gain access to more spells from class list
- **Wizard:** 10 spells known (expanding)
- **Cleric:** Access to all divine spells on list

### Spell Heightening
- Cast lower-rank spell in higher-rank slot
- Spell effect scales (more damage, targets, duration)
- Each spell defines its heightening (data-driven)

**Example:**
- Magic Missile (rank 1): 1 missile
- Heightened to rank 2: 2 missiles
- Costs rank 2 spell slot

---

## Feat Progression

### Feat Acquisition
PF2e standard:
- **Level 1:** Ancestry feat, background feat, class feat
- **Level 2:** Skill feat, class feat
- **Level 3:** General feat, skill feat
- **Level 4:** Skill feat, class feat

### Limited Feat Selection
- Not all PF2e feats included
- **Curated list:** 3-5 options per feat slot
- Data-driven: Easy to expand post-launch

### Example Class Feats (Fighter)
- **Level 1:** Power Attack, Sudden Charge, Point-Blank Shot
- **Level 2:** Intimidating Strike, Lunge, Double Slice
- **Level 4:** Knockdown, Furious Focus

---

## Ability Scores

### Starting Scores (Level 1)
- **Ancestry boosts:** +2 to specific abilities (varies by ancestry)
- **Background boosts:** +2 to two abilities
- **Class boost:** +2 to key ability
- **Free boosts:** +2 to four abilities of choice

### Ability Score Increases
- **Level 1:** Standard starting boosts
- **No increases at levels 2-4**

(PF2e grants ability boosts at levels 5, 10, 15, 20 - out of scope)

---

## Health & Defenses

### Hit Points
- **Level 1:** Class HP + Con modifier
- **Each level:** +Class HP + Con modifier

**Class HP per level:**
- Fighter: 10 HP
- Rogue: 8 HP
- Cleric: 8 HP
- Wizard: 6 HP

### Armor Class (AC)
- **Base:** 10 + Dex modifier (or armor)
- **Proficiency:** Class-based (Fighter > Cleric/Rogue > Wizard)
- **Increases with level:** Proficiency bonus scales

### Saving Throws
- **Fortitude, Reflex, Will**
- Class determines proficiency in each
- **Fighter:** Fort (expert), Ref (trained), Will (trained)
- **Rogue:** Fort (trained), Ref (expert), Will (expert)
- **Cleric:** Fort (trained), Ref (trained), Will (expert)
- **Wizard:** Fort (trained), Ref (trained), Will (expert)

---

## Conditions

### 8 Core Conditions
Affect character capabilities:

1. **Flat-footed:** AC -2, easier to hit
2. **Frightened X:** All checks -X (decrements each turn)
3. **Prone:** Melee attacks easier, ranged harder; costs action to stand
4. **Stunned X:** Lose X actions (decrements)
5. **Slowed X:** Lose X actions each turn
6. **Grabbed:** Can't move, flat-footed
7. **Sickened X:** All checks -X
8. **Clumsy/Enfeebled X:** Dex/Str-based checks -X

### Condition Tracking (UI)
- Display active conditions on unit
- Show value (Frightened 2, Slowed 1)
- Indicate when conditions decrement (end of turn)

---

## Recruitment & Permadeath

### Permadeath
- Unit death is permanent
- No resurrection (in v1.0)
- Recruit replacements at HQ

### Recruitment
- **Location:** HQ barracks menu
- **New recruits:** TBD - Start at level 1 or scale to party level?
- **Cost:** TBD - Free or resource cost?
- **Roster limit:** TBD - 8? 12? Unlimited?

### Attachment vs Replaceability
- Permadeath creates stakes
- Progression (feats, spells) makes veterans valuable
- New recruits are weaker (if starting at low level)

---

## Progression Rewards Per Level

### Level 2 Gains
- +Class HP
- +Skill feat
- +Class feat
- Proficiency increases

### Level 3 Gains
- +Class HP
- **Rank 2 spells unlocked** (casters)
- +General feat
- +Skill feat
- Proficiency increases

### Level 4 Gains
- +Class HP
- More rank 2 spell slots (casters)
- +Skill feat
- +Class feat
- Proficiency increases

---

## Out of Scope (v1.0)

- **Multiclassing:** Single-class only
- **Archetypes:** No PF2e archetypes
- **Prestige classes:** Not applicable
- **Ability score increases:** None between levels 1-4
- **Advanced class features:** Limited to core features per class

---

## Open Questions

🔄 **Ancestries:**
- Which 3 ancestries fit the setting? (Human, Elf, Dwarf generic, or culturally specific?)

🔄 **Backgrounds:**
- Define 8 backgrounds relevant to Yuga narrative

🔄 **Recruitment scaling:**
- New recruits at level 1 (high permadeath penalty) or scale to party (easier)?

🔄 **Feat curation:**
- Which specific feats per class? Need curated lists.

🔄 **Spell school focus (Wizard):**
- Wizards in PF2e choose spell school. Implement or generic?

---

*See also: [[Classes]], [[Spells]], [[Campaign Structure]]*
