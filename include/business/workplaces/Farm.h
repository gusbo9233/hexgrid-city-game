#ifndef FARM_H
#define FARM_H

#include "Workplace.h"

class Farm : public Workplace {
    public:
        Farm();
        Farm(const sf::Vector2f& position);
        ~Farm();
        
        // Override Building methods
        BuildingType getType() const override { return BuildingType::Farm; }
        float getScaleFactor() const override { return 0.8f; }
        std::string getImagePath() const override { return "assets/images/farm.png"; }
        Product getProductType() const override { return Product::FOOD; }
        // Farm-specific methods
        void generateProduct();
};

#endif