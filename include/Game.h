#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <memory>
#include "graphics/HexGrid.h"
#include "InputHandler.h"
#include "graphics/Renderer.h"
#include "characters/Soldier.h"
#include "buildings/City.h"
#include "buildings/Building.h"
#include "graphics/VisibilitySystem.h"
#include "resources/Resource.h"

class Game {
public:
    enum class HighlightAxis {
        None,
        Q,
        R
    };
    
    Game();
    ~Game();
    void run();
    
    // Input event handlers (called from InputHandler)
    void onLeftClick(const sf::Vector2f& worldPos);
    void onRightClick(const sf::Vector2f& worldPos);
    void onKeyPress(sf::Keyboard::Key key);
    void moveSelectedCharacter(Hexagon* source, Hexagon* target, Character* character);
    Hexagon* getSourceHex(Character* character);

private:
    sf::RenderWindow mWindow;
    sf::Clock mClock;
    float mDeltaTime;
    
    sf::View mCamera; // Camera view
    sf::Vector2f mCameraPosition; // Current camera position
    sf::FloatRect mGridBounds; // Boundaries of the hex grid
    
    InputHandler mInputHandler;
    Renderer mRenderer;
    VisibilitySystem mVisibilitySystem;
    
    HexGrid mGrid;
    Hexagon::CubeCoord mSelectedCoord;

    bool mHasSelection;
    
    // Track which axis to highlight
    HighlightAxis mCurrentAxis;
    
    // Use unique_ptr for owned Soldier
    std::unique_ptr<Soldier> mSoldier;
    
    // This is a non-owning reference, so keep as raw pointer
    std::optional<Character*> mSelectedCharacter;
    
    // Store cities for the game
    std::vector<std::unique_ptr<City>> mCities;
    
    // Store resources for the game with proper ownership
    std::vector<std::unique_ptr<Resource>> mResources;
    
    // Store standalone buildings (like oil refineries) with proper ownership
    std::vector<std::unique_ptr<Building>> mBuildings;
    
    // Toggle for fog of war
    bool mFogOfWarEnabled = true;
    
    void update();
    void render();
    void updateCamera(const sf::Vector2f& movement);
    void highlightAxis(HighlightAxis axis);
    
    void generateCityHexesPositions();
    // Clamp camera position to ensure it stays within grid bounds
    sf::Vector2f clampCameraPosition(const sf::Vector2f& position);
    
    // Update visibility based on game entities
    void updateVisibility();
    
    // Helper to toggle fog of war on/off
    void toggleFogOfWar();
    
    // Get all characters for visibility calculations
    std::vector<Character*> getCharacters() const;
    
    // Get all buildings for visibility calculations
    std::vector<Building*> getBuildings() const;
};

#endif // GAME_H 