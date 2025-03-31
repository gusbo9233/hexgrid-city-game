#include "../../include/characters/Character.h"
#include <iostream>
#include <filesystem>
#include "../../include/projectiles/Bullet.h"
#include "../../include/projectiles/TankAmmo.h"

Character::Character(int q, int r, Allegiance allegiance) 
    : GameObject(0.0f, 0.0f, allegiance), // Position will be set later
      mQ(q), mR(r), health(100), maxHealth(100), 
      attackPower(10), defensePower(5), speed(1), range(1),
      mTargetPosition(std::nullopt) {
    // Position will be set externally based on hex coordinates
}

void Character::setPosition(const sf::Vector2f& pixelPos) {
    // Update GameObject position
    GameObject::setPosition(pixelPos);
}

// Default implementation of doRender from GameObject
void Character::doRender(sf::RenderWindow& window) const {
    // Use the GameObject's draw functionality
    GameObject::doRender(window);
}

void Character::setHexCoord(int q, int r) {
    mQ = q;
    mR = r;
}

void Character::setHexCoord(const Hexagon::CubeCoord& coord) {
    mQ = coord.q;
    mR = coord.r;
}

void Character::move(Hexagon* targetHex) {
    if (!targetHex) return;
    
    // Update hex coordinates
    setHexCoord(targetHex->getCoord());
    
    // Update position
    setPosition(targetHex->getPosition());
}

void Character::resetShootCooldown(int cooldownTime) {
    shootCooldown = cooldownTime;
}

void Character::updateCooldowns(float deltaTime) {
    // Decrease cooldown based on elapsed time
    if (shootCooldown > 0) {
        shootCooldown -= 1; // Simple frame-based cooldown
    }
}

std::optional<Projectile> Character::shootProjectile() {
    // Debug all conditions that might prevent shooting
    if (!canShoot()) {
        std::cout << "Cannot shoot: cooldown is " << shootCooldown << std::endl;
        return std::nullopt;
    }
    
    if (!hasTarget()) {
        std::cout << "Cannot shoot: no target set" << std::endl;
        return std::nullopt;
    }
    
    if (!mProjectileType) {
        std::cout << "Cannot shoot: no projectile type defined" << std::endl;
        return std::nullopt;
    }
    
    // Reset cooldown when shooting
    resetShootCooldown();
    
    // Get the current character position
    sf::Vector2f startPos = getPosition();
    
    // Get the target position
    sf::Vector2f targetPos = mTargetPosition.value();
    
    // Calculate direction vector from start to target
    sf::Vector2f direction = targetPos - startPos;
    
    // Calculate distance to target
    float distanceToTarget = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    // Debug output to log positions and distance
    std::cout << "Shooting from (" << startPos.x << "," << startPos.y 
              << ") to (" << targetPos.x << "," << targetPos.y 
              << ") at distance " << distanceToTarget << std::endl;
    
    // Special case for very close enemies - adjust the direction slightly to ensure hit
    if (distanceToTarget < 30.0f) {
        std::cout << "Target is very close! Adjusting aim to ensure hit." << std::endl;
        // For close targets, we'll use the exact vector to the target instead of normalizing
        // This ensures the projectile doesn't overshoot at close range
        
        // If the distance is extremely small, give it a minimum length to avoid zero-length vectors
        if (distanceToTarget < 5.0f) {
            direction.x = (direction.x == 0) ? 0.1f : direction.x;
            direction.y = (direction.y == 0) ? 0.1f : direction.y;
        }
        
        // For close targets, don't normalize - use a slower speed to make sure we hit
        switch (mProjectileType.value()) {
            case ProjectileType::BULLET:
                return Bullet(startPos.x, startPos.y, direction.x / 10.0f, direction.y / 10.0f, getAllegiance());
            case ProjectileType::TANK_AMMO:
                return TankAmmo(startPos.x, startPos.y, direction.x / 10.0f, direction.y / 10.0f, getAllegiance());
            default:
                std::cout << "Unknown projectile type" << std::endl;
                return std::nullopt;
        }
    }
    
    // For normal range targets, normalize the direction vector
    if (distanceToTarget > 0) {
        direction.x /= distanceToTarget;
        direction.y /= distanceToTarget;
    }
    
    std::cout << "Character shooting projectile with normalized direction: (" 
              << direction.x << "," << direction.y << ")" << std::endl;
    
    switch (mProjectileType.value()) {
        case ProjectileType::BULLET:
            return Bullet(startPos.x, startPos.y, direction.x, direction.y, getAllegiance());
        case ProjectileType::TANK_AMMO:
            return TankAmmo(startPos.x, startPos.y, direction.x, direction.y, getAllegiance());
        default:
            std::cout << "Unknown projectile type" << std::endl;
            return std::nullopt;
    }
}

void Character::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        std::cout << "Character died" << std::endl;
    }
}


