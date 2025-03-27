#include "../../include/economy/Government.h"

Government::Government() : money(0), debt(0), mTaxRate(0.2), mInterestRate(0.05) {
    // Initialize government with zero money and debt, and default tax and interest rates
}

Government::~Government() {
    // Cleanup resources if needed
}

void Government::addMoney(double amount) {
    money += amount;
}

double Government::getMoney() const {
    return money;
}

// Methods moved from EconomicPolicy
void Government::setTaxRate(double taxRate) {
    mTaxRate = taxRate;
}

void Government::setInterestRate(double interestRate) {
    mInterestRate = interestRate;
}

double Government::getTaxRate() const {
    return mTaxRate;
}

double Government::getInterestRate() const {
    return mInterestRate;
} 