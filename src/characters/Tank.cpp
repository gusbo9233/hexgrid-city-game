#include "../../include/characters/Tank.h"
#include <iostream>

Tank::Tank(int q, int r) : Character(q, r) {
    // Load texture from file
    loadTexture(TANK_TEXTURE);
    
    // Set tank-specific properties
    setVisibilityRange(TANK_VISIBILITY_RANGE);
    mProjectileType = ProjectileType::TANK_AMMO;
    shootCooldown = 100;  // Longer cooldown for balance
    
    // Set combat properties
    range = 2;  // Tanks can shoot from further away
    attackPower = 30;  // Higher attack power
    health = 150;      // More health than soldiers
    maxHealth = 150;
    
    // Tanks can only traverse plains and urban terrain (not forest or water)
    mTraversableTerrain.clear();  // Clear default terrains from Character constructor
    mTraversableTerrain.push_back(TerrainType::PLAINS);
    mTraversableTerrain.push_back(TerrainType::URBAN);
    
    // Debug to verify traversable terrain is set correctly
    // std::cout << "Tank traversable terrain count: " << mTraversableTerrain.size() << std::endl;
    // std::cout << "Tank traversable terrain types: ";
    // for (const auto& terrain : mTraversableTerrain) {
    //     std::cout << static_cast<int>(terrain) << " ";
    // }
    // std::cout << std::endl;
    
    // Debug output to verify initialization
    // std::cout << "Created tank at (" << q << "," << r << ") with range " << range << std::endl;
    // std::cout << "Tank can only traverse PLAINS and URBAN terrain (not FOREST or WATER)" << std::endl;
}


