#ifndef RESOURCE_H
#define RESOURCE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "ResourceType.h"
#include "GameObject.h"

// Base class for all resources
class Resource : public GameObject {
public:
    Resource();
    virtual ~Resource() = default;
    
    // Position methods - inherited from GameObject
    void setPosition(const sf::Vector2f& position) override;
    
    // Type identification
    virtual ResourceType getType() const = 0;
    bool isResourceType(ResourceType type) const { return getType() == type; }
    
    // Visibility range methods
    int getVisibilityRange() const { return mVisibilityRange; }
    void setVisibilityRange(int range) { mVisibilityRange = range; }
    
    // Sprite access for rendering
    const sf::Sprite* getSprite() const { return mSprite.get(); }
    
    // Method to load a texture from a file path
    bool loadTexture(const std::string& path);
    
protected:
    int mVisibilityRange = 1;
    
    // Resource appearance settings - override in GameObject methods
    float getScaleFactor() const override { return 0.5f; }
    sf::Vector2f getSize() const override { return {25.0f, 25.0f}; }
    
    // Override the render implementation if needed
    void doRender(sf::RenderWindow& window) const override;
    
    // Image and sprite handling
    sf::Texture mTexture;
    std::unique_ptr<sf::Sprite> mSprite;
};

#endif // RESOURCE_H 