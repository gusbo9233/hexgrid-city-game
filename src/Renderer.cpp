#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& window)
    : mWindow(window),
      mBackgroundColor(sf::Color(30, 30, 30)) {
}

void Renderer::render(const HexGrid& grid) {
    // Render the grid
    grid.draw(mWindow);
}

void Renderer::render(const Character& character) {
    // Render the character
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