#ifndef GOVERNMENT_H
#define GOVERNMENT_H

class Government {
    public:
        Government();
        ~Government();
        void addMoney(double amount);
        double getMoney() const;

    private:
        double money;
        double debt;
};


#endif
