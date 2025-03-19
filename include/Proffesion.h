#ifndef PROFESSION_H
#define PROFESSION_H

#include <string>

class Profession {
    public:
        Profession();
        ~Profession();
    private:
        std::string name;
        int compensation;
        // How much one point of education increases productivity
        int educationProductivityFactor;
};

#endif // PROFESSION_H