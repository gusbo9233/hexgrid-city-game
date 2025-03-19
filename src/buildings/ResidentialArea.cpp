#include "ResidentialArea.h"

ResidentialArea::ResidentialArea() : Building({0, 0}) {
    // Initialize the residential area with default values
    initializeShape();
}

ResidentialArea::~ResidentialArea() {
    // Clean up households if needed
}
    