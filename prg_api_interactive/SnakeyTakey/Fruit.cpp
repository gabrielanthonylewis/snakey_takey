#include "Fruit.h"

Fruit::Fruit(int mapWidth, int mapHeight)
: _mapWidth(mapWidth) , _mapHeight(mapHeight)
{
  // Randomly generate the amount of time left to become active.
  _timeLeft = rand() % 8 + 1; // range of 0 to 8

  // Randomly generate a reward value.
  _reward = rand() % 3 + 1;

  // Enlarge/Reduce size of the fruit dependent on the reward.
  _radius = (4.0F + _reward) / 2.0F;
}

Fruit::~Fruit(){}

void Fruit::Tick(std::vector<Fruit>* allFruit, std::vector<Obstacle>* allObstacles,
                 LootBox* pLootBox, LootBox* pLootBoxAI, std::vector<Portal>* pAllPortals)
{
  // Reduce the amount of time left by one.
  _timeLeft--;

  // If there is no time left then Spawn the fruit!
  if(_timeLeft <= 0)
    SpawnFruit(allFruit, allObstacles, pLootBox, pLootBoxAI, pAllPortals);
}

void Fruit::Eat()
{
  // Once eaten, the fruit becomes inactive
  // and the time left to spawn is randomly chosen again.
  _isActive = false;
  _timeLeft = rand() % 10 + 1; // range of 0 to 10
}

void Fruit::SpawnFruit(std::vector<Fruit>* pAllFruit, std::vector<Obstacle>* pAllObstacles,
                       LootBox* pLootBox, LootBox* pLootBoxAI, std::vector<Portal>* pAllPortals)
{
  // Is the proposed position occupied?
  bool occupied = false;

  do
  {
    // Random proposed x and y position.
    unsigned int xPos = rand() % (_mapWidth - 15) + 10;
    unsigned int yPos = rand() % (_mapHeight - 68) + 40;
    occupied = false;

    // Check to see if the proposed position is free...
    // Note:: I double the radius just to all for more space.

    // Collision detection between the proposed position and each obstacle.
    if(pAllObstacles)
    {
      for(unsigned int i = 0; i < pAllObstacles->size(); i++)
      {
        // Prevent the fruit from spawning within an obstacle.
        if((*pAllObstacles)[i].WithinBounds({xPos, yPos}, _radius + 4.0F))
        {
          occupied = true;
          break;
        }

        // Prevent the fruit from spawning in line with any side as this causes an AI navigation issue.
        if((yPos <= (*pAllObstacles)[i].GetPosition().y + (*pAllObstacles)[i].GetHeight() / 2.0f + _radius * 2.0F &&
            yPos >= (*pAllObstacles)[i].GetPosition().y + (*pAllObstacles)[i].GetHeight() / 2.0f - _radius * 2.0F)
           ||
           (yPos <= (*pAllObstacles)[i].GetPosition().y - (*pAllObstacles)[i].GetHeight() / 2.0f + _radius * 2.0F &&
            yPos >= (*pAllObstacles)[i].GetPosition().y - (*pAllObstacles)[i].GetHeight() / 2.0f - _radius * 2.0F)
           ||
           (xPos <= (*pAllObstacles)[i].GetPosition().x + (*pAllObstacles)[i].GetWidth() / 2.0f + _radius * 2.0F &&
            xPos >= (*pAllObstacles)[i].GetPosition().x + (*pAllObstacles)[i].GetWidth() / 2.0f - _radius * 2.0F)
           ||
           (xPos <= (*pAllObstacles)[i].GetPosition().x - (*pAllObstacles)[i].GetWidth() / 2.0f + _radius * 2.0F &&
            xPos >= (*pAllObstacles)[i].GetPosition().x - (*pAllObstacles)[i].GetWidth() / 2.0f - _radius * 2.0F)
           )
        {
          occupied = true;
          break;
        }

      }
    }

    // Collision detection between the proposed position and each fruit.
    if(pAllFruit)
    {
      for(unsigned int i = 0; i < pAllFruit->size(); i++)
      {
        if(xPos + _radius <= (*pAllFruit)[i].GetPosition().x + (*pAllFruit)[i].GetRadius() * 2.0F &&
           xPos - _radius >= (*pAllFruit)[i].GetPosition().x - (*pAllFruit)[i].GetRadius() * 2.0F &&
           yPos + _radius <= (*pAllFruit)[i].GetPosition().y + (*pAllFruit)[i].GetRadius() * 2.0F &&
           yPos - _radius <= (*pAllFruit)[i].GetPosition().y - (*pAllFruit)[i].GetRadius() * 2.0F)
        {
          occupied = true;
          break;
        }
      }
    }

    // Collision detection between the proposed position and the Player's loot box.
    if(pLootBox)
    {
      if(xPos + _radius * 2.0F < (*pLootBox).GetPosition().x + (*pLootBox).GetRadius() * 3.0F &&
         xPos - _radius * 2.0F > (*pLootBox).GetPosition().x - (*pLootBox).GetRadius() * 3.0F &&
         yPos + _radius * 2.0F < (*pLootBox).GetPosition().y + (*pLootBox).GetRadius() * 3.0F &&
         yPos - _radius * 2.0F < (*pLootBox).GetPosition().y - (*pLootBox).GetRadius() * 3.0F)
      {
        occupied = true;
        break;
      }
    }

    // Collision detection between the proposed position and the AI's loot box.
    if(pLootBoxAI)
    {
      if(xPos + _radius * 2.0F < (*pLootBoxAI).GetPosition().x + (*pLootBoxAI).GetRadius() * 3.0F &&
         xPos - _radius * 2.0F > (*pLootBoxAI).GetPosition().x - (*pLootBoxAI).GetRadius() * 3.0F &&
         yPos + _radius * 2.0F < (*pLootBoxAI).GetPosition().y + (*pLootBoxAI).GetRadius() * 3.0F &&
         yPos - _radius * 2.0F < (*pLootBoxAI).GetPosition().y - (*pLootBoxAI).GetRadius() * 3.0F)
      {
        occupied = true;
        break;
      }
    }

    // Collision detection between the proposed position and the portals.
    if(pAllPortals)
    {
      for(unsigned int i = 0; i < pAllPortals->size(); i++)
      {
        if((*pAllPortals)[i].GetPointA()->WithinBounds({xPos, yPos}, _radius + 4.0F) ||
           (*pAllPortals)[i].GetPointB()->WithinBounds({xPos, yPos}, _radius + 4.0F))
        {
          occupied = true;
          break;
        }
      }
    }

    // Update position if the proposed position isn't occupied.
    if(occupied == false)
    {
      _position.x = xPos;
      _position.y = yPos;
    }

  }
  while(occupied);

  // Set fruit to become active (ready for consumption).
  _isActive = true;
}

void Fruit::Render(prg::Canvas& canvas) const
{
  // Render the fruit (green circle with tightly packed inner circles).
  for(float i = _radius * 2.0F; i > 0; i--)
    canvas.drawCircle(_position.x, _position.y, _radius * 2.0F - i, prg::Colour::GREEN);
}

