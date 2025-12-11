#pragma once
#include "NPC.h"
#include "Observer.h"
#include <memory>

class BattleVisitor {
private:
    std::shared_ptr<NPC> attacker; 
    bool* defenderDies;
    bool* attackerDies;
    Subject* subject;
    
public:
    BattleVisitor(std::shared_ptr<NPC> attacker, bool* defenderDies, 
                  bool* attackerDies, Subject* subject);
    
    void visit(Knight& knight);
    void visit(Elf& elf);
    void visit(Dragon& dragon);
};
