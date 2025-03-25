#include "resources/Resource.h"
#include <iostream>
#include <filesystem>

Resource::Resource() : mPosition(0.f, 0.f) {
    // Default resource initialization
}

void Resource::setPosition(const sf::Vector2f& position) {
    mPosition = position;
    
    // Update sprite position if it exists
    if (mSprite) {
        mSprite->setPosition(position);
    }
}

bool Resource::loadTexture(const std::string& path) {
    // Check if file exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "Warning: Could not find texture file: " << path << std::endl;
    }

    if (!mTexture.loadFromFile(path)) {
        // Create a fallback colored square texture if texture loading fails
        sf::Image img;
        img.resize({32, 32});
        for (unsigned int i = 0; i < 32; ++i) {
            for (unsigned int j = 0; j < 32; ++j) {
                img.setPixel({i, j}, sf::Color::Magenta);
            }
        }
        
        if (!mTexture.loadFromImage(img)) {
            std::cerr << "Error: Failed to create fallback texture" << std::endl;
            return false;
        }
    }
    
    // Create sprite using the loaded texture
    mSprite = std::make_unique<sf::Sprite>(mTexture);
    
    // Center the sprite origin
    sf::FloatRect bounds = mSprite->getLocalBounds();
    mSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    
    // Scale the sprite to match the intended size
    sf::Vector2f objectSize = getSize();
    float scaleX = (objectSize.x * getScaleFactor()) / bounds.size.x;
    float scaleY = (objectSize.y * getScaleFactor()) / bounds.size.y;
    mSprite->setScale({scaleX, scaleY});
    
    // Set the sprite's position to match the position
    mSprite->setPosition(mPosition);
    
    return true;
}

// NVI pattern implementation for render
void Resource::render(sf::RenderWindow& window) const {
    // Common pre-rendering operations could go here
    doRender(window);
    // Common post-rendering operations could go here
}

void Resource::doRender(sf::RenderWindow& window) const {
    // Default implementation just draws the sprite if it exists
    if (mSprite) {
        window.draw(*mSprite);
    } else {
        // Draw a placeholder if no sprite is available
        sf::RectangleShape shape({20.f, 20.f});
        shape.setFillColor(sf::Color::Magenta);
        shape.setPosition(mPosition);
        shape.setOrigin({10.f, 10.f}); // Center origin
        window.draw(shape);
    }
} 