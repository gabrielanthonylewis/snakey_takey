#ifndef FRUITMANAGER_H
#define FRUITMANAGER_H

#include <vector> // vector container

#include <prg_interactive.hpp>
#include "Fruit.h"
#include "Obstacle.h"
#include "LootBox.h"

class FruitManager final : public prg::ITimerEvent
{
  public:
    FruitManager(int numberOfFruit, std::vector<Obstacle>* obstacleVector,
                 LootBox* pLootBox, LootBox* pLootBoxAI, std::vector<Portal>* portalVector);
    ~FruitManager();

    // Resets the timer and fruit therefore resetting the instance without removing references.
    void Reset();

    // Getter Functions
    std::vector<Fruit>* GetpFruit() { return &_fruit; }
    prg::Timer& GetTimer() { return *_pSpawnTimer; }

  private:
    // Used to invoke "Tick()" on each fruit.
    void onTimer(prg::Timer& timer) override;

  private:
    // The timer which directly affects the spawning of the fruit.
    prg::Timer* _pSpawnTimer = nullptr;

    // A reference to all of the fruit spawned within the level.
    std::vector<Fruit> _fruit;

    // References (for avoidance purposes).
    std::vector<Obstacle>* _pObstacles = nullptr;
    std::vector<Portal>* _pPortals = nullptr;
    LootBox* _pLootBox = nullptr;
    LootBox* _pLootBoxAI = nullptr;
};

#endif // FRUITMANAGER_H
