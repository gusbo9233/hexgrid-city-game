#include "Game.h"
#include "Soldier.h"
#include <cmath>
#include <algorithm>
#include "City.h"
Game::Game() 
    : mWindow(sf::VideoMode({1200, 800}), "Hexagonal Grid - WASD to move, Q/R to highlight axes"), 
      mDeltaTime(0.f),
      mCamera(sf::Vector2f(0.f, 0.f), sf::Vector2f(1200.f, 800.f)),
      mCameraPosition(0.f, 0.f),
      mInputHandler(mWindow),
      mRenderer(mWindow),
      mGrid(20), // Create a larger grid with radius 20
      mSelectedCoord(0, 0, 0),
      mHasSelection(false),
      mCurrentAxis(HighlightAxis::None),
      mSoldier(nullptr) {
    mWindow.setFramerateLimit(60);
    
    // Initialize the camera view
    mWindow.setView(mCamera);
    
    // Calculate the grid bounds for camera limits
    mGridBounds = mGrid.getBounds();
    
    // Get the center hex
    Hexagon* centerHex = mGrid.getHexAt({0, 0, 0});
    if (centerHex) {
        // Create a soldier at the center hex coordinates (0,0,0)
        mSoldier = new Soldier(0, 0);
        
        // Set the soldier in the hex and highlight it
        centerHex->setCharacter(mSoldier);
        
        // Position the soldier's sprite at the hex's pixel position
        mSoldier->setPosition(centerHex->getPosition());
    } else {
        // Fallback: create at origin if center hex not found
        mSoldier = new Soldier(0, 0);
    }
    mSelectedCharacter = nullptr;
    
    // Generate city positions
    generateCityHexesPositions();
}

// Add destructor to clean up memory
Game::~Game() {
    if (mSoldier) {
        delete mSoldier;
        mSoldier = nullptr;
    }
    
    // Clean up cities
    for (auto* city : mCities) {
        delete city;
    }
    mCities.clear();
}

void Game::run() {
    while (mWindow.isOpen()) {
        mDeltaTime = mClock.restart().asSeconds();
        
        // Process inputs
        mInputHandler.processInputs(*this, mDeltaTime);
        
        // Update camera based on input
        updateCamera(mInputHandler.getCameraMovement(mDeltaTime));
        
        update();
        render();
    }
}

void Game::onLeftClick(const sf::Vector2f& worldPos) {

    if (mHasSelection) {
        mGrid.resetHighlights();
        mHasSelection = false;
        mCurrentAxis = HighlightAxis::None;
        return;
    }
    // Get the hex at the clicked position
    Hexagon* hex = mGrid.getHexAtPixel(worldPos);
    if (hex) {
        mGrid.resetHighlights();
        mSelectedCoord = hex->getCoord();
        mHasSelection = true;
        
        // Highlight the selected hex
        if (hex->getCharacter()) 
        {
            hex->highlight(sf::Color::Yellow);
            mGrid.highlightAdjacentHexes(hex->getCoord(), sf::Color::Green);
        }
        
        // Reset current axis highlight
        mCurrentAxis = HighlightAxis::None;

        //Get the character from the hex
        Character* character = hex->getCharacter();
        if (character) {
            //Set the character as selected
            mSelectedCharacter = character;
        }
    }
}

void Game::onRightClick(const sf::Vector2f& worldPos) {
    // Get the hex at the clicked position
    Hexagon* hex = mGrid.getHexAtPixel(worldPos);
    if (hex && mSelectedCharacter.has_value()) {
        // Get the character from the clicked hex
        Character* character = hex->getCharacter();
        if (!character) {
            // Get the source hex for the selected character
            Hexagon* sourceHex = this->getSourceHex(mSelectedCharacter.value());
            if (sourceHex && mGrid.areAdjacent(sourceHex->getCoord(), hex->getCoord())) {
                // Move the selected character to the clicked hex
                this->moveSelectedCharacter(sourceHex, hex, mSelectedCharacter.value());
                
                // Reset highlighting and selection
                mGrid.resetHighlights();
                mHasSelection = false;
                mCurrentAxis = HighlightAxis::None;
            }
        }
    }
}

