#include "include/economy/EconomicPolicy.h"

EconomicPolicy::EconomicPolicy() {
    mTaxRate = 0.2;
    mInterestRate = 0.05;
}

void EconomicPolicy::setTaxRate(double taxRate) {
    mTaxRate = taxRate;
}

void EconomicPolicy::setInterestRate(double interestRate) {
    mInterestRate = interestRate;
}

double EconomicPolicy::getTaxRate() const {
    return mTaxRate;
}   
