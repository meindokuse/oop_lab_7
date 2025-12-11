#include "BattleVisitor.h"
#include <sstream>

BattleVisitor::BattleVisitor(std::shared_ptr<NPC> attacker, bool* defenderDies, 
                             bool* attackerDies, Subject* subject)
    : attacker(attacker), defenderDies(defenderDies), 
      attackerDies(attackerDies), subject(subject) {}

void BattleVisitor::visit(Knight& knight) {
    if (auto dragon = std::dynamic_pointer_cast<Dragon>(attacker)) {
        int attackPower = dragon->rollDice();
        int defensePower = knight.rollDice();
        
        if (attackPower > defensePower) {
            *defenderDies = true;
            std::ostringstream msg;
            msg << "Dragon " << dragon->getName() << " (" << attackPower 
                << ") killed Knight " << knight.getName() << " (" << defensePower << ")";
            subject->notify(msg.str());
        }
    } else if (auto elf = std::dynamic_pointer_cast<Elf>(attacker)) {
        int attackPower = elf->rollDice();
        int defensePower = knight.rollDice();
        
        if (defensePower > attackPower) {
            *attackerDies = true;
            std::ostringstream msg;
            msg << "Knight " << knight.getName() << " (" << defensePower 
                << ") killed Elf " << elf->getName() << " (" << attackPower << ")";
            subject->notify(msg.str());
        }
    }
}

void BattleVisitor::visit(Elf& elf) {
    if (auto dragon = std::dynamic_pointer_cast<Dragon>(attacker)) {
        int attackPower = dragon->rollDice();
        int defensePower = elf.rollDice();
        
        if (attackPower > defensePower) {
            *defenderDies = true;
            std::ostringstream msg;
            msg << "Dragon " << dragon->getName() << " (" << attackPower 
                << ") killed Elf " << elf.getName() << " (" << defensePower << ")";
            subject->notify(msg.str());
        }
    } else if (auto knight = std::dynamic_pointer_cast<Knight>(attacker)) {
        int attackPower = elf.rollDice();
        int defensePower = knight->rollDice();
        
        if (attackPower > defensePower) {
            *defenderDies = true;
            std::ostringstream msg;
            msg << "Elf " << elf.getName() << " (" << attackPower 
                << ") killed Knight " << knight->getName() << " (" << defensePower << ")";
            subject->notify(msg.str());
        }
    }
}

void BattleVisitor::visit(Dragon& dragon) {
    if (auto dragonAttacker = std::dynamic_pointer_cast<Dragon>(attacker)) {
        int attackPower = dragonAttacker->rollDice();
        int defensePower = dragon.rollDice();
        
        if (attackPower > defensePower) {
            *defenderDies = true;
            *attackerDies = true;
            std::ostringstream msg;
            msg << "Dragons " << dragonAttacker->getName() << " (" << attackPower 
                << ") and " << dragon.getName() << " (" << defensePower 
                << ") killed each other";
            subject->notify(msg.str());
        }
    } else if (auto knight = std::dynamic_pointer_cast<Knight>(attacker)) {
        int attackPower = knight->rollDice();
        int defensePower = dragon.rollDice();
        
        if (attackPower > defensePower) {
            *defenderDies = true;
            std::ostringstream msg;
            msg << "Knight " << knight->getName() << " (" << attackPower 
                << ") killed Dragon " << dragon.getName() << " (" << defensePower << ")";
            subject->notify(msg.str());
        }
    } else if (auto elf = std::dynamic_pointer_cast<Elf>(attacker)) {
        int attackPower = dragon.rollDice();
        int defensePower = elf->rollDice();
        
        if (attackPower > defensePower) {
            *attackerDies = true;
            std::ostringstream msg;
            msg << "Dragon " << dragon.getName() << " (" << attackPower 
                << ") killed Elf " << elf->getName() << " (" << defensePower << ")";
            subject->notify(msg.str());
        }
    }
}
