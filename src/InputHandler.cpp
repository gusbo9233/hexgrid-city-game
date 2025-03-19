#include "InputHandler.h"
#include "Game.h"
#include <cmath>

InputHandler::InputHandler(sf::RenderWindow& window)
    : mWindow(window), 
      mCameraSpeed(400.f),
      mMovingUp(false),
      mMovingDown(false),
      mMovingLeft(false),
      mMovingRight(false) {
}

void InputHandler::processInputs(Game& game, float deltaTime) {
    // Process SFML events
    std::optional<sf::Event> event;
    while ((event = mWindow.pollEvent())) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent) {
                sf::Vector2f worldPos = mWindow.mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});
                
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    // Handle left click
                    game.onLeftClick(worldPos);
                }
                else if (mouseEvent->button == sf::Mouse::Button::Right) {
                    // Handle right click
                    game.onRightClick(worldPos);
                }
            }
        }
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (keyEvent) {
                // Update movement flags
                if (keyEvent->code == sf::Keyboard::Key::W) mMovingUp = true;
                if (keyEvent->code == sf::Keyboard::Key::S) mMovingDown = true;
                if (keyEvent->code == sf::Keyboard::Key::A) mMovingLeft = true;
                if (keyEvent->code == sf::Keyboard::Key::D) mMovingRight = true;
                
                // Process other key presses
                handleKeyPress(keyEvent->code, game);
            }
        }
        else if (event->is<sf::Event::KeyReleased>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyReleased>();
            if (keyEvent) {
                // Update movement flags
                if (keyEvent->code == sf::Keyboard::Key::W) mMovingUp = false;
                if (keyEvent->code == sf::Keyboard::Key::S) mMovingDown = false;
                if (keyEvent->code == sf::Keyboard::Key::A) mMovingLeft = false;
                if (keyEvent->code == sf::Keyboard::Key::D) mMovingRight = false;
            }
        }
    }
}

sf::Vector2f InputHandler::getCameraMovement(float deltaTime) const {
    // Calculate camera movement based on input flags
    sf::Vector2f movement(0.f, 0.f);
    
    if (mMovingUp) {
        // do not go above the highest hex in the grid
        movement.y -= 1.0f;
    }
    if (mMovingDown) {
        // do not go below the lowest hex in the grid
        movement.y += 1.0f;
    }
    if (mMovingLeft) {
        // do not go left of the left edge of the grid
        movement.x -= 1.0f;
    }
    if (mMovingRight) {
        // do not go right of the right edge of the grid
        movement.x += 1.0f;
    }
    
    // Normalize diagonal movement
    if (movement.x != 0.f && movement.y != 0.f) {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement.x /= length;
        movement.y /= length;
    }
    
    // Apply camera speed and delta time
    movement *= mCameraSpeed * deltaTime;
    
    return movement;
}

void InputHandler::handleKeyPress(sf::Keyboard::Key key, Game& game) {
    // Let the game handle non-movement key presses
    if (key == sf::Keyboard::Key::Q || 
        key == sf::Keyboard::Key::R || 
        key == sf::Keyboard::Key::Escape) {
        game.onKeyPress(key);
    }
} 