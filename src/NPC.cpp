#include "NPC.h"
#include <cmath>
#include <sstream>
#include <random>
#include <algorithm>
#include <BattleVisitor.h>

NPC::NPC(const std::string& name, double x, double y,
         double moveRange, double killRange)
    : name(name), x(x), y(y),
      dead(false), moveRange(moveRange), killRange(killRange) {}

std::string NPC::serialize() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream oss;
    oss << getType() << " " << name << " " << x << " " << y << " " << dead;
    return oss.str();
}

std::string NPC::getName() const {
    std::lock_guard<std::mutex> lock(mtx);
    return name;
}

double NPC::getX() const {
    std::lock_guard<std::mutex> lock(mtx);
    return x;
}

double NPC::getY() const {
    std::lock_guard<std::mutex> lock(mtx);
    return y;
}

bool NPC::isDead() const {
    std::lock_guard<std::mutex> lock(mtx);
    return dead;
}

void NPC::setDead(bool value) {
    std::lock_guard<std::mutex> lock(mtx);
    dead = value;
}

double NPC::distanceTo(const NPC& other) const {
    double dx = getX() - other.getX();
    double dy = getY() - other.getY();
    return std::sqrt(dx * dx + dy * dy);
}

void NPC::moveRandomStep(double mapSize) {
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis(-moveRange, moveRange);  

    std::lock_guard<std::mutex> lock(mtx);
    x = std::clamp(x + dis(gen), 0.0, mapSize);
    y = std::clamp(y + dis(gen), 0.0, mapSize);
}

int NPC::rollDice() {
    static thread_local std::mt19937 gen(std::random_device{}());
    static thread_local std::uniform_int_distribution<> dis(1, 6);
    return dis(gen);
}

double NPC::getKillRange() const {
    std::lock_guard<std::mutex> lock(mtx);
    return killRange;
}

Knight::Knight(const std::string& name, double x, double y)
    : NPC(name, x, y, 30.0, 10.0) {}

void Knight::accept(BattleVisitor& visitor) {
    visitor.visit(*this);
}

Elf::Elf(const std::string& name, double x, double y)
    : NPC(name, x, y, 10.0, 50.0) {}

void Elf::accept(BattleVisitor& visitor) {
    visitor.visit(*this);
}

Dragon::Dragon(const std::string& name, double x, double y)
    : NPC(name, x, y, 50.0, 30.0) {}

void Dragon::accept(BattleVisitor& visitor) {
    visitor.visit(*this);
}
