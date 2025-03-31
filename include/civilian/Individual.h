#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../business/Proffesion.h"

class Individual {
    public:
        Individual(int age, int intelligence, int happiness, int health);
        ~Individual();
        int getAge() const;
        int getIntelligence() const;
        int getHappiness() const;
        int getHealth() const;
        int getIncome() const;
    private:
    int age;
    int intelligence;
    int health;
    int happiness;
    int income;
};

#endif // INDIVIDUAL_H