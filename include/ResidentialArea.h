#define RESIDENTIALAREA_H

#include "Building.h"
#include "Household.h"
class ResidentialArea : public Building {
    public:
        ResidentialArea();
        ~ResidentialArea();
    private:
        std::vector<Household*> households;
};