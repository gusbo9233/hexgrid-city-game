#define HOUSEHOLD_H

#include "Individual.h"
#include <vector>

class Household {
    public: 
        Household();
        ~Household();
    private:
        std::vector<Individual*> individuals;
};
