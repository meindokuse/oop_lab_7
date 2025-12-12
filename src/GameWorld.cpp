#include "GameWorld.h"
#include "NPCFactory.h"
#include "BattleVisitor.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

GameWorld::GameWorld(double mapSize) 
    : mapSize(mapSize) {
    subject.attach(std::make_shared<ConsoleObserver>());
    subject.attach(std::make_shared<FileObserver>("log.txt"));
}

void GameWorld::addNPC(std::shared_ptr<NPC> npc) {
    std::unique_lock lock(npcs_mtx);
    npcs.push_back(npc);
}

void GameWorld::generateRandomNPCs(size_t count) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> pos(0.0, mapSize);
    std::uniform_int_distribution<> type(0, 2);
    
    const std::vector<std::string> types = {"Knight", "Elf", "Dragon"};
    
    for (size_t i = 0; i < count; ++i) {
        std::string npcType = types[type(gen)];
        std::string name = npcType + "_" + std::to_string(i);
        auto npc = NPCFactory::createNPC(npcType, name, pos(gen), pos(gen));
        addNPC(npc);
    }
}

void GameWorld::movementThread() {
    while (!stopFlag) {
        {
            std::unique_lock lock(npcs_mtx);
            
            for (auto& npc : npcs) {
                if (!npc->isDead()) {
                    npc->moveRandomStep(mapSize);
                }
            }
            
            for (size_t i = 0; i < npcs.size(); ++i) {
                if (npcs[i]->isDead()) continue;
                
                for (size_t j = i + 1; j < npcs.size(); ++j) {
                    if (npcs[j]->isDead()) continue;
                    
                    double range_i = npcs[i]->getKillRange();
                    double range_j = npcs[j]->getKillRange();
                    double maxRange = std::max(range_i, range_j);

                    if (npcs[i]->distanceTo(*npcs[j]) <= maxRange) {
                        std::lock_guard<std::mutex> qlock(queue_mtx);
                        fightQueue.emplace(npcs[i], npcs[j]);
                    }
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void GameWorld::battleThread() {
    while (!stopFlag) {
        FightTask task(nullptr, nullptr);
        
        {
            std::lock_guard<std::mutex> lock(queue_mtx);
            if (!fightQueue.empty()) {
                task = fightQueue.front();
                fightQueue.pop();
            }
        }
        
        if (!task.attacker || !task.defender) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        
        // Проверяем, что оба живы
        if (task.attacker->isDead() || task.defender->isDead()) {
            continue;
        }
        
        // Проводим бой
        bool attackerDies = false, defenderDies = false;
        
        BattleVisitor visitor(task.attacker, &defenderDies, &attackerDies, &subject);
        task.defender->accept(visitor);
        
        if (defenderDies) {
            task.defender->setDead(true);
        }
        if (attackerDies) {
            task.attacker->setDead(true);
        }
    }
}

void GameWorld::printMap() {
    std::shared_lock lock(npcs_mtx);
    
    std::cout << "\n=== MAP STATE ===" << std::endl;
    size_t aliveCount = 0;
    
    for (const auto& npc : npcs) {
        if (!npc->isDead()) {
            std::cout << npc->getType() << " \"" << npc->getName() 
                      << "\" at (" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
            ++aliveCount;
        }
    }
    
    std::cout << "Alive: " << aliveCount << " / " << npcs.size() << "\n" << std::endl;
}

void GameWorld::start(int durationSeconds) {
    std::cout << "Game starting for " << durationSeconds << " seconds..." << std::endl;
    
    std::thread t_move(&GameWorld::movementThread, this);
    std::thread t_battle(&GameWorld::battleThread, this);
    
    auto startTime = std::chrono::steady_clock::now();
    
    while (true) {
        printMap();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startTime
        ).count();
        
        if (elapsed >= durationSeconds) {
            break;
        }
    }
    
    stop();
    t_move.join();
    t_battle.join();
    
    std::cout << "\n=== FINAL SURVIVORS ===" << std::endl;
    printMap();
}

void GameWorld::stop() {
    stopFlag = true;
}

size_t GameWorld::getAliveCount() const {
    std::shared_lock lock(npcs_mtx);
    size_t count = 0;
    for (const auto& npc : npcs) {
        if (!npc->isDead()) ++count;
    }
    return count;
}
