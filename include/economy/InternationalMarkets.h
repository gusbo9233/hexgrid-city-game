#ifndef INTERNATIONAL_MARKETS_H
#define INTERNATIONAL_MARKETS_H

#include "../business/Product.h"
#include <map>

class InternationalMarkets {
    public:
        InternationalMarkets();
        ~InternationalMarkets();
        void setProductPrice(Product product, int price);
        int getProductPrice(Product product);
        
    private:
        std::map<Product, int> mProductPrices;
};


#endif // INTERNATIONAL_MARKETS_H
