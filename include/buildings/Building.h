#ifndef BUILDING_H
#define BUILDING_H
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "BuildingType.h"
#include "Allegiance.h"

// Forward declaration
class Hexagon;

class Building {
    public:
        Building(const sf::Vector2f& position, Allegiance allegiance = Allegiance::FRIENDLY);
        virtual ~Building() = default;
        
        // Non-Virtual Interface pattern for draw
        void draw(sf::RenderWindow& window) const;
        
        // Update position
        void setPosition(const sf::Vector2f& position);
        sf::Vector2f getPosition() const { return mPosition; }
        
        // Type identification - pure virtual
        virtual BuildingType getType() const = 0;
        bool isBuildingType(BuildingType type) const { return getType() == type; }
        
        // Allegiance methods
        Allegiance getAllegiance() const { return mAllegiance; }
        void setAllegiance(Allegiance allegiance) { mAllegiance = allegiance; }
        bool isFriendly() const { return mAllegiance == Allegiance::FRIENDLY; }
        bool isEnemy() const { return mAllegiance == Allegiance::ENEMY; }
        
        // Virtual method for custom behavior
        virtual void update(float deltaTime) {}
        
        // Visibility range methods
        int getVisibilityRange() const { return mVisibilityRange; }
        void setVisibilityRange(int range) { mVisibilityRange = range; }
    
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
        virtual std::string getImagePath() const { return ""; }
        
        // NVI implementation - can be overridden for custom drawing
        virtual void doDraw(sf::RenderWindow& window) const;
        
        sf::RectangleShape mShape;
        sf::Vector2f mPosition;
        std::unique_ptr<sf::Texture> mTexture;
        bool mHasTexture;
        
        // Allegiance - default to friendly
        Allegiance mAllegiance = Allegiance::FRIENDLY;
        
        // Default visibility range - can be overridden by specific building types
        int mVisibilityRange = 2;
};

#endif