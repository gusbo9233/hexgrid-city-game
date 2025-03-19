#include "Product.h"

Product::Product() : price(0), demand(0), supply(0) {
    name = "Default Product";
}

Product::~Product() {
    // No resource cleanup needed as we don't own the resources
}

