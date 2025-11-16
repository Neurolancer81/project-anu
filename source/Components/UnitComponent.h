// UnitComponent.h
// Unigine Component for units (player and enemy)
// Exposes PF2e stats to the Editor

#pragma once

#include <UnigineComponentSystem.h>
#include "../Grid/GridCell.h"

class UnitComponent : public Unigine::ComponentBase
{
public:
    COMPONENT_DEFINE(UnitComponent, Unigine::ComponentBase);
    COMPONENT_INIT(init);
    COMPONENT_UPDATE(update);
    COMPONENT_SHUTDOWN(shutdown);

    // Unit identification (exposed to Editor)
    PROP_PARAM(String, unit_id);
    PROP_PARAM(String, unit_name);
    PROP_PARAM(Int, level, 1);

    // Combat stats
    PROP_PARAM(Int, max_hp, 10);
    PROP_PARAM(Int, current_hp, 10);
    PROP_PARAM(Int, armor_class, 10);
    PROP_PARAM(Int, speed, 25); // PF2e Speed in feet (25 = 5 squares)

    // Ability scores (PF2e)
    PROP_PARAM(Int, strength, 10);
    PROP_PARAM(Int, dexterity, 10);
    PROP_PARAM(Int, constitution, 10);
    PROP_PARAM(Int, intelligence, 10);
    PROP_PARAM(Int, wisdom, 10);
    PROP_PARAM(Int, charisma, 10);

    // Saves
    PROP_PARAM(Int, fortitude_save, 0);
    PROP_PARAM(Int, reflex_save, 0);
    PROP_PARAM(Int, will_save, 0);

    // Attack stats
    PROP_PARAM(Int, attack_bonus, 0);
    PROP_PARAM(String, weapon_damage, "1d6+2"); // Format: XdY+Z

    // Grid position (not exposed to editor, set by code)
    GridPosition grid_position;

    // Methods
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive() const { return current_hp > 0; }

    // Ability modifiers (PF2e: (score - 10) / 2)
    int getStrengthMod() const { return (strength - 10) / 2; }
    int getDexterityMod() const { return (dexterity - 10) / 2; }
    int getConstitutionMod() const { return (constitution - 10) / 2; }
    int getIntelligenceMod() const { return (intelligence - 10) / 2; }
    int getWisdomMod() const { return (wisdom - 10) / 2; }
    int getCharismaMod() const { return (charisma - 10) / 2; }

protected:
    void init();
    void update();
    void shutdown();
};
