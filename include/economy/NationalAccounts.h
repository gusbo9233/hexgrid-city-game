#ifndef NATIONAL_ACCOUNTS_H
#define NATIONAL_ACCOUNTS_H
#include "Product.h"
#include <map>
class NationalAccounts {
    public:
        NationalAccounts();
        ~NationalAccounts();
        void addProduct(Product product, int amount);
        void removeProduct(Product product, int amount);
        int getProductAmount(Product product);
        int getMoney();
        void addMoney(int amount);
        void removeMoney(int amount);
    private:
        std::map<Product, int> mProducts;
        int money;
};

#endif // NATIONAL_ACCOUNTS_H
