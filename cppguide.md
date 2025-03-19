# C++ Programming Guide
## Using Examples from a Hexagon Game

## Introduction to C++

C++ is a powerful, general-purpose programming language that extends the C language with object-oriented features. It was designed by Bjarne Stroustrup and is widely used for system/software development, game programming, and performance-critical applications.

This guide will teach C++ concepts using real-world examples from our hexagon-based game project.

## Basic C++ Concepts

### Classes and Objects

Classes are the building blocks of object-oriented programming in C++. A class is a blueprint that defines attributes (data members) and behaviors (member functions) of objects.

```cpp
// Basic Class Definition (Character.h)
class Character {
    public:
        Character(int q, int r);
        virtual ~Character() = default;
        
        // Position setting methods
        void setPosition(const sf::Vector2f& pixelPos);
        void setHexCoord(int q, int r);
        
    private:
        int mQ; // Q coordinate in hex grid
        int mR; // R coordinate in hex grid
        
        int health;
        int maxHealth;
        int attackPower;
};
```

The `Character` class defines the basic properties and behaviors of game characters. Objects are instances of classes.

### Encapsulation

Encapsulation is hiding implementation details and restricting access to data through access specifiers:

- `public`: Members accessible from anywhere
- `protected`: Members accessible within the class and derived classes
- `private`: Members accessible only within the class

```cpp
// Encapsulation Example (Building.h)
class Building {
    public:
        // Public interface - accessible to all
        void draw(sf::RenderWindow& window) const;
        void setPosition(const sf::Vector2f& position);
        sf::Vector2f getPosition() const { return mPosition; }
    
    protected:
        // Protected members - accessible to derived classes
        void initializeShape();
        bool tryLoadTexture(const std::string& imagePath);
        static constexpr float STANDARD_SIZE = 25.0f;
        
    private:
        // Private members - only accessible within Building
        sf::RectangleShape mShape;
        sf::Vector2f mPosition;
        std::unique_ptr<sf::Texture> mTexture;
        bool mHasTexture;
};
```

### Headers and Source Files

C++ programs typically split class declarations (headers) from implementations (source files):

```cpp
// Header File (Soldier.h)
#ifndef SOLDIER_H
#define SOLDIER_H

#include "Character.h"

class Soldier final : public Character {
public:
    Soldier(int q, int r);
    
    // Methods declarations
    CharacterType getType() const override { return CharacterType::Soldier; }
    void attack(Character* target);
    void move(Hexagon* targetHex);

protected:
    void doRender(sf::RenderWindow& window) const override;
};

#endif
```

```cpp
// Source File (Soldier.cpp)
#include "../../include/Soldier.h"
#include <iostream>

Soldier::Soldier(int q, int r) : Character(q, r) {
    loadTexture(SOLDIER_TEXTURE);
}

void Soldier::doRender(sf::RenderWindow& window) const {
    Character::doRender(window);
    // Soldier-specific rendering
}

void Soldier::attack(Character* target) {
    if (!target) return;
    std::cout << "Soldier attacks!" << std::endl;
}

void Soldier::move(Hexagon* targetHex) {
    if (!targetHex) return;
    setHexCoord(targetHex->getCoord());
    setPosition(targetHex->getPosition());
}
```

## Object-Oriented Programming

### Inheritance

Inheritance allows a class to reuse and extend the functionality of another class.

```cpp
// Inheritance Example
// Base class
class Character {
    // Common functionality for all characters
};

// Derived class
class Soldier final : public Character {
    // Soldier-specific functionality
    // "final" means no further derivation allowed
};
```

### Polymorphism

Polymorphism allows objects of different classes to be treated as objects of a common base class, with specific behaviors determined at runtime.

```cpp
// Polymorphism Example
// Base class with virtual function
class Character {
public:
    virtual CharacterType getType() const = 0; // Pure virtual
    virtual void update(float deltaTime) {}
};

// Derived class implementations
class Soldier final : public Character {
public:
    CharacterType getType() const override { return CharacterType::Soldier; }
    void update(float deltaTime) override { /* Soldier update */ }
};

// Using polymorphism
void gameLoop(std::vector<Character*>& characters, float deltaTime) {
    for (auto character : characters) {
        character->update(deltaTime); // Calls appropriate version
    }
}
```

### Abstract Classes and Interfaces

