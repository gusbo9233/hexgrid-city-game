#include "../../include/projectiles/projectile.h"

Projectile::Projectile(int damage, int speed, int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance)
    : damage(damage), speed(speed), xPos(xPos), yPos(yPos), xMagnitude(xMagnitude), yMagnitude(yMagnitude), allegiance(allegiance) {
    // Initialize sprite
}

void Projectile::update() {
    // Update position
    xPos += xMagnitude * speed;
    yPos += yMagnitude * speed;
}

void Projectile::draw(sf::RenderWindow& window) {
    // Draw sprite
}
