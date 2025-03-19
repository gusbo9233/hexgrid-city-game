#ifndef SOLDIER_H
#define SOLDIER_H

#include "Character.h"

class Soldier : public Character {
public:
    Soldier(int x, int y);
    
    // Optional: Add specific methods for Soldier if needed
    // void attack(Character* target);
    // void move(Hexagon* targetHex);

protected:
    // Define soldier-specific constants
    static constexpr float SOLDIER_SIZE = 15.0f;  // Soldiers are 15 units in size
    static constexpr float SOLDIER_SCALE = 1.0f;  // No additional scaling
    static constexpr const char* SOLDIER_TEXTURE = "assets/images/soldier.png";
    
    // Override size methods
    float getScaleFactor() const override { return SOLDIER_SCALE; }
    sf::Vector2f getSize() const override { return {SOLDIER_SIZE, SOLDIER_SIZE}; }
};

#endif 