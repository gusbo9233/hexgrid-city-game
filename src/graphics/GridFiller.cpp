#include "../../include/graphics/GridFiller.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "../../include/buildings/City.h"
#include "../../include/resources/Oil.h"
#include "../../include/Allegiance.h"

GridFiller::GridFiller(HexGrid& grid) : mGrid(grid) {
    std::cout << "GridFiller constructor" << std::endl;
    // No additional initialization needed
}

GridFiller::~GridFiller() {
    std::cout << "GridFiller destructor" << std::endl;
}

void GridFiller::fillGrid() {
    std::cout << "GridFiller::fillGrid() start" << std::endl;
    
    // Generate friendly cities in bottom half
    generateCities(Allegiance::FRIENDLY);
    
    // Generate enemy cities in top half
    generateCities(Allegiance::ENEMY);
    
    // Generate resources around cities
    generateResources();
    
    std::cout << "GridFiller::fillGrid() complete, created " << mCities.size() << " cities" << std::endl;
}

std::vector<std::unique_ptr<City>> GridFiller::getCities() {
    // Transfer ownership of cities to the caller
    return std::move(mCities);
}

std::vector<std::unique_ptr<Resource>> GridFiller::getResources() {
    // Transfer ownership of resources to the caller
    return std::move(mResources);
}

void GridFiller::generateCities(Allegiance allegiance) {
    std::string side = (allegiance == Allegiance::FRIENDLY) ? "friendly (bottom)" : "enemy (top)";
    std::cout << "GridFiller::generateCities() start for " << side << " cities" << std::endl;
    
    // Define vectors to store hexes for each city
    std::vector<Hexagon*> city1Hexes, city2Hexes, city3Hexes;
    
    // Define city sizes
    const int CITY1_SIZE = 7; // Large city
    const int CITY2_SIZE = 5; // Medium city
    const int CITY3_SIZE = 6; // Another medium-large city
    
    // Seed positions for cities based on allegiance
    Hexagon::CubeCoord city1Seed(0, 0, 0);
    Hexagon::CubeCoord city2Seed(0, 0, 0);
    Hexagon::CubeCoord city3Seed(0, 0, 0);
    
    if (allegiance == Allegiance::FRIENDLY) {
        // Bottom half (positive r) for friendly cities
        city1Seed = Hexagon::CubeCoord(-4, 8, -4);   // Bottom left
        city2Seed = Hexagon::CubeCoord(4, 6, -10);   // Bottom right
        city3Seed = Hexagon::CubeCoord(0, 10, -10);  // Bottom center
    } else {
        // Top half (negative r) for enemy cities - mirror of friendly positions
        city1Seed = Hexagon::CubeCoord(4, -8, 4);    // Top right
        city2Seed = Hexagon::CubeCoord(-4, -6, 10);  // Top left
        city3Seed = Hexagon::CubeCoord(0, -10, 10);  // Top center
    }
    
    // Helper lambda to grow a city from a seed hex
    auto growCity = [this](const Hexagon::CubeCoord& seedCoord, int size) -> std::vector<Hexagon*> {
        std::vector<Hexagon*> cityHexes;
        std::vector<Hexagon*> frontier;
        std::vector<Hexagon::CubeCoord> visited;
        
        // Start with the seed hex
        Hexagon* seedHex = mGrid.getHexAt(seedCoord);
        if (!seedHex) return cityHexes;
        
        frontier.push_back(seedHex);
        visited.push_back(seedCoord);
        
        // Grow the city using breadth-first approach
        while (!frontier.empty() && cityHexes.size() < size) {
            Hexagon* current = frontier.front();
            frontier.erase(frontier.begin());
            
            // Skip if the hex already has content
            if (current && !current->hasBuilding() && !current->hasCharacter() && !current->hasResource()) {
                cityHexes.push_back(current);
                
                // Add adjacent hexes to frontier
                for (const auto& neighborCoord : mGrid.getAdjacentHexes(current->getCoord())) {
                    // Check if we've already visited this hex
                    if (std::find(visited.begin(), visited.end(), neighborCoord) == visited.end()) {
                        Hexagon* neighbor = mGrid.getHexAt(neighborCoord);
                        if (neighbor) {
                            frontier.push_back(neighbor);
                            visited.push_back(neighborCoord);
                        }
                    }
                }
            }
        }
        
        return cityHexes;
    };
    
    // Grow each city
    city1Hexes = growCity(city1Seed, CITY1_SIZE);
    city2Hexes = growCity(city2Seed, CITY2_SIZE);
    city3Hexes = growCity(city3Seed, CITY3_SIZE);
    
    // Create cities from the hexes with the appropriate allegiance
    if (!city1Hexes.empty()) {
        // Set all city hexes to URBAN terrain type
        for (auto hex : city1Hexes) {
            hex->setTerrainType(TerrainType::URBAN);
            // Set concrete color based on terrain type
            hex->setColor(mGrid.getTerrainColor(TerrainType::URBAN));
        }
        auto city1 = std::make_unique<City>(city1Hexes, allegiance);
        mCities.push_back(std::move(city1));
    }
    
    if (!city2Hexes.empty()) {
        // Set all city hexes to URBAN terrain type
        for (auto hex : city2Hexes) {
            hex->setTerrainType(TerrainType::URBAN);
            // Set concrete color based on terrain type
            hex->setColor(mGrid.getTerrainColor(TerrainType::URBAN));
        }
        auto city2 = std::make_unique<City>(city2Hexes, allegiance);
        mCities.push_back(std::move(city2));
    }
    
    if (!city3Hexes.empty()) {
        // Set all city hexes to URBAN terrain type
        for (auto hex : city3Hexes) {
            hex->setTerrainType(TerrainType::URBAN);
            // Set concrete color based on terrain type
            hex->setColor(mGrid.getTerrainColor(TerrainType::URBAN));
        }
        auto city3 = std::make_unique<City>(city3Hexes, allegiance);
        mCities.push_back(std::move(city3));
    }
    
    std::cout << "GridFiller::generateCities() complete for " << side 
              << ", created " << (city1Hexes.empty() ? 0 : 1) + (city2Hexes.empty() ? 0 : 1) + (city3Hexes.empty() ? 0 : 1) 
              << " cities" << std::endl;
}

