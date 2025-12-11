#pragma once
#include <memory>

class NPC;

struct FightTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
    
    FightTask(std::shared_ptr<NPC> a, std::shared_ptr<NPC> d)
        : attacker(a), defender(d) {}
};
