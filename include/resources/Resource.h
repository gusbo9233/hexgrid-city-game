#ifndef RESOURCE_H
#define RESOURCE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "ResourceType.h"

// Base class for all resources
class Resource {
public:
    Resource();
    virtual ~Resource() = default;
    
    // Position methods
    virtual void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const { return mPosition; }
    
    // Type identification
    virtual ResourceType getType() const = 0;
    bool isResourceType(ResourceType type) const { return getType() == type; }
    
    // Non-Virtual Interface pattern for rendering
    void render(sf::RenderWindow& window) const;
    
    // Visibility range methods
    int getVisibilityRange() const { return mVisibilityRange; }
    void setVisibilityRange(int range) { mVisibilityRange = range; }
    
    // Sprite access for rendering
    const sf::Sprite* getSprite() const { return mSprite.get(); }
    
    // Method to load a texture from a file path
    bool loadTexture(const std::string& path);
    
protected:
    sf::Vector2f mPosition;
    int mVisibilityRange;
    
    // Image and sprite handling
    sf::Texture mTexture;
    std::unique_ptr<sf::Sprite> mSprite;
    
    // Resource appearance settings - override in derived classes
    virtual float getScaleFactor() const { return 0.5f; }
    virtual sf::Vector2f getSize() const { return {25.0f, 25.0f}; }
    
    // NVI implementation - can be overridden by derived classes
    virtual void doRender(sf::RenderWindow& window) const;
};

#endif // RESOURCE_H 