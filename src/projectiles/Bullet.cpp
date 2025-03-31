#include "../../include/projectiles/Bullet.h"
#include <iostream>
#include <filesystem>

Bullet::Bullet(int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance)
    : Projectile(xPos, yPos, xMagnitude, yMagnitude, allegiance) {
    // Set bullet-specific properties
    speed = BULLET_SPEED;
    damage = BULLET_DAMAGE;
    
    // Try to load the bullet texture
    bool textureLoaded = false;
    if (std::filesystem::exists(BULLET_TEXTURE)) {
        textureLoaded = loadTexture(BULLET_TEXTURE);
        if (textureLoaded) {
            std::cout << "Loaded bullet texture from " << BULLET_TEXTURE << std::endl;
        }
    }
    
    // Fall back to yellow circle if texture loading failed
    if (!textureLoaded) {
        createShape(sf::Color::Yellow, true);
        std::cout << "Created bullet with yellow circle (texture not found)" << std::endl;
    }
    
    // Set size and origin based on bullet configuration
    setSize(getSize(), getScaleFactor());
}

void Bullet::update() {
    // Call base class update to handle movement
    Projectile::update();
}

float Bullet::getScaleFactor() const {
    return 3.0f;  // Increased from 0.8f to 3.0f to make bullets much bigger
}
