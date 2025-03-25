#ifndef GRID_FILLER_H
#define GRID_FILLER_H

#include <vector>
#include <memory>
#include <random>
#include "HexGrid.h"
#include "../buildings/City.h"
#include "../resources/Resource.h"
#include "../Allegiance.h"

class GridFiller {
public:
    GridFiller(HexGrid& grid);
    ~GridFiller();
    
    // Fill the grid with cities and resources
    void fillGrid();
    
    // Transfer ownership of cities to the caller
    std::vector<std::unique_ptr<City>> getCities();
    
    // Transfer ownership of resources to the caller
    std::vector<std::unique_ptr<Resource>> getResources();
    
    // Generate cities on the grid with specified allegiance
    void generateCities(Allegiance allegiance);
    
    // Generate resources on the grid
    void generateResources();
    
private:
    // Helper method to place oil resources in a specific area
    int placeOilResources(std::vector<Hexagon*>& emptyHexes, int count, std::mt19937& gen, Allegiance allegiance);

    HexGrid& mGrid;
    std::vector<std::unique_ptr<City>> mCities;
    std::vector<std::unique_ptr<Resource>> mResources; // Store resources with proper ownership
};

#endif // GRID_FILLER_H 