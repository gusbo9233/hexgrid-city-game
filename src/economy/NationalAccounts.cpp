#include "../../include/economy/NationalAccounts.h"

NationalAccounts::NationalAccounts() {

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
}

void NationalAccounts::removeMoney(int amount) {
    money -= amount;
}