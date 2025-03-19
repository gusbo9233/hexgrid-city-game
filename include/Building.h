#ifndef BUILDING_H
#define BUILDING_H
#include <SFML/Graphics.hpp>

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
        
        // Standard size for buildings (relative to hex size)
        static constexpr float STANDARD_SIZE = 25.0f;
        
        // Each building type should override these to define its appearance
        virtual float getScaleFactor() const = 0;
        virtual sf::Vector2f getSize() const { return {STANDARD_SIZE, STANDARD_SIZE}; }
        
        sf::RectangleShape mShape;
        sf::Vector2f mPosition;
};

#endif