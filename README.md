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

# C++ Hex Grid Game

## Polymorphic Best Practices

This codebase follows these polymorphic best practices:

1. **Virtual Destructors**: All base classes have virtual destructors to ensure proper cleanup of derived objects.

2. **`override` Specifier**: All overridden methods are marked with the `override` specifier, which:
   - Documents the intention to override a base class method
   - Catches errors at compile time if the base class method changes
   - Makes inheritance relationships clearer

3. **`final` for Leaf Classes**: Classes that are not meant to be further derived from are marked as `final`, which:
   - Prevents unplanned inheritance
   - Enables compiler optimizations
   - Makes the design intent explicit

4. **Non-Virtual Interface (NVI) Pattern**: Public methods are non-virtual and call protected virtual methods for customization:
   - `draw()` calls `doDraw()`
   - `render()` calls `doRender()`
   - This allows for consistent pre/post processing

5. **Pure Virtual Methods**: Abstract methods that must be implemented by derived classes are marked as pure virtual:
   - `getType()`
   - `getScaleFactor()`

6. **Smart Pointers**: Using `std::unique_ptr` for owned objects to prevent memory leaks and ensure proper cleanup.

7. **Clear Ownership Semantics**: Each object has a clear owner:
   - Containers (`City`, `Game`) own their contained objects (Buildings, Cities)
   - Non-owning references use raw pointers

8. **Virtual Behavior Methods**: Common behaviors like `update()` are declared virtual to allow derived classes to customize them.

This approach provides the benefits of inheritance while avoiding common pitfalls. 