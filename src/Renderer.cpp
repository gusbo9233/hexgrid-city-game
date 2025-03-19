#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& window)
    : mWindow(window),
      mBackgroundColor(sf::Color(30, 30, 30)) {
}

void Renderer::render(const HexGrid& grid) {
    // Instead of directly rendering the grid, we'll handle each hex individually
    // to apply fog of war effects
    const auto& hexagons = grid.getHexagons();
    
    for (const auto& [coord, hex] : hexagons) {
        renderHex(mWindow, hex.get());
    }
}

void Renderer::render(const Character& character) {
    // Only render the character if it's on a visible hex
    const Hexagon::CubeCoord& charCoord = character.getHexCoord();
    // We would need a way to check if this hex is visible
    // For now, let's assume characters are only rendered if they're in visible areas
    character.render(mWindow);
}

void Renderer::clear() {
    mWindow.clear(mBackgroundColor);
}

void Renderer::display() {
    mWindow.display();
}

void Renderer::setView(const sf::View& view) {
    mWindow.setView(view);
}

void Renderer::renderHex(sf::RenderWindow& window, const Hexagon* hex) {
    if (!mFogOfWarEnabled) {
        // If fog of war is disabled, render everything normally
        hex->draw(window);
        return;
    }
    
    if (hex->isVisible()) {
        // Fully visible - render normally
        hex->draw(window);
    } 
    else {
        // For non-visible hexes, show only black fog
        renderFogOfWar(window, hex);
    }
}

void Renderer::renderFogOfWar(sf::RenderWindow& window, const Hexagon* hex) {
    // Create a hexagon shape that matches the hex
    sf::ConvexShape shape;
    shape.setPointCount(6);
    
    // Get the hex position
    sf::Vector2f position = hex->getPosition();
    
    // Hexagon dimensions (should match the ones in Hexagon class)
    float size = 25.0f; // Match the SIZE in Hexagon
    
    // Define the points of the hexagon
    for (int i = 0; i < 6; ++i) {
        float angle = i * 60.0f + 30.0f; // 30 degrees offset to point upward
        float rads = angle * 3.14159f / 180.0f;
        float x = position.x + size * cos(rads);
        float y = position.y + size * sin(rads);
        shape.setPoint(i, sf::Vector2f(x, y));
    }
    
    // Set the fill color for non-visible areas
    shape.setFillColor(mUnexploredColor);
    
    // Draw the fog overlay
    window.draw(shape);
} 