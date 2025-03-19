#include "VisibilitySystem.h"

VisibilitySystem::VisibilitySystem() {
    // Constructor (currently empty)
}

void VisibilitySystem::updateVisibility(HexGrid& grid, 
                                         const std::vector<Character*>& characters,
                                         const std::vector<Building*>& buildings,
                                         const std::vector<City*>& cities) {
    // First reset all visibility
    grid.resetVisibility();
    
    // Set hexes visible around characters
    for (const auto& character : characters) {
        if (character) {
            // Use the character's own visibility range
            setHexesVisibleAroundEntity(grid, character->getHexCoord(), character->getVisibilityRange());
        }
    }
    
    // Set hexes visible around buildings
    for (const auto& building : buildings) {
        if (building) {
            // Convert pixel position to hex coordinate
            Hexagon::CubeCoord buildingCoord = grid.pixelToCube(building->getPosition());
            
            // Use the building's own visibility range
            setHexesVisibleAroundEntity(grid, buildingCoord, building->getVisibilityRange());
        }
    }
    
    // Set hexes visible around cities
    for (const auto& city : cities) {
        if (city) {
            // Convert pixel position to hex coordinate
            Hexagon::CubeCoord cityCoord = grid.pixelToCube(city->getPosition());
            
            // Use the city's own visibility range
            setHexesVisibleAroundEntity(grid, cityCoord, city->getVisibilityRange());
        }
    }
}

void VisibilitySystem::setHexesVisibleAroundEntity(HexGrid& grid, const Hexagon::CubeCoord& center, int range) {
    // Get all hexes within range
    auto hexesInRange = grid.getHexesInRange(center, range);
    
    // Set them all visible only - no longer tracking explored state
    for (auto hex : hexesInRange) {
        hex->setVisible(true);
        // No longer setting explored state
    }
}

void VisibilitySystem::resetAllVisibility(HexGrid& grid) {
    for (auto hex : grid.getAllHexes()) {
        hex->setVisible(false);
        // No longer resetting explored state
    }
}

// This method is no longer needed since we're using entity-specific visibility ranges
int VisibilitySystem::getBuildingVisibilityRange(BuildingType type) const {
    switch (type) {
        case BuildingType::CityCenter:
            return 4; // City centers provide good visibility
        case BuildingType::ResidentialArea:
            return 3; // Standard visibility
        case BuildingType::Base:
            return 5; // Military bases have better visibility
        default:
            return BUILDING_VISIBILITY_RANGE;
    }
} 