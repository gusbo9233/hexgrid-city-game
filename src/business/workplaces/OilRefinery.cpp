#include "../../../include/business/workplaces/OilRefinery.h"
#include "resources/Oil.h"

OilRefinery::OilRefinery(const sf::Vector2f& position)
    : Workplace(position) {
    // Don't use Oil type directly, just use the ResourceType
    // requiredHexResource = new Oil();
    
    // Initialize the oil refinery with default values
}

OilRefinery::~OilRefinery() {
    // Clean up resources if needed
    // delete requiredHexResource;
}
