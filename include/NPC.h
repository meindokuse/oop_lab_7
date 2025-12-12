#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <random>

class BattleVisitor;

class NPC : public std::enable_shared_from_this<NPC> {
protected:
    std::string name;
    double x, y;
    bool dead;
    mutable std::mutex mtx;

    double moveRange;     
    double killRange;
    
public:
    NPC(const std::string& name, double x, double y,
        double moveRange, double killRange);
    virtual ~NPC() = default;
    
    virtual void accept(BattleVisitor& visitor) = 0;
    virtual std::string getType() const = 0;
    virtual std::string serialize() const;
    
    std::string getName() const;
    double getX() const;
    double getY() const;
    bool isDead() const;
    void setDead(bool value);
    
    double distanceTo(const NPC& other) const;
    void moveRandomStep(double mapSize);

    double getKillRange() const;
    
    int rollDice();  // Бросок кубика
};

class Knight : public NPC {
public:
    Knight(const std::string& name, double x, double y);
    void accept(BattleVisitor& visitor) override;
    std::string getType() const override { return "Knight"; }
};

class Elf : public NPC {
public:
    Elf(const std::string& name, double x, double y);
    void accept(BattleVisitor& visitor) override;
    std::string getType() const override { return "Elf"; }
};

class Dragon : public NPC {
public:
    Dragon(const std::string& name, double x, double y);
    void accept(BattleVisitor& visitor) override;
    std::string getType() const override { return "Dragon"; }
};
