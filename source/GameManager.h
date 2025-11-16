// GameManager.h
// Central game manager - owns all game systems
// Keeps AppWorldLogic clean by encapsulating all game logic

#pragma once

// Forward declarations (full includes in .cpp)
class GridSystem;
class TurnManager;
class CombatResolver;
class SpellSystem;
class GridRenderer;

class GameManager {
public:
    GameManager();
    ~GameManager();

    // Lifecycle
    void init();
    void shutdown();

    // Update loops
    void update(float dt);        // Game logic (called from AppWorldLogic::updatePhysics)
    void handleInput();           // Input handling (called from AppWorldLogic::update)

    // Systems (public for access from UI, debug, Components)
    GridSystem* grid;
    TurnManager* turn_manager;
    CombatResolver* combat;
    SpellSystem* spells;
    GridRenderer* grid_renderer;

    // Game state
    bool isInCombat() const { return in_combat; }
    void startCombat();
    void endCombat();

private:
    bool in_combat;

    // Prevent copying
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
};
