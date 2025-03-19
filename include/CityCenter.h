#ifndef CITYCENTER_H
#define CITYCENTER_H

#include "Building.h"

class CityCenter : public Building {
public:
    CityCenter(const sf::Vector2f& position);
    ~CityCenter() override = default;

protected:
    // Building appearance customization
    float getScaleFactor() const override { return 1.2f; } // Slightly larger than standard
    sf::Vector2f getSize() const override { return {STANDARD_SIZE * 1.2f, STANDARD_SIZE * 1.2f}; }
    std::string getImagePath() const override { return "assets/images/CityCenter.png"; }
};

#endif
