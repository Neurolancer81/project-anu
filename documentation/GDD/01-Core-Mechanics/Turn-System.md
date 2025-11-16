---
title: "Turn System & 3-Action Economy"
type: mechanic
section: 01-Core-Mechanics
status: complete
tags:
  - turns
  - actions
  - pf2e
  - initiative
created: 2025-11-16
updated: 2025-11-16
priority: high
---

# Turn System & 3-Action Economy

PF2e's 3-action economy and turn-based combat flow.

---

## Turn Structure

### Initiative Order
- **Roll initiative:** Perception check at combat start
- **Turn order:** Highest to lowest initiative
- **Simultaneous ties:** Player units act before enemy units

### Turn Phases
Each unit's turn consists of:
1. **Start of turn:** Trigger effects (conditions decrement, ongoing damage)
2. **Action phase:** Spend 3 actions
3. **End of turn:** Trigger end-of-turn effects
4. **Reactions:** Available during other units' turns (if triggered)

---

## 3-Action Economy

### Core Concept
Every unit gets **3 actions** per turn to spend on activities.

### Action Costs
Activities cost 1, 2, or 3 actions:

**1-Action Activities:**
- Stride (move)
- Strike (basic attack)
- Climb
- Interact (open door, drink potion)
- Raise Shield
- Step (5ft move, doesn't trigger reactions)
- Many class abilities

**2-Action Activities:**
- Most spells (2-action casting)
- Some class abilities
- Some skill actions

**3-Action Activities:**
- Some powerful spells (e.g., high-level rituals)
- Some ultimate abilities (rare)

**Free Actions:**
- Drop item
- Release spell (end concentration)
- Some triggered abilities

**Reactions:**
- Occur during other units' turns
- Triggered by specific conditions
- Each unit gets 1 reaction per round (refreshes at start of their turn)

### Action Flexibility
Key advantage of 3-action system:
- **3 Strikes:** Attack 3 times (with MAP penalty)
- **Move + Strike + Move:** Reposition mid-attack
- **Move + Move + Strike:** Traverse battlefield and attack
- **Spell + Move:** Cast and reposition
- **Move + Move + Move:** Full retreat (3 Stride actions)

---

## Multiple Attack Penalty (MAP)

### Penalty on Repeated Attacks
Attacking multiple times in one turn incurs cumulative penalties.

**Penalty progression:**
- 1st Strike: No penalty
- 2nd Strike: -5 penalty
- 3rd Strike: -10 penalty

**Agile weapons (daggers, rapiers):**
- 1st Strike: No penalty
- 2nd Strike: -4 penalty
- 3rd Strike: -8 penalty

### MAP Resets
- Resets at the start of your next turn
- Only applies to **attack rolls** (Strikes, spell attacks)
- Does NOT apply to spells requiring saves

### Tactical Implications
- Encourages mixing Strikes with other actions
- Spells with saves avoid MAP
- Buff/debuff between Strikes to maximize efficiency

**Example turn:**
- Strike (no penalty)
- Demoralize (intimidate, inflicts Frightened)
- Strike (−5, but target has −1 AC from Frightened)

---

## Common Actions

### Movement
- **Stride (1 action):** Move up to Speed horizontally, descend freely
- **Climb (1 action):** Move up to Speed vertically upward
- **Step (1 action):** Move 5ft without triggering reactions

### Combat
- **Strike (1 action):** Melee or ranged attack, incurs MAP
- **Raise Shield (1 action):** +2 AC until start of next turn
- **Cast a Spell (2 actions typical):** Varies by spell

### Skills
- **Demoralize (1 action):** Intimidation check, inflict Frightened on success
- **Feint (1 action):** Deception check, target Flat-footed on success
- **Trip (1 action):** Athletics check, knock target Prone

### Class-Specific
Vary by class, typically 1-2 actions:
- **Power Attack (Fighter, 2 actions):** Single Strike, more damage
- **Sneak Attack (Rogue, passive):** Bonus damage when target Flat-footed
- **Channel Energy (Cleric, 2 actions):** Heal or harm AOE

---

## Reactions

### Attack of Opportunity (Fighter)
- **Trigger:** Enemy moves within reach, uses ranged attack, or uses manipulate action
- **Effect:** Make a melee Strike against triggering creature

### Other Reactions
- **Shield Block:** Reduce damage when hit (if shield raised)
- **Retributive Strike (some classes):** React to ally being hit

### Reaction Timing
- Occurs on someone else's turn
- Each unit gets 1 reaction per round
- Refreshes at start of unit's turn

---

## Turn Economy Examples

### Aggressive Fighter
- **Action 1:** Stride (move into melee)
- **Action 2:** Strike (attack)
- **Action 3:** Strike (attack at -5)

### Defensive Cleric
- **Action 1:** Stride (move to ally)
- **Action 2-3:** Heal spell (2-action spell)

### Mobile Rogue
- **Action 1:** Stride (move to flank)
- **Action 2:** Strike (with Sneak Attack)
- **Action 3:** Stride (move away)

### Positioning Wizard
- **Action 1:** Stride (move to vantage point)
- **Action 2:** Climb (ascend to elevation)
- **Action 3:** Delay until later in initiative order (free action, then 1-action spell on delayed turn)

---

## Initiative & Turn Order

### Initiative Roll
- **Check:** Perception + modifiers
- **Roll:** 1d20 + Perception
- Rolled once at combat start

### Turn Order Display (UI)
- Show initiative order on screen
- Highlight current unit
- Display upcoming units
- Show when player units act vs enemy units

### Delaying
- **Delay (free action):** Act later in initiative order
- New initiative = when you act
- Cannot delay past end of round

---

## End of Turn

### Condition Updates
- **Frightened, Stunned, Slowed:** Decrease value by 1
- **Persistent damage:** Roll flat check to remove, or take damage
- **Buffs/debuffs:** Check duration, remove if expired

### Status Display (UI)
- Show active conditions
- Display durations
- Indicate which conditions decrement end-of-turn

---

## Out of Scope (v1.0)

- **Delayed readied actions:** No readied actions initially
- **Complex reactions:** Only Attack of Opportunity, Shield Block
- **Simultaneous turns:** Strict turn order, no parallel actions
- **Speed changes mid-turn:** Fixed Speed per turn

---

## Design Rationale

### Why 3-Action Economy?
- **Flexibility:** Players make meaningful choices every turn
- **No "move or attack" limitation:** Move AND attack in same turn
- **Depth:** Combining actions creates tactical variety
- **Video game friendly:** Clear action budget, easy to visualize

### Why MAP?
- Discourages "full attack" spam
- Encourages mixing actions (move, buff, debuff)
- Maintains PF2e balance
- Creates risk/reward for 2nd/3rd attacks

### Why Reactions?
- Adds counterplay
- Punishes reckless movement
- Creates zone control (Attack of Opportunity)
- Rewards tactical positioning

---

*See also: [[Movement & Grid System]], [[Combat Resolution]], [[Abilities Framework]]*
