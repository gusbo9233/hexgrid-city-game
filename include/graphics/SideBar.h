#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <SFML/Graphics.hpp>
#include <array>
#include <functional>

class SideBar {
    public:
        // Enum for cell identifiers
        enum class CellId {
            Cell1,
            Cell2,
            Cell3,
            Cell4
        };
        
        // Constructor takes window dimensions to position properly
        SideBar(const sf::Vector2f& windowSize);
        ~SideBar();
        
        // Draw the sidebar
        void draw(sf::RenderWindow& window);
        
        // Handle mouse click - returns true if a cell was clicked
        bool handleClick(const sf::Vector2f& position, CellId& outClickedCell);
        
        // Set callback for when a cell is clicked
        void setCallback(CellId cell, std::function<void()> callback);
        
        // Update position if window is resized
        void updatePosition(const sf::Vector2f& windowSize);
        
    private:
        static constexpr int NUM_CELLS = 4;
        static constexpr float WIDTH = 150.f;
        
        sf::RectangleShape mBackground;
        std::array<sf::RectangleShape, NUM_CELLS> mCells;
        std::array<std::function<void()>, NUM_CELLS> mCallbacks;
        
        // Converts cell ID to array index
        size_t cellIdToIndex(CellId id) const { return static_cast<size_t>(id); }
};

#endif