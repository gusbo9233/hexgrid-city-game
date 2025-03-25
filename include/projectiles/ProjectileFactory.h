#ifndef PROJECTILE_FACTORY_H
#define PROJECTILE_FACTORY_H

#include "projectiles/ProjectileType.h"
#include "Allegiance.h"
#include "projectiles/Projectile.h"
class ProjectileFactory {
    public:
        ProjectileFactory();
        Projectile createProjectile(ProjectileType type, int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance);
};

#endif // PROJECTILE_FACTORY_H