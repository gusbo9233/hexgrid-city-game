#ifndef CITY_H
#define CITY_H

#include "../graphics/HexGrid.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include "../Allegiance.h"

class Building;

class City {
    public:
        // Constructor with city hexes, no color parameter
        City(std::vector<Hexagon*> cityHexes, Allegiance allegiance = Allegiance::NEUTRAL);
        
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
        
        // Allegiance accessor
        Allegiance getAllegiance() const { return mAllegiance; }

    private:
        std::vector<Hexagon*> cityHexes;
        //map of hex coordinates to buildings
        std::string cityName;
        // Use unique_ptr for owned buildings
        std::map<Hexagon::CubeCoord, std::unique_ptr<Building>> buildings;
        Allegiance mAllegiance;
        //generate buildings
        void generateBuildings();
};

#endif 