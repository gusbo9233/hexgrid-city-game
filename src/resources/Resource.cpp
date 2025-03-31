#include "resources/Resource.h"
#include <iostream>
#include <filesystem>

Resource::Resource() 
    : GameObject(0.0f, 0.0f) {
    // Default resource initialization
}

void Resource::setPosition(const sf::Vector2f& position) {
    // Use GameObject position setter
    GameObject::setPosition(position);
}

bool Resource::loadTexture(const std::string& path) {
    // Use GameObject's loadTexture method
    return GameObject::loadTexture(path);
}

void Resource::doRender(sf::RenderWindow& window) const {
    // Default implementation - use GameObject's renderer or custom placeholder
    if (hasSprite()) {
        GameObject::doRender(window);
    } else {
        // Draw a placeholder if no sprite is available
        sf::RectangleShape shape({20.f, 20.f});
        shape.setFillColor(sf::Color::Magenta);
        shape.setPosition(getPosition());
        shape.setOrigin({10.f, 10.f}); // Center origin
        window.draw(shape);
    }
} 