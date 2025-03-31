#include "../../include/civilian/Individual.h"

Individual::Individual(int age, int intelligence, int happiness, int health) {
    this->age = age;
    this->intelligence = intelligence;
    this->happiness = happiness;
    this->health = health;
}

Individual::~Individual() {
    // Destructor implementation - nothing to clean up
}

int Individual::getAge() const {
    return age;
}

int Individual::getIntelligence() const {
    return intelligence;
}

int Individual::getHappiness() const {
    return happiness;
}

int Individual::getHealth() const {
    return health;
}

int Individual::getIncome() const {
    return income;
}




