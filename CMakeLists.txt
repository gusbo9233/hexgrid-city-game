cmake_minimum_required(VERSION 3.10)
project(CPPGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML packages
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add executable
add_executable(CPPGame 
    src/main.cpp
    src/Game.cpp
    src/Hexagon.cpp
    src/InputHandler.cpp
    src/Player.cpp
    src/City.cpp
    src/GameObject.cpp
    
    # Character files
    src/characters/Character.cpp
    src/characters/Soldier.cpp
    src/characters/Tank.cpp
    
    # Building files
    src/buildings/Building.cpp
    src/buildings/Base.cpp
    src/buildings/ResidentialArea.cpp
    src/buildings/CityCenter.cpp
    
    # Business files
    src/business/workplaces/Workplace.cpp
    src/business/Proffesion.cpp
    src/business/workplaces/OilRefinery.cpp
    src/business/workplaces/Farm.cpp
    
    # Graphics files
    src/graphics/HexGrid.cpp
    src/graphics/Renderer.cpp
    src/graphics/VisibilitySystem.cpp
    src/graphics/GridFiller.cpp
    src/graphics/SideBar.cpp
    src/graphics/TextureManager.cpp
    
    # Resource files
    src/resources/Resource.cpp
    src/resources/Oil.cpp
    
    # Projectile files
    src/projectiles/projectile.cpp
    src/projectiles/Bullet.cpp
    src/projectiles/TankAmmo.cpp
    src/projectiles/ProjectileFactory.cpp
    
    # Economy files
    src/economy/NationalAccounts.cpp
    src/economy/InternationalMarkets.cpp
    src/economy/Government.cpp

    # Civilian files
    src/civilians/Individual.cpp
    src/civilians/Household.cpp
)

# Link SFML libraries
target_link_libraries(CPPGame SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Copy assets to build directory
add_custom_command(TARGET CPPGame PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
    ${CMAKE_BINARY_DIR}/assets
)

add_custom_command(TARGET CPPGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
) 