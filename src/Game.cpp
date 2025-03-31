#include <SFML/Graphics.hpp>
#include "../include/Game.h"
#include "../include/characters/Soldier.h"
#include <cmath>
#include <algorithm>
#include "../include/buildings/City.h"
#include "../include/graphics/GridFiller.h"
#include "../include/resources/Resource.h"
#include "../include/business/workplaces/Workplace.h"
#include <iostream>
#include <sstream>  // For stringstream
#include <memory>
#include <optional>
#include <string>
#include <limits>
#include <vector>

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
      mFogOfWarEnabled(true),
      mSideBar(sf::Vector2f(1200.f, 800.f)) {
    mWindow.setFramerateLimit(60);
    
    // Initialize the camera view
    mWindow.setView(mCamera);
    
    // Calculate the grid bounds for camera limits
    mGridBounds = mGrid.getBounds();
    
    // Use GridFiller to populate the grid with cities and resources
    GridFiller gridFiller(mGrid);
    gridFiller.fillGrid();
    
    // Transfer vector contents to lists
    auto citiesVector = gridFiller.getCities();
    for (auto& city : citiesVector) {
        mCities.push_back(std::move(city));
    }
    
    // Transfer resources
    auto resourcesVector = gridFiller.getResources();
    for (auto& resource : resourcesVector) {
        mResources.push_back(std::move(resource));
    }
    
    // Transfer buildings
    auto buildingsVector = gridFiller.getBuildings();
    for (auto& building : buildingsVector) {
        mBuildings.push_back(std::move(building));
    }
    
    mNationalAccounts = NationalAccounts();
    mInternationalMarkets = InternationalMarkets();
    mGovernment = Government();
    
    // Initialize font for UI text
    if (!mFont.openFromFile("assets/fonts/arial.ttf")) {
        // Fallback to default system font if asset not found
        std::cerr << "Warning: Could not load font from assets/fonts/arial.ttf" << std::endl;
    } else {
        // Create the text object with the loaded font - font parameter must be first
        mGovDataText.emplace(mFont, "", 16);
        mGovDataText->setFillColor(sf::Color::White);
        mGovDataText->setPosition({10.f, 10.f});
    }
    
    // Transfer characters from GridFiller to Game
    auto charactersVector = gridFiller.getCharacters();
    for (auto& character : charactersVector) {
        mCharacters.push_back(std::move(character));
    }
    
    mSelectedCharacter = nullptr;
    
    // Find a friendly soldier to center the view on
    Hexagon* startHex = nullptr;
    for (const auto& character : mCharacters) {
        if (character->getAllegiance() == Allegiance::FRIENDLY) {
            // Get the hex at the character's position
            startHex = mGrid.getHexAt(character->getHexCoord());
            if (startHex) break;
        }
    }
    
    // Center camera on friendly soldier position if found
    if (startHex) {
        mCameraPosition = startHex->getPosition();
        mCamera.setCenter(mCameraPosition);
        mWindow.setView(mCamera);
        mRenderer.setView(mCamera);
    }
    
    // No need to call generateCityHexesPositions() anymore since GridFiller handles it
    
    // Set up sidebar callbacks
    mSideBar.setCallback(SideBar::CellId::Cell1, [this]() { 
        std::cout << "Sidebar Cell 1 clicked!" << std::endl;
        // Add functionality for Cell 1
    });
    
    mSideBar.setCallback(SideBar::CellId::Cell2, [this]() { 
        std::cout << "Sidebar Cell 2 clicked!" << std::endl;
        // Add functionality for Cell 2
    });
    
    mSideBar.setCallback(SideBar::CellId::Cell3, [this]() { 
        std::cout << "Sidebar Cell 3 clicked!" << std::endl;
        // Add functionality for Cell 3
    });
    
    mSideBar.setCallback(SideBar::CellId::Cell4, [this]() { 
        std::cout << "Sidebar Cell 4 clicked!" << std::endl;
        // Add functionality for Cell 4
    });
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
    // Convert window position to world coordinates
    sf::Vector2f screenPos = mWindow.mapPixelToCoords(
        sf::Vector2i(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y)),
        mWindow.getDefaultView() // Use default view for UI elements
    );
    
    // Check if the sidebar was clicked
    SideBar::CellId clickedCell;
    if (mSideBar.handleClick(screenPos, clickedCell)) {
        handleSideBarClick(clickedCell);
        return; // Don't process grid clicks if sidebar was clicked
    }

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
            
            // Different behavior based on character type and allegiance
            if (character->isCharacterType(CharacterType::Soldier)) {
                // Only show movement options for friendly soldiers
                if (character->getAllegiance() == Allegiance::FRIENDLY) {
                    // Soldiers can move to adjacent hexes
                    mGrid.highlightAdjacentHexes(hex->getCoord(), sf::Color::Green);
                }
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
        // Check if the selected character is friendly - only allow moving friendly soldiers
        if (mSelectedCharacter.value()->getAllegiance() != Allegiance::FRIENDLY) {
            // Cannot move enemy soldiers
            return;
        }
        
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
        if (target->hasBuilding()) {
            if (target->getBuilding()->getAllegiance() != character->getAllegiance() 
            && target->getBuilding()->getDefenses() > 0) {
               return;
            }
        }
        if (target->hasCharacter()) {
            return;
        }
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
    float maxX = mGridBounds.position.x + mGridBounds.size.x - halfWidth + 75.f; // Added 75 to accommodate wider sidebar
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
    // Update cooldowns for all characters
    for (auto* character : getCharacters()) {
        character->updateCooldowns(mDeltaTime);
    }
    
    generateProducts();
    // Update visibility for fog of war
    updateVisibility();
    mNationalAccounts.nextDay();
    setCharactersTargetPosition();
    moveProjectiles();
}

