#include "../../include/graphics/SideBar.h"

SideBar::SideBar(const sf::Vector2f& windowSize) {
    // Set up the background
    mBackground.setSize({WIDTH, windowSize.y});
    mBackground.setPosition({windowSize.x - WIDTH, 0.f});
    mBackground.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent dark gray
    
    // Set up the cell dimensions
    float cellHeight = windowSize.y / NUM_CELLS;
    
    // Set up each cell
    for (int i = 0; i < NUM_CELLS; ++i) {
        mCells[i].setSize({WIDTH - 10.f, cellHeight - 10.f}); // Slightly smaller than background for padding
        mCells[i].setPosition({windowSize.x - WIDTH + 5.f, 5.f + i * cellHeight});
        mCells[i].setFillColor(sf::Color(70, 70, 70)); // Dark gray
        mCells[i].setOutlineThickness(2.f);
        mCells[i].setOutlineColor(sf::Color(100, 100, 100)); // Light gray outline
        
        // Initialize callbacks to do nothing
        mCallbacks[i] = [](){};
    }
}

SideBar::~SideBar() {
    // No dynamic resources to clean up
}

void SideBar::draw(sf::RenderWindow& window) {
    // Draw background
    window.draw(mBackground);
    
    // Draw each cell
    for (const auto& cell : mCells) {
        window.draw(cell);
    }
}

bool SideBar::handleClick(const sf::Vector2f& position, CellId& outClickedCell) {
    // Check if the click is within the sidebar area
    if (position.x < mBackground.getPosition().x) {
        return false; // Click is outside sidebar
    }
    
    // Check each cell
    for (int i = 0; i < NUM_CELLS; ++i) {
        if (mCells[i].getGlobalBounds().contains(position)) {
            outClickedCell = static_cast<CellId>(i);
            
            // Execute the callback for this cell
            mCallbacks[i]();
            
            return true;
        }
    }
    
    return false; // Click was in sidebar but not on any cell
}

void SideBar::setCallback(CellId cell, std::function<void()> callback) {
    size_t index = cellIdToIndex(cell);
    if (index < NUM_CELLS) {
        mCallbacks[index] = callback;
    }
}

void SideBar::updatePosition(const sf::Vector2f& windowSize) {
    // Update background position
    mBackground.setSize({WIDTH, windowSize.y});
    mBackground.setPosition({windowSize.x - WIDTH, 0.f});
    
    // Update cell positions
    float cellHeight = windowSize.y / NUM_CELLS;
    for (int i = 0; i < NUM_CELLS; ++i) {
        mCells[i].setSize({WIDTH - 10.f, cellHeight - 10.f});
        mCells[i].setPosition({windowSize.x - WIDTH + 5.f, 5.f + i * cellHeight});
    }
} 