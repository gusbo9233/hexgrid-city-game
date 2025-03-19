#ifndef BUILDING_H
#define BUILDING_H
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

// Forward declaration
class Hexagon;

class Building {
    public:
        Building(const sf::Vector2f& position);
        virtual ~Building() = default;
        void draw(sf::RenderWindow& window) const;
        
        // Update position
        void setPosition(const sf::Vector2f& position);
        sf::Vector2f getPosition() const { return mPosition; }
    
    protected:
        // Initialize the shape - should be called by subclasses after construction
        void initializeShape();
        
        // Try to load image, fall back to shape if not found
        bool tryLoadTexture(const std::string& imagePath);
        
        // Standard size for buildings (relative to hex size)
        static constexpr float STANDARD_SIZE = 25.0f;
        
        // Each building type should override these to define its appearance
        virtual float getScaleFactor() const = 0;
        virtual sf::Vector2f getSize() const { return {STANDARD_SIZE, STANDARD_SIZE}; }
        virtual std::string getImagePath() const { return ""; } // Default empty, subclasses can override
        
        sf::RectangleShape mShape;
        sf::Vector2f mPosition;
        std::unique_ptr<sf::Texture> mTexture;
        bool mHasTexture;
};

#endif