#include "../../include/Soldier.h"
#include <iostream>

Soldier::Soldier(int q, int r) : Character(q, r) {
    loadTexture(SOLDIER_TEXTURE);
}