Abstract classes have at least one pure virtual function and cannot be instantiated.

```cpp
// Abstract Class Example
class Character {
public:
    virtual CharacterType getType() const = 0; // Pure virtual
    virtual float getScaleFactor() const = 0; // Pure virtual
    
    // Concrete method using abstract methods
    void render(sf::RenderWindow& window) const {
        // Implementation that calls virtual methods
    }
};
```

## Modern C++ Features

### Smart Pointers

Smart pointers manage memory automatically to prevent leaks.

```cpp
// Smart Pointers Example (Game.h)
// Use unique_ptr for owned Soldier
std::unique_ptr<Soldier> mSoldier;

// This is a non-owning reference, so keep as raw pointer
std::optional<Character*> mSelectedCharacter;

// Vector of unique_ptrs for owned cities
std::vector<std::unique_ptr<City>> mCities;
```

### Override and Final Specifiers

```cpp
// Override and Final Example
// Base class
class Character {
public:
    virtual void update(float deltaTime) {}
};

// Derived class - marked final, cannot be derived from further
class Soldier final : public Character {
public:
    // override ensures this overrides a base class method
    void update(float deltaTime) override { /* implementation */ }
};
```

### Non-Virtual Interface Pattern

The NVI pattern provides a stable interface while allowing customization.

```cpp
// NVI Pattern Example
class Building {
public:
    // Non-virtual public interface
    void draw(sf::RenderWindow& window) const {
        // Pre-conditions, common code...
        doDraw(window);
        // Post-conditions, common code...
    }
    
protected:
    // Virtual implementation method
    virtual void doDraw(sf::RenderWindow& window) const;
};
```

## Memory Management

### RAII (Resource Acquisition Is Initialization)

C++ uses RAII to tie resource management to object lifetime.

```cpp
// RAII Example
// Resource (texture) is acquired in constructor
Building::Building(const sf::Vector2f& position) : 
    mPosition(position), 
    mHasTexture(false) {
    mTexture = std::make_unique<sf::Texture>();
    initializeShape();
}

// Resource is automatically released when object is destroyed
// No explicit cleanup code needed because of smart pointers
```

### Ownership Semantics

Clear ownership semantics prevent memory leaks and double-free errors.

```cpp
// Ownership Example (Game.h)
class Game {
private:
    // Game owns the soldier (unique_ptr)
    std::unique_ptr<Soldier> mSoldier;
    
    // Game doesn't own the selected character (raw pointer)
    std::optional<Character*> mSelectedCharacter;
    
    // Game owns all cities (vector of unique_ptrs)
    std::vector<std::unique_ptr<City>> mCities;
};
```

## Best Practices

### Polymorphic Best Practices

```cpp
// Polymorphic Best Practices
// 1. Virtual destructors
virtual ~Character() = default;

// 2. Use override for all overridden methods
void update(float deltaTime) override;

// 3. Mark leaf classes as final
class Soldier final : public Character {...};

// 4. Non-Virtual Interface pattern
// Public non-virtual:
void render(sf::RenderWindow& window) const;
// Protected virtual:
virtual void doRender(sf::RenderWindow& window) const;

// 5. Pure virtual methods for required implementations
virtual BuildingType getType() const = 0;

// 6. Smart pointers for memory management
std::unique_ptr<sf::Texture> mTexture;

// 7. Clear ownership semantics
// Owner: std::unique_ptr<T>
// Non-owner: T* or std::optional<T*>
```

## Advanced C++ Concepts

### Templates

Templates enable generic programming, allowing algorithms and classes to work with any data type.

```cpp
// Templates Example (Standard Library Usage)
// Using vector template with Building pointers
std::vector<std::unique_ptr<Building>> buildings;

// Using make_unique template function
auto soldier = std::make_unique<Soldier>(q, r);
```

### Const Correctness

Functions that don't modify object state should be marked const.

```cpp
// Const Correctness Example
// Const methods - don't modify object state
sf::Vector2f getPosition() const { return mPosition; }
void render(sf::RenderWindow& window) const;
CharacterType getType() const override;

// Non-const methods - modify object state
void setPosition(const sf::Vector2f& position);
void update(float deltaTime);
```

## Conclusion

This guide covered fundamental and advanced C++ concepts with practical examples from a real game project. The hexagon game demonstrates many C++ best practices including class design, inheritance, polymorphism, memory management, and modern C++ features. 

