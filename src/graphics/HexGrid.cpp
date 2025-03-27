#include "../../include/graphics/HexGrid.h"
#include "../../include/graphics/PerlinNoise.h"
#include <limits>
#include <ctime>


HexGrid::HexGrid(int radius) : mRadius(radius) {
    // Create hexagons in a spiral pattern from the center
    for (int q = -radius; q <= radius; q++) {
        int r1 = std::max(-radius, -q - radius);
        int r2 = std::min(radius, -q + radius);
        for (int r = r1; r <= r2; r++) {
            // Calculate s based on the constraint q + r + s = 0
            int s = -q - r;
            
            // Create a new hexagon and add it to the grid
            auto cubeCoord = Hexagon::CubeCoord(q, r, s);
            mHexagons[cubeCoord] = std::make_unique<Hexagon>(cubeCoord);
            
            // The cubeToPixel function now handles the correct positioning
            sf::Vector2f pos = Hexagon::cubeToPixel(cubeCoord, mHexSize);
            mHexagons[cubeCoord]->setPosition(pos);
        }
    }
    generateTerrain();
}

void HexGrid::generateTerrain() {
    // Use PerlinNoise or SimplexNoise instead of pure randomness
    unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
    PerlinNoise noise(seed);
    
    auto allHexes = getAllHexes();
    for (auto hex : allHexes) {
        // Get normalized coordinates
        float nx = hex->getPosition().x * 0.01f;
        float ny = hex->getPosition().y * 0.01f;
        
        // Generate noise value (0.0 to 1.0)
        float noiseValue = noise.noise(nx, ny);
        
        // Distribute terrain types more evenly with water at ~10%
        if (noiseValue < 0.1f) {
            hex->setTerrainType(TerrainType::PLAINS);
        } else if (noiseValue < 0.7f) {
            hex->setTerrainType(TerrainType::FOREST);
        } else {
            hex->setTerrainType(TerrainType::WATER);
        }
        
        hex->setColor(getTerrainColor(hex->getTerrainType()));
    }
}

sf::Color HexGrid::getTerrainColor(TerrainType type) {
    // Base colors with slight random variation
    int variation = rand() % 30 - 15;  // -15 to +15
    
    switch (type) {
        case TerrainType::PLAINS: {
            int g = std::min(255, std::max(0, 180 + variation));
            int r = std::min(255, std::max(0, 140 + variation));
            int b = std::min(255, std::max(0, 80 + variation/2));
            return sf::Color(r, g, b);
        }
        case TerrainType::WATER: {
            int b = std::min(255, std::max(0, 200 + variation));
            int g = std::min(255, std::max(0, 130 + variation/2));
            int r = std::min(255, std::max(0, 70 + variation/3));
            return sf::Color(r, g, b);
        }
        case TerrainType::FOREST: {
            int g = std::min(255, std::max(0, 100 + variation));
            int r = std::min(255, std::max(0, 50 + variation/2));
            int b = std::min(255, std::max(0, 30 + variation/3));
            return sf::Color(r, g, b);
        }
        case TerrainType::URBAN: {
            int v = std::min(255, std::max(0, 150 + variation/2));
            return sf::Color(v, v, v); // Concrete gray color
        }
    }
    return sf::Color::White;
}


void HexGrid::draw(sf::RenderWindow& window) const {
    // Get the current view
    sf::View currentView = window.getView();
    sf::Vector2f viewCenter = currentView.getCenter();
    sf::Vector2f viewSize = currentView.getSize();
    
    // Expanded view bounds with margin to prevent hexes from popping in/out abruptly
    float margin = mHexSize * 4; // Increased margin for smoother camera movement
    sf::Vector2f topLeft = {
        viewCenter.x - viewSize.x / 2 - margin,
        viewCenter.y - viewSize.y / 2 - margin
    };
    sf::Vector2f bottomRight = {
        viewCenter.x + viewSize.x / 2 + margin,
        viewCenter.y + viewSize.y / 2 + margin
    };
    
    // Only draw hexes that are visible in the current view (with margin)
    for (const auto& [coord, hex] : mHexagons) {
        sf::Vector2f hexPos = hex->getPosition();
        
        // Check if the hex is inside the expanded view bounds
        if (hexPos.x >= topLeft.x && hexPos.x <= bottomRight.x &&
            hexPos.y >= topLeft.y && hexPos.y <= bottomRight.y) {
            hex->draw(window);
        }
    }
}

sf::FloatRect HexGrid::getBounds() const {
    // If no hexagons, return empty rect
    if (mHexagons.empty()) {
        return sf::FloatRect({0, 0}, {0, 0});
    }
    
    // Find the min/max coordinates
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    
    for (const auto& [coord, hex] : mHexagons) {
        sf::Vector2f pos = hex->getPosition();
        
        minX = std::min(minX, pos.x);
        minY = std::min(minY, pos.y);
        maxX = std::max(maxX, pos.x);
        maxY = std::max(maxY, pos.y);
    }
    
    // Add a small padding based on hex size
    float padding = mHexSize * 2;
    
    sf::Vector2f position(minX - padding, minY - padding);
    sf::Vector2f size((maxX - minX) + padding * 2, (maxY - minY) + padding * 2);
    
    return sf::FloatRect(position, size);
}

