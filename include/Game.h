#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include "HexGrid.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Soldier.h"
#include "City.h"

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
    
    HexGrid mGrid;
    Hexagon::CubeCoord mSelectedCoord;

    bool mHasSelection;
    
    // Track which axis to highlight
    HighlightAxis mCurrentAxis;
    
    Soldier* mSoldier;
    std::optional<Character*> mSelectedCharacter;
    
    // Cities in the game
    std::vector<City*> mCities;
    
    void update();
    void render();
    void updateCamera(const sf::Vector2f& movement);
    void highlightAxis(HighlightAxis axis);
    
    void generateCityHexesPositions();
    // Clamp camera position to ensure it stays within grid bounds
    sf::Vector2f clampCameraPosition(const sf::Vector2f& position);
};

#endif // GAME_H 