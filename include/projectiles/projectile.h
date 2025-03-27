#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Allegiance.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Projectile {
    public:
        Projectile(int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance);
        void update();
        void draw(sf::RenderWindow& window);
    private:
        int damage;
        int speed;
        int xPos;
        int yPos;
        double xMagnitude;
        double yMagnitude;
        Allegiance allegiance;
        sf::Image mImage;
        sf::Texture mTexture;
        std::unique_ptr<sf::Sprite> mSprite;
};

#endif // PROJECTILE_H
