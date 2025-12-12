#pragma once
#include "NPC.h"
#include "Observer.h"
#include "FightTask.h"
#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <atomic>

class GameWorld {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::queue<FightTask> fightQueue;
    
    mutable std::shared_mutex npcs_mtx;
    std::mutex queue_mtx;
    
    Subject subject;
    std::atomic<bool> stopFlag{false};
    
    double mapSize;
    
public:
    GameWorld(double mapSize = 100.0);
    
    void addNPC(std::shared_ptr<NPC> npc);
    void generateRandomNPCs(size_t count);
    
    void movementThread();
    void battleThread();
    void printMap();
    
    void start(int durationSeconds = 30);
    void stop();
    
    size_t getAliveCount() const;
};
