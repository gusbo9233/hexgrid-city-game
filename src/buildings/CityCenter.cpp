#include "CityCenter.h"

CityCenter::CityCenter(const sf::Vector2f& position) : Building(position) {
    // Initialize the building shape with our custom properties
    initializeShape();
}
