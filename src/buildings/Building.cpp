#include "../../include/buildings/Building.h"
#include "Hexagon.h"
#include <iostream>

Building::Building(const sf::Vector2f& position, Allegiance allegiance)
    : mPosition(position), mHasTexture(false), mAllegiance(allegiance) {
    // We'll initialize the shape in initializeShape() which should be called by derived classes
    // in their constructors after the Building constructor finishes
}

// This method should be called by subclasses after construction
void Building::initializeShape() {
    // Try to load the texture if path is provided
    std::string imagePath = getImagePath();
    if (!imagePath.empty()) {
        mHasTexture = tryLoadTexture(imagePath);
    }
    
    // Calculate size based on subclass definition
    sf::Vector2f size = getSize();
    size.x *= getScaleFactor();
    size.y *= getScaleFactor();
    
    // Setup the shape
    mShape.setPosition(mPosition);
    mShape.setSize(size);
    
    if (mHasTexture && mTexture) {
        mShape.setTexture(mTexture.get());
        // No outline for textured buildings
        mShape.setOutlineThickness(0.f);
    } else {
        // Fallback to colored square with outline
        mShape.setFillColor(sf::Color(200, 200, 200));
        mShape.setOutlineThickness(2.f);
        mShape.setOutlineColor(sf::Color::Black);
    }
    
    // Center the shape on its position
    sf::FloatRect bounds = mShape.getLocalBounds();
    mShape.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
}

bool Building::tryLoadTexture(const std::string& imagePath) {
    mTexture = std::make_unique<sf::Texture>();
    if (!mTexture->loadFromFile(imagePath)) {
        std::cerr << "Failed to load texture: " << imagePath << std::endl;
        mTexture.reset();
        return false;
    }
    return true;
}

void Building::setPosition(const sf::Vector2f& position) {
    mPosition = position;
    mShape.setPosition(mPosition);
}

// Non-Virtual Interface (NVI) pattern implementation
void Building::draw(sf::RenderWindow& window) const {
    // Common pre-processing could go here
    doDraw(window);
    // Common post-processing could go here
}

// Default implementation of doDraw
void Building::doDraw(sf::RenderWindow& window) const {
    window.draw(mShape);
} 
