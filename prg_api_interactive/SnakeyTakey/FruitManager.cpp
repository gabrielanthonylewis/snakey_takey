#include "FruitManager.h"

FruitManager::FruitManager(int numberOfFruit, std::vector<Obstacle>* obstacleVector,
                           LootBox* pLootBox, LootBox* pLootBoxAI, std::vector<Portal>* portalVector)
{
  // Initialise and add "numberOfFruit" amount of fruit.
  for(int i = 0; i < numberOfFruit; i++)
  {
    Fruit temp(prg::application.getScreenWidth(),  prg::application.getScreenHeight());
    _fruit.push_back(temp);
  }

  // Assign references.
  _pObstacles = obstacleVector;
  _pLootBox = pLootBox;
  _pLootBoxAI = pLootBoxAI;
  _pPortals = portalVector;

  // Initialise timer.
  _pSpawnTimer = new prg::Timer{8,1000, *this};

  // If the reference exists, begin the timer.
  if(_pSpawnTimer)
    _pSpawnTimer->start();
}

FruitManager::~FruitManager()
{
  // Clean up.
  if(_pSpawnTimer)
    delete _pSpawnTimer;
  _pSpawnTimer = nullptr;

  _pObstacles = nullptr;
  _pLootBox = nullptr;
  _pLootBoxAI = nullptr;
  _pPortals = nullptr;
}

void FruitManager::Reset()
{
  // Initialise and add "numberOfFruit" amount of fruit.
  unsigned int numberOfFruit = _fruit.size();
  _fruit.clear();
  for(unsigned int i = 0;i < numberOfFruit; i++)
  {
    Fruit temp(prg::application.getScreenWidth(),  prg::application.getScreenHeight());
    _fruit.push_back(temp);
  }

  // Reset or Initialise the Timer
  if(_pSpawnTimer)
    _pSpawnTimer->reset(1000);
  else
    _pSpawnTimer = new prg::Timer{8, 1000, *this};

  // If the reference exists, begin the timer.
  if(_pSpawnTimer)
    _pSpawnTimer->start();
}

void FruitManager::onTimer(prg::Timer& timer)
{
  // Return if the timer has stopped or doesn't exist.
  if(!_pSpawnTimer || !_pSpawnTimer->running())
    return;

  // Every 'second' "tick" each fruit (reduces time left).
  for(unsigned int i = 0; i < _fruit.size(); i++)
    _fruit[i].Tick(&_fruit, _pObstacles, _pLootBox, _pLootBoxAI, _pPortals);
}
