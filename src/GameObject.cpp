#include "../include/GameObject.h"
#include <iostream>

GameObject::GameObject(float xPos, float yPos, Allegiance allegiance)
    : mXPos(xPos), mYPos(yPos), mAllegiance(allegiance), mTexture(nullptr), mSprite(nullptr) {
    // Base initialization - no default texture or shape
}

GameObject::GameObject(GameObject&& other) noexcept
    : mXPos(other.mXPos),
      mYPos(other.mYPos),
      mAllegiance(other.mAllegiance),
      mImage(std::move(other.mImage)),
      mTexture(other.mTexture),
      mSprite(std::move(other.mSprite)),
      mTexturePath(std::move(other.mTexturePath)) {
    // The texture pointer is copied, not moved, since it's managed by TextureManager
    other.mTexture = nullptr;
}

GameObject& GameObject::operator=(GameObject&& other) noexcept {
    if (this != &other) {
        mXPos = other.mXPos;
        mYPos = other.mYPos;
        mAllegiance = other.mAllegiance;
        mImage = std::move(other.mImage);
        mTexture = other.mTexture;
        mSprite = std::move(other.mSprite);
        mTexturePath = std::move(other.mTexturePath);
        
        // The texture pointer is copied, not moved, since it's managed by TextureManager
        other.mTexture = nullptr;
    }
    return *this;
}

void GameObject::setPosition(const sf::Vector2f& position) {
    mXPos = position.x;
    mYPos = position.y;
    
    if (mSprite) {
        mSprite->setPosition(position);
    }
}

sf::Vector2f GameObject::getPosition() const {
    if (mSprite) {
        return mSprite->getPosition();
    }
    return sf::Vector2f(mXPos, mYPos);
}

void GameObject::render(sf::RenderWindow& window) const {
    // NVI pattern - call the implementation method
    doRender(window);
}

void GameObject::doRender(sf::RenderWindow& window) const {
    // Base implementation - just draw the sprite if it exists
    if (mSprite) {
        window.draw(*mSprite);
    }
}

bool GameObject::loadTexture(const std::string& path) {
    std::cout << "Attempting to load texture from: " << path << std::endl;
    
    // Store the path for reference
    mTexturePath = path;
    
    // Get the texture from the TextureManager
    mTexture = TextureManager::getInstance().getTexture(path);
    if (!mTexture) {
        std::cout << "Failed to load texture: " << path << std::endl;
        return false;
    }
    
    std::cout << "Successfully loaded texture" << std::endl;
    
    // Create sprite using the loaded texture
    mSprite = std::make_unique<sf::Sprite>(*mTexture);
    std::cout << "Created sprite" << std::endl;
    
    // Set size and origin
    sf::Vector2f size = getSize();
    setSize(size, getScaleFactor());
    
    // Center origin
    sf::FloatRect bounds = mSprite->getLocalBounds();
    setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    
    // Update position
    mSprite->setPosition(sf::Vector2f(mXPos, mYPos));
    
    return true;
}

void GameObject::createShape(sf::Color color, bool isCircle) {
    // Create a fallback texture
    sf::Image fallbackImage;
    fallbackImage = sf::Image(sf::Vector2u(16, 16), sf::Color::Transparent);
    
    if (isCircle) {
        // Draw a filled circle
        for (unsigned int x = 0; x < 16; ++x) {
            for (unsigned int y = 0; y < 16; ++y) {
                float dx = static_cast<float>(x) - 8.0f;
                float dy = static_cast<float>(y) - 8.0f;
                if (dx * dx + dy * dy <= 64.0f) { // radius of 8 pixels
                    fallbackImage.setPixel(sf::Vector2u(x, y), color);
                }
            }
        }
    } else {
        // Draw a filled square
        for (unsigned int x = 0; x < 16; ++x) {
            for (unsigned int y = 0; y < 16; ++y) {
                fallbackImage.setPixel(sf::Vector2u(x, y), color);
            }
        }
    }
    
    mImage = fallbackImage;
    
    // Create a unique shape name for the TextureManager
    static int shapeCounter = 0;
    std::string shapeName = "shape_" + std::to_string(shapeCounter++);
    
    // Create texture in TextureManager
    sf::Texture texture;
    if (!texture.loadFromImage(mImage)) {
        std::cout << "Failed to create texture from fallback image" << std::endl;
        return;
    }
    
    // Add texture to manager
    mTexturePath = shapeName;
    TextureManager::getInstance().getTexture(shapeName); // This will add an empty entry
    mTexture = TextureManager::getInstance().getTexture(shapeName);
    *mTexture = texture; // Copy the texture data
    
    // Create sprite using the texture from manager
    mSprite = std::make_unique<sf::Sprite>(*mTexture);
    std::cout << "Created fallback sprite" << std::endl;
    
    // Set size and origin
    sf::Vector2f size = getSize();
    setSize(size, getScaleFactor());
    
    // Update position
    mSprite->setPosition(sf::Vector2f(mXPos, mYPos));
}

void GameObject::setSize(const sf::Vector2f& size, float scaleFactor) {
    if (mSprite) {
        sf::FloatRect bounds = mSprite->getLocalBounds();
        float scaleX = (size.x * scaleFactor) / bounds.size.x;
        float scaleY = (size.y * scaleFactor) / bounds.size.y;
        mSprite->setScale(sf::Vector2f(scaleX, scaleY));
    }
}

void GameObject::setOrigin(const sf::Vector2f& origin) {
    if (mSprite) {
        mSprite->setOrigin(origin);
    }
}

sf::FloatRect GameObject::getBoundingBox() const {
    if (mSprite) {
        // Get the sprite's global bounds, which includes its position, scale, and origin
        return mSprite->getGlobalBounds();
    } else {
        // If no sprite exists, create a basic bounding box based on position and size
        sf::Vector2f size = getSize();
        float scaleFactor = getScaleFactor();
        sf::Vector2f scaledSize = {size.x * scaleFactor, size.y * scaleFactor};
        
        // Center the box at the object's position
        sf::Vector2f position(mXPos - scaledSize.x / 2.0f, mYPos - scaledSize.y / 2.0f);
        
        // In SFML 3.0, FloatRect constructor takes two Vector2f objects (position and size)
        return sf::FloatRect(position, scaledSize);
    }
}

bool GameObject::collidesWith(const GameObject& other) const {
    // Simple AABB (Axis-Aligned Bounding Box) collision detection
    sf::FloatRect thisBounds = getBoundingBox();
    sf::FloatRect otherBounds = other.getBoundingBox();
    
    // In SFML 3.0, findIntersection returns an optional<Rect> rather than a bool
    auto intersection = thisBounds.findIntersection(otherBounds);
    
    // Returns true if the optional contains a value (i.e., the rectangles intersect)
    return intersection.has_value();
} 