// GameManager.cpp
#include "GameManager.h"

#include <UnigineLog.h>
#include <UnigineWorld.h>
#include <UnigineComponentSystem.h>

// System includes
#include "Grid/GridSystem.h"
#include "Core/TurnManager.h"
#include "UI/GridRenderer.h"
#include "Components/GridConfigComponent.h"
// #include "Combat/CombatResolver.h"
// #include "Spells/SpellSystem.h"

GameManager::GameManager()
    : grid(nullptr)
    , turn_manager(nullptr)
    , combat(nullptr)
    , spells(nullptr)
    , grid_renderer(nullptr)
    , in_combat(false)
{
}

GameManager::~GameManager() {
    shutdown();
}

void GameManager::init() {
    Unigine::Log::message("GameManager::init() - Initializing game systems...\n");

    // Find the GridConfig node in the world
    Unigine::NodePtr config_node = Unigine::World::getNodeByName("GridConfig");
    if (!config_node) {
        Unigine::Log::error("GameManager::init() - GridConfig node not found! Create a dummy node named 'GridConfig' with GridConfigComponent in the Editor.\n");
        return;
    }

    // Get the GridConfigComponent
    GridConfigComponent* grid_config = Unigine::ComponentSystem::get()->getComponent<GridConfigComponent>(config_node);
    if (!grid_config) {
        Unigine::Log::error("GameManager::init() - GridConfigComponent not found on GridConfig node!\n");
        return;
    }

    Unigine::Log::message("GameManager::init() - Found GridConfig: %dx%d grid, %.2fm cells\n",
        grid_config->grid_width, grid_config->grid_height, grid_config->cell_size);

    // Create grid system with dimensions from config
    grid = new GridSystem(grid_config->grid_width, grid_config->grid_height);

    // Create turn manager
    turn_manager = new TurnManager();

    // Create grid renderer with config and visualize the grid
    grid_renderer = new GridRenderer(grid, grid_config);
    grid_renderer->createGridVisuals();

    // Create other systems (will be implemented as we build them)
    // combat = new CombatResolver();
    // spells = new SpellSystem();

    Unigine::Log::message("GameManager::init() - Complete\n");
}

void GameManager::shutdown() {
    Unigine::Log::message("GameManager::shutdown() - Cleaning up game systems...\n");

    // Delete systems in reverse order
    delete grid_renderer;
    // delete spells;        // Not created yet
    // delete combat;        // Not created yet
    delete turn_manager;
    delete grid;

    // Reset pointers
    grid_renderer = nullptr;
    spells = nullptr;
    combat = nullptr;
    turn_manager = nullptr;
    grid = nullptr;

    Unigine::Log::message("GameManager::shutdown() - Complete\n");
}

void GameManager::update(float dt) {
    if (!in_combat) return;

    // Update game logic (called at fixed 60 FPS from AppWorldLogic::updatePhysics)
    // TODO: Update turn system
    // TODO: Update AI
    // TODO: Update combat state
}

void GameManager::handleInput() {
    // Process player input (called per-frame from AppWorldLogic::update)
    // TODO: Mouse clicks for unit selection
    // TODO: Keyboard input
    // TODO: UI button callbacks
}

void GameManager::startCombat() {
    Unigine::Log::message("GameManager::startCombat() - Starting combat encounter\n");
    in_combat = true;

    // TODO: Get actual player and enemy units from the scene
    // For now, just log that turn manager is ready
    // turn_manager->startCombat(player_units, enemy_units);
}

void GameManager::endCombat() {
    Unigine::Log::message("GameManager::endCombat() - Ending combat encounter\n");
    in_combat = false;

    if (turn_manager) {
        turn_manager->endCombat();
    }

    // TODO: Show victory/defeat screen
}
