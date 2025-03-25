#ifndef OILREFINERY_H
#define OILREFINERY_H

#include "Workplace.h"
#include "../../resources/ResourceType.h"
#include "../../resources/Oil.h"

class OilRefinery : public Workplace {
    public:
        OilRefinery();
        OilRefinery(const sf::Vector2f& position);
        ~OilRefinery();
        
        // Get required resource type
        ResourceType getRequiredResourceType() const override { return ResourceType::OIL; }
        
        // Override the type identification from Workplace
        BuildingType getType() const override { return BuildingType::OilRefinery; }
};
#endif