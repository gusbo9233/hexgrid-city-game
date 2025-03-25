#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../business/Proffesion.h"

class Individual {
    public:
        Individual();
        ~Individual();
        
    private:
    int age;
    int health;
    int happiness;
    Profession* profession;
    int education;
};

#endif // INDIVIDUAL_H