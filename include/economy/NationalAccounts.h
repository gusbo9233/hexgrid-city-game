#ifndef NATIONAL_ACCOUNTS_H
#define NATIONAL_ACCOUNTS_H
#include "../business/Product.h"
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
        void nextDay();
        void calculateGDP();
        double getGDP();
        
    private:
        std::map<Product, int> mProducts;
        int money;
        int days;
        double GDP;
        std::map<int, double> dailyEarnings;
};

#endif // NATIONAL_ACCOUNTS_H
