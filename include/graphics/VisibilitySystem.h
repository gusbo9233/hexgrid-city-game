#ifndef VISIBILITY_SYSTEM_H
#define VISIBILITY_SYSTEM_H

#include "HexGrid.h"
#include "../characters/Character.h"
#include "../buildings/Building.h"
#include "../buildings/City.h"
#include <vector>
#include <memory>

class VisibilitySystem {
public:
    // Default visibility range for generic buildings
    static constexpr int BUILDING_VISIBILITY_RANGE = 2;
    
    VisibilitySystem();
    
    // Update visibility based on entities
    void updateVisibility(HexGrid& grid, 
                          const std::vector<Character*>& characters,
                          const std::vector<Building*>& buildings,
                          const std::vector<City*>& cities);
    
    // Set hexes visible in a radius around a coordinate
    void setHexesVisibleAroundEntity(HexGrid& grid, const Hexagon::CubeCoord& center, int range);
    
    // Reset visibility for debugging/testing
    void resetAllVisibility(HexGrid& grid);
    
private:
    // Get appropriate visibility range based on building type (deprecated)
    int getBuildingVisibilityRange(BuildingType type) const;
};

#endif // VISIBILITY_SYSTEM_H 