#ifndef GOVERNMENT_H
#define GOVERNMENT_H

class Government {
    public:
        Government();
        ~Government();
        void addMoney(double amount);
        double getMoney() const;
        
        // Methods moved from EconomicPolicy
        void setTaxRate(double taxRate);
        void setInterestRate(double interestRate);
        double getTaxRate() const;
        double getInterestRate() const;

    private:
        double money;
        double debt;
        
        // Variables moved from EconomicPolicy
        double mTaxRate;
        double mInterestRate;
};


#endif
