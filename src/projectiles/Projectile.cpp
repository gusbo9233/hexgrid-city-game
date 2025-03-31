#include "../../include/projectiles/Projectile.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Projectile::Projectile(int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance)
    : GameObject(static_cast<float>(xPos), static_cast<float>(yPos), allegiance),
      xMagnitude(xMagnitude), yMagnitude(yMagnitude),
      damage(0), speed(1.0f) {
    // Base initialization - no textures/shapes are loaded here
    // Derived classes should handle their own appearance
}

void Projectile::update() {
    // Print position and movement info
    std::cout << "Projectile position: " << mXPos << ", " << mYPos << std::endl;
    std::cout << "Projectile magnitude: " << xMagnitude << ", " << yMagnitude << std::endl;
    std::cout << "Projectile speed: " << speed << std::endl;
    
    // Update position based on magnitude and speed
    mXPos += static_cast<float>(xMagnitude * speed);
    mYPos += static_cast<float>(yMagnitude * speed);
    
    // Update sprite position
    setPosition(sf::Vector2f(mXPos, mYPos));
}

void Projectile::setPosition(const sf::Vector2f& position) {
    // Override to ensure both GameObject's position and our position are updated
    GameObject::setPosition(position);
}

Projectile::Projectile(Projectile&& other) noexcept
    : GameObject(std::move(other)),
      damage(other.damage),
      speed(other.speed),
      xMagnitude(other.xMagnitude),
      yMagnitude(other.yMagnitude) {
}

Projectile& Projectile::operator=(Projectile&& other) noexcept {
    if (this != &other) {
        GameObject::operator=(std::move(other));
        damage = other.damage;
        speed = other.speed;
        xMagnitude = other.xMagnitude;
        yMagnitude = other.yMagnitude;
    }
    return *this;
}
