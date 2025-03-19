#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Hexagon.h"

class Character {
    public:
        Character(int q, int r);
        virtual ~Character() = default;
        void setPosition(const sf::Vector2f& pixelPos);
        void setHexCoord(int q, int r);
        void setHexCoord(const Hexagon::CubeCoord& coord);
        virtual void render(sf::RenderWindow& window) const;
        
        int getQ() const { return mQ; }
        int getR() const { return mR; }
        int getS() const { return -mQ - mR; }
        
        Hexagon::CubeCoord getHexCoord() const { return {mQ, mR, getS()}; }
        
    protected:
        static constexpr float STANDARD_SIZE = 25.0f;
        
        virtual float getScaleFactor() const = 0;
        virtual sf::Vector2f getSize() const { return {STANDARD_SIZE, STANDARD_SIZE}; }
        
        bool loadTexture(const std::string& path);
        
        sf::Image mImage;
        sf::Texture mTexture;
        std::unique_ptr<sf::Sprite> mSprite;
        
    private:
        int mQ;
        int mR;
        
        int health;
        int maxHealth;
        int attackPower;
        int defensePower;
        int speed;
        int range;
};

#endif
