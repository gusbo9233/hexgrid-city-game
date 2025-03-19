#ifndef OIL_REFINERY_H
#define OIL_REFINERY_H

#include "Workplace.h"

class OilRefinery : public Workplace {
    public:
        OilRefinery(const sf::Vector2f& position);
        ~OilRefinery();
        
};
#endif