void Game::render() {
    mRenderer.clear();
    
    // Set fog of war state in renderer
    mRenderer.setFogOfWarEnabled(mFogOfWarEnabled);
    
    // Render the grid (which now handles visibility)
    mRenderer.render(mGrid);
    
    // Render all characters that are on visible hexes
    for (const auto& character : mCharacters) {
        // Get the hex at the character's position
        Hexagon* hex = mGrid.getHexAt(character->getHexCoord());
        
        // Only render the character if its hex is visible or fog of war is disabled
        if (!mFogOfWarEnabled || (hex && hex->isVisible())) {
            mRenderer.render(*character);
        }
    }
    
    // Render all projectiles using the GameObject renderer
    for (const auto& projectile : mProjectiles) {
        // For projectiles, we could check if they're in visible area
        // But for gameplay purposes, always show projectiles
        mRenderer.render(*projectile);
    }
    
    // Store the current view
    sf::View currentView = mWindow.getView();
    
    // Switch to default view for UI elements
    mWindow.setView(mWindow.getDefaultView());
    
    // Render the sidebar
    mSideBar.draw(mWindow);
    
    // Render government data on screen
    renderGovernmentData();
    
    // Restore the game view
    mWindow.setView(currentView);
    
    // Display the rendered frame
    mRenderer.display();
}

void Game::renderGovernmentData() {
    // Only render if text was successfully created
    if (!mGovDataText.has_value()) {
        return;
    }
    
    // Create text to display
    std::stringstream ss;
    ss << "Government Data:" << std::endl
       << "Treasury: $" << mGovernment.getMoney() << std::endl
       << "Tax Rate: " << (mGovernment.getTaxRate() * 100) << "%" << std::endl
       << "Interest Rate: " << (mGovernment.getInterestRate() * 100) << "%" << std::endl
       << "GDP: $" << mNationalAccounts.getGDP();
    
    mGovDataText->setString(ss.str());
    
    // Draw text directly to window
    mWindow.draw(*mGovDataText);
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
    
    // Add all characters from the list
    for (const auto& character : mCharacters) {
        characters.push_back(character.get());
    }
    
    return characters;
}

std::vector<Building*> Game::getBuildings() const {
    std::vector<Building*> buildings;
    
    // Search for all buildings in the grid
    for (const auto& hex : mGrid.getAllHexes()) {
        if (hex->hasBuilding()) {
            buildings.push_back(hex->getBuilding());
        }
    }
    
    // Also add buildings from our list
    for (const auto& building : mBuildings) {
        buildings.push_back(building.get());
    }
    
    return buildings;
}

void Game::generateProducts() {
    std::vector<Building*> mBuildings = getBuildings();
    for (const auto& building : mBuildings) {
        Workplace* workplace = dynamic_cast<Workplace*>(building);
        if (workplace) {
            //std::cout << "Generating product for workplace" << std::endl;
            workplace->generateProduct();
            int sellPrice = mInternationalMarkets.getProductPrice(workplace->getProductType());
            mNationalAccounts.addMoney(sellPrice);

            int taxAmount = sellPrice * mGovernment.getTaxRate();
            bool soldProduct = workplace->sellProduct(workplace->getProductType(), 1, sellPrice - taxAmount);
            if (soldProduct) {
                mGovernment.addMoney(taxAmount);
                std::cout << "Sold product for " << sellPrice << " money" << std::endl;
                std::cout<<mGovernment.getMoney()<<std::endl;
            }
        }
    }
}

