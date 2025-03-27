#include "../../../include/business/workplaces/Farm.h"

Farm::Farm() : Workplace() {
    name = "Farm";
    productType = FOOD;
    maxEmployees = 5;
    minStock = 2000;
    stock = 50;    // Start with some initial stock
    initializeShape();
}

Farm::Farm(const sf::Vector2f& position) : Workplace(position) {
    name = "Farm";
    productType = FOOD;
    maxEmployees = 5;
    minStock = 2000;
    stock = 50;
    initializeShape();
}

Farm::~Farm() {
    // Clean up resources if needed
}

void Farm::generateProduct() {
    // Farms produce food at a higher rate than generic workplaces
    stock += 1;
} 