#ifndef WORKPLACE_H
#define WORKPLACE_H

#include "Building.h"
#include "Proffesion.h"
#include "Individual.h"
#include <vector>
#include "Product.h"
#include "Resource.h"
class Workplace : public Building {
    public:
        Workplace();
        Workplace(const sf::Vector2f& position);
        ~Workplace();
        void addEmployee(Individual* employee);
        void removeEmployee(Individual* employee);
        
        // Implement type identification
        BuildingType getType() const override { return BuildingType::Workplace; }
        
    private:
        std::string name;
        Profession* profession;
        int maxEmployees;
        std::vector<Individual*> employees;
        int currentEmployees;
        Product* product;
        std::vector<Resource*> availableResources;
};

#endif
