#ifndef TANK_AMMO_H
#define TANK_AMMO_H

#include "Projectile.h"

class TankAmmo : public Projectile {
    public:
        TankAmmo(int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance);
        void update() override;

    protected:
        float getScaleFactor() const override;

    private:
        static constexpr float TANK_AMMO_SPEED = 0.7f;  // Slower than bullet
        static constexpr int TANK_AMMO_DAMAGE = 50;     // Higher damage than bullet
        static constexpr const char* TANK_AMMO_TEXTURE = "assets/images/projectiles/tank_ammo.png";
};

#endif // TANK_AMMO_H