#include "Player.h"

Player::Player() : mSpeed(200.f) {
    mShape.setSize({50.f, 50.f});
    mShape.setFillColor(sf::Color::Red);
    mShape.setPosition({400.f, 300.f});
    mVelocity = {0.f, 0.f};
}

void Player::update(float deltaTime) {
    mShape.move(mVelocity * deltaTime);
    
    // Keep player within window bounds
    sf::Vector2f position = mShape.getPosition();
    if (position.x < 0) {
        position.x = 0;
    } else if (position.x > 800 - mShape.getSize().x) {
        position.x = 800 - mShape.getSize().x;
    }
    
    if (position.y < 0) {
        position.y = 0;
    } else if (position.y > 600 - mShape.getSize().y) {
        position.y = 600 - mShape.getSize().y;
    }
    
    mShape.setPosition(position);
}

void Player::handleInput() {
    mVelocity = {0.f, 0.f};
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        mVelocity.y = -mSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        mVelocity.y = mSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        mVelocity.x = -mSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        mVelocity.x = mSpeed;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(mShape);
} 