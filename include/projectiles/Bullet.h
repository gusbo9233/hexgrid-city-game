#ifndef BULLET_H
#define BULLET_H

#include "Projectile.h"

class Bullet : public Projectile {
    public:
        Bullet(int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance);
        void update() override;

    protected:
        float getScaleFactor() const override;

    private:
        static constexpr float BULLET_SPEED = 1.0f;
        static constexpr int BULLET_DAMAGE = 20;
        static constexpr const char* BULLET_TEXTURE = "assets/images/projectiles/bullet.png";
};

#endif // BULLET_H
