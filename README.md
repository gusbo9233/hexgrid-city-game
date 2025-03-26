# C++ Game

A simple 2D game created with C++ and SFML.

## Requirements

- CMake 3.10 or higher
- C++17 compatible compiler
- SFML 3.0.0

## Installation

### Install SFML

#### macOS (using Homebrew)
```
brew install sfml
```

#### Ubuntu/Debian
```
# For Ubuntu/Debian, you may need to build SFML 3.0.0 from source
# Check the SFML website for installation instructions
```

#### Windows
Download SFML from the [official website](https://www.sfml-dev.org/download.php) and follow the installation instructions.

## Building the Game

```bash
# Create a build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```

## Running the Game

```bash
# From the build directory
./CPPGame
```

## Game Controls

- W: Move up
- A: Move left
- S: Move down
- D: Move right