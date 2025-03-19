//base
#include "../include/Base.h"

Base::Base(Hexagon* hexagon) 
    : Building(hexagon->getPosition()) {
    mHexagon = hexagon;
    setPosition(hexagon->getPosition());
    initializeShape();  // Initialize the shape with Base-specific size
    
    // Set the visibility range for this building type
    setVisibilityRange(BASE_VISIBILITY_RANGE);
}

Base::~Base() {
}

void Base::update(float deltaTime) {
}   