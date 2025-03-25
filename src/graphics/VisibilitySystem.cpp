#include "../../include/graphics/VisibilitySystem.h"

VisibilitySystem::VisibilitySystem() {
    // Constructor (currently empty)
}

void VisibilitySystem::updateVisibility(HexGrid& grid, 
                                         const std::vector<Character*>& characters,
                                         const std::vector<Building*>& buildings,
                                         const std::vector<City*>& cities) {
    // First reset all visibility
    grid.resetVisibility();
    
    // Set hexes visible around friendly characters only
    for (const auto& character : characters) {
        if (character && character->getAllegiance() == Allegiance::FRIENDLY) {
            // Use the character's own visibility range
            setHexesVisibleAroundEntity(grid, character->getHexCoord(), character->getVisibilityRange());
        }
    }
    
    // Set hexes visible around friendly buildings only
    for (const auto& building : buildings) {
        if (building && building->getAllegiance() == Allegiance::FRIENDLY) {
            // Convert pixel position to hex coordinate
            Hexagon::CubeCoord buildingCoord = grid.pixelToCube(building->getPosition());
            
            // Use the building's own visibility range
            setHexesVisibleAroundEntity(grid, buildingCoord, building->getVisibilityRange());
        }
    }
    
    // Set hexes visible around friendly cities only
    for (const auto& city : cities) {
        if (city && city->getAllegiance() == Allegiance::FRIENDLY) {
            // Convert pixel position to hex coordinate
            Hexagon::CubeCoord cityCoord = grid.pixelToCube(city->getPosition());
            
            // Use the city's own visibility range - note: this was incomplete in original code
            // We'll assume cities have a default visibility range of 2 (can adjust if needed)
            setHexesVisibleAroundEntity(grid, cityCoord, 2);
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
            return 3; // City centers provide good visibility
        case BuildingType::ResidentialArea:
            return 2; // Standard visibility
        case BuildingType::Base:
            return 3; // Military bases have better visibility
        default:
            return BUILDING_VISIBILITY_RANGE;
    }
} 