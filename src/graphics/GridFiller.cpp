#include "../../include/graphics/GridFiller.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "../../include/buildings/City.h"
#include "../../include/resources/Oil.h"
#include "../../include/Allegiance.h"
#include "../../include/business/workplaces/OilRefinery.h"
#include "../../include/business/workplaces/Farm.h"
#include "../../include/characters/Tank.h"

GridFiller::GridFiller(HexGrid& grid) : mGrid(grid) {
    std::cout << "GridFiller constructor" << std::endl;
    // No additional initialization needed
}

GridFiller::~GridFiller() {
    std::cout << "GridFiller destructor" << std::endl;
}

void GridFiller::fillGrid() {
    std::cout << "GridFiller::fillGrid() start" << std::endl;
    
    // Generate cities
    generateCities(Allegiance::FRIENDLY);
    generateCities(Allegiance::ENEMY);
    
    // Generate resources
    generateResources();
    
    // Generate oil refineries
    generateOilRefinery(Allegiance::FRIENDLY);
    generateOilRefinery(Allegiance::ENEMY);
    
    // Generate farms
    generateFarms(Allegiance::FRIENDLY);
    generateFarms(Allegiance::ENEMY);
    
    // Generate soldiers
    generateSoldiers(Allegiance::FRIENDLY);
    generateSoldiers(Allegiance::ENEMY);
    
    // Generate tanks
    generateTank(Allegiance::FRIENDLY);
    generateTank(Allegiance::ENEMY);
    
    std::cout << "GridFiller::fillGrid() complete, created " << mCities.size() << " cities, " 
              << mResources.size() << " resources, " << mCharacters.size() << " characters" << std::endl;
}

std::vector<std::unique_ptr<City>> GridFiller::getCities() {
    // Transfer ownership of cities to the caller
    return std::move(mCities);
}

std::vector<std::unique_ptr<Resource>> GridFiller::getResources() {
    // Transfer ownership of resources to the caller
    return std::move(mResources);
}

std::vector<std::unique_ptr<Building>> GridFiller::getBuildings() {
    // Transfer ownership of buildings to the caller
    return std::move(mBuildings);
}

std::vector<std::unique_ptr<Character>> GridFiller::getCharacters() {
    // Transfer ownership of characters to the caller
    return std::move(mCharacters);
}

void GridFiller::generateTank(Allegiance allegiance) {
    std::cout << "GridFiller::generateTank() placing tanks for " 
              << (allegiance == Allegiance::FRIENDLY ? "friendly" : "enemy") << " cities" << std::endl;
    
    int tanksPlaced = 0;
    
    // Get all cities with the matching allegiance
    for (const auto& city : mCities) {
        if (city->getAllegiance() == allegiance) {
            // Get all hexes for this city
            const auto& cityHexes = city->getHexes();
            
            // Filter to find hexes without characters
            std::vector<Hexagon*> availableHexes;
            for (auto hex : cityHexes) {
                if (!hex->hasCharacter()) {
                    availableHexes.push_back(hex);
                }
            }
            
            if (!availableHexes.empty()) {
                // Choose a random hex in the city
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, availableHexes.size() - 1);
                Hexagon* selectedHex = availableHexes[dist(gen)];
                
                // Create coordinates from the selected hex
                Hexagon::CubeCoord coord = selectedHex->getCoord();
                
                // Create a new tank
                auto tank = std::make_unique<Tank>(coord.q, coord.r);
                tank->setAllegiance(allegiance);
                
                // Position the tank's sprite at the hex's position
                tank->setPosition(selectedHex->getPosition());
                
                // Get a raw pointer before moving it to the list
                Tank* tankPtr = tank.get();
                
                // Set the tank in the hex
                selectedHex->setCharacter(tankPtr);
                
                // Add to character list
                mCharacters.push_back(std::move(tank));
                
                tanksPlaced++;
                std::cout << "Placed a tank in city at (" 
                          << coord.q << "," << coord.r << "," << coord.s << ")" << std::endl;
                
                // Only place one tank per allegiance
                break;
            }
        }
    }
    
    std::cout << "GridFiller::generateTank() complete, placed " << tanksPlaced 
              << " tanks for " << (allegiance == Allegiance::FRIENDLY ? "friendly" : "enemy") << " cities" << std::endl;
}

void GridFiller::generateSoldiers(Allegiance allegiance) {
    std::cout << "GridFiller::generateSoldiers() placing soldiers for " 
              << (allegiance == Allegiance::FRIENDLY ? "friendly" : "enemy") << " cities" << std::endl;
    
    // We'll place one soldier per city of the specified allegiance
    int soldiersPlaced = 0;
    
    // Get all cities with the matching allegiance
    for (const auto& city : mCities) {
        if (city->getAllegiance() == allegiance) {
            // Get all hexes for this city
            const auto& cityHexes = city->getHexes();
            
            // Filter to find hexes without characters
            std::vector<Hexagon*> availableHexes;
            for (auto hex : cityHexes) {
                if (!hex->hasCharacter()) {
                    availableHexes.push_back(hex);
                }
            }
            
            if (!availableHexes.empty()) {
                // Choose a random hex in the city
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, availableHexes.size() - 1);
                Hexagon* selectedHex = availableHexes[dist(gen)];
                
                // Create coordinates from the selected hex
                Hexagon::CubeCoord coord = selectedHex->getCoord();
                
                // Create a new soldier
                auto soldier = std::make_unique<Soldier>(coord.q, coord.r);
                soldier->setAllegiance(allegiance);
                
                // Position the soldier's sprite at the hex's position
                soldier->setPosition(selectedHex->getPosition());
                
                // Get a raw pointer before moving it to the list
                Soldier* soldierPtr = soldier.get();
                
                // Set the soldier in the hex
                selectedHex->setCharacter(soldierPtr);
                
                // Add to character list
                mCharacters.push_back(std::move(soldier));
                
                soldiersPlaced++;
                std::cout << "Placed a soldier in city at (" 
                          << coord.q << "," << coord.r << "," << coord.s << ")" << std::endl;
            }
        }
    }
    
    std::cout << "GridFiller::generateSoldiers() complete, placed " << soldiersPlaced 
              << " soldiers for " << (allegiance == Allegiance::FRIENDLY ? "friendly" : "enemy") << " cities" << std::endl;
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

