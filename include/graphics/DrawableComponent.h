#ifndef DRAWABLE_COMPONENT_H
#define DRAWABLE_COMPONENT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <string>

class DrawableComponent {
public:
    DrawableComponent();
    ~DrawableComponent() = default;
    
    // Move constructor and assignment operator
    DrawableComponent(DrawableComponent&& other) noexcept;
    DrawableComponent& operator=(DrawableComponent&& other) noexcept;
    
    // Delete copy constructor and assignment operator
    DrawableComponent(const DrawableComponent&) = delete;
    DrawableComponent& operator=(const DrawableComponent&) = delete;
    
    // Draw the component
    void draw(sf::RenderWindow& window) const;
    
    // Load a texture from a file
    bool loadTexture(const std::string& path);
    
    // Create a fallback texture (circle or square)
    void createFallbackTexture(sf::Color color, bool isCircle = true);
    
    // Position methods
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    
    // Sprite access
    sf::Sprite* getSprite() { return mSprite.get(); }
    const sf::Sprite* getSprite() const { return mSprite.get(); }
    
    // Size and scale
    void setSize(const sf::Vector2f& size, float scaleFactor = 1.0f);
    void setOrigin(const sf::Vector2f& origin);
    
    // State check
    bool hasSprite() const { return mSprite != nullptr; }

private:
    sf::Image mImage;
    sf::Texture mTexture;
    std::unique_ptr<sf::Sprite> mSprite;
};

#endif // DRAWABLE_COMPONENT_H 