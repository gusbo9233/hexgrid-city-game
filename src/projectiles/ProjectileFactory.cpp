#include "projectiles/ProjectileFactory.h"
#include "projectiles/Bullet.h"
#include "projectiles/TankAmmo.h"

ProjectileFactory::ProjectileFactory() {
    // Initialize sprite
}

Projectile ProjectileFactory::createProjectile(ProjectileType type, int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance) {
    switch (type) {
        case ProjectileType::BULLET:
            return Bullet(xPos, yPos, xMagnitude, yMagnitude, allegiance);
        case ProjectileType::TANK_AMMO:
            return TankAmmo(xPos, yPos, xMagnitude, yMagnitude, allegiance);
        default:
            return Bullet(xPos, yPos, xMagnitude, yMagnitude, allegiance); // Default to bullet if unknown
    }
}
