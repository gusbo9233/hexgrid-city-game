#include "../../../include/business/workplaces/OilRefinery.h"
#include "../../../include/resources/Oil.h"
#include <iostream> // Add for debug output

OilRefinery::OilRefinery(const sf::Vector2f& position)
    : Workplace(position) {
    // Debug output
    std::cout << "OilRefinery constructor called at position (" 
              << position.x << ", " << position.y << ")" << std::endl;
              
    // Don't use Oil type directly, just use the ResourceType
    // requiredHexResource = new Oil();
    
    // Initialize the oil refinery with default values
    setVisibilityRange(3); // Oil refineries can see further
    
    // Initialize shape
    initializeShape();
}

OilRefinery::~OilRefinery() {
    // Clean up resources if needed
    // delete requiredHexResource;
}

float OilRefinery::getScaleFactor() const {
    return 0.8f; // Scale factor for the oil refinery sprite
}

sf::Vector2f OilRefinery::getSize() const {
    return sf::Vector2f(35.0f, 35.0f); // Custom size for oil refinery
}
