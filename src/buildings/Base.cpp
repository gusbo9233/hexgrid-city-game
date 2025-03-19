//base
#include "../include/Base.h"

Base::Base(Hexagon* hexagon) 
    : Building(hexagon->getPosition()) {
    mHexagon = hexagon;
    setPosition(hexagon->getPosition());
    initializeShape();  // Initialize the shape with Base-specific size
}

Base::~Base() {
}

void Base::update(float deltaTime) {
}   