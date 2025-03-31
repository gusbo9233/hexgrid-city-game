#define HOUSEHOLD_H

#include "Individual.h"
#include <vector>

class Household {
    public: 
        Household();
        ~Household();
        void initialize();
        std::vector<Individual*> getIndividuals() const;
        Individual* generateIndividual(bool isChild);
        void update();
        void proCreate();
        int getIncome() const;
        int getHappiness() const;
    private:
        std::vector<Individual*> individuals;
        int income;
        int happiness;

};