void GridFiller::generateResources() {
    std::cout << "GridFiller::generateResources() start" << std::endl;
    
    // Number of oil resources to place per side (friendly/enemy)
    const int NUM_OIL_RESOURCES_PER_SIDE = 6;
    
    // Get all hexes from the grid
    auto allHexes = mGrid.getAllHexes();
    
    // Filter and divide hexes into top half (enemy) and bottom half (friendly)
    std::vector<Hexagon*> topEmptyHexes, bottomEmptyHexes;
    for (auto hex : allHexes) {
        if (!hex->hasBuilding() && !hex->hasCharacter() && !hex->hasResource()) {
            // r < 0 means top half, r > 0 means bottom half
            if (hex->getCoord().r < 0) {
                topEmptyHexes.push_back(hex);
            } else if (hex->getCoord().r > 0) {
                bottomEmptyHexes.push_back(hex);
            }
        }
    }
    
    // Seed random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Place resources in bottom half (friendly territory)
    int resourcesPlaced = placeOilResources(bottomEmptyHexes, NUM_OIL_RESOURCES_PER_SIDE, gen, Allegiance::FRIENDLY);
    
    // Place resources in top half (enemy territory)
    resourcesPlaced += placeOilResources(topEmptyHexes, NUM_OIL_RESOURCES_PER_SIDE, gen, Allegiance::ENEMY);
    
    std::cout << "GridFiller::generateResources() complete, placed " << resourcesPlaced << " resources" << std::endl;
}

int GridFiller::placeOilResources(std::vector<Hexagon*>& emptyHexes, int count, std::mt19937& gen, Allegiance allegiance) {
    if (emptyHexes.empty()) return 0;
    
    std::uniform_int_distribution<> distrib(0, emptyHexes.size() - 1);
    int resourcesPlaced = 0;
    
    // Place oil resources randomly
    for (int i = 0; i < count && !emptyHexes.empty(); ++i) {
        // Pick a random empty hex
        int randomIndex = distrib(gen);
        Hexagon* hex = emptyHexes[randomIndex];
        
        // Create a new oil resource with unique_ptr for proper ownership
        auto oil = std::make_unique<Oil>();
        oil->setPosition(hex->getPosition());
        
        // Set the resource in the hex (hex holds a non-owning pointer)
        hex->setResource(oil.get());
        
        // Store the unique_ptr in our collection to maintain ownership
        mResources.push_back(std::move(oil));
        
        resourcesPlaced++;
        
        // Remove this hex from available hexes to avoid placing multiple resources
        emptyHexes.erase(emptyHexes.begin() + randomIndex);
        
        // Update distribution range for the smaller vector
        if (!emptyHexes.empty()) {
            distrib = std::uniform_int_distribution<>(0, emptyHexes.size() - 1);
        }
    }
    
    return resourcesPlaced;
}
