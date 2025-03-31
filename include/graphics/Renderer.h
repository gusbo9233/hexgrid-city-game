#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "HexGrid.h"
#include "../GameObject.h"

class Renderer {
public:
    Renderer(sf::RenderWindow& window);
    
    // Main render function
    void render(const HexGrid& grid);
    
    // Generic render function for any GameObject
    void render(const GameObject& gameObject);
    
    // Clear the screen with background color
    void clear();
    
    // Display the rendered frame
    void display();
    
    // Set the view/camera
    void setView(const sf::View& view);
    
    // Render settings for fog of war
    void setFogOfWarEnabled(bool enabled) { mFogOfWarEnabled = enabled; }
    bool isFogOfWarEnabled() const { return mFogOfWarEnabled; }
    
    // Color for invisible (fog of war) areas
    void setFogOfWarColor(const sf::Color& color) { mUnexploredColor = color; }
    
    // Render a hex based on its visibility state
    void renderHex(sf::RenderWindow& window, const Hexagon* hex);

private:
    sf::RenderWindow& mWindow;
    sf::Color mBackgroundColor;
    
    // Fog of war settings
    bool mFogOfWarEnabled = true;
    sf::Color mUnexploredColor = sf::Color(20, 20, 20, 255);  // Black for non-visible areas
    
    // Render a fog of war overlay
    void renderFogOfWar(sf::RenderWindow& window, const Hexagon* hex);
};

#endif // RENDERER_H 