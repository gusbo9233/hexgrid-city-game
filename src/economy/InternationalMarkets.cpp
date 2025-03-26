#include "include/economy/InternationalMarkets.h"

InternationalMarkets::InternationalMarkets() {
    mProductPrices[Product::FOOD] = 10;
    mProductPrices[Product::RefinedOil] = 10;
}

int InternationalMarkets::getProductPrice(Product product) {
    return mProductPrices[product];
}

void InternationalMarkets::setProductPrice(Product product, int price) {
    mProductPrices[product] = price;
}
