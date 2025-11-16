// UnitComponent.cpp
#include "UnitComponent.h"
#include <UnigineLog.h>

// Register component with Unigine (automatic registration)
REGISTER_COMPONENT(UnitComponent);

void UnitComponent::init() {
    // Initialize unit
    Unigine::Log::message("UnitComponent::init() - Unit '%s' (Level %d) initialized\n",
        unit_name.get(), level.get());

    // Initialize grid position to origin (will be set by game logic)
    grid_position = GridPosition(0, 0, 0);

    // Ensure current_hp doesn't exceed max_hp
    if (current_hp > max_hp) {
        current_hp = max_hp;
    }
}

void UnitComponent::update() {
    // Per-frame update (animations, visual feedback, etc.)
    // Game logic is handled in GameManager::update()
}

void UnitComponent::shutdown() {
    Unigine::Log::message("UnitComponent::shutdown() - Unit '%s' destroyed\n", unit_name.get());
}

void UnitComponent::takeDamage(int amount) {
    if (amount <= 0) return;

    current_hp = current_hp - amount;
    if (current_hp < 0) {
        current_hp = 0;
    }

    Unigine::Log::message("Unit '%s' took %d damage (HP: %d/%d)\n",
        unit_name.get(), amount, current_hp.get(), max_hp.get());

    if (!isAlive()) {
        Unigine::Log::message("Unit '%s' has been defeated!\n", unit_name.get());
    }
}

void UnitComponent::heal(int amount) {
    if (amount <= 0) return;

    current_hp = current_hp + amount;
    if (current_hp > max_hp) {
        current_hp = max_hp;
    }

    Unigine::Log::message("Unit '%s' healed %d HP (HP: %d/%d)\n",
        unit_name.get(), amount, current_hp.get(), max_hp.get());
}
