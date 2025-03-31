#ifndef TANK_H
#define TANK_H

#include "Character.h"

// Tank is a leaf class that should not be derived from further
class Tank final : public Character {
public:
    Tank(int q, int r);
    
    // Implement type identification
    CharacterType getType() const override { return CharacterType::Tank; }
    
    // Override update method with tank-specific behavior
    void update(float deltaTime) override {}

protected:
    // Define tank-specific constants
    static constexpr float TANK_SIZE = 30.0f;  // Tanks are larger than soldiers
    static constexpr float TANK_SCALE = 1.5f;  // Slightly larger scale
    static constexpr const char* TANK_TEXTURE = "assets/images/tank.png";
    static constexpr int TANK_VISIBILITY_RANGE = 2;  // Tanks can see further than soldiers
    
    // Override size methods
    float getScaleFactor() const override { return TANK_SCALE; }
    sf::Vector2f getSize() const override { return {TANK_SIZE, TANK_SIZE}; }
    
    // Optional: Override doRender for tank-specific rendering
};

#endif

