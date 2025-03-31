#include "../../include/buildings/ResidentialArea.h"

ResidentialArea::ResidentialArea() : Building({0, 0}) {
    // Initialize the residential area with default values
    initializeShape();
    setVisibilityRange(RESIDENTIAL_VISIBILITY_RANGE);
}

// Add implementation for the constructor with position and allegiance
ResidentialArea::ResidentialArea(const sf::Vector2f& position, Allegiance allegiance)
    : Building(position, allegiance) {
    // Initialize the residential area
    initializeShape();
    setVisibilityRange(RESIDENTIAL_VISIBILITY_RANGE);
}

ResidentialArea::~ResidentialArea() {
    // Clean up households if needed
}

void ResidentialArea::update(float deltaTime) {
    // Update residential area simulation
    // For example, could update households, population, etc.
}
    