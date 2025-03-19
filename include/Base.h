#ifndef BASE_H
#define BASE_H
//Base is a building that is always on the hexagon
#include "Building.h"
#include "Hexagon.h"

class Base : public Building {
    public:
        Base(Hexagon* hexagon);
        ~Base();
        void update(float deltaTime);
        
    protected:
        // Define base-specific size
        float getScaleFactor() const override { return 1.0f; }
        sf::Vector2f getSize() const override { return {40.0f, 40.0f}; }
        
    private:
        Hexagon* mHexagon;
};
#endif
