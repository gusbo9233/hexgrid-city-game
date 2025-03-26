#include "../include/Game.h"
#include "../include/characters/Soldier.h"
#include <cmath>
#include <algorithm>
#include "../include/buildings/City.h"
#include "../include/graphics/GridFiller.h"
#include "../include/resources/Resource.h"
#include <Workplace.h>

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
      mSoldier(nullptr),
      mFogOfWarEnabled(true) {
    mWindow.setFramerateLimit(60);
    
    // Initialize the camera view
    mWindow.setView(mCamera);
    
    // Calculate the grid bounds for camera limits
    mGridBounds = mGrid.getBounds();
    
    // Use GridFiller to populate the grid with cities and resources
    GridFiller gridFiller(mGrid);
    gridFiller.fillGrid();
    mCities = gridFiller.getCities();
    mResources = gridFiller.getResources(); // Take ownership of resources
    mBuildings = gridFiller.getBuildings();
    mNationalAccounts = NationalAccounts();
    mInternationalMarkets = InternationalMarkets();
    mEconomicPolicy = EconomicPolicy();
    mGovernment = Government();
    // Starting position for soldier in bottom half of map (positive r value)
    Hexagon::CubeCoord soldierStartPos = {2, 5, -7};
    
    // Get the hex at the bottom half coordinate
    Hexagon* startHex = mGrid.getHexAt(soldierStartPos);
    if (startHex) {
        // Create a soldier at the bottom half coordinate with unique_ptr
        mSoldier = std::make_unique<Soldier>(soldierStartPos.q, soldierStartPos.r);
        
        // Set the soldier in the hex and highlight it
        startHex->setCharacter(mSoldier.get());
        
        // Position the soldier's sprite at the hex's pixel position
        mSoldier->setPosition(startHex->getPosition());
    } else {
        // Fallback: create at origin if hex not found
        mSoldier = std::make_unique<Soldier>(0, 0);
    }
    mSelectedCharacter = nullptr;
    
    // Center camera on soldier position
    if (startHex) {
        mCameraPosition = startHex->getPosition();
        mCamera.setCenter(mCameraPosition);
        mWindow.setView(mCamera);
        mRenderer.setView(mCamera);
    }
    
    // No need to call generateCityHexesPositions() anymore since GridFiller handles it
}

// Add destructor to clean up memory
Game::~Game() {
    // Smart pointers automatically clean up mSoldier and mCities
    // No explicit cleanup needed
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
        mSelectedCharacter = std::nullopt;
        return;
    }
    // Get the hex at the clicked position
    Hexagon* hex = mGrid.getHexAtPixel(worldPos);
    if (hex) {
        mGrid.resetHighlights();
        mSelectedCoord = hex->getCoord();
        mHasSelection = true;
        
        // Select and highlight based on what's on the hex
        if (hex->getCharacter()) {
            Character* character = hex->getCharacter();
            
            // Highlight character hex in yellow
            hex->highlight(sf::Color::Yellow);
            
            // Different behavior based on character type
            if (character->isCharacterType(CharacterType::Soldier)) {
                // Soldiers can move to adjacent hexes
                mGrid.highlightAdjacentHexes(hex->getCoord(), sf::Color::Green);
            }
            
            // Set the character as selected
            mSelectedCharacter = character;
        } 
        else if (hex->hasBuilding()) {
            Building* building = hex->getBuilding();
            
            // Different highlighting based on building type
            if (building->isBuildingType(BuildingType::CityCenter)) {
                // City center gets an orange highlight
                hex->highlight(sf::Color(255, 165, 0)); // Orange
            } 
            else if (building->isBuildingType(BuildingType::ResidentialArea)) {
                // Residential areas get a light blue highlight
                hex->highlight(sf::Color(135, 206, 235)); // Sky blue
            }
            else {
                // Default building highlight
                hex->highlight(sf::Color(255, 200, 0)); // Gold
            }
            
            // No character selection
            mSelectedCharacter = std::nullopt;
        }
        else if (hex->hasResource()) {
            Resource* resource = hex->getResource();
            
            // Highlight resource hex in purple
            hex->highlight(sf::Color(160, 32, 240)); // Purple
            
            // No character selection
            mSelectedCharacter = std::nullopt;
        }
        else {
            // Highlight empty hex in cyan
            hex->highlight(sf::Color(0, 255, 255)); // Cyan
            
            // No character selection
            mSelectedCharacter = std::nullopt;
        }
        
        // Reset current axis highlight
        mCurrentAxis = HighlightAxis::None;
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
    
    // Add fog of war toggle with 'F' key
    if (key == sf::Keyboard::Key::F) {
        toggleFogOfWar();
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
    
    // Update visibility for fog of war
    updateVisibility();
}

void Game::render() {
    mRenderer.clear();
    
    // Set fog of war state in renderer
    mRenderer.setFogOfWarEnabled(mFogOfWarEnabled);
    
    // Render the grid (which now handles visibility)
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

void Game::updateVisibility() {
    if (!mFogOfWarEnabled) {
        // If fog of war is disabled, make everything visible
        for (auto hex : mGrid.getAllHexes()) {
            hex->setVisible(true);
            // No longer setting explored state
        }
        return;
    }
    
    // Get all game entities for visibility calculation
    std::vector<Character*> characters = getCharacters();
    std::vector<Building*> buildings = getBuildings();
    std::vector<City*> cities;
    
    // Extract raw pointers to cities
    for (const auto& city : mCities) {
        cities.push_back(city.get());
    }
    
    // Update visibility
    mVisibilitySystem.updateVisibility(mGrid, characters, buildings, cities);
}

void Game::toggleFogOfWar() {
    mFogOfWarEnabled = !mFogOfWarEnabled;
    
    // Update visibility immediately
    updateVisibility();
}

std::vector<Character*> Game::getCharacters() const {
    std::vector<Character*> characters;
    
    // Add the soldier if it exists
    if (mSoldier) {
        characters.push_back(mSoldier.get());
    }
    
    // Add any other characters here
    
    return characters;
}

std::vector<Building*> Game::getBuildings() const {
    std::vector<Building*> buildings;
    
    // Collect buildings from all cities
    for (const auto& city : mCities) {
        for (const auto& [coord, building] : city->getBuildings()) {
            buildings.push_back(building.get());
        }
    }
    
    // Add standalone buildings (like oil refineries)
    for (const auto& building : mBuildings) {
        buildings.push_back(building.get());
    }
    
    return buildings;
}

void Game::generateProducts() {
    std::vector<Building*> mBuildings = getBuildings();
    for (const auto& building : mBuildings) {
        if (building->isBuildingType(BuildingType::Workplace)) {
            Workplace* workplace = dynamic_cast<Workplace*>(building);
            workplace->generateProduct();
            int sellPrice = mInternationalMarkets.getProductPrice(workplace->getProductType());
            int taxAmount = sellPrice * mEconomicPolicy.getTaxRate();
            bool soldProduct = workplace->sellProduct(workplace->getProductType(), 1, sellPrice - taxAmount);
            if (soldProduct) {
                mNationalAccounts.addMoney(sellPrice);
                mGovernment.addMoney(taxAmount);
            }
        }
    }
}