Hexagon* Game::getSourceHex(Character* character) {
    // Get the hex at the character's position
    return mGrid.getHexAt(character->getHexCoord());
}

void Game::moveSelectedCharacter(Hexagon* source, Hexagon* target, Character* character) {
    if (mGrid.areAdjacent(source->getCoord(), target->getCoord())) {
        // Remove the character from the source hex
        source->removeCharacter();

        // Set the character in the target hex - this will also update the character's hex coordinates
        target->setCharacter(character);
        
        // Update the character's position for rendering
        character->setPosition(target->getPosition());
        
        // Make sure the character's hex coordinates are updated
        character->setHexCoord(target->getCoord());
    }
}

void Game::onKeyPress(sf::Keyboard::Key key) {
    if (mHasSelection) {
        if (key == sf::Keyboard::Key::Q) {
            mCurrentAxis = HighlightAxis::Q;
            highlightAxis(mCurrentAxis);
        }
        else if (key == sf::Keyboard::Key::R) {
            mCurrentAxis = HighlightAxis::R;
            highlightAxis(mCurrentAxis);
        }
        else if (key == sf::Keyboard::Key::Escape) {
            mGrid.resetHighlights();
            mHasSelection = false;
            mCurrentAxis = HighlightAxis::None;
        }
    }
}

sf::Vector2f Game::clampCameraPosition(const sf::Vector2f& position) {
    // Get the current view size
    sf::Vector2f viewSize = mCamera.getSize();
    
    // Calculate the view half-size
    float halfWidth = viewSize.x / 2.0f;
    float halfHeight = viewSize.y / 2.0f;
    
    // Calculate allowed camera region (ensure the camera doesn't show outside the grid bounds)
    float minX = mGridBounds.position.x + halfWidth;
    float maxX = mGridBounds.position.x + mGridBounds.size.x - halfWidth;
    float minY = mGridBounds.position.y + halfHeight;
    float maxY = mGridBounds.position.y + mGridBounds.size.y - halfHeight;
    
    // If the grid is smaller than the view, center the camera on the grid
    if (mGridBounds.size.x < viewSize.x) {
        minX = maxX = mGridBounds.position.x + mGridBounds.size.x / 2.0f;
    }
    
    if (mGridBounds.size.y < viewSize.y) {
        minY = maxY = mGridBounds.position.y + mGridBounds.size.y / 2.0f;
    }
    
    // Clamp the camera position
    sf::Vector2f clampedPosition;
    clampedPosition.x = std::clamp(position.x, minX, maxX);
    clampedPosition.y = std::clamp(position.y, minY, maxY);
    
    return clampedPosition;
}

void Game::updateCamera(const sf::Vector2f& movement) {
    // Update camera position
    mCameraPosition += movement;
    
    // Clamp position to grid bounds
    mCameraPosition = clampCameraPosition(mCameraPosition);
    
    // Apply the clamped position
    mCamera.setCenter(mCameraPosition);
    
    // Update both the window and renderer views
    mWindow.setView(mCamera);
    mRenderer.setView(mCamera);
}

void Game::update() {
    // No additional updates needed
}

void Game::render() {
    // Clear the screen
    mRenderer.clear();
    
    // Render the hex grid
    mRenderer.render(mGrid);
    
    // Render the soldier
    if (mSoldier) {
        mRenderer.render(*mSoldier);
    }
    
    // Display the rendered frame
    mRenderer.display();
}

