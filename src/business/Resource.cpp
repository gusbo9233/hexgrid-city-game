#include "Resource.h"

Resource::Resource() : amount(0), price(0), demand(0), supply(0) {
    name = "Default Resource";
}

Resource::~Resource() {
    // No dynamic memory to clean up
}

