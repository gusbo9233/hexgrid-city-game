#ifndef CITY_H
#define CITY_H

#include "HexGrid.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <SFML/Graphics/Color.hpp>

class City {
    public:
        City(std::vector<Hexagon*> cityHexes, const sf::Color& cityColor = sf::Color(200, 200, 200));
        ~City();
        void updateHexColors(); // Ensures city hexes maintain their color
        
        // Get a reference to the buildings
        const std::map<Hexagon::CubeCoord, std::unique_ptr<Building>>& getBuildings() const {
            return buildings;
        }
        
        // Get the city position (center of the first hex for now)
        sf::Vector2f getPosition() const {
            return cityHexes.empty() ? sf::Vector2f(0, 0) : cityHexes[0]->getPosition();
        }
        
        // Visibility range methods
        int getVisibilityRange() const { return mVisibilityRange; }
        void setVisibilityRange(int range) { mVisibilityRange = range; }

    private:
        std::vector<Hexagon*> cityHexes;
        //map of hex coordinates to buildings
        std::string cityName;
        // Use unique_ptr for owned buildings
        std::map<Hexagon::CubeCoord, std::unique_ptr<Building>> buildings;
        sf::Color color;
        //generate buildings
        void generateBuildings();
        
        // Default visibility range for cities
        int mVisibilityRange = 5;
};

#endif