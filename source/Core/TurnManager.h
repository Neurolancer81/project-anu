// TurnManager.h
// Manages PF2e turn-based combat: initiative, turn order, 3-action economy, MAP, reactions
// Implements the rules from GDD/01-Core-Mechanics/Turn-System.md

#pragma once

#include <UnigineVector.h>
#include <UniginePtr.h>

namespace Unigine {
    class Node;
}

class UnitComponent;

// Represents a unit in the initiative order
struct InitiativeEntry {
    Unigine::NodePtr unit_node;     // The unit's scene node
    UnitComponent* unit_component;  // Cached pointer to UnitComponent
    int initiative_value;           // Initiative roll result (Perception + 1d20)
    bool is_player_unit;            // Player units win ties

    InitiativeEntry()
        : unit_node(nullptr)
        , unit_component(nullptr)
        , initiative_value(0)
        , is_player_unit(false)
    {}
};

// Action types for tracking MAP
enum class ActionType {
    NONE,           // Not an attack action
    STRIKE,         // Standard Strike (triggers MAP)
    SPELL_ATTACK,   // Spell with attack roll (triggers MAP)
    SPELL_SAVE,     // Spell with save (does NOT trigger MAP)
    ABILITY         // Class ability (may or may not trigger MAP)
};

class TurnManager {
public:
    TurnManager();
    ~TurnManager();

    // Combat lifecycle
    void startCombat(const Unigine::Vector<Unigine::NodePtr>& player_units,
                     const Unigine::Vector<Unigine::NodePtr>& enemy_units);
    void endCombat();
    bool isCombatActive() const { return combat_active; }

    // Initiative & turn order
    void rollInitiative(const Unigine::Vector<Unigine::NodePtr>& player_units,
                        const Unigine::Vector<Unigine::NodePtr>& enemy_units);
    void sortInitiativeOrder();

    // Turn progression
    void startNextTurn();
    void endCurrentTurn();
    void advanceRound();

    // Action economy (3 actions per turn)
    bool canSpendActions(int action_cost) const;
    void spendActions(int action_cost, ActionType type = ActionType::NONE);
    int getActionsRemaining() const { return actions_remaining; }
    int getCurrentMAP() const;

    // Reactions (1 per round, refreshes at turn start)
    bool hasReaction() const;
    void spendReaction();

    // Current turn queries
    UnitComponent* getCurrentUnit() const;
    Unigine::NodePtr getCurrentUnitNode() const;
    int getCurrentTurnIndex() const { return current_turn_index; }
    int getCurrentRound() const { return current_round; }
    bool isPlayerTurn() const;

    // Initiative order queries
    int getInitiativeCount() const { return initiative_order.size(); }
    const InitiativeEntry& getInitiativeEntry(int index) const { return initiative_order[index]; }

    // Delay action (free action, changes initiative)
    void delayTurn();
    void insertDelayedUnit(int new_initiative_index);

private:
    bool combat_active;
    int current_round;
    int current_turn_index;     // Index into initiative_order
    int actions_remaining;      // 3 actions per turn
    int attacks_this_turn;      // For MAP calculation
    bool used_agile_weapon;     // Agile weapons have reduced MAP (-4/-8 instead of -5/-10)

    Unigine::Vector<InitiativeEntry> initiative_order;  // Sorted highest to lowest

    // Helper: Get initiative value for a unit (Perception + 1d20)
    int rollInitiativeForUnit(UnitComponent* unit) const;

    // Helper: Apply end-of-turn effects (decrement conditions, etc.)
    void applyEndOfTurnEffects(UnitComponent* unit);

    // Helper: Reset turn-specific state for a unit
    void resetTurnState(UnitComponent* unit);
};
