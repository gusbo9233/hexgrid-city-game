#include "../../include/Soldier.h"
#include <iostream>

Soldier::Soldier(int q, int r) : Character(q, r) {
    loadTexture(SOLDIER_TEXTURE);
    setVisibilityRange(SOLDIER_VISIBILITY_RANGE);
}

// Implement doRender if needed with soldier-specific rendering
void Soldier::doRender(sf::RenderWindow& window) const {
    // Call the base implementation which handles sprite drawing
    Character::doRender(window);
    
    // Add soldier-specific rendering here if needed
    // For example, could draw health bar, status effects, etc.
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