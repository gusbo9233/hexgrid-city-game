#ifndef BASE_H
#define BASE_H
//Base is a building that is always on the hexagon
#include "Building.h"
#include "Hexagon.h"

class Base final : public Building {
    public:
        Base(Hexagon* hexagon);
        ~Base() override;
        void update(float deltaTime) override;
        
        // Implement type identification
        BuildingType getType() const override { return BuildingType::Base; }
        
    protected:
        // Building constants
        static constexpr float BASE_SIZE = 40.0f;
        static constexpr int BASE_VISIBILITY_RANGE = 7; // Military bases have excellent visibility
        
        // Define base-specific size
        float getScaleFactor() const override { return 1.0f; }
        sf::Vector2f getSize() const override { return {BASE_SIZE, BASE_SIZE}; }
        std::string getImagePath() const override { return "assets/images/Base.png"; }
        
    private:
        Hexagon* mHexagon;
};
#endif