void Game::handleSideBarClick(SideBar::CellId cell) {
    // This function can be used to add specific actions when sidebar cells are clicked
    // The callbacks set up in the constructor will already have executed
    switch (cell) {
        case SideBar::CellId::Cell1:
            // Additional handling for Cell 1 if needed
            break;
        case SideBar::CellId::Cell2:
            // Additional handling for Cell 2 if needed
            break;
        case SideBar::CellId::Cell3:
            // Additional handling for Cell 3 if needed
            break;
        case SideBar::CellId::Cell4:
            // Additional handling for Cell 4 if needed
            break;
    }
}

void Game::setCharactersTargetPosition() {
    for (const auto& character : getCharacters()) {
        std::vector<Hexagon*> hexesInRange = mGrid.getHexesInRange(character->getHexCoord(), character->getRange());
        
        std::cout << "Character at (" << character->getQ() << "," << character->getR() 
                  << ") with range " << character->getRange()
                  << " found " << hexesInRange.size() << " hexes in range" << std::endl;
        
        // Clear any existing target
        character->clearTargetPosition();
        
        // HIGHEST PRIORITY: Check for ADJACENT enemy characters first
        Character* adjacentEnemy = nullptr;
        std::vector<Hexagon::CubeCoord> adjacentHexes = mGrid.getAdjacentHexes(character->getHexCoord());
        
        for (const auto& adjacentCoord : adjacentHexes) {
            Hexagon* adjacentHex = mGrid.getHexAt(adjacentCoord);
            if (adjacentHex && adjacentHex->hasCharacter()) {
                Character* targetCharacter = adjacentHex->getCharacter();
                if (targetCharacter->getAllegiance() != character->getAllegiance()) {
                    adjacentEnemy = targetCharacter;
                    std::cout << "Found ADJACENT enemy at (" << adjacentCoord.q << "," 
                              << adjacentCoord.r << ")" << std::endl;
                    break; // Found an adjacent enemy, no need to check others
                }
            }
        }
        
        // If we found an adjacent enemy, immediately target it with highest priority
        if (adjacentEnemy) {
            sf::Vector2f targetPos = Hexagon::cubeToPixel(adjacentEnemy->getHexCoord(), 25.0f);
            character->setTargetPosition(targetPos);
            std::cout << "Setting ADJACENT enemy as target at position: (" 
                      << targetPos.x << "," << targetPos.y << ")" << std::endl;
                    
            // Try to shoot at the adjacent enemy
            std::optional<Projectile> projectile = character->shootProjectile();
            if (projectile) {
                std::cout << "Shot projectile at adjacent enemy" << std::endl;
                mProjectiles.push_back(std::make_unique<Projectile>(std::move(projectile.value())));
            }
            continue; // Skip the rest of the targeting logic
        }
        
        // If no adjacent enemies, then proceed with the original targeting logic
        // First try to find the closest enemy character in range
        float closestCharacterDistance = std::numeric_limits<float>::max();
        Character* closestCharacter = nullptr;
        
        int enemyCharactersFound = 0;
        
        for (const auto& hex : hexesInRange) {
            if (hex->hasCharacter()) {
                Character* targetCharacter = hex->getCharacter();
                
                std::cout << "  Found character at hex (" << hex->getCoord().q << "," << hex->getCoord().r
                          << ") with allegiance " << (targetCharacter->getAllegiance() == Allegiance::FRIENDLY ? "FRIENDLY" : "ENEMY")
                          << " (our allegiance: " << (character->getAllegiance() == Allegiance::FRIENDLY ? "FRIENDLY" : "ENEMY") << ")" << std::endl;
                
                // Check if it's an enemy to our character (don't target friendlies)
                if (targetCharacter->getAllegiance() != character->getAllegiance()) {
                    enemyCharactersFound++;
                    
                    // Calculate distance
                    float distance = Hexagon::distance(character->getHexCoord(), targetCharacter->getHexCoord());
                    
                    std::cout << "    Enemy character found at distance " << distance << std::endl;
                    
                    if (distance < closestCharacterDistance) {
                        closestCharacterDistance = distance;
                        closestCharacter = targetCharacter;
                        std::cout << "    This is the closest enemy so far" << std::endl;
                    }
                }
            }
        }
        
        std::cout << "Found " << enemyCharactersFound << " enemy characters in range" << std::endl;
        
        // If we found a character in range, set it as the target
        if (closestCharacter) {
            sf::Vector2f targetPos = Hexagon::cubeToPixel(closestCharacter->getHexCoord(), 25.0f);
            character->setTargetPosition(targetPos);
            std::cout << "Setting closest character as target at position: (" 
                      << targetPos.x << "," << targetPos.y << ")" << std::endl;
            continue; // We're done with this character
        }
        
        // If no character was found, look for buildings
        float closestBuildingDistance = std::numeric_limits<float>::max();
        Building* closestBuilding = nullptr;
        
        int enemyBuildingsFound = 0;
        
        for (const auto& hex : hexesInRange) {
            if (hex->hasBuilding()) {
                Building* targetBuilding = hex->getBuilding();
                
                // Only target buildings of opposing allegiance
                if (targetBuilding->getAllegiance() != character->getAllegiance()) {
                    enemyBuildingsFound++;
                    
                    // Calculate distance
                    float distance = Hexagon::distance(character->getHexCoord(), hex->getCoord());
                    
                    if (distance < closestBuildingDistance) {
                        closestBuildingDistance = distance;
                        closestBuilding = targetBuilding;
                    }
                }
            }
        }
        
        std::cout << "Found " << enemyBuildingsFound << " enemy buildings in range" << std::endl;
        
        // If we found a building in range, set it as the target
        if (closestBuilding) {
            sf::Vector2f targetPos = closestBuilding->getPosition();
            character->setTargetPosition(targetPos);
            std::cout << "Setting building as target at position: (" 
                      << targetPos.x << "," << targetPos.y << ")" << std::endl;
        }
        
        // Debug output only if character has a target
        if (character->hasTarget()) {
            std::optional<Projectile> projectile = character->shootProjectile();
            if (projectile) {
                std::cout << "Shot projectile" << std::endl;
                // Store the projectile in our collection
                // Since Character::shootProjectile already returns the correct projectile type (Bullet)
                mProjectiles.push_back(std::make_unique<Projectile>(std::move(projectile.value())));
            }
            std::cout << "Character has target at: " << character->getTargetPosition().x << ", " 
                      << character->getTargetPosition().y << std::endl;
        } else {
            std::cout << "Character has no target" << std::endl;
        }
    }
}

