#ifndef BUILDING_H
#define BUILDING_H
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "BuildingType.h"
#include "Allegiance.h"
#include "GameObject.h"

// Forward declaration
class Hexagon;

class Building : public GameObject {
    public:
        Building(const sf::Vector2f& position, Allegiance allegiance = Allegiance::FRIENDLY);
        virtual ~Building() = default;
        
        // Type identification - pure virtual
        virtual BuildingType getType() const = 0;
        bool isBuildingType(BuildingType type) const { return getType() == type; }
        
        // Virtual method for custom behavior
        virtual void update(float deltaTime) {}
        
        // Visibility range methods
        int getVisibilityRange() const { return mVisibilityRange; }
        void setVisibilityRange(int range) { mVisibilityRange = range; }
        
        // Health and defenses methods
        int getHealth() const { return health; }
        int getDefenses() const { return defenses; }
        int getMaxHealth() const { return maxHealth; }
        int getMaxDefenses() const { return maxDefenses; }
        void takeDamage(int damage);
        void repair(int amount);
        
        // Get render size
        sf::Vector2f getRenderSize() const { return mShape.getSize(); }
    
    protected:
        // Initialize the shape - should be called by subclasses after construction
        void initializeShape();
        
        // Try to load image, fall back to shape if not found
        bool tryLoadTexture(const std::string& imagePath);
        
        // Standard size for buildings (relative to hex size)
        static constexpr float STANDARD_SIZE = 25.0f;
        
        // Each building type should override these to define its appearance
        virtual float getScaleFactor() const override = 0;
        virtual sf::Vector2f getSize() const override { return {STANDARD_SIZE, STANDARD_SIZE}; }
        virtual std::string getImagePath() const { return ""; }
        
        // Override render implementation
        void doRender(sf::RenderWindow& window) const override;
        
        // Rectangle shape for non-textured shapes
        sf::RectangleShape mShape;
        
        bool mHasTexture;
        
        // Default visibility range - can be overridden by specific building types
        int mVisibilityRange = 2;

        // Health and defenses
        int defenses = 100;
        int health = 100;
        int maxHealth = 100;
        int maxDefenses = 100;
};

#endif