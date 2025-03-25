#include "resources/Oil.h"
#include <iostream>

Oil::Oil() : Resource() {
    // Load oil texture
    if (!loadTexture(OIL_TEXTURE)) {
        std::cerr << "Failed to load oil texture" << std::endl;
    }
}

// Override for custom oil rendering if needed
void Oil::doRender(sf::RenderWindow& window) const {
    if (mSprite) {
        window.draw(*mSprite);
    } else {
        // Draw a custom placeholder for oil if texture fails
        sf::CircleShape shape(15.f);
        shape.setFillColor(sf::Color(0, 0, 0)); // Black like oil
        shape.setOutlineThickness(2.f);
        shape.setOutlineColor(sf::Color(30, 30, 30)); // Dark grey outline
        shape.setPosition(mPosition);
        shape.setOrigin({15.f, 15.f}); // Center origin
        window.draw(shape);
    }
}

// The getScaleFactor and getSize methods are already defined in the header file
// Removing duplicate implementations 