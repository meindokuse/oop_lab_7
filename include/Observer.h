#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class ConsoleObserver : public Observer {
public:
    void update(const std::string& message) override;
};

class FileObserver : public Observer {
private:
    std::ofstream file;
    
public:
    FileObserver(const std::string& filename);
    ~FileObserver();
    void update(const std::string& message) override;
};

class Subject {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    
public:
    void attach(std::shared_ptr<Observer> observer);
    void notify(const std::string& message);
};
