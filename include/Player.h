#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
private:
    sf::RectangleShape mShape;
    sf::Vector2f mVelocity;
    float mSpeed;

public:
    Player();
    void update(float deltaTime);
    void handleInput();
    void draw(sf::RenderWindow& window);
};

#endif // PLAYER_H 