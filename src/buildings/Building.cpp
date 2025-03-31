#include "../../include/buildings/Building.h"
#include "Hexagon.h"
#include <iostream>

Building::Building(const sf::Vector2f& position, Allegiance allegiance)
    : GameObject(position.x, position.y, allegiance), mHasTexture(false) {
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
    sf::Vector2f position = getPosition();
    mShape.setPosition(position);
    mShape.setSize(size);
    
    if (mHasTexture) {
        // No outline for textured buildings
        mShape.setOutlineThickness(0.f);
        
        // The actual rendering will be done by GameObject's drawing functionality
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
    // Use GameObject's loadTexture method
    return loadTexture(imagePath);
}

// Default implementation of doRender
void Building::doRender(sf::RenderWindow& window) const {
    // Let the GameObject handle sprite drawing, this is just for non-textured fallback
    if (!mHasTexture) {
        window.draw(mShape);
    } else {
        // Call parent class implementation to render the sprite
        GameObject::doRender(window);
    }
}

void Building::takeDamage(int damage) {
    // First reduce defenses, then health
    if (defenses > 0) {
        defenses -= damage;
        if (defenses < 0) {
            // If damage exceeds defenses, carry over to health
            health += defenses; // defenses is negative here
            defenses = 0;
        }
    } else {
        // No defenses left, damage health directly
        health -= damage;
    }
    
    // Health can't go below 0
    if (health < 0) {
        health = 0;
    }
    
    std::cout << "Building damaged! Health: " << health << ", Defenses: " << defenses << std::endl;
}

void Building::repair(int amount) {
    // First repair health, then defenses
    if (health < maxHealth) {
        health += amount;
        if (health > maxHealth) {
            // If repair exceeds max health, carry over to defenses
            int excess = health - maxHealth;
            health = maxHealth;
            defenses += excess;
            if (defenses > maxDefenses) {
                defenses = maxDefenses;
            }
        }
    } else {
        // Health is full, repair defenses
        defenses += amount;
        if (defenses > maxDefenses) {
            defenses = maxDefenses;
        }
    }
} 
