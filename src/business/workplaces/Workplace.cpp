#include "../../../include/business/workplaces/Workplace.h"
#include <algorithm> // For std::find

Workplace::Workplace() : Building({0, 0}), maxEmployees(10), currentEmployees(0), 
                         profession(nullptr), productType(FOOD) {
    name = "Default Workplace";
    initializeShape();
}

Workplace::Workplace(const sf::Vector2f& position) : Building(position), maxEmployees(10), 
                                                    currentEmployees(0), profession(nullptr), 
                                                    productType(FOOD) {
    name = "Default Workplace";
    minStock = 2000;
    initializeShape();
}

Workplace::~Workplace() {
    // Clean up resources if they were dynamically allocated
    // Note: We don't delete employees or profession because they might be shared/managed elsewhere
}

void Workplace::addEmployee(Individual* employee) {
    if (currentEmployees < maxEmployees && employee != nullptr) {
        employees.push_back(employee);
        currentEmployees++;
    }
}

void Workplace::removeEmployee(Individual* employee) {
    if (employee != nullptr) {
        auto it = std::find(employees.begin(), employees.end(), employee);
        if (it != employees.end()) {
            employees.erase(it);
            currentEmployees--;
        }
    }
}

bool Workplace::sellProduct(Product product, int amount, double price) {
    if (stock >= amount && stock > minStock) {
        money += amount * price;
        stock -= amount;
        return true;
    }
    return false;
}

void Workplace::generateProduct() {
    // Simple implementation - increase stock by 1 for now
    stock++;
}