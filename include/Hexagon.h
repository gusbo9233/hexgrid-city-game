#ifndef HEXAGON_H
#define HEXAGON_H

#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include <memory>
#include "buildings/Building.h"

// Forward declarations
class Character;
class Resource;

//terrain types
enum class TerrainType {
    PLAINS,
    WATER,
    FOREST,
    URBAN
};

class Hexagon {
public:
    // Cube coordinates (q, r, s)
    struct CubeCoord {
        int q, r, s;
        
        CubeCoord(int q, int r, int s) : q(q), r(r), s(s) {
            // Ensure q + r + s = 0
            if (q + r + s != 0) {
                throw std::invalid_argument("Cube coordinates must satisfy q + r + s = 0");
            }
        }
        
        // Overload operators for cube coordinates
        CubeCoord operator+(const CubeCoord& other) const {
            return CubeCoord(q + other.q, r + other.r, s + other.s);
        }
        
        bool operator==(const CubeCoord& other) const {
            return q == other.q && r == other.r && s == other.s;
        }
        
        // Less-than operator for use with std::map
        bool operator<(const CubeCoord& other) const {
            // Lexicographical ordering: first compare q, then r, then s
            if (q != other.q) return q < other.q;
            if (r != other.r) return r < other.r;
            return s < other.s;
        }
    };
    
    // Define the directions in cube coordinates
    static const std::array<CubeCoord, 6> directions;
    
    Hexagon(int q, int r);
    Hexagon(const CubeCoord& coord);
    
    void setFillColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);
    void setOutlineThickness(float thickness);
    void setPosition(const sf::Vector2f& position);
    
    // We take raw pointers but don't own them - the owner manages lifetime
    void setBuilding(Building* building);
    void setCharacter(Character* character);
    void setResource(Resource* resource);
    
    // Return non-owning pointers
    Building* getBuilding() const;
    Character* getCharacter() const;
    Resource* getResource() const;
    
    void removeBuilding();
    void removeCharacter();
    void removeResource();
    
    bool hasCharacter() const;
    bool hasResource() const;
    
    sf::Vector2f getPosition() const;
    
    CubeCoord getCoord() const;
    void draw(sf::RenderWindow& window) const;
    
    // Convert between cube coordinates and pixel coordinates
    static sf::Vector2f cubeToPixel(const CubeCoord& cube, float size);
    static CubeCoord pixelToCube(const sf::Vector2f& pixel, float size);
    
    // Get neighbor in a given direction
    static CubeCoord neighbor(const CubeCoord& cube, int direction);
    
    // Calculate distance between two hexes
    static int distance(const CubeCoord& a, const CubeCoord& b);
    void setColor(const sf::Color& color);
    
    // Building-related methods
    bool hasBuilding() const;
    
    // New methods
    void setBaseColor(const sf::Color& color);
    void highlight(const sf::Color& color);
    void removeHighlight();
    
    // Visibility methods
    bool isVisible() const { return mIsVisible; }
    void setVisible(bool visible) { mIsVisible = visible; }
    
    bool isExplored() const { return mIsExplored; }
    void setExplored(bool explored) { mIsExplored = explored; }
    
    TerrainType getTerrainType() const { return mTerrainType; }
    void setTerrainType(TerrainType type) { mTerrainType = type; }
    
private:
    CubeCoord mCoord;
    sf::ConvexShape mShape;
    static constexpr float SIZE = 25.0f; // Smaller size for a better fit
    
    // Use optional pointers - these are non-owning references
    std::optional<Building*> mBuilding;
    std::optional<Character*> mCharacter;
    std::optional<Resource*> mResource;
    
    sf::Color color;            // Base/permanent color
    bool isHighlighted;         // Tracking if currently highlighted
    sf::Color highlightColor;   // Current highlight color if any
    TerrainType mTerrainType;
    // Visibility state
    bool mIsVisible = false;    // Currently visible
    bool mIsExplored = false;   // Has been seen before
    
    void createHexagonShape();

};

#endif // HEXAGON_H 