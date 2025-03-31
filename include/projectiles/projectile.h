#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Allegiance.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "GameObject.h"

class Projectile : public GameObject {
    public:
        Projectile(int xPos, int yPos, double xMagnitude, double yMagnitude, Allegiance allegiance);
        virtual ~Projectile() = default;
        
        // Move constructor and assignment operator
        Projectile(Projectile&& other) noexcept;
        Projectile& operator=(Projectile&& other) noexcept;
        
        // Delete copy constructor and assignment operator
        Projectile(const Projectile&) = delete;
        Projectile& operator=(const Projectile&) = delete;
        
        // Movement
        virtual void update();
        
        // Override for inherited position method
        void setPosition(const sf::Vector2f& position) override;

        int getDamage() const { return damage; }

    protected:
        // Graphics configuration
        static constexpr float STANDARD_SIZE = 10.0f;  // Half the size of characters
        
        // Override GameObject methods
        float getScaleFactor() const override { return 1.0f; }
        sf::Vector2f getSize() const override { return {STANDARD_SIZE, STANDARD_SIZE}; }
        
        // Movement properties
        int damage;
        float speed;
        double xMagnitude;
        double yMagnitude;
};

#endif // PROJECTILE_H
