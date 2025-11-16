---
title: "Movement & Grid System"
type: mechanic
section: 01-Core-Mechanics
status: complete
tags:
  - grid
  - movement
  - elevation
  - cover
  - pf2e
created: 2025-11-16
updated: 2025-11-16
priority: high
---

# Movement & Grid System

Core movement and grid mechanics for tactical combat.

---

## Grid Foundation

### Square Grid
- **Grid type:** Square grid (not hex)
- **Cell size:** 5 feet per square (PF2e standard)
- **Diagonal movement:** 5-5-10 rule (alternating diagonal costs)
  - 1st diagonal: 5ft (1 square)
  - 2nd diagonal: 10ft (2 squares)
  - 3rd diagonal: 5ft (1 square)
  - Pattern repeats

### Map Size
- **Range:** TBD (likely 20x25 to 30x30)
- **Squad size:** 4-6 units per mission
- **Enemy count:** Scales with mission

---

## Movement Actions

### Stride (1 Action)
- Move up to your Speed horizontally
- Default Speed: 25 feet (5 squares)
- Can descend elevation freely (within movement range)
- Cannot move upward (requires Climb)
- **Diagonal cost:** 5-5-10 rule applies

**Example:**
- Speed 25ft = 5 squares of movement
- Move 3 straight + 1 diagonal = 3 + 1 = 4 squares ✓
- Move 2 diagonal + 2 straight = (1 + 2) + 2 = 5 squares ✓
- Move 3 diagonal = 1 + 2 + 1 = 4 squares ✓

### Climb (1 Action)
- Move up to your Speed **vertically upward only**
- Default Speed: 25 feet = 5 elevation levels
- Automatic success (no Athletics check)
- Cannot move horizontally during Climb action

**Example:**
- Climb 2 elevation levels (10ft) = 1 action
- Climb 5 elevation levels (25ft) = 1 action (full Speed)

### Descending
- Descending is part of **Stride** action (free within movement)
- Moving off elevated edge without Stride = **Fall**

### Ramps & Stairs
- Count as horizontal movement (use Stride)
- No Climb action required for gradual inclines

---

## Elevation System

### Discrete Elevation Levels
- Each grid cell has elevation value: 0, 1, 2, 3, etc.
- 1 elevation level = 5 feet of height
- Characters can be at different elevations

### Height Advantage
- **Bonus:** +1 circumstance bonus to attacks per 10ft elevation difference
- **Maximum:** +4 (at 40ft / 8 elevation levels above target)
- Applies to melee and ranged attacks

**Example:**
- Attacker at elevation 4, target at elevation 0 = 20ft difference
- Height advantage: +2 to attack rolls

### Cover Reduction from Height
- Every 10ft of elevation difference reduces cover by 1 level
- **Standard cover (+2 AC)** at 10ft above → **Lesser cover (+1 AC)**
- **Standard cover (+2 AC)** at 20ft above → **No cover**

**Example:**
- Target behind standard cover (+2 AC)
- Attacker 20ft above target (4 elevation levels)
- Cover reduced by 2 levels: Standard → No cover
- Target loses cover bonus entirely

### Combined Height Effects
Height advantage and cover reduction stack powerfully.

**Example:**
- Attacker 20ft above target behind standard cover
- Height advantage: +2 to attack
- Cover reduction: +2 AC negated
- **Net benefit: +2 to hit + cover ignored**

### Falling
- Moving off elevated edge without descending via Stride = Fall
- **Fall damage:** 1d6 per 10ft fallen (PF2e standard)
- Land Prone (condition)

**Example:**
- Fall from elevation 4 to elevation 0 = 20ft
- Damage: 2d6
- Condition: Prone

---

## Cover System

### PF2e Corner-Based Cover
Cover is determined by drawing lines from **attacker's corner** to **target's corner**.

### Cover Types
**Lesser Cover (+1 AC, +1 Reflex):**
- Small obstacles, creatures, foliage
- Partially obscures target

**Standard Cover (+2 AC, +2 Reflex):**
- Walls, trees, large obstacles
- Target peeking around corner

**Greater Cover (+4 AC, +4 Reflex):**
- Arrow slits, murder holes, tiny openings
- Only small portion of target visible

### Cover Calculation
1. Draw line from any corner of attacker's square to any corner of target's square
2. If line passes through/along terrain:
   - Check terrain type (lesser/standard/greater)
   - Apply highest cover type encountered
3. If no line is blocked, no cover

### Cover & Elevation
- Cover level reduced by elevation difference (see Height Advantage above)
- Attacker above target can shoot "over" low cover

---

## Movement Visualization (UI)

### Grid Display
- Grid visible during unit's turn
- Highlight reachable squares with remaining actions
- Color-coded by action cost:
  - 1 action range
  - 2 action range
  - 3 action range (if all actions spent on movement)

### Movement Preview
- Click destination: Show path and action cost
- Display elevation changes in path
- Warn if path requires Climb action

### Cover Indicators
- Show cover type for selected target
- Visualize height advantage/disadvantage
- Display final hit chance modifiers

---

## Out of Scope (v1.0)

- **Flying units:** No flight movement
- **Swimming/water:** No water terrain
- **Difficult terrain:** Not implemented initially
- **Squeeze/crawl:** Standard movement only
- **Jumping:** Use Climb or fall
- **Teleportation:** No teleport spells in v1.0 spell list

---

## Design Rationale

### Why Climb as Separate Action?
- Makes height advantage tactical: You trade actions for positioning
- Prevents dominant "climb and shoot same turn" strategy
- Creates interesting risk/reward: Climb now (vulnerable) or attack from low ground?

### Why 5-5-10 Diagonal Cost?
- PF2e standard, familiar to TTRPG players
- Avoids fractional movement costs
- Simple to calculate

### Why Automatic Climbing?
- Avoids RNG skill checks mid-combat
- Video game expects deterministic movement
- Simplifies implementation (no Athletics system needed)

### Why Height Advantage So Strong?
- Makes 3D terrain matter tactically
- Climb action cost balances the benefit
- Encourages vertical map design

---

*See also: [[Turn System]], [[Cover & Flanking]], [[Combat Resolution]]*
