#include "Building.h"
#include "Hexagon.h"

Building::Building(const sf::Vector2f& position)
    : mPosition(position) {
    // We'll initialize the shape in initializeShape() which should be called by derived classes
    // in their constructors after the Building constructor finishes
}

// This method should be called by subclasses after construction
void Building::initializeShape() {
    // Calculate size based on subclass definition
    sf::Vector2f size = getSize();
    size.x *= getScaleFactor();
    size.y *= getScaleFactor();
    
    // Setup the shape
    mShape.setPosition(mPosition);
    mShape.setSize(size);  
    mShape.setFillColor(sf::Color(200, 200, 200));
    mShape.setOutlineThickness(2.f);
    mShape.setOutlineColor(sf::Color::Black);
    
    // Center the shape on its position
    sf::FloatRect bounds = mShape.getLocalBounds();
    mShape.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
}

void Building::setPosition(const sf::Vector2f& position) {
    mPosition = position;
    mShape.setPosition(mPosition);
}

void Building::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
} 
