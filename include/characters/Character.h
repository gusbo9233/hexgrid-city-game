#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Hexagon.h"
#include "CharacterType.h"
#include "Allegiance.h"
#include "projectiles/ProjectileType.h"
class Character {
    public:
        Character(int q, int r, Allegiance allegiance = Allegiance::FRIENDLY);
        virtual ~Character() = default;
        
        // Position setting methods
        void setPosition(const sf::Vector2f& pixelPos);
        void setHexCoord(int q, int r);
        void setHexCoord(const Hexagon::CubeCoord& coord);
        
        // NVI pattern for rendering
        void render(sf::RenderWindow& window) const;
        
        // Coordinate getters
        int getQ() const { return mQ; }
        int getR() const { return mR; }
        int getS() const { return -mQ - mR; }
        Hexagon::CubeCoord getHexCoord() const { return {mQ, mR, getS()}; }
        
        // Type identification
        virtual CharacterType getType() const = 0;
        bool isCharacterType(CharacterType type) const { return getType() == type; }
        
        // Allegiance methods
        Allegiance getAllegiance() const { return mAllegiance; }
        void setAllegiance(Allegiance allegiance) { mAllegiance = allegiance; }
        bool isFriendly() const { return mAllegiance == Allegiance::FRIENDLY; }
        bool isEnemy() const { return mAllegiance == Allegiance::ENEMY; }
        
        // Common virtual behavior
        virtual void update(float deltaTime) {}
        
        // Visibility range methods
        int getVisibilityRange() const { return mVisibilityRange; }
        void setVisibilityRange(int range) { mVisibilityRange = range; }
        
    protected:
        static constexpr float STANDARD_SIZE = 25.0f;
        
        virtual float getScaleFactor() const = 0;
        virtual sf::Vector2f getSize() const { return {STANDARD_SIZE, STANDARD_SIZE}; }
        
        // NVI implementation - can be overridden by derived classes
        virtual void doRender(sf::RenderWindow& window) const;
        
        bool loadTexture(const std::string& path);
        
        sf::Image mImage;
        sf::Texture mTexture;
        std::unique_ptr<sf::Sprite> mSprite;
        std::optional<ProjectileType> mProjectileType;
        void shootProjectile();

        
    private:
        int mQ;
        int mR;
        
        int health;
        int maxHealth;
        int attackPower;
        int defensePower;
        int speed;
        int range;
        
        // Allegiance - default to friendly
        Allegiance mAllegiance;
        
        // Default visibility range - can be overridden by specific character types
        int mVisibilityRange = 3;
};

#endif
