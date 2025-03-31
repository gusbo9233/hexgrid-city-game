#include "../../include/characters/Soldier.h"
#include <iostream>

Soldier::Soldier(int q, int r) : Character(q, r) {
    // Load texture from file
    loadTexture(SOLDIER_TEXTURE);
    
    // Set soldier-specific properties
    setVisibilityRange(SOLDIER_VISIBILITY_RANGE);
    mProjectileType = ProjectileType::BULLET;
    shootCooldown = 60;  // Shorter cooldown for faster shooting
    mTraversableTerrain = {TerrainType::PLAINS, TerrainType::URBAN, TerrainType::FOREST};
    
    // Set combat properties
    range = 1;  // Increase range to allow targeting from further away
    attackPower = 15;
    
    // Debug output to verify initialization
    std::cout << "Created soldier at (" << q << "," << r << ") with range " << range << std::endl;
} 