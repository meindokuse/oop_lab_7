#pragma once
#include "NPC.h"
#include "Observer.h"
#include <vector>
#include <memory>

class Editor {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    Subject subject;
    
public:
    Editor();
    
    void addNPC(std::shared_ptr<NPC> npc);
    void print() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void startBattle(double range);
    
    size_t getNPCCount() const { return npcs.size(); }
};
