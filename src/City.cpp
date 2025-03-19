#include "../include/City.h"

City::City(std::vector<Hexagon*> cityHexes, const sf::Color& cityColor) 
    : cityHexes(cityHexes), color(cityColor) {
    generateBuildings();
    updateHexColors();
}

City::~City() {
    // Clean up any buildings if necessary
    for (auto& [coord, building] : buildings) {
        if (building) {
            delete building;
        }
    }
    buildings.clear();
}

void City::updateHexColors() {
    for (auto* hex : cityHexes) {
        if (hex) {
            hex->setBaseColor(color);
        }
    }
}

void City::generateBuildings() {
    // Will be implemented later to place buildings in the city
    updateHexColors(); // Ensure colors are maintained after generating buildings
}