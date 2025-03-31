#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

// Singleton class to manage textures
class TextureManager {
public:
    // Get the singleton instance
    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }
    
    // Delete copy constructor and assignment operator
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    
    // Get a texture; loads it if not already loaded
    sf::Texture* getTexture(const std::string& filename) {
        // Check if the texture is already loaded
        auto it = mTextures.find(filename);
        if (it != mTextures.end()) {
            return it->second.get();
        }
        
        // Load the texture if not already loaded
        std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filename)) {
            std::cerr << "TextureManager: Failed to load texture " << filename << std::endl;
            return nullptr;
        }
        
        std::cout << "TextureManager: Successfully loaded texture " << filename << std::endl;
        
        // Store the texture and return it
        sf::Texture* result = texture.get();
        mTextures[filename] = std::move(texture);
        return result;
    }
    
    // Clear all textures
    void clearAll() {
        mTextures.clear();
    }
    
private:
    // Private constructor for singleton
    TextureManager() = default;
    
    // Storage for textures
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> mTextures;
};

#endif // TEXTURE_MANAGER_H 