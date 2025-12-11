#include <GameWorld.h>
int main() {
    GameWorld world(100.0);   // карта 100x100, радиус боя 10

    world.generateRandomNPCs(50);   // создаём 50 случайных NPC

    world.start(30);                // играем 30 секунд

    return 0;
}