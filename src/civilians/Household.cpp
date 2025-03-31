#include "../../include/civilian/Household.h"
#include "../../include/civilian/Individual.h"

//constructor
Household::Household() {
    initialize();
}

//destructor
Household::~Household() {
    for (Individual* individual : individuals) {
        delete individual;
    }
}

//update the household
void Household::update() {
   
}

void Household::proCreate() {
    // check if there are two individuals in the household over the age of 18
    if (individuals.size() == 2 && individuals[0]->getAge() > 18 && individuals[1]->getAge() > 18) {
        //randomly procreate based on household size, income and happiness
        //household size negative effect
        int houseHoldSizeNegativeEffect = individuals.size() * -0.1;
        //income positive effect
        int incomePositiveEffect = getIncome() * 0.1;
        //happiness positive effect
        int happinessPositiveEffect = getHappiness() * 0.1;
        //calculate the total effect
        int totalEffect = houseHoldSizeNegativeEffect + incomePositiveEffect + happinessPositiveEffect;
        //randomly select if the household will procreate
        int random = rand() % 100;
        if (random < totalEffect) {
            //procreate
            individuals.push_back(generateIndividual(false));
        }
    }
}
//initialize the household
void Household::initialize() {
    //randomly select if the household is a single person, couple or a family
    int random = rand() % 3;
    if (random == 0) {
        //random age between 18 and 100
        //single person
        individuals.push_back(generateIndividual(false));
    }
    else if (random == 1) {
        //two random adult ages between 20 and 60
        
        //couple
        individuals.push_back(generateIndividual(false));
        individuals.push_back(generateIndividual(false));       
    }
    else {
        //Two random adult ages between 20 and 60 and one child age between 0 and 18
        
        //family
        individuals.push_back(generateIndividual(false));
        individuals.push_back(generateIndividual(false));
        individuals.push_back(generateIndividual(true));
    }
}
Individual* Household::generateIndividual(bool isChild) {
    int age = 0;
    if (isChild) {
        //random age between 0 and 18
        age = rand() % 19;
    }
    
    else {
        age = rand() % 83 + 18;
    }
    //random intelligence between 0 and 10
    int intelligence = rand() % 11;
    //random happines between 0 and 10
    int happiness = rand() % 11;
    //random health between 0 and 10 based on age
    int health = rand() % (11 - age / 10);

    return new Individual(age, intelligence, happiness, health);
}

int Household::getIncome() const {
    //calculate total income of the household
    int totalIncome = 0;
    for (Individual* individual : individuals) {
        totalIncome += individual->getIncome();
    }
    return totalIncome;
}

int Household::getHappiness() const {
    //calculate average happiness of the household
    int totalHappiness = 0;
    for (Individual* individual : individuals) {
        totalHappiness += individual->getHappiness();
    }
    int averageHappiness = totalHappiness / individuals.size();
    return averageHappiness;
}