void Game::highlightAxis(HighlightAxis axis) {
    mGrid.resetHighlights();
    
    // Highlight the selected hex
    auto* selectedHex = mGrid.getHexAt(mSelectedCoord);
    if (selectedHex) {
        selectedHex->setFillColor(sf::Color::Yellow);
    }
    
    // Highlight hexes based on the selected axis
    switch (axis) {
        case HighlightAxis::Q:
            // Highlight all hexes with the same q coordinate
            mGrid.highlightHexes(
                [q = mSelectedCoord.q](const Hexagon::CubeCoord& coord) {
                    return coord.q == q;
                },
                sf::Color::Green
            );
            break;
            
        case HighlightAxis::R:
            // Highlight all hexes with the same r coordinate
            mGrid.highlightHexes(
                [r = mSelectedCoord.r](const Hexagon::CubeCoord& coord) {
                    return coord.r == r;
                },
                sf::Color::Blue
            );
            break;
            
        default:
            break;
    }
} 

void Game::generateCityHexesPositions() {
    // Create vectors to store the hexes for each city
    std::vector<Hexagon*> city1Hexes;
    std::vector<Hexagon*> city2Hexes;
    std::vector<Hexagon*> city3Hexes;
    
    // Define city sizes (number of hexes per city)
    const int city1Size = 7;
    const int city2Size = 5;
    const int city3Size = 6;
    
    // Define seed positions for cities in the bottom half of the grid (positive r)
    // We'll use different quadrants to keep cities separated
    Hexagon::CubeCoord city1Seed(5, 5, -10);   // Bottom right
    Hexagon::CubeCoord city2Seed(-8, 10, -2);  // Bottom left
    Hexagon::CubeCoord city3Seed(-2, 8, -6);   // Bottom center
    
    // Helper function to grow a city from a seed
    auto growCity = [this](Hexagon::CubeCoord seed, int size, std::vector<Hexagon*>& cityHexes) {
        // Get the seed hex
        Hexagon* seedHex = mGrid.getHexAt(seed);
        if (!seedHex) return;
        
        // Start with the seed
        cityHexes.push_back(seedHex);
        
        // Use a breadth-first approach to add adjacent hexes
        std::vector<Hexagon*> frontier = {seedHex};
        std::vector<Hexagon*> nextFrontier;
        
        while (cityHexes.size() < size && !frontier.empty()) {
            nextFrontier.clear();
            
            for (const auto& current : frontier) {
                // Check all six directions
                for (const auto& dir : Hexagon::directions) {
                    Hexagon::CubeCoord neighborCoord = current->getCoord() + dir;
                    
                    // Check if the hex is valid and not already in city
                    Hexagon* neighborHex = mGrid.getHexAt(neighborCoord);
                    if (neighborHex && 
                        std::find(cityHexes.begin(), cityHexes.end(), neighborHex) == cityHexes.end()) {
                        
                        // Add to city if we haven't reached the size limit
                        if (cityHexes.size() < size) {
                            cityHexes.push_back(neighborHex);
                            nextFrontier.push_back(neighborHex);
                        } else {
                            break;
                        }
                    }
                }
            }
            
            frontier = nextFrontier;
        }
    };
    
    // Grow each city from its seed
    growCity(city1Seed, city1Size, city1Hexes);
    growCity(city2Seed, city2Size, city2Hexes);
    growCity(city3Seed, city3Size, city3Hexes);
    
    // Define unique, visually distinct colors for each city (more greyish)
    sf::Color city1Color(144, 131, 131); // Muted reddish-grey
    sf::Color city2Color(140, 170, 140); // Muted greenish-grey
    sf::Color city3Color(140, 140, 180); // Muted bluish-grey
    
    // Create city objects with their unique colors
    City* city1 = new City(city1Hexes, city1Color);
    City* city2 = new City(city2Hexes, city2Color);
    City* city3 = new City(city3Hexes, city3Color);
    
    // Add cities to the game's collection
    mCities.push_back(city1);
    mCities.push_back(city2);
    mCities.push_back(city3);
}