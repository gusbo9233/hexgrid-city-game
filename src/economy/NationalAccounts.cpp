#include "../../include/economy/NationalAccounts.h"

NationalAccounts::NationalAccounts() : money(0), days(0), GDP(0) {
    // Initialize with empty values
}

NationalAccounts::~NationalAccounts() {

}

void NationalAccounts::addProduct(Product product, int amount) {
    mProducts[product] += amount;
}

void NationalAccounts::removeProduct(Product product, int amount) {
    mProducts[product] -= amount;
}

int NationalAccounts::getProductAmount(Product product) {
    return mProducts[product];
}

int NationalAccounts::getMoney() {
    return money;
}

void NationalAccounts::addMoney(int amount) {
    money += amount;
    
    // Initialize the map entry if it doesn't exist
    if (dailyEarnings.find(days) == dailyEarnings.end()) {
        dailyEarnings[days] = 0;
    }
    
    dailyEarnings[days] += amount;
}

void NationalAccounts::removeMoney(int amount) {
    money -= amount;
}

void NationalAccounts::nextDay() {
    days++;
    calculateGDP();
}

void NationalAccounts::calculateGDP() {
    //calculate earnings for the most recent 365 days
    if (days < 365) {
        GDP = 0;
        return;
    }
    double earnings = 0;
    for (int i = days - 365; i < days; i++) {
        earnings += dailyEarnings[i];
    }
    GDP = earnings;
}

double NationalAccounts::getGDP() {
    return GDP;
}
