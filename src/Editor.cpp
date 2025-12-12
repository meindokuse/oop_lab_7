#include "Editor.h"
#include "NPCFactory.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <BattleVisitor.h>

Editor::Editor() {
    subject.attach(std::make_shared<ConsoleObserver>());
    subject.attach(std::make_shared<FileObserver>("log.txt"));
}

void Editor::addNPC(std::shared_ptr<NPC> npc) {
    npcs.push_back(std::move(npc));
}

void Editor::print() const {
    std::cout << "\n=== NPC List ===" << std::endl;
    for (const auto& npc : npcs) {
        std::cout << npc->getType() << " \"" << npc->getName() 
                  << "\" at (" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
    }
    std::cout << "Total: " << npcs.size() << " NPCs\n" << std::endl;
}

void Editor::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    for (const auto& npc : npcs) {
        file << npc->serialize() << std::endl;
    }
}

void Editor::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    
    npcs.clear();
    while (std::getline(file, line)) {
        if (!line.empty()) {
            npcs.push_back(NPCFactory::loadFromString(line));
        }
    }
}

void Editor::startBattle(double range) {
    std::cout << "\n=== Battle Mode (range: " << range << " meters) ===" << std::endl;
    
    bool anyKilled = true;
    while (anyKilled) {
        anyKilled = false;
        
        for (size_t i = 0; i < npcs.size(); ++i) {
            for (size_t j = i + 1; j < npcs.size(); ++j) {
                if (npcs[i]->distanceTo(*npcs[j]) <= range) {
                    bool iDies = false, jDies = false;
                    
                    BattleVisitor visitorJ(npcs[i], &jDies, &iDies, &subject);
                    npcs[j]->accept(visitorJ);
                    
                    if (jDies) {
                        npcs.erase(npcs.begin() + j);
                        anyKilled = true;
                        --j;
                    }
                    
                    if (iDies && i < npcs.size()) {
                        npcs.erase(npcs.begin() + i);
                        anyKilled = true;
                        --i;
                        break;
                    }
                }
            }
        }
    }
    
    std::cout << "Battle ended. " << npcs.size() << " NPCs survived.\n" << std::endl;
}
