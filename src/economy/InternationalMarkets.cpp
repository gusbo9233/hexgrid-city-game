#include "../../include/economy/InternationalMarkets.h"

InternationalMarkets::InternationalMarkets() {
    mProductPrices[Product::FOOD] = 5;
    mProductPrices[Product::RefinedOil] = 20;
}

InternationalMarkets::~InternationalMarkets() {
    // No resources to clean up
}

int InternationalMarkets::getProductPrice(Product product) {
    return mProductPrices[product];
}

void InternationalMarkets::setProductPrice(Product product, int price) {
    mProductPrices[product] = price;
}
