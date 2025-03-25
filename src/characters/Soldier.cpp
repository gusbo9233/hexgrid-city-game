#include "../../include/characters/Soldier.h"
#include <iostream>

Soldier::Soldier(int q, int r) : Character(q, r) {
    // Load texture from file
    loadTexture(SOLDIER_TEXTURE);
    setVisibilityRange(SOLDIER_VISIBILITY_RANGE);
}

// Basic attack implementation
void Soldier::attack(Character* target) {
    if (!target) return;
    
    // Simple attack logic - could be expanded
    std::cout << "Soldier attacks!" << std::endl;
    
    // Could implement actual attack logic here
}

// Basic move implementation 
void Soldier::move(Hexagon* targetHex) {
    if (!targetHex) return;
    
    // Update hex coordinates
    setHexCoord(targetHex->getCoord());
    
    // Update position
    setPosition(targetHex->getPosition());
} 