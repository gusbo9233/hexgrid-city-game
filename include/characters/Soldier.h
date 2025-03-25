#ifndef SOLDIER_H
#define SOLDIER_H

#include "Character.h"

// Soldier is a leaf class that should not be derived from further
class Soldier final : public Character {
public:
    Soldier(int q, int r);
    
    // Implement type identification
    CharacterType getType() const override { return CharacterType::Soldier; }
    
    // Override update method with soldier-specific behavior
    void update(float deltaTime) override {}
    
    // Soldier-specific methods
    void attack(Character* target);
    void move(Hexagon* targetHex);

protected:
    // Define soldier-specific constants
    static constexpr float SOLDIER_SIZE = 15.0f;  // Soldiers are 15 units in size
    static constexpr float SOLDIER_SCALE = 1.0f;  // No additional scaling
    static constexpr const char* SOLDIER_TEXTURE = "assets/images/soldier.png";
    static constexpr int SOLDIER_VISIBILITY_RANGE = 1;  // Soldiers can only see adjacent hexes
    
    // Override size methods
    float getScaleFactor() const override { return SOLDIER_SCALE; }
    sf::Vector2f getSize() const override { return {SOLDIER_SIZE, SOLDIER_SIZE}; }
    
    // Optional: Override doRender for soldier-specific rendering
};

#endif 