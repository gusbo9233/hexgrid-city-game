#ifndef CITYCENTER_H
#define CITYCENTER_H

#include "Building.h"

// CityCenter is a leaf class that should not be further derived from
class CityCenter final : public Building {
public:
    CityCenter(const sf::Vector2f& position);
    ~CityCenter() override = default;
    
    // Implement type identification
    BuildingType getType() const override { return BuildingType::CityCenter; }
    
    // Override update method with custom behavior if needed
    void update(float deltaTime) override {}

protected:
    // Building constants
    static constexpr float CITYCENTER_SCALE = 1.2f;
    static constexpr int CITYCENTER_VISIBILITY_RANGE = 6; // City centers have excellent visibility
    
    // Building appearance customization
    float getScaleFactor() const override { return CITYCENTER_SCALE; } // Slightly larger than standard
    sf::Vector2f getSize() const override { return {STANDARD_SIZE * CITYCENTER_SCALE, STANDARD_SIZE * CITYCENTER_SCALE}; }
    std::string getImagePath() const override { return "assets/images/CityCenter.png"; }
};

#endif
