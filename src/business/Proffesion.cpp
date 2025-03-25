#include "../../include/business/Proffesion.h"

Profession::Profession() : compensation(0), educationProductivityFactor(1) {
    name = "Default Profession";
}

Profession::~Profession() {
    // No dynamic memory to clean up
}
