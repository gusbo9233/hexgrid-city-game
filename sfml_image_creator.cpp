#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Create image for oil
    sf::Image oilImage;
    // Use resize instead of create for SFML 3.0
    oilImage.resize({64, 64});
    
    // Fill with transparent first
    for (unsigned int x = 0; x < 64; ++x) {
        for (unsigned int y = 0; y < 64; ++y) {
            oilImage.setPixel({x, y}, sf::Color::Transparent);
        }
    }
    
    // Draw a black circle for oil
    for (unsigned int x = 0; x < 64; ++x) {
        for (unsigned int y = 0; y < 64; ++y) {
            float dx = x - 32.0f;
            float dy = y - 32.0f;
            float distance = std::sqrt(dx*dx + dy*dy);
            
            if (distance < 24.0f) {
                // Black oil color
                sf::Color oilColor = sf::Color(10, 10, 10);
                
                // Add highlight
                if (x > 20 && x < 40 && y > 15 && y < 25) {
                    oilColor = sf::Color(60, 60, 60);
                }
                
                oilImage.setPixel({x, y}, oilColor);
            }
        }
    }
    
    // Save to file
    if (oilImage.saveToFile("assets/images/oil.png")) {
        std::cout << "Created oil.png successfully\n";
        return 0;
    } else {
        std::cerr << "Failed to save oil.png\n";
        return 1;
    }
} 