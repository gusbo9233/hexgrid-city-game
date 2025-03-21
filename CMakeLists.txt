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
    src/HexGrid.cpp
    src/InputHandler.cpp
    src/Renderer.cpp
    src/Character.cpp
    src/characters/Soldier.cpp
    src/Building.cpp
    src/buildings/Base.cpp
    src/buildings/ResidentialArea.cpp
    src/buildings/CityCenter.cpp
    src/City.cpp
    src/business/Workplace.cpp
    src/business/Product.cpp
    src/business/Resource.cpp
    src/business/Proffesion.cpp
    src/business/workplaces/OilRefinery.cpp
    src/VisibilitySystem.cpp
)

# Link SFML libraries
target_link_libraries(CPPGame SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Copy assets to build directory
add_custom_command(TARGET CPPGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
) 