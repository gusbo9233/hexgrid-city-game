#include "projectiles/ProjectileFactory.h"

ProjectileFactory::ProjectileFactory() {
    // Initialize sprite
}

Projectile ProjectileFactory::createProjectile(ProjectileType type, int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance) {
    switch (type) {
        case ProjectileType::BULLET:
            return Bullet(xPos, yPos, xMagnitude, yMagnitude, allegiance);
    }
}