void Game::moveProjectiles() {
    // Update all projectiles
    for (auto it = mProjectiles.begin(); it != mProjectiles.end();) {
        (*it)->update();
        // Print projectile position
        //std::cout << "Projectile position: " << (*it)->getPosition().x << ", " << (*it)->getPosition().y << std::endl;
        
        // Check for collisions and get whether to remove the projectile
        bool removeProjectile = checkCollisions(it->get());
        
        // Either remove the projectile or advance to the next one
        if (removeProjectile) {
            it = mProjectiles.erase(it); // erase() returns iterator to next element
        } else {
            ++it;
        }
    }
}

bool Game::checkCollisions(Projectile* projectile) {
    // Check if the projectile is colliding with any buildings
    for (const auto& building : getBuildings()) {
        if (building->getAllegiance() != projectile->getAllegiance()) {
            auto intersection = projectile->getBoundingBox().findIntersection(building->getBoundingBox());
            if (intersection.has_value()) {
                // Handle collision with the building
                std::cout << "Projectile collided with building" << std::endl;
                building->takeDamage(projectile->getDamage());
                return true; // Remove projectile
            }
        }
    }
    
    // Get characters for collision testing
    auto characters = getCharacters();
    
    for (auto character : characters) {
        if (character->getAllegiance() != projectile->getAllegiance()) {
            auto intersection = projectile->getBoundingBox().findIntersection(character->getBoundingBox());
            if (intersection.has_value()) {
                character->takeDamage(projectile->getDamage());
                std::cout << "Projectile collided with character" << std::endl;
                
                // Check if the character is dead after taking damage
                if (character->isDead()) {
                    std::cout << "Character died, removing from hex and game" << std::endl;
                    
                    // Find the hex containing this character and remove it
                    Hexagon* hex = mGrid.getHexAt(character->getHexCoord());
                    if (hex) {
                        hex->removeCharacter();
                    }
                    
                    // Find and remove the character from our list (which will delete it)
                    auto it = std::find_if(mCharacters.begin(), mCharacters.end(),
                        [character](const std::unique_ptr<Character>& c) {
                            return c.get() == character;
                        });
                    
                    if (it != mCharacters.end()) {
                        // If this was the selected character, clear the selection
                        if (mSelectedCharacter.has_value() && mSelectedCharacter.value() == character) {
                            mSelectedCharacter.reset();
                        }
                        
                        // Erase from the list (which will call the destructor via unique_ptr)
                        mCharacters.erase(it);
                    }
                }
                
                return true; // Remove projectile
            }
        }
    }
    
    return false; // Don't remove the projectile
}   