void HexGrid::highlightHexes(const std::function<bool(const Hexagon::CubeCoord&)>& criteria, sf::Color color) {
    for (auto& [coord, hex] : mHexagons) {
        if (criteria(coord)) {
            hex->highlight(color);
        }
    }
}

void HexGrid::highlightAdjacentHexes(const Hexagon::CubeCoord& coord, sf::Color color) {
    auto adjacentHexes = getAdjacentHexes(coord);
    for (const auto& adjCoord : adjacentHexes) {
        auto hexIter = mHexagons.find(adjCoord);
        if (hexIter != mHexagons.end()) {
            hexIter->second->highlight(color);   
        }
    }
}

void HexGrid::highlightPath(const std::vector<Hexagon::CubeCoord>& path, sf::Color color) {
    for (const auto& coord : path) {
        auto hexIter = mHexagons.find(coord);
        if (hexIter != mHexagons.end()) {
            hexIter->second->highlight(color);
        }
    }
}

void HexGrid::resetHighlights() {
    for (auto& [coord, hex] : mHexagons) {
        hex->removeHighlight();
    }
}

Hexagon* HexGrid::getHexAt(const Hexagon::CubeCoord& coord) {
    auto hexIter = mHexagons.find(coord);
    if (hexIter != mHexagons.end()) {
        return hexIter->second.get();
    }
    return nullptr;
}

Hexagon* HexGrid::getHexAtPixel(const sf::Vector2f& pixelPos) {
    // Convert pixel coordinates to cube coordinates
    Hexagon::CubeCoord coord = Hexagon::pixelToCube(pixelPos, mHexSize);
    return getHexAt(coord);
}

std::vector<Hexagon::CubeCoord> HexGrid::getAdjacentHexes(const Hexagon::CubeCoord& coord) {
    std::vector<Hexagon::CubeCoord> adjacentHexes;
    
    // For each of the 6 directions
    for (const auto& direction : Hexagon::directions) {
        // Calculate neighbor coordinate
        Hexagon::CubeCoord neighborCoord(
            coord.q + direction.q,
            coord.r + direction.r,
            coord.s + direction.s
        );
        
        // Check if the neighbor exists in our grid
        if (mHexagons.find(neighborCoord) != mHexagons.end()) {
            adjacentHexes.push_back(neighborCoord);
        }
    }
    
    return adjacentHexes;
}

bool HexGrid::areAdjacent(const Hexagon::CubeCoord& coord1, const Hexagon::CubeCoord& coord2) {
    // Check if the two coordinates are adjacent in any direction
    for (const auto& direction : Hexagon::directions) {
        if (coord1 + direction == coord2) {
            return true;    
        }
    }
    return false;
}

const std::unordered_map<Hexagon::CubeCoord, std::unique_ptr<Hexagon>>& HexGrid::getHexagons() const {
    return mHexagons;
}

// Get all hexes within a certain range of a center hex
std::vector<Hexagon*> HexGrid::getHexesInRange(const Hexagon::CubeCoord& center, int range) {
    std::vector<Hexagon*> result;
    
    // Iterate through all possible cube coordinates in range
    for (int q = -range; q <= range; q++) {
        for (int r = std::max(-range, -q - range); r <= std::min(range, -q + range); r++) {
            int s = -q - r; // Calculate s from q + r + s = 0
            Hexagon::CubeCoord coord(q + center.q, r + center.r, s + center.s);
            
            // Check if this hex exists in our grid
            if (mHexagons.find(coord) != mHexagons.end()) {
                // Make sure it's actually within the range (Manhattan distance in cube coordinates)
                if (Hexagon::distance(center, coord) <= range) {
                    result.push_back(mHexagons[coord].get());
                }
            }
        }
    }
    
    return result;
}

// Convert pixel coordinates to cube coordinates
Hexagon::CubeCoord HexGrid::pixelToCube(const sf::Vector2f& pixel) const {
    return Hexagon::pixelToCube(pixel, mHexSize);
}

// Reset visibility for all hexes
void HexGrid::resetVisibility() {
    for (auto& [coord, hex] : mHexagons) {
        hex->setVisible(false);
    }
}

// Get all hexes
std::vector<Hexagon*> HexGrid::getAllHexes() const {
    std::vector<Hexagon*> result;
    result.reserve(mHexagons.size());
    
    for (const auto& [coord, hex] : mHexagons) {
        result.push_back(hex.get());
    }
    
    return result;
} 