#include "projectiles/Bullet.h"

Bullet::Bullet(int damage, int speed, int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance)
    : Projectile(damage, speed, xPos, yPos, xMagnitude, yMagnitude, allegiance) {
    // Initialize sprite
}
