#include "../include/Hexagon.h"
#include "../include/characters/Character.h"
#include "../include/resources/Resource.h"
#include <cmath>

// Initialize directions in cube coordinates
const std::array<Hexagon::CubeCoord, 6> Hexagon::directions = {
    CubeCoord(1, -1, 0),  // East
    CubeCoord(1, 0, -1),  // Southeast
    CubeCoord(0, 1, -1),  // Southwest
    CubeCoord(-1, 1, 0),  // West
    CubeCoord(-1, 0, 1),  // Northwest
    CubeCoord(0, -1, 1)   // Northeast
};

Hexagon::Hexagon(int q, int r) : mCoord(q, r, -q-r) {
    color = sf::Color(100, 100, 100);
    isHighlighted = false;
    highlightColor = sf::Color::Transparent;
    createHexagonShape();
    mShape.setPosition(cubeToPixel(mCoord, SIZE));
}

Hexagon::Hexagon(const CubeCoord& coord) : mCoord(coord) {
    color = sf::Color(100, 100, 100); // Default gray
    isHighlighted = false;
    highlightColor = sf::Color::Transparent;
    createHexagonShape();
    mShape.setPosition(cubeToPixel(mCoord, SIZE));
}

void Hexagon::createHexagonShape() {
    mShape = sf::ConvexShape(6);
    
    // Pointy-top hexagon
    for (int i = 0; i < 6; ++i) {
        float angle = (i * 60 + 30) * M_PI / 180.0;  // Convert to radians
        float x = SIZE * std::cos(angle);
        float y = SIZE * std::sin(angle);
        mShape.setPoint(i, {x, y});
    }
    
    mShape.setFillColor(color);
    mShape.setOutlineColor(sf::Color::Black);
    mShape.setOutlineThickness(1.0f);
    // Center the origin on the hexagon
    mShape.setOrigin({0, 0});
}

void Hexagon::setFillColor(const sf::Color& c) {
    // If not highlighted, also update the base color
    if (!isHighlighted) {
        color = c;
    }
    mShape.setFillColor(c);
}

void Hexagon::setOutlineColor(const sf::Color& color) {
    mShape.setOutlineColor(color);
}

void Hexagon::setOutlineThickness(float thickness) {
    mShape.setOutlineThickness(thickness);
}

void Hexagon::setPosition(const sf::Vector2f& position) {
    mShape.setPosition(position);
}

sf::Vector2f Hexagon::getPosition() const {
    return mShape.getPosition();
}

Hexagon::CubeCoord Hexagon::getCoord() const {
    return mCoord;
}

void Hexagon::draw(sf::RenderWindow& window) const {
    window.draw(mShape);

    // Render resource if present
    if (mResource.has_value() && *mResource) {
        (*mResource)->render(window);
    }
    
    // Render building if present
    if (mBuilding.has_value() && *mBuilding) {
        (*mBuilding)->render(window);
    }
}

// Pointy-top hexagon layout
sf::Vector2f Hexagon::cubeToPixel(const CubeCoord& cube, float size) {
    float x = size * 1.732f * (cube.q + cube.r/2.0) * 0.95f; // Slightly reduce spacing to prevent gaps
    float y = size * 1.5f * cube.r * 0.95f;
    return {x, y};
}

Hexagon::CubeCoord Hexagon::pixelToCube(const sf::Vector2f& pixel, float size) {
    // For pointy-top
    float q = (pixel.x * 1.0/(1.732f * size * 0.95f) - pixel.y * 1.0/(3.0f * size * 0.95f));
    float r = pixel.y * 2.0/(3.0f * size * 0.95f);
    float s = -q - r;
    
    // Round to nearest hex
    float rq = std::round(q);
    float rr = std::round(r);
    float rs = std::round(s);
    
    float q_diff = std::abs(rq - q);
    float r_diff = std::abs(rr - r);
    float s_diff = std::abs(rs - s);
    
    if (q_diff > r_diff && q_diff > s_diff) {
        rq = -rr - rs;
    } else if (r_diff > s_diff) {
        rr = -rq - rs;
    } else {
        rs = -rq - rr;
    }
    
    return CubeCoord(static_cast<int>(rq), static_cast<int>(rr), static_cast<int>(rs));
}

// Get neighbor in a given direction
Hexagon::CubeCoord Hexagon::neighbor(const CubeCoord& cube, int direction) {
    direction = direction % 6;
    return cube + directions[direction];
}

// Calculate distance between two hexes
int Hexagon::distance(const CubeCoord& a, const CubeCoord& b) {
    return (std::abs(a.q - b.q) + std::abs(a.r - b.r) + std::abs(a.s - b.s)) / 2;
} 

void Hexagon::setBuilding(Building* building) {
    if (!mBuilding.has_value()) {
        mBuilding = building;
        if (building) {
            // Update the building's position to match this hex's center
            building->setPosition(mShape.getPosition());
        }
    }
}

Building* Hexagon::getBuilding() const {
    return mBuilding.value_or(nullptr);
}

void Hexagon::removeBuilding() {
    mBuilding.reset();
}

bool Hexagon::hasBuilding() const {
    return mBuilding.has_value() && *mBuilding != nullptr;
}

void Hexagon::setCharacter(Character* character) {
    if (!mCharacter.has_value()) {
        mCharacter = character;
        if (character) {
            // Update character's position to center of hex
            character->setPosition(mShape.getPosition());
            
            // Update character's hex coordinates
            character->setHexCoord(mCoord);
        }
    }
}

Character* Hexagon::getCharacter() const {
    return mCharacter.value_or(nullptr);
}

void Hexagon::removeCharacter() {
    mCharacter.reset();
}

bool Hexagon::hasCharacter() const {
    return mCharacter.has_value() && *mCharacter != nullptr;
}

void Hexagon::setColor(const sf::Color& color) {
    this->color = color;
    mShape.setFillColor(color);
}

// Use this for permanent color changes (from cities)
void Hexagon::setBaseColor(const sf::Color& newColor) {
    color = newColor;
    if (!isHighlighted) {
        mShape.setFillColor(color);
    }
}

// Use this for temporary highlighting
void Hexagon::highlight(const sf::Color& hColor) {
    isHighlighted = true;
    highlightColor = hColor;
    mShape.setFillColor(highlightColor);
}

// Call this to remove highlighting
void Hexagon::removeHighlight() {
    isHighlighted = false;
    mShape.setFillColor(color);
}

// Resource methods
void Hexagon::setResource(Resource* resource) {
    if (!mResource.has_value()) {
        mResource = resource;
        if (resource) {
            // Update the resource's position to match this hex's center
            resource->setPosition(mShape.getPosition());
        }
    }
}

Resource* Hexagon::getResource() const {
    return mResource.value_or(nullptr);
}

void Hexagon::removeResource() {
    mResource.reset();
}

bool Hexagon::hasResource() const {
    return mResource.has_value() && *mResource != nullptr;
}