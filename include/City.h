#ifndef CITY_H
#define CITY_H

#include "HexGrid.h"
#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics/Color.hpp>

class City {
    public:
        City(std::vector<Hexagon*> cityHexes, const sf::Color& cityColor = sf::Color(200, 200, 200));
        ~City();
        void updateHexColors(); // Ensures city hexes maintain their color

    private:
        std::vector<Hexagon*> cityHexes;
        //map of hex coordinates to buildings
        std::string cityName;
        std::map<Hexagon::CubeCoord, Building*> buildings;
        sf::Color color;
        //generate buildings
        void generateBuildings();
};

#endif