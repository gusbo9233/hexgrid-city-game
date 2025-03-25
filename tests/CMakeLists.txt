enable_testing()

# Add test executable
add_executable(
    unit_tests
    unit_tests/character_test.cpp
    unit_tests/hex_grid_test.cpp
    unit_tests/visibility_test.cpp
)

# Link libraries
target_link_libraries(
    unit_tests
    GTest::gtest_main
    SFML::Graphics
    SFML::Window
    SFML::System
)

# Include directories
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Discover tests
include(GoogleTest)
gtest_discover_tests(unit_tests) 