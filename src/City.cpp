#include "../include/buildings/City.h"
#include "../include/buildings/CityCenter.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include "../include/buildings/ResidentialArea.h"
City::City(std::vector<Hexagon*> cityHexes, Allegiance allegiance) 
    : cityHexes(cityHexes), mAllegiance(allegiance)
{
    generateBuildings();
}

City::~City() {
    // Smart pointers handle cleanup automatically
}

void City::generateBuildings() {
    if (cityHexes.empty()) {
        return; // No hexes in the city
    }

    // Find the middlemost hex by calculating the "center of mass"
    // First calculate the average coordinates
    int sumQ = 0, sumR = 0, sumS = 0;
    for (const auto* hex : cityHexes) {
        Hexagon::CubeCoord coord = hex->getCoord();
        sumQ += coord.q;
        sumR += coord.r;
        sumS += coord.s;
    }
    
    float avgQ = static_cast<float>(sumQ) / cityHexes.size();
    float avgR = static_cast<float>(sumR) / cityHexes.size();
    float avgS = static_cast<float>(sumS) / cityHexes.size();
    
    // Find the hex closest to the average
    Hexagon* centerHex = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    for (auto* hex : cityHexes) {
        Hexagon::CubeCoord coord = hex->getCoord();
        float distance = std::sqrt(
            std::pow(coord.q - avgQ, 2) +
            std::pow(coord.r - avgR, 2) + 
            std::pow(coord.s - avgS, 2)
        );
        
        if (distance < minDistance) {
            minDistance = distance;
            centerHex = hex;
        }
    }
    
    // Place a CityCenter on the center hex
    if (centerHex) {
        // Create a new CityCenter with a unique_ptr
        auto cityCenter = std::make_unique<CityCenter>(centerHex->getPosition(), mAllegiance);
        
        // Add the building to the hex - need raw pointer for this API
        centerHex->setBuilding(cityCenter.get());
        
        // Store the building in our map, transferring ownership
        buildings[centerHex->getCoord()] = std::move(cityCenter);
    }
    
    // Get residential areas around the city center
    if (centerHex && cityHexes.size() > 1) {
        // Find a suitable hex for a residential area
        for (auto* hex : cityHexes) {
            // Skip the center hex where we already placed the city center
            if (hex == centerHex) continue;
            
            // Skip hexes that already have buildings
            if (hex->hasBuilding()) continue;
            
            // Create a residential area
            auto residentialArea = std::make_unique<ResidentialArea>(hex->getPosition(), mAllegiance);
            
            // Add the building to the hex
            hex->setBuilding(residentialArea.get());
            
            // Store the building in our map
            buildings[hex->getCoord()] = std::move(residentialArea);
            
            break; // Just add one residential area for now
        }
    }
}