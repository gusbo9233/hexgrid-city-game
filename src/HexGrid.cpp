#include "HexGrid.h"
#include <limits>

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