#include "Observer.h"

void ConsoleObserver::update(const std::string& message) {
    std::cout << message << std::endl;
}

FileObserver::FileObserver(const std::string& filename) {
    file.open(filename, std::ios::app);
}

FileObserver::~FileObserver() {
    if (file.is_open()) {
        file.close();
    }
}

void FileObserver::update(const std::string& message) {
    if (file.is_open()) {
        file << message << std::endl;
    }
}

void Subject::attach(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void Subject::notify(const std::string& message) {
    for (auto& observer : observers) {
        observer->update(message);
    }
}
