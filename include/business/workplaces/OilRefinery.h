#ifndef OILREFINERY_H
#define OILREFINERY_H

#include "Workplace.h"
#include "../../resources/ResourceType.h"
#include "../../resources/Oil.h"

class OilRefinery : public Workplace {
    public:
        OilRefinery() : Workplace() {
            // Initialize the oil refinery with default values
            setVisibilityRange(3); // Oil refineries can see further
            initializeShape();
        }
        OilRefinery(const sf::Vector2f& position);
        ~OilRefinery();
        
        // Get required resource type
        ResourceType getRequiredResourceType() const override { return ResourceType::OIL; }
        
        // Override the type identification from Workplace
        BuildingType getType() const override { return BuildingType::OilRefinery; }
        Product getProductType() const override { return Product::RefinedOil; }
        std::string getImagePath() const override { return "assets/images/OilRefinery.png"; }
        
        // Implement the pure virtual methods from Building
        float getScaleFactor() const override;
        sf::Vector2f getSize() const override;
};
#endif