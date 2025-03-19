#include "../../include/BuildingFactory.h"
#include "../../include/Base.h"
#include "../../include/HexGrid.h"

// We need access to the grid to find the hex by coordinates
Building* BuildingFactory::createBuilding(const std::string& type, Hexagon* hex) {
    if (!hex) return nullptr;
    
    if (type == "Base") {
        return new Base(hex);
    }
    return nullptr;
}
