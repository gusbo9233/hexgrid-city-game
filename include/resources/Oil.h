#ifndef OIL_H
#define OIL_H

#include "Resource.h"

class Oil : public Resource {
public:
    Oil();
    ~Oil() = default;
    
    // Type identification
    ResourceType getType() const override { return ResourceType::OIL; }
    
    // Oil-specific constants
    static constexpr const char* OIL_TEXTURE = "assets/images/oil.png";
    static constexpr float OIL_SIZE = 30.0f;
    static constexpr float OIL_SCALE = 0.8f;
    
protected:
    // Override size and scale for proper appearance
    float getScaleFactor() const override { return OIL_SCALE; }
    sf::Vector2f getSize() const override { return {OIL_SIZE, OIL_SIZE}; }
    
    // Override doRender only if needed for custom rendering
    void doRender(sf::RenderWindow& window) const override;
};

#endif // OIL_H 