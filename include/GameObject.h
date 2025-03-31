#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Allegiance.h"
#include "graphics/TextureManager.h"

class GameObject {
public:
    GameObject(float xPos = 0.0f, float yPos = 0.0f, Allegiance allegiance = Allegiance::NEUTRAL);
    virtual ~GameObject() = default;
    
    // Move constructor and assignment operator
    GameObject(GameObject&& other) noexcept;
    GameObject& operator=(GameObject&& other) noexcept;
    
    // Delete copy constructor and assignment operator
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    
    // Position methods
    virtual void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    
    // Allegiance methods
    Allegiance getAllegiance() const { return mAllegiance; }
    void setAllegiance(Allegiance allegiance) { mAllegiance = allegiance; }
    bool isFriendly() const { return mAllegiance == Allegiance::FRIENDLY; }
    bool isEnemy() const { return mAllegiance == Allegiance::ENEMY; }
    
    // Rendering
    virtual void render(sf::RenderWindow& window) const;
    
    // Graphics methods
    bool loadTexture(const std::string& path);
    void createShape(sf::Color color, bool isCircle = true);
    
    // Sprite access
    sf::Sprite* getSprite() { return mSprite.get(); }
    const sf::Sprite* getSprite() const { return mSprite.get(); }
    
    // Size and scale
    virtual float getScaleFactor() const { return 1.0f; }
    virtual sf::Vector2f getSize() const { return {25.0f, 25.0f}; }
    void setSize(const sf::Vector2f& size, float scaleFactor = 1.0f);
    void setOrigin(const sf::Vector2f& origin);
    
    // Bounding box for collision detection
    sf::FloatRect getBoundingBox() const;
    
    // State check
    bool hasSprite() const { return mSprite != nullptr; }
    
    // For collision detection
    bool collidesWith(const GameObject& other) const;
    
protected:
    // Position data
    float mXPos;
    float mYPos;
    
    // Team/faction data
    Allegiance mAllegiance;
    
    // NVI pattern for rendering
    virtual void doRender(sf::RenderWindow& window) const;
    
    // Graphics members
    sf::Image mImage;
    sf::Texture* mTexture; // Now just a pointer to a texture managed by TextureManager
    std::unique_ptr<sf::Sprite> mSprite;
    std::string mTexturePath; // Store the texture path for reference
};

#endif // GAME_OBJECT_H 