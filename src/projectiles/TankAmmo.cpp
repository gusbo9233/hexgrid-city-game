#include "../../include/projectiles/TankAmmo.h"
#include <iostream>
#include <filesystem>

TankAmmo::TankAmmo(int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance)
    : Projectile(xPos, yPos, xMagnitude, yMagnitude, allegiance) {
    // Set tank ammo-specific properties
    speed = TANK_AMMO_SPEED;
    damage = TANK_AMMO_DAMAGE;
    
    // Try to load the tank ammo texture
    bool textureLoaded = false;
    if (std::filesystem::exists(TANK_AMMO_TEXTURE)) {
        textureLoaded = loadTexture(TANK_AMMO_TEXTURE);
        if (textureLoaded) {
            std::cout << "Loaded tank ammo texture from " << TANK_AMMO_TEXTURE << std::endl;
        }
    }
    
    // Fall back to orange circle if texture loading failed
    if (!textureLoaded) {
        createShape(sf::Color(255, 165, 0), true); // Orange color
        std::cout << "Created tank ammo with orange circle (texture not found)" << std::endl;
    }
    
    // Set size and origin based on tank ammo configuration
    setSize(getSize(), getScaleFactor());
}

void TankAmmo::update() {
    // Call base class update to handle movement
    Projectile::update();
    
    // Tank ammo specific update logic can be added here
    // For example, we could add smoke trail or explosion effects
}

float TankAmmo::getScaleFactor() const {
    return 4.0f;  // Even bigger than bullets to represent tank shells
} 