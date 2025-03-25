#ifndef ALLEGIANCE_H
#define ALLEGIANCE_H

// Enum to distinguish between player-controlled (friendly) and enemy entities
enum class Allegiance {
    FRIENDLY,  // Player-controlled
    ENEMY,     // AI-controlled opponent
    NEUTRAL    // Not affiliated with either side
};

#endif // ALLEGIANCE_H 