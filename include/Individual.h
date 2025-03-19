#define INDIVIDUAL_H

#include "Proffesion.h"

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