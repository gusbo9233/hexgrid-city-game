#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "HexGrid.h"
#include "Character.h"

class Renderer {
public:
    Renderer(sf::RenderWindow& window);
    
    // Main render function
    void render(const HexGrid& grid);
    
    // Render a character
    void render(const Character& character);
    
    // Clear the screen with background color
    void clear();
    
    // Display the rendered frame
    void display();
    
    // Set the view/camera
    void setView(const sf::View& view);

private:
    sf::RenderWindow& mWindow;
    sf::Color mBackgroundColor;
};

#endif // RENDERER_H 