# Modeling Individual Relationships in C++

## Overview

This document outlines a system for modeling individuals and their interconnected relationships, where events affecting one person can propagate to others based on relationship types and strengths.

## Data Structure Options

### 1. Graph Representation

The most natural way to model this network is using a graph:
- **Nodes**: Individual people
- **Edges**: Relationships between people
- **Edge weights/types**: Relationship strength/type

```cpp
class Individual {
    std::string name;
    // Personal attributes
    int happiness;
    int health;
    int wealth;
    // Other attributes...
    
    // Vector of relationships to other individuals
    std::vector<Relationship*> relationships;
};

class Relationship {
    Individual* person1;
    Individual* person2;
    RelationshipType type; // Family, Friend, etc.
    float strength; // 0.0 to 1.0
    
    // Methods to update relationship strength
    void strengthen(float amount);
    void weaken(float amount);
};
```

### 2. Event Propagation System

To handle events and their ripple effects:

```cpp
class Event {
public:
    EventType type;
    float intensity; // How strong is this event
    
    // Apply this event to an individual and propagate effects
    void applyTo(Individual* target) {
        // Apply direct effects to target
        applyDirectEffects(target);
        
        // Propagate to connections based on relationship type/strength
        propagateToConnections(target);
    }
    
private:
    void applyDirectEffects(Individual* target) {
        // Update target's attributes based on event type
        switch(type) {
            case EventType::POSITIVE_CAREER:
                target->wealth += intensity * 10;
                target->happiness += intensity * 5;
                break;
            case EventType::HEALTH_CRISIS:
                target->health -= intensity * 15;
                target->happiness -= intensity * 8;
                break;
            // Other event types...
        }
    }
    
    void propagateToConnections(Individual* target) {
        for (auto relationship : target->relationships) {
            Individual* connection = (relationship->person1 == target) ? 
                                    relationship->person2 : relationship->person1;
            
            // Calculate effect intensity based on relationship
            float propagationFactor = calculatePropagationFactor(relationship);
            float propagatedIntensity = intensity * propagationFactor;
            
            // If significant enough, apply reduced effect to the connection
            if (propagatedIntensity > 0.1f) {
                applyPropagatedEffect(connection, propagatedIntensity);
            }
        }
    }
    
    float calculatePropagationFactor(Relationship* relationship) {
        // Family relationships might have higher propagation
        if (relationship->type == RelationshipType::FAMILY) {
            return relationship->strength * 0.8f;
        }
        // Friends affected less
        else if (relationship->type == RelationshipType::FRIEND) {
            return relationship->strength * 0.5f;
        }
        // Acquaintances affected minimally
        return relationship->strength * 0.2f;
    }
    
    void applyPropagatedEffect(Individual* connection, float propagatedIntensity) {
        // Similar to direct effects but with reduced intensity
        switch(type) {
            case EventType::POSITIVE_CAREER:
                connection->happiness += propagatedIntensity * 3; // Happy for them
                break;
            case EventType::HEALTH_CRISIS:
                connection->happiness -= propagatedIntensity * 5; // Concerned
                break;
            // Other propagated effects...
        }
    }
};
```

## Representing Different Relationship Types

You could use inheritance to model different relationship types with specialized behaviors:

```cpp
enum class RelationshipType {
    FAMILY_PARENT,
    FAMILY_CHILD,
    FAMILY_SIBLING,
    FRIEND_CLOSE,
    FRIEND_CASUAL,
    ROMANTIC_PARTNER,
    COLLEAGUE
};

// Base relationship class
class Relationship {
public:
    Individual* person1;
    Individual* person2;
    RelationshipType type;
    float strength;
    
    virtual float getInfluenceFactor() const = 0;
    virtual void handleEvent(Event* event, Individual* affected) = 0;
};

// Derived relationship types
class FamilyRelationship : public Relationship {
public:
    float getInfluenceFactor() const override {
        // Family typically has stronger influence
        return strength * 0.9f;
    }
    
    void handleEvent(Event* event, Individual* affected) override {
        // Family-specific response to events
    }
};

class FriendshipRelationship : public Relationship {
public:
    float getInfluenceFactor() const override {
        return strength * 0.6f;
    }
    
    void handleEvent(Event* event, Individual* affected) override {
        // Friendship-specific response to events
    }
};
```

## Managing Relationship Network

To efficiently manage the entire network, you might want a container class:

```cpp
class SocialNetwork {
private:
    std::vector<std::unique_ptr<Individual>> individuals;
    std::vector<std::unique_ptr<Relationship>> relationships;
    
public:
    // Add a new individual to the network
    Individual* addIndividual(const std::string& name, /* other attributes */) {
        individuals.push_back(std::make_unique<Individual>(name, /* other attributes */));
        return individuals.back().get();
    }
    
    // Create a relationship between two individuals
    Relationship* createRelationship(Individual* person1, Individual* person2, 
                                    RelationshipType type, float initialStrength) {
        // Create appropriate relationship type based on type enum
        std::unique_ptr<Relationship> relationship;
        
        switch(type) {
            case RelationshipType::FAMILY_PARENT:
            case RelationshipType::FAMILY_CHILD:
            case RelationshipType::FAMILY_SIBLING:
                relationship = std::make_unique<FamilyRelationship>();
                break;
            case RelationshipType::FRIEND_CLOSE:
            case RelationshipType::FRIEND_CASUAL:
                relationship = std::make_unique<FriendshipRelationship>();
                break;
            // Other types...
        }
        
        // Setup relationship properties
        relationship->person1 = person1;
        relationship->person2 = person2;
        relationship->type = type;
        relationship->strength = initialStrength;
        
        // Add to person's relationships
        person1->relationships.push_back(relationship.get());
        person2->relationships.push_back(relationship.get());
        
        // Add to network and return pointer
        relationships.push_back(std::move(relationship));
        return relationships.back().get();
    }
    
    // Apply an event to an individual and handle propagation
    void applyEvent(Individual* target, EventType type, float intensity) {
        Event event{type, intensity};
        event.applyTo(target);
    }
    
    // Find all affected individuals above a certain threshold after an event
    std::vector<Individual*> getSignificantlyAffected(Event* event, float threshold) {
        // Implementation to find all individuals affected above threshold
    }
};
```

## Advanced Features

1. **Multi-hop propagation**: Effects could propagate multiple steps through the network with diminishing strength

2. **Temporal dynamics**: Relationships that change over time
```cpp
// Update relationships based on interaction frequency
void updateRelationshipStrengths(float timeElapsed) {
    for (auto& relationship : relationships) {
        // Relationships naturally decay over time without interaction
        float decayFactor = 0.05f * timeElapsed;
        relationship->strength = std::max(0.0f, relationship->strength - decayFactor);
    }
}
```

3. **Memory of past events**: Individuals remember significant events
```cpp
class Individual {
    // ...other attributes
    std::vector<PastEvent> significantEvents;
    
    // When something significant happens, remember it
    void rememberEvent(Event* event, float personalImpact) {
        if (personalImpact > 0.5f) {
            significantEvents.push_back({event->type, personalImpact, getCurrentTime()});
        }
    }
};
```

## Practical Example

```cpp
int main() {
    SocialNetwork network;
    
    // Create family
    auto alice = network.addIndividual("Alice", 100, 100, 50);
    auto bob = network.addIndividual("Bob", 100, 100, 60);
    auto charlie = network.addIndividual("Charlie", 100, 100, 40);
    
    // Create relationships
    network.createRelationship(alice, bob, RelationshipType::ROMANTIC_PARTNER, 0.9f);
    network.createRelationship(alice, charlie, RelationshipType::FAMILY_PARENT, 0.95f);
    network.createRelationship(bob, charlie, RelationshipType::FAMILY_PARENT, 0.9f);
    
    // Add friends
    auto david = network.addIndividual("David", 100, 100, 55);
    network.createRelationship(alice, david, RelationshipType::FRIEND_CLOSE, 0.8f);
    
    // Simulate events
    network.applyEvent(alice, EventType::HEALTH_CRISIS, 0.7f);
    
    // Print status after event
    printIndividualStatus(alice);
    printIndividualStatus(bob);
    printIndividualStatus(charlie);
    printIndividualStatus(david);
    
    return 0;
}
```

## Conclusion

This system allows you to model realistic social dynamics where events ripple through connected individuals with varying intensity based on relationship types and strengths. It could be particularly useful for:

- Social simulation games
- Agent-based social modeling
- City or community simulation games
- Strategy games with character relationships 