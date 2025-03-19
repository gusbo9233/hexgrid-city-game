#ifndef HEXGRID_H
#define HEXGRID_H

#include "Hexagon.h"
#include <vector>
#include <unordered_map>
#include <functional>

// Hash function for CubeCoord to use in unordered_map
namespace std {
    template <>
    struct hash<Hexagon::CubeCoord> {
        size_t operator()(const Hexagon::CubeCoord& coord) const {
            // Pairing function to hash the three coordinates
            int a = coord.q;
            int b = coord.r;
            int c = coord.s;
            
            // Cantor pairing function to combine the three values
            int ab = ((a + b) * (a + b + 1)) / 2 + b;
            return ((ab + c) * (ab + c + 1)) / 2 + c;
        }
    };
}

class HexGrid {
public:
    HexGrid(int radius);
    
    // Draw all visible hexagons
    void draw(sf::RenderWindow& window) const;
    
    // Highlight hexes based on criteria (useful for visualizing q, r, or s = constant)
    void highlightHexes(const std::function<bool(const Hexagon::CubeCoord&)>& criteria, sf::Color color);
    
    // Highlight a specific path or set of hexes
    void highlightPath(const std::vector<Hexagon::CubeCoord>& path, sf::Color color);
    
    // Reset all hexagons to their default color
    void resetHighlights();
    
    // Get the hex at the given coordinates
    Hexagon* getHexAt(const Hexagon::CubeCoord& coord);
    
    // Get hex at pixel coordinates
    Hexagon* getHexAtPixel(const sf::Vector2f& pixelPos);
    
    // Get all hexagons (for external rendering)
    const std::unordered_map<Hexagon::CubeCoord, std::unique_ptr<Hexagon>>& getHexagons() const;
    
    // Get the bounds of the hex grid in world coordinates
    sf::FloatRect getBounds() const;
    
    // Get the radius of the grid
    int getRadius() const { return mRadius; }

    // Get adjacent hexes to a given hex
    std::vector<Hexagon::CubeCoord> getAdjacentHexes(const Hexagon::CubeCoord& coord);

    // Are two hexes adjacent?
    bool areAdjacent(const Hexagon::CubeCoord& coord1, const Hexagon::CubeCoord& coord2);

    // Highlight adjacent hexes
    void highlightAdjacentHexes(const Hexagon::CubeCoord& coord, sf::Color color);
    
private:
    std::unordered_map<Hexagon::CubeCoord, std::unique_ptr<Hexagon>> mHexagons;
    const float mHexSize = 25.0f; // Make this match the SIZE in Hexagon.h
    int mRadius;
};

#endif // HEXGRID_H 