#pragma once
#include "NPC.h"
#include <memory>
#include <string>

class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(const std::string& type, 
                                          const std::string& name, 
                                          double x, double y);
    
    static std::shared_ptr<NPC> loadFromString(const std::string& line);
};
