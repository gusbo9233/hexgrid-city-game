#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <optional>
#include "Hexagon.h"
#include "CharacterType.h"
#include "Allegiance.h"
#include "projectiles/ProjectileType.h"
#include "projectiles/Projectile.h"
#include "GameObject.h"

class Character : public GameObject {
    public:
        Character(int q, int r, Allegiance allegiance = Allegiance::FRIENDLY);
        virtual ~Character() = default;
        
        // Position methods - overriding from GameObject
        void setPosition(const sf::Vector2f& pixelPos) override;
        
        // Hex coordinate methods
        void setHexCoord(int q, int r);
        void setHexCoord(const Hexagon::CubeCoord& coord);
        
        // Movement method
        void move(Hexagon* targetHex);
        
        // Coordinate getters
        int getQ() const { return mQ; }
        int getR() const { return mR; }
        int getS() const { return -mQ - mR; }
        Hexagon::CubeCoord getHexCoord() const { return {mQ, mR, getS()}; }
        
        // Type identification
        virtual CharacterType getType() const = 0;
        bool isCharacterType(CharacterType type) const { return getType() == type; }
        
        // Common virtual behavior
        virtual void update(float deltaTime) {}
        
        // Visibility range methods
        int getVisibilityRange() const { return mVisibilityRange; }
        void setVisibilityRange(int range) { mVisibilityRange = range; }
        
        // Target position methods
        void setTargetPosition(const sf::Vector2f& position) { mTargetPosition = position; }
        void clearTargetPosition() { mTargetPosition.reset(); }
        bool hasTarget() const { return mTargetPosition.has_value(); }
        sf::Vector2f getTargetPosition() const { 
            return mTargetPosition.value_or(sf::Vector2f(0, 0)); 
        }
        std::optional<sf::Vector2f> getOptionalTargetPosition() const { 
            return mTargetPosition; 
        }
        
        int getRange() const { return range; }
        std::optional<Projectile> shootProjectile();
        void resetShootCooldown(int cooldownTime = 60);
        int getShootCooldown() const { return shootCooldown; }
        void updateShootCooldown(float deltaTime) { shootCooldown -= deltaTime; }
        bool canShoot() const { return shootCooldown <= 0; }
        void updateCooldowns(float deltaTime);
        void takeDamage(int damage);
        
        // Health status methods
        int getHealth() const { return health; }
        bool isDead() const { return health <= 0; }
        std::vector<TerrainType> getTraversableTerrain() const { return mTraversableTerrain; }
    protected:
        static constexpr float STANDARD_SIZE = 25.0f;
        
        // Override GameObject methods
        float getScaleFactor() const override = 0;
        sf::Vector2f getSize() const override { return {STANDARD_SIZE, STANDARD_SIZE}; }
        
        // Override the render implementation
        void doRender(sf::RenderWindow& window) const override;
        
        std::optional<ProjectileType> mProjectileType;
        std::optional<sf::Vector2f> mTargetPosition;
        std::vector<TerrainType> mTraversableTerrain;
        
        int mQ;
        int mR;
        
        int health;
        int maxHealth;
        int attackPower;
        int defensePower;
        int speed;
        int range;
        
        // Default visibility range - can be overridden by specific character types
        int mVisibilityRange = 3;
        int shootCooldown = 0;
        float shootCooldownMax = 500;
};

#endif
