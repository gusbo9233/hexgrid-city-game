#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "Hexagon.h"

// Forward declarations
class Game;

class InputHandler {
public:
    InputHandler(sf::RenderWindow& window);
    
    // Process all inputs for the current frame
    void processInputs(Game& game, float deltaTime);
    
    // Camera movement 
    sf::Vector2f getCameraMovement(float deltaTime) const;
    
    // Keyboard handlers
    void handleKeyPress(sf::Keyboard::Key key, Game& game);

private:
    sf::RenderWindow& mWindow;
    float mCameraSpeed;
    
    // Movement flags for continuous input
    bool mMovingUp;
    bool mMovingDown;
    bool mMovingLeft;
    bool mMovingRight;
};

#endif // INPUT_HANDLER_H 