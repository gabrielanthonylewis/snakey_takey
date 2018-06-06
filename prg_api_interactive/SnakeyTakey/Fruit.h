#ifndef FRUIT_H
#define FRUIT_H

#include <stdlib.h> // srand, rand
#include <time.h> // time

#include <prg_interactive.hpp>
#include "Position.h"
#include "Obstacle.h"
#include "LootBox.h"
#include "Portal.h"

class Fruit final
{
  public:
    Fruit(int mapHeight, int mapWidth);
    ~Fruit();

    // Render the fruit to the screen.
    void Render(prg::Canvas& canvas) const;

    // "Eats" the fruit, hiding it and resetting the time left to spawn again.
    void Eat();

    // Every "tick" the time left is reduced and if 0 then the fruit is spawned.
    void Tick(std::vector<Fruit>* allFruit, std::vector<Obstacle>* allObstacles,
              LootBox* pLootBox, LootBox* pLootBoxAI, std::vector<Portal>* pAllPortals);

    // Manipulates the value of _timeLeft.
    void ManipTimeLeft(int val) { _timeLeft += val;}

    // Getter functions
    bool IsActive() const { return _isActive; }
    float GetRadius() const { return _radius; }
    Position GetPosition() const { return _position; }
    unsigned int GetReward() const { return _reward; }
    unsigned int GetTimeLeft() const { return _timeLeft; }

  private:
    // Spawns the fruit on the map avoiding collision with other objects.
    void SpawnFruit(std::vector<Fruit>* allFruit, std::vector<Obstacle>* allObstacles,
                    LootBox* pLootBox, LootBox* pLootBoxAI, std::vector<Portal>* pAllPortals);

  private:
    // Position on the map.
    Position _position {0,0};

    // If active then the fruit is visible and consumable.
    bool _isActive = false;

    // The map width and height.
    unsigned int _mapWidth = 0, _mapHeight = 0;

    // The amount of time left before the fruit is spawned.
    unsigned int _timeLeft = 0;

    // The reward given when consumed.
    int _reward = 1;

    // The radius of the fruit.
    float _radius = 0.0F;
};

#endif // FRUIT_H
