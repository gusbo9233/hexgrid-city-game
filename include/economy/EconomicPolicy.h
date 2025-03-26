#ifndef ECONOMIC_POLICY_H
#define ECONOMIC_POLICY_H

class EconomicPolicy {
    public:
        EconomicPolicy();
        ~EconomicPolicy();
        void setTaxRate(double taxRate);
        void setInterestRate(double interestRate);
        double getTaxRate() const;
        double getInterestRate() const;
        
    private:
        double mTaxRate;
        double mInterestRate;
        
};

#endif
