#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include "Resource.h"

class Product {
    public:
        Product();
        ~Product();
    private:
        std::string name;
        int price;
        int demand;
        int supply;
        //Required resources
        std::vector<Resource*> requiredResources;
};

#endif // PRODUCT_H