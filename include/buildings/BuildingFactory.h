#ifndef BUILDING_FACTORY_H
#define BUILDING_FACTORY_H

#include "Building.h"
#include <unordered_map>
#include <string>
#include "Hexagon.h"

// Factory class for creating buildings
class BuildingFactory {
    public:
        static Building* createBuilding(const std::string& type, Hexagon* hex);

    private:
        
};

#endif