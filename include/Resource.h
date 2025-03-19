#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource {
    public:
        Resource();
        ~Resource();
    private:
        std::string name;
        int amount;
        int price;
        int demand;
        int supply;
};

#endif // RESOURCE_H
