#ifndef WORKPLACE_H
#define WORKPLACE_H

#include "../../buildings/Building.h"
#include "../Proffesion.h"
#include "../../civilian/Individual.h"
#include <vector>
#include "../Product.h"
#include "../../resources/ResourceType.h"

class Workplace : public Building {
    public:
        Workplace();
        Workplace(const sf::Vector2f& position);
        ~Workplace();
        void addEmployee(Individual* employee);
        void removeEmployee(Individual* employee);
        
        // Get required resource type - default is UNKNOWN, subclasses can override
        virtual ResourceType getRequiredResourceType() const { return ResourceType::UNKNOWN; }
        
        // Implement type identification
        BuildingType getType() const override { return BuildingType::Workplace; }
        
        // Default implementation of the required pure virtual method from Building
        float getScaleFactor() const override { return 0.7f; }
        
    protected:
        std::string name;
        Profession* profession;
        int maxEmployees;
        std::vector<Individual*> employees;
        int currentEmployees;
        Product* product;
        
        // Using ResourceType instead
        std::vector<ResourceType> availableResourceTypes;
};

#endif
