#include "NPCFactory.h"
#include <sstream>
#include <stdexcept>

std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, 
                                           const std::string& name, 
                                           double x, double y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::invalid_argument("Coordinates must be in range [0, 500]");
    }
    
    if (type == "Knight") {
        return std::make_shared<Knight>(name, x, y);
    } else if (type == "Elf") {
        return std::make_shared<Elf>(name, x, y);
    } else if (type == "Dragon") {
        return std::make_shared<Dragon>(name, x, y);
    }
    
    throw std::invalid_argument("Unknown NPC type: " + type);
}

std::shared_ptr<NPC> NPCFactory::loadFromString(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name;
    double x, y;
    
    iss >> type >> name >> x >> y;
    
    return createNPC(type, name, x, y);
}
