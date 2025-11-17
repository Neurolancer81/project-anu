// TurnManager.cpp
#include "TurnManager.h"
#include "../Components/UnitComponent.h"
#include <UnigineNode.h>
#include <UnigineLog.h>
#include <UnigineGame.h>
#include <random>

TurnManager::TurnManager()
    : combat_active(false)
    , current_round(0)
    , current_turn_index(-1)
    , actions_remaining(3)
    , attacks_this_turn(0)
    , used_agile_weapon(false)
{
}

TurnManager::~TurnManager() {
    endCombat();
}

void TurnManager::startCombat(const Unigine::Vector<Unigine::NodePtr>& player_units,
                                const Unigine::Vector<Unigine::NodePtr>& enemy_units) {
    Unigine::Log::message("TurnManager::startCombat() - Rolling initiative...\n");

    combat_active = true;
    current_round = 1;
    current_turn_index = -1;
    initiative_order.clear();

    // Roll initiative for all units
    rollInitiative(player_units, enemy_units);

    // Sort by initiative (highest to lowest)
    sortInitiativeOrder();

    // Log initiative order
    Unigine::Log::message("Initiative order:\n");
    for (int i = 0; i < initiative_order.size(); i++) {
        const InitiativeEntry& entry = initiative_order[i];
        if (entry.unit_component) {
            Unigine::Log::message("  %d. %s (Initiative: %d) %s\n",
                i + 1,
                entry.unit_component->unit_name.get(),
                entry.initiative_value,
                entry.is_player_unit ? "[PLAYER]" : "[ENEMY]");
        }
    }

    // Start first turn
    startNextTurn();
}

void TurnManager::endCombat() {
    if (!combat_active) return;

    Unigine::Log::message("TurnManager::endCombat() - Combat ended\n");

    combat_active = false;
    current_round = 0;
    current_turn_index = -1;
    initiative_order.clear();
}

void TurnManager::rollInitiative(const Unigine::Vector<Unigine::NodePtr>& player_units,
                                   const Unigine::Vector<Unigine::NodePtr>& enemy_units) {
    // Roll for player units
    for (int i = 0; i < player_units.size(); i++) {
        UnitComponent* unit = Unigine::ComponentSystem::get()->getComponent<UnitComponent>(player_units[i]);
        if (unit && unit->isAlive()) {
            InitiativeEntry entry;
            entry.unit_node = player_units[i];
            entry.unit_component = unit;
            entry.initiative_value = rollInitiativeForUnit(unit);
            entry.is_player_unit = true;
            initiative_order.append(entry);
        }
    }

    // Roll for enemy units
    for (int i = 0; i < enemy_units.size(); i++) {
        UnitComponent* unit = Unigine::ComponentSystem::get()->getComponent<UnitComponent>(enemy_units[i]);
        if (unit && unit->isAlive()) {
            InitiativeEntry entry;
            entry.unit_node = enemy_units[i];
            entry.unit_component = unit;
            entry.initiative_value = rollInitiativeForUnit(unit);
            entry.is_player_unit = false;
            initiative_order.append(entry);
        }
    }
}

void TurnManager::sortInitiativeOrder() {
    // Sort by initiative value (highest to lowest)
    // On ties, player units go first
    for (int i = 0; i < initiative_order.size() - 1; i++) {
        for (int j = i + 1; j < initiative_order.size(); j++) {
            bool should_swap = false;

            if (initiative_order[i].initiative_value < initiative_order[j].initiative_value) {
                // j has higher initiative
                should_swap = true;
            } else if (initiative_order[i].initiative_value == initiative_order[j].initiative_value) {
                // Tie: player units go first
                if (!initiative_order[i].is_player_unit && initiative_order[j].is_player_unit) {
                    should_swap = true;
                }
            }

            if (should_swap) {
                InitiativeEntry temp = initiative_order[i];
                initiative_order[i] = initiative_order[j];
                initiative_order[j] = temp;
            }
        }
    }
}

void TurnManager::startNextTurn() {
    if (!combat_active) return;

    // End previous turn if there was one
    if (current_turn_index >= 0) {
        endCurrentTurn();
    }

    // Advance to next unit
    current_turn_index++;

    // Check if we need to advance round
    if (current_turn_index >= initiative_order.size()) {
        advanceRound();
        current_turn_index = 0;
    }

    // Reset turn state
    actions_remaining = 3;
    attacks_this_turn = 0;
    used_agile_weapon = false;

    UnitComponent* current_unit = getCurrentUnit();
    if (current_unit) {
        // Reset unit's turn-specific state
        resetTurnState(current_unit);

        Unigine::Log::message("\n=== TURN %d (Round %d) ===\n", current_turn_index + 1, current_round);
        Unigine::Log::message("Current unit: %s (%s)\n",
            current_unit->unit_name.get(),
            isPlayerTurn() ? "PLAYER" : "ENEMY");
        Unigine::Log::message("Actions: %d | Reaction: %s\n",
            actions_remaining,
            current_unit->has_reaction.get() ? "Yes" : "No");
    }
}

void TurnManager::endCurrentTurn() {
    UnitComponent* current_unit = getCurrentUnit();
    if (current_unit) {
        Unigine::Log::message("Ending turn for %s\n", current_unit->unit_name.get());

        // Apply end-of-turn effects (condition decrements, etc.)
        applyEndOfTurnEffects(current_unit);
    }
}

void TurnManager::advanceRound() {
    current_round++;
    Unigine::Log::message("\n========== ROUND %d ==========\n", current_round);
}

bool TurnManager::canSpendActions(int action_cost) const {
    return actions_remaining >= action_cost;
}

void TurnManager::spendActions(int action_cost, ActionType type) {
    if (!canSpendActions(action_cost)) {
        Unigine::Log::warning("TurnManager::spendActions() - Not enough actions! Need %d, have %d\n",
            action_cost, actions_remaining);
        return;
    }

    actions_remaining -= action_cost;

    // Track attacks for MAP calculation
    if (type == ActionType::STRIKE || type == ActionType::SPELL_ATTACK) {
        attacks_this_turn++;
        Unigine::Log::message("Attack #%d this turn (MAP: %d)\n", attacks_this_turn, getCurrentMAP());
    }

    Unigine::Log::message("Actions remaining: %d\n", actions_remaining);
}

int TurnManager::getCurrentMAP() const {
    // No penalty on first attack
    if (attacks_this_turn == 0) return 0;

    // Agile weapons: -4/-8
    if (used_agile_weapon) {
        if (attacks_this_turn == 1) return -4;
        return -8;
    }

    // Standard weapons: -5/-10
    if (attacks_this_turn == 1) return -5;
    return -10;
}

bool TurnManager::hasReaction() const {
    UnitComponent* current_unit = getCurrentUnit();
    return current_unit ? current_unit->has_reaction.get() : false;
}

void TurnManager::spendReaction() {
    UnitComponent* current_unit = getCurrentUnit();
    if (current_unit && current_unit->has_reaction.get()) {
        current_unit->has_reaction = false;
        Unigine::Log::message("Reaction spent\n");
    }
}

UnitComponent* TurnManager::getCurrentUnit() const {
    if (current_turn_index < 0 || current_turn_index >= initiative_order.size()) {
        return nullptr;
    }
    return initiative_order[current_turn_index].unit_component;
}

Unigine::NodePtr TurnManager::getCurrentUnitNode() const {
    if (current_turn_index < 0 || current_turn_index >= initiative_order.size()) {
        return nullptr;
    }
    return initiative_order[current_turn_index].unit_node;
}

bool TurnManager::isPlayerTurn() const {
    if (current_turn_index < 0 || current_turn_index >= initiative_order.size()) {
        return false;
    }
    return initiative_order[current_turn_index].is_player_unit;
}

void TurnManager::delayTurn() {
    // TODO: Implement delay action
    // Player chooses to act later in initiative order
    // Their new initiative = when they choose to act
    Unigine::Log::message("TurnManager::delayTurn() - Not yet implemented\n");
}

void TurnManager::insertDelayedUnit(int new_initiative_index) {
    // TODO: Implement inserting a delayed unit back into initiative order
    Unigine::Log::message("TurnManager::insertDelayedUnit() - Not yet implemented\n");
}

int TurnManager::rollInitiativeForUnit(UnitComponent* unit) const {
    // Initiative = 1d20 + Perception modifier
    // For now, use Wisdom modifier as Perception (PF2e default)
    int perception_mod = unit->getWisdomMod();

    // Use C++11 random for better quality RNG
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> d20(1, 20);

    int d20_roll = d20(gen);  // 1d20
    int initiative = d20_roll + perception_mod;

    // Store initiative in unit component
    unit->initiative = initiative;

    Unigine::Log::message("  %s: rolled %d + %d (Perception) = %d\n",
        unit->unit_name.get(), d20_roll, perception_mod, initiative);

    return initiative;
}

void TurnManager::applyEndOfTurnEffects(UnitComponent* unit) {
    // TODO: When condition system is implemented, decrement conditions here
    // - Frightened: Decrease by 1
    // - Stunned: Decrease by 1
    // - Slowed: Decrease by 1
    // - Persistent damage: Roll flat check to remove, or take damage

    // For now, just log
    Unigine::Log::message("  [End of turn effects applied]\n");
}

void TurnManager::resetTurnState(UnitComponent* unit) {
    // Reset action economy
    unit->actions_remaining = 3;

    // Refresh reaction (1 per round, refreshes at start of turn)
    unit->has_reaction = true;

    // Reset MAP tracking (will be tracked by TurnManager during turn)
    unit->current_map = 0;
}
