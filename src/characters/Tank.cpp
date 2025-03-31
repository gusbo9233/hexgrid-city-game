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
    
    // Debug output to verify initialization
    std::cout << "Created tank at (" << q << "," << r << ") with range " << range << std::endl;
}