void GridFiller::generateOilRefinery(Allegiance allegiance) {
    std::string side = (allegiance == Allegiance::FRIENDLY) ? "friendly (bottom)" : "enemy (top)";
    std::cout << "GridFiller::generateOilRefinery() start for " << side << std::endl;
    
    // Find suitable hexes with oil resources in the appropriate half
    std::vector<Hexagon*> oilHexes;
    auto allHexes = mGrid.getAllHexes();
    
    std::cout << "Total hexes to check: " << allHexes.size() << std::endl;
    int oilHexesFound = 0;
    
    for (auto hex : allHexes) {
        // Check if the hex has an oil resource
        if (hex->hasResource()) {
            Resource* resource = hex->getResource();
            
            // Add debug safeguard against null resources
            if (!resource) {
                std::cout << "Warning: Hex has resource but resource pointer is null!" << std::endl;
                continue;
            }
            
            ResourceType resourceType = resource->getType();
            
            if (resourceType == ResourceType::OIL) {
                oilHexesFound++;
                
                // Check if the hex is in the correct half (r < 0 for enemy, r > 0 for friendly)
                bool isCorrectHalf = (allegiance == Allegiance::FRIENDLY && hex->getCoord().r > 0) ||
                                     (allegiance == Allegiance::ENEMY && hex->getCoord().r < 0);
                
                if (isCorrectHalf) {
                    // This hex has oil and is in the correct half - it's suitable
                    oilHexes.push_back(hex);
                }
            }
        }
    }
    
    std::cout << "Oil hexes found: " << oilHexesFound << ", suitable oil hexes: " << oilHexes.size() << std::endl;
    
    if (oilHexes.empty()) {
        std::cout << "No suitable oil hexes found in " << side << " territory" << std::endl;
        return;
    }
    
    // Pick a random oil hex for the refinery
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, oilHexes.size() - 1);
    Hexagon* selectedOilHex = oilHexes[distrib(gen)];
    
    std::cout << "Selected oil hex at coordinates (" 
              << selectedOilHex->getCoord().q << "," 
              << selectedOilHex->getCoord().r << "," 
              << selectedOilHex->getCoord().s << ")" << std::endl;
    
    // Get the position of the hex
    sf::Vector2f position = selectedOilHex->getPosition();
    
    std::cout << "About to create OilRefinery on oil hex..." << std::endl;
    
    // Create and place the oil refinery directly on the oil hex
    try {
        auto oilRefinery = std::make_unique<OilRefinery>(position);
        
        std::cout << "OilRefinery created successfully!" << std::endl;
        
        // Set allegiance - important to avoid pure virtual function calls 
        oilRefinery->setAllegiance(allegiance);
        
        // Set the building in the hex
        selectedOilHex->setBuilding(oilRefinery.get());
        
        // Store in our collection
        mBuildings.push_back(std::move(oilRefinery));
        
        std::cout << "Created oil refinery in " << side << " territory at (" 
                  << selectedOilHex->getCoord().q << "," << selectedOilHex->getCoord().r << "," 
                  << selectedOilHex->getCoord().s << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception creating OilRefinery: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception creating OilRefinery!" << std::endl;
    }
}

void GridFiller::generateFarms(Allegiance allegiance) {
    std::string side = (allegiance == Allegiance::FRIENDLY) ? "friendly (bottom)" : "enemy (top)";
    std::cout << "GridFiller::generateFarms() start for " << side << std::endl;
    
    // Number of farms to place
    const int NUM_FARMS = 3;
    
    // Get all hexes
    auto allHexes = mGrid.getAllHexes();
    
    // Filter empty hexes in correct half based on allegiance
    std::vector<Hexagon*> emptyHexes;
    for (auto hex : allHexes) {
        if (!hex->hasBuilding() && !hex->hasCharacter() && !hex->hasResource()) {
            // Choose hexes in proper territory based on allegiance
            if ((allegiance == Allegiance::FRIENDLY && hex->getCoord().r > 0) ||
                (allegiance == Allegiance::ENEMY && hex->getCoord().r < 0)) {
                
                // Only place farms on plains terrain
                if (hex->getTerrainType() == TerrainType::PLAINS) {
                    emptyHexes.push_back(hex);
                }
            }
        }
    }
    
    // Seed random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Shuffle hexes to get random positions
    std::shuffle(emptyHexes.begin(), emptyHexes.end(), gen);
    
    // Place farms
    int farmsPlaced = 0;
    for (auto hex : emptyHexes) {
        if (farmsPlaced >= NUM_FARMS) break;
        
        // Create a farm
        auto farm = std::make_unique<Farm>(hex->getPosition());
        farm->setAllegiance(allegiance);
        
        // Place the farm on the hex
        hex->setBuilding(farm.get());
        
        // Store the farm in the buildings collection
        mBuildings.push_back(std::move(farm));
        
        farmsPlaced++;
    }
    
    std::cout << "GridFiller::generateFarms() complete for " << side 
              << ", placed " << farmsPlaced << " farms" << std::endl;
}