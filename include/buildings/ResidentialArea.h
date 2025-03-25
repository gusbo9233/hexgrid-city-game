#ifndef RESIDENTIAL_AREA_H
#define RESIDENTIAL_AREA_H

#include "Building.h"
#include "../civilian/Household.h"

// ResidentialArea is a leaf class
class ResidentialArea final : public Building {
    public:
        ResidentialArea();
        ~ResidentialArea() override;
        
        // Implement type identification
        BuildingType getType() const override { return BuildingType::ResidentialArea; }
        
        // Override for custom behavior
        void update(float deltaTime) override;
        
    protected:
        // Building constants
        static constexpr float RESIDENTIAL_SCALE = 1.1f;
        static constexpr int RESIDENTIAL_VISIBILITY_RANGE = 3; // Residential areas have moderate visibility
        
        // Override for appearance customization
        float getScaleFactor() const override { return 1.0f; }
        sf::Vector2f getSize() const override { return {STANDARD_SIZE * RESIDENTIAL_SCALE, STANDARD_SIZE * RESIDENTIAL_SCALE}; }
        std::string getImagePath() const override { return "assets/images/ResidentialArea.png"; }
        
    private:
        std::vector<Household*> households;
};

#endif