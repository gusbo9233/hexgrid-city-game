#include "Character.h"
#include <iostream>
#include <filesystem>

Character::Character(int q, int r) : mQ(q), mR(r), health(100), maxHealth(100), 
    attackPower(10), defensePower(5), speed(1), range(1), mSprite(nullptr) {
    // Default character initialization - sprite will be created when texture is set
}

bool Character::loadTexture(const std::string& path) {
    if (!mTexture.loadFromFile(path)) {
        // Create a fallback red square texture if texture loading fails
        sf::Image img;
        img.resize({32, 32});
        for (unsigned int i = 0; i < 32; ++i) {
            for (unsigned int j = 0; j < 32; ++j) {
                img.setPixel({i, j}, sf::Color::Red);
            }
        }
        if (!mTexture.loadFromImage(img)) {
            return false;
        }
    }
    
    // Create sprite using the loaded texture
    mSprite = std::make_unique<sf::Sprite>(mTexture);
    
    // Center the sprite origin
    sf::FloatRect bounds = mSprite->getLocalBounds();
    mSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    
    // Scale the sprite to match the character's intended size
    sf::Vector2f characterSize = getSize();
    float scaleX = (characterSize.x * getScaleFactor()) / bounds.size.x;
    float scaleY = (characterSize.y * getScaleFactor()) / bounds.size.y;
    mSprite->setScale({scaleX, scaleY});
    
    return true;
}

void Character::setPosition(const sf::Vector2f& pixelPos) {
    if (mSprite) {
        mSprite->setPosition(pixelPos);
    }
}

// NVI pattern implementation for render
void Character::render(sf::RenderWindow& window) const {
    // Common pre-rendering operations could go here
    doRender(window);
    // Common post-rendering operations could go here
}

// Default implementation of doRender
void Character::doRender(sf::RenderWindow& window) const {
    // Only draw if we have a valid sprite
    if (mSprite) {
        window.draw(*mSprite);
    }
}

void Character::setHexCoord(int q, int r) {
    mQ = q;
    mR = r;
}

void Character::setHexCoord(const Hexagon::CubeCoord& coord) {
    mQ = coord.q;
    mR = coord.r;
}