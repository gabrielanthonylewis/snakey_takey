#include "AIController.h"

#include <iostream> // for testing

AIController::AIController(std::vector<Fruit>* fruitVector, Position spawnPos, Position lootBoxPos,
                           std::vector<Obstacle>* obstacleVector, SnakeAbs* controllee,
                           unsigned int difficulty) : _difficulty(difficulty)
{
  _controllee = controllee;

  // Assign a reference to the fruit vector if it exists.
  if(fruitVector)
    _pFruit = fruitVector;

  // Assign a reference to the obstacle vector if it exists.
  if(obstacleVector)
    _pObstacles = obstacleVector;

  // Initialise snake and loot box position.
  _controllee-> SetPosition(spawnPos);
  _lootBoxPos = lootBoxPos;
}

AIController::~AIController()
{
  _pFruitTarget = nullptr;
  _pObstacles = nullptr;
  _pOpponent = nullptr;
}

Fruit* AIController::CalculateFruitTarget() const
{
  // Return if there are no fruit.
  if(!_pFruit)
    return nullptr;

  // Fruit target to return.
  Fruit* pFruitTarget = nullptr;

  // Easy/Medium difficulty (No closest fruit)
  if(_difficulty < 2)
  {
    // If Medium difficulty then don't look for fruit if there already is a target.
    if(_difficulty == 1 && _pFruitTarget != nullptr &&_pFruitTarget->IsActive() == true)
      return _pFruitTarget;

    // On easy difficulty the snake may pick another fruit randomly.
    for(unsigned int i = 0; i < _pFruit->size(); i++)
    {
      if((*_pFruit)[i].IsActive())
        pFruitTarget = &(*_pFruit)[i];
    }

    return pFruitTarget;
  }

  // Find the closest fruit.
  int smallestDist = 9999;
  for(unsigned int i = 0; i < _pFruit->size(); i++)
  {
    // If the fruit is inactive then continue.
    if(!((*_pFruit)[i].IsActive()))
      continue;

    int xDist = -1;
    // If the fruit is to the right..
    if((*_pFruit)[i].GetPosition().x > _controllee->GetPosition().x)
      xDist = (*_pFruit)[i].GetPosition().x - _controllee->GetPosition().x;
    else // the fruit is to the left
      xDist = _controllee->GetPosition().x - (*_pFruit)[i].GetPosition().x;

    int yDist = -1;
    // If the fruit is above.
    if((*_pFruit)[i].GetPosition().y > _controllee->GetPosition().y)
      yDist = (*_pFruit)[i].GetPosition().y - _controllee->GetPosition().y;
    else // the fruit is below.
      yDist = _controllee->GetPosition().y - (*_pFruit)[i].GetPosition().y;

    // Total distance using Pythagoras (square root of a squared + b squared).
    int tDistance = sqrt(pow(xDist, 2) + pow(yDist, 2));

    // If the total distance is less than the current smallest distance..
    if(tDistance <= smallestDist)
    {
      // Update the new smallest distance.
      smallestDist = tDistance;

      // Update the closest fruit.
      pFruitTarget = &(*_pFruit)[i];

      // If the smallest distance is 0 then return nothing (on top of a fruit so eat).
      if(smallestDist == 0)
        return nullptr;
    }
  }

  return pFruitTarget;
}

DirectionNESW AIController::CalculateDirection()
{
  // If the tail size is greater than 3 then head towards the LootBox.
  if(_controllee->GetTailSize() > 3)
  {
    // Don't head towards a fruit.
    _pFruitTarget = nullptr;

    // If the AI isn't on the y-axis..
    if(_controllee->GetPosition().y < _lootBoxPos.y  - 6 ||
       _controllee->GetPosition().y > _lootBoxPos.y  + 6)
    {
      // Move in Y direction
      if(_lootBoxPos.y > _controllee->GetPosition().y)
        return DirectionNESW::North;
      else
        return DirectionNESW::South;
    }
    else // else if the AI is on the y-axis but not on the x point..
    {
      // Move in X direction (NO CHECK AS WILL PICK UP CAUSING NULL)
      if(_lootBoxPos.x > _controllee->GetPosition().x)
        return DirectionNESW::East;
      else
        return DirectionNESW::West;
    }
  }

  // If there isn't a fruit target then move in a random direction.
  if(!_pFruitTarget)
  {
    DirectionNESW tempDir = _direction;
    int randVal = rand() % 100 + 1; // range 0 to 100.

    // If the AI is moving upwards and it would cause a collision with the top border, avoid!
    if(tempDir == DirectionNESW::North &&
       _controllee->GetPosition().y + 10 >= prg::application.getScreenHeight())
    {
      // Random chance of south, east, or west...
      if(randVal > 50)
        tempDir = DirectionNESW::East;
      else if(randVal < 25)
        tempDir = DirectionNESW::West;
      else
        tempDir = DirectionNESW::South;
    }

    // If moving right would cause a collision with the right border then avoid!
    if(_controllee->GetPosition().x + 10 >= prg::application.getScreenWidth())
    {
      // Random chance of south, east, or west...
      if(randVal > 90)
        tempDir = DirectionNESW::West;
      else if(randVal < 10)
      {
        if(tempDir == DirectionNESW::East && _controllee->GetPosition().y + 10 >= prg::application.getScreenHeight())
          tempDir = DirectionNESW::South;
        else if(_controllee->GetPosition().y + 10 < prg::application.getScreenHeight())
          tempDir = DirectionNESW::North;
        else
          tempDir = DirectionNESW::West;
      }
      else
        tempDir = DirectionNESW::South;
    }

    // If moving downwards would cause a collision with the bottom border then avoid!
    if(_controllee->GetPosition().y - 10 <= 0)
    {
      if(randVal > 90)
        tempDir = DirectionNESW::North;
      else
        tempDir = DirectionNESW::West;
    }

    // If the AI is moving left and it would cause a collision with the top border, avoid!
    if(tempDir == DirectionNESW::West && _controllee->GetPosition().x - 10 <= 0)
    {
      if(_controllee->GetPosition().y + 10 >= prg::application.getScreenHeight())
        tempDir = DirectionNESW::South;
      else
        tempDir = DirectionNESW::North;
    }
    else if(_controllee->GetPosition().x - 10 <= 0)
    {
      if(randVal > 90)
        tempDir = DirectionNESW::East;
    }

    // If there is no possible collision then there's a small chance of changing direction.
    if(_controllee->GetPosition().x - 10 > 0 &&
       _controllee->GetPosition().y + 10 < prg::application.getScreenHeight() &&
       _controllee->GetPosition().x + 10 < prg::application.getScreenWidth() &&
       _controllee->GetPosition().y - 10 > 0)
    {
      if(randVal > 98)
        tempDir = DirectionNESW::North;
      else if(randVal > 96)
        tempDir = DirectionNESW::South;
      else if(randVal > 94)
        tempDir = DirectionNESW::East;
      else if(randVal > 92)
        tempDir = DirectionNESW::West;
    }

    return tempDir;
  }

  // If there is a fruit target then move towards it.
  if(_pFruitTarget)
  {
    DirectionNESW tempDir;
    bool changeMade = false;

    // If the AI isn't on the y-axis..
    if(_controllee->GetPosition().y < _pFruitTarget->GetPosition().y - 6 ||
       _controllee->GetPosition().y > _pFruitTarget->GetPosition().y + 6)
    {
      // Move in Y direction
      if(_pFruitTarget->GetPosition().y > _controllee->GetPosition().y)
      {
        changeMade = true;
        tempDir = DirectionNESW::North;
      }
      else
      {
        changeMade = true;
        tempDir = DirectionNESW::South;
      }
    }
    else // else move on the x-axis towards the fruit.
    {
      // Move in X direction (NO EXTRA CHECK AS WILL PICK UP CAUSING NULL)
      if(_pFruitTarget->GetPosition().x > _controllee->GetPosition().x)
      {
        changeMade = true;
        tempDir = DirectionNESW::East;
      }
      else
      {
        changeMade = true;
        tempDir = DirectionNESW::West;
      }
    }

    if(changeMade)
      return tempDir;
  }

  return DirectionNESW::North;
}

bool AIController::Move()
{
  // If there is wall collision then return false (lose).
  if(_controllee->GetPosition().x >= prg::application.getScreenWidth() ||
     _controllee->GetPosition().x <= 0 ||
     _controllee->GetPosition().y >= prg::application.getScreenHeight() ||
     _controllee->GetPosition().y <= 39)
  {
    return false;
  }

  // If there isn't a fruit target then attempt to find fruit.
  if(!_pFruitTarget)
    _pFruitTarget = CalculateFruitTarget();
  else
  {
    if(!_pFruitTarget->IsActive())
    {
      _pFruitTarget = CalculateFruitTarget();
      return true;
    }
  }

  // Find the next fruit target.
  _pFruitTarget = CalculateFruitTarget();

  // Calculate direction to move in.
  _direction = CalculateDirection();

  // Update tail position.
  if(_controllee->GetTailSize() > 0)
  {
    for(int i = _controllee->GetTailSize() - 1; i > 0; i--)
      _controllee->SetTailPosAt(i, _controllee->GetpTail()[i - 1]);

    _controllee->GetpTail()[0].x = _controllee->GetPosition().x;
    _controllee->GetpTail()[0].y = _controllee->GetPosition().y;
  }

  //std::cout << "----PREDirection-- " << (int)_direction << std::endl;

  // Very tiny chance that avoiding collision will fail. (On Medium and Easy difficulty).
  if(_difficulty < 2)
  {
    int dontAvoid = rand() % 250;
    if(dontAvoid > 0)
      _direction = AvoidCollision(_direction);
    else
      std::cout << "[DONT AVOID]" << std::endl;
  }
  else
    _direction = AvoidCollision(_direction);

  //std::cout << "-AFTDirection- " << (int)_direction << std::endl;

  // Update the position depending on the direction calculated.
  switch(_direction)
  {
    case DirectionNESW::North:
      _controllee->SetPosition({ _controllee->GetPosition().x,_controllee->GetPosition().y + 10 });
      break;
    case DirectionNESW::East:
      _controllee->SetPosition({ _controllee->GetPosition().x + 10,_controllee->GetPosition().y });
      break;
    case DirectionNESW::South:
      _controllee->SetPosition({ _controllee->GetPosition().x,_controllee->GetPosition().y - 10 });
      break;
    case DirectionNESW::West:
      _controllee->SetPosition({ _controllee->GetPosition().x - 10,_controllee->GetPosition().y });
      break;
  }

  // If there are fruit..
  if(_pFruit)
  {
    // Check for a collision.
    int _fruitC = FruitCollision();

    // If there is a fruit collision eat it and grow the tail.
    if(_fruitC > -1 )
    {
      for(unsigned int i = 0; i < (*_pFruit)[_fruitC].GetReward(); i++)
        _controllee->AddTail();

      (*_pFruit)[_fruitC].Eat();
      _pFruitTarget = nullptr;
    }
  }

  return true;
}

bool AIController::DirectionSafe(DirectionNESW dir) const
{
  // Check if moving in the direction chosen will not result in a collision.
  if(AvoidOpponentCollision(dir) == dir &&
     AvoidSelfCollision(dir) == dir &&
     AvoidBorder(dir) == dir &&
     AvoidObstacleCollision(dir) == dir)
  {
    return true;
  }

  return false;
}

DirectionNESW AIController::AvoidCollision(DirectionNESW currDirection) const
{
  // Check which directions are safe to travel in.
  bool northFree = DirectionSafe(DirectionNESW::North);
  bool southFree = DirectionSafe(DirectionNESW::South);
  bool westFree = DirectionSafe(DirectionNESW::West);
  bool eastFree = DirectionSafe(DirectionNESW::East);

  //std::cout << "NorthFree: " << northFree << " \tSouthFree: " << southFree << " \tWestFree" << westFree << " \tEastFree" << eastFree <<std::endl;

  // START - The following chunk of code overrides the direction to get around obstacles
  // and under certain conditions to prevent infinite loops (up,down,up,down etc.)

  // Is there a wall to the left/up/down/right/bottom right/bottom left/top right/top left?
  int uInd = -1, dInd = -1, lInd = -1, rInd = -1, blInd = -1, brInd = -1, tlInd = -1, trInd = -1;
  for(unsigned int i = 0; i < _pObstacles->size(); i++)
  {
    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x, _controllee->GetPosition().y + 10}))
      uInd = i;

    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x, _controllee->GetPosition().y - 10}))
      dInd = i;

    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x - 10, _controllee->GetPosition().y}))
      lInd = i;

    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x + 10, _controllee->GetPosition().y}))
      rInd = i;

    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x + 10, _controllee->GetPosition().y + 10}) &&
       uInd == -1 && rInd == -1) // and the obstacle is not above or to the right.
      blInd = i;

    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x - 10, _controllee->GetPosition().y + 10}) &&
        uInd == -1 && lInd == -1) // and the obstacle is not above or to the left.
      brInd = i;

    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x + 10, _controllee->GetPosition().y - 10}) &&
        dInd == -1 && rInd == -1) // and the obstacle is not below or to the right.
      tlInd = i;

    if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x - 10, _controllee->GetPosition().y - 10}) &&
        dInd == -1 && lInd == -1) // and the obstacle is not below or to the left.
      trInd = i;
  }

  //    o
  // [______]
  //   p
  // If there is an obstacle above and AI is going north...
  // GO left/right (depending on quickest route).
  if(uInd > -1 && currDirection == DirectionNESW::North)
  {
    if(_pFruitTarget &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[uInd].GetPosition().y - ((*_pObstacles)[uInd].GetHeight()/2.0F) &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[uInd].GetPosition().y + ((*_pObstacles)[uInd].GetHeight()/2.0F))
    {
      if(westFree && _pFruitTarget->GetPosition().x < (*_pObstacles)[uInd].GetPosition().x)
        return DirectionNESW::West;
      else if(eastFree)
        return DirectionNESW::East;
    }

    if(_controllee->GetTailSize() > 3 &&
       _lootBoxPos.y >= (*_pObstacles)[uInd].GetPosition().y - ((*_pObstacles)[uInd].GetHeight()/2.0F) &&
       _lootBoxPos.y <= (*_pObstacles)[uInd].GetPosition().y + ((*_pObstacles)[uInd].GetHeight()/2.0F))
    {
      if(westFree && _lootBoxPos.x < (*_pObstacles)[uInd].GetPosition().x)
        return DirectionNESW::West;
      else if(eastFree)
        return DirectionNESW::East;
    }

    if(westFree && _controllee->GetPosition().x < (*_pObstacles)[uInd].GetPosition().x)
      return DirectionNESW::West;
    else if(eastFree)
      return DirectionNESW::East;
  }

  //     p
  // [______]
  //   o
  // If there is an obstacle below and AI is going south...
  // GO left/right (depending on quickest route).
  if(dInd > -1 && currDirection == DirectionNESW::South)
  {
    if(_pFruitTarget &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[dInd].GetPosition().y - ((*_pObstacles)[dInd].GetHeight()/2.0F) &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[dInd].GetPosition().y + ((*_pObstacles)[dInd].GetHeight()/2.0F))
    {
      if(westFree && _pFruitTarget->GetPosition().x < (*_pObstacles)[dInd].GetPosition().x)
        return DirectionNESW::West;
      else if(eastFree)
        return DirectionNESW::East;
    }

    if(_controllee->GetTailSize() > 3 &&
       _lootBoxPos.y >= (*_pObstacles)[dInd].GetPosition().y - ((*_pObstacles)[dInd].GetHeight()/2.0F) &&
       _lootBoxPos.y <= (*_pObstacles)[dInd].GetPosition().y + ((*_pObstacles)[dInd].GetHeight()/2.0F))
    {
      if(westFree && _lootBoxPos.x < (*_pObstacles)[dInd].GetPosition().x)
        return DirectionNESW::West;
      else if(eastFree)
        return DirectionNESW::East;
    }

    if(westFree && _controllee->GetPosition().x < (*_pObstacles)[dInd].GetPosition().x)
      return DirectionNESW::West;
    else if(eastFree)
      return DirectionNESW::East;
  }

  // o [    ]
  //   [____] p
  // If there is an obstacle to the left and AI is going west...
  // GO up/down (depending on quickest route).
  if(lInd > -1 &&
     ((_pFruitTarget && _pFruitTarget->GetPosition().x < _controllee->GetPosition().x &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[lInd].GetPosition().y - ((*_pObstacles)[lInd].GetHeight()/2.0F) &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[lInd].GetPosition().y + ((*_pObstacles)[lInd].GetHeight()/2.0F))
      ||
      (_controllee->GetTailSize() > 3 && _lootBoxPos.x < _controllee->GetPosition().x &&
       _lootBoxPos.y >= (*_pObstacles)[lInd].GetPosition().y - ((*_pObstacles)[lInd].GetHeight()/2.0F) &&
       _lootBoxPos.y <= (*_pObstacles)[lInd].GetPosition().y + ((*_pObstacles)[lInd].GetHeight()/2.0F))
      ||
      currDirection == DirectionNESW::West
      ))
  {
    if(southFree && _controllee->GetPosition().y <= (*_pObstacles)[lInd].GetPosition().y)
      return DirectionNESW::South;
    else if(northFree)
      return DirectionNESW::North;
  }

  //   [    ] o
  // p [____]
  // If there is an obstacle to the right and AI is going east...
  // GO up/down (depending on quickest route).
  if(rInd > -1 &&
     ((_pFruitTarget && _pFruitTarget->GetPosition().x > _controllee->GetPosition().x &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[rInd].GetPosition().y - ((*_pObstacles)[rInd].GetHeight()/2.0F) &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[rInd].GetPosition().y + ((*_pObstacles)[rInd].GetHeight()/2.0F))
      ||
      (_controllee->GetTailSize() > 3 && _lootBoxPos.x > _controllee->GetPosition().x
       &&_lootBoxPos.y >= (*_pObstacles)[rInd].GetPosition().y - ((*_pObstacles)[rInd].GetHeight()/2.0F)
       && _lootBoxPos.y <= (*_pObstacles)[rInd].GetPosition().y + ((*_pObstacles)[rInd].GetHeight()/2.0F))
      || currDirection == DirectionNESW::East
      ))
  {
    if(southFree && _controllee->GetPosition().y < (*_pObstacles)[rInd].GetPosition().y)
      return DirectionNESW::South;
    else if(northFree)
      return DirectionNESW::North;
  }

  // o [____]
  //          p
  // If the AI is at the bottom right of an obstacle and the target is left of it.
  // GO left
  if(brInd > -1 &&
     ((_pFruitTarget && _pFruitTarget->GetPosition().x < _controllee->GetPosition().x &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[brInd].GetPosition().y - ((*_pObstacles)[brInd].GetHeight()/2.0F) + 4.0F &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[brInd].GetPosition().y + ((*_pObstacles)[brInd].GetHeight()/2.0F) - 4.0F)
      ||
      (_controllee->GetTailSize() > 3 && _lootBoxPos.x < _controllee->GetPosition().x &&
       _lootBoxPos.y >= (*_pObstacles)[brInd].GetPosition().y - ((*_pObstacles)[brInd].GetHeight()/2.0F) &&
       _lootBoxPos.y <= (*_pObstacles)[brInd].GetPosition().y + ((*_pObstacles)[brInd].GetHeight()/2.0F))
      ))
  {
    if(westFree)
      return DirectionNESW::West;
  }

  //         p
  // o [____]
  // If the AI is at the top right of an obstacle and the target is left of it.
  // GO left
  if(trInd > -1 &&
     (// NOTE THAT THIS DOES ENTIRE LINE NOT JUST o[] . e.g. ------[__]------ (but because of < x its just -----[__])
      (_pFruitTarget && _pFruitTarget->GetPosition().x < _controllee->GetPosition().x &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[trInd].GetPosition().y - ((*_pObstacles)[trInd].GetHeight()/2.0F) + 4.0F &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[trInd].GetPosition().y + ((*_pObstacles)[trInd].GetHeight()/2.0F) - 4.0F)
      ||
      (_controllee->GetTailSize() > 3 && _lootBoxPos.x < _controllee->GetPosition().x &&
       _lootBoxPos.y >= (*_pObstacles)[trInd].GetPosition().y - ((*_pObstacles)[trInd].GetHeight()/2.0F) &&
       _lootBoxPos.y <= (*_pObstacles)[trInd].GetPosition().y + ((*_pObstacles)[trInd].GetHeight()/2.0F))
      ||
      currDirection == DirectionNESW::West
      ))
  {
    if(westFree)
      return DirectionNESW::West;
  }

  //   [____] o
  //  p
  // If the AI is at the bottom left of an obstacle and the target is right of it.
  // GO right
  if(blInd > -1 &&
     ((_pFruitTarget &&
       _pFruitTarget->GetPosition().x > _controllee->GetPosition().x &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[blInd].GetPosition().y - ((*_pObstacles)[blInd].GetHeight()/2.0F) + 4.0F &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[blInd].GetPosition().y + ((*_pObstacles)[blInd].GetHeight()/2.0F) - 4.0F)
      ||
      (_controllee->GetTailSize() > 3 && _lootBoxPos.x > _controllee->GetPosition().x &&
       _lootBoxPos.y >= (*_pObstacles)[blInd].GetPosition().y - ((*_pObstacles)[blInd].GetHeight()/2.0F) &&
       _lootBoxPos.y <= (*_pObstacles)[blInd].GetPosition().y + ((*_pObstacles)[blInd].GetHeight()/2.0F))
      ))
  {
    if(eastFree)
      return DirectionNESW::East;
  }

  // p
  //  [____] o
  // If the AI is at the top left of an obstacle and the target is right of it.
  // GO right
  if(tlInd > -1 &&
     ((_pFruitTarget &&
       _pFruitTarget->GetPosition().x > _controllee->GetPosition().x &&
       _pFruitTarget->GetPosition().y >= (*_pObstacles)[tlInd].GetPosition().y - ((*_pObstacles)[tlInd].GetHeight()/2.0F) + 4.0F &&
       _pFruitTarget->GetPosition().y <= (*_pObstacles)[tlInd].GetPosition().y + ((*_pObstacles)[tlInd].GetHeight()/2.0F) - 4.0F)
      ||
      (_controllee->GetTailSize() > 3 &&
       _lootBoxPos.x > _controllee->GetPosition().x &&
       _lootBoxPos.y >= (*_pObstacles)[tlInd].GetPosition().y - ((*_pObstacles)[tlInd].GetHeight()/2.0F) &&
       _lootBoxPos.y <= (*_pObstacles)[tlInd].GetPosition().y + ((*_pObstacles)[tlInd].GetHeight()/2.0F))
      ))
  {
    if(eastFree)
      return DirectionNESW::East;
  }

  // End of the chunk of code.

  // If the current direction is free then return that.
  switch(currDirection)
  {
    case DirectionNESW::North:
      if(northFree)
        return currDirection;
        break;
    case DirectionNESW::South:
      if(southFree)
        return currDirection;
        break;
    case DirectionNESW::West:
      if(westFree)
        return currDirection;
        break;
    case DirectionNESW::East:
      if(eastFree)
        return currDirection;
        break;
  }

  if(!northFree && !eastFree && !southFree && !westFree)
  {
    //std::cout << "<>-<NOTHING IS FREE>-<>" << std::endl;
    return currDirection;
  }
  else
    return RandomDirection(northFree, eastFree, southFree, westFree);
}

DirectionNESW AIController::RandomDirection(bool northFree, bool eastFree, bool southFree, bool westFree) const
{
  // The following code picks a random direction depending on which directions are free.
  int threeCondition = rand() % 3;
  int twoCondition = rand() % 2;

  if(northFree && southFree && eastFree)
  {
    if(threeCondition == 0)
      return DirectionNESW::North;
    if(threeCondition == 1)
      return DirectionNESW::South;
    if(threeCondition == 2)
      return DirectionNESW::East;
  }
  else if(northFree && southFree && westFree)
  {
    if(threeCondition == 0)
      return DirectionNESW::North;
    if(threeCondition == 1)
      return DirectionNESW::South;
    if(threeCondition == 2)
      return DirectionNESW::West;
  }
  else if(northFree && eastFree && westFree)
  {
    if(threeCondition == 0)
      return DirectionNESW::North;
    if(threeCondition == 1)
      return DirectionNESW::East;
    if(threeCondition == 2)
      return DirectionNESW::West;
  }
  else if(southFree && eastFree && westFree)
  {
    if(threeCondition == 0)
      return DirectionNESW::South;
    if(threeCondition == 1)
      return DirectionNESW::East;
    if(threeCondition == 2)
      return DirectionNESW::West;
  }
  else if(northFree && southFree)
  {
    if(twoCondition == 0)
      return DirectionNESW::North;
    if(twoCondition == 1)
      return DirectionNESW::South;
  }
  else if(northFree && westFree)
  {
    if(twoCondition == 0)
      return DirectionNESW::North;
    if(twoCondition == 1)
      return DirectionNESW::West;
  }
  else if(northFree && eastFree)
  {
    if(twoCondition == 0)
      return DirectionNESW::North;
    if(twoCondition == 1)
      return DirectionNESW::East;
  }
  else if(southFree && westFree)
  {
    if(twoCondition == 0)
      return DirectionNESW::South;
    if(twoCondition == 1)
      return DirectionNESW::West;
  }
  else if(southFree && eastFree)
  {
    if(twoCondition == 0)
      return DirectionNESW::South;
    if(twoCondition == 1)
      return DirectionNESW::East;
  }
  else if(eastFree && westFree)
  {
    if(twoCondition == 0)
      return DirectionNESW::East;
    if(twoCondition == 1)
      return DirectionNESW::West;
  }
  else if(northFree)
    return DirectionNESW::North;
  else if(southFree)
    return DirectionNESW::South;
  else if(eastFree)
    return DirectionNESW::East;
  else if(westFree)
    return DirectionNESW::West;

  // Nothing is free so just return North.
  return DirectionNESW::North;
}

DirectionNESW AIController::AvoidBorder(DirectionNESW currDirection) const
{
  // If there is a possible border collision then avoid it.
  switch(currDirection)
  {
    case DirectionNESW::North:
      if(_controllee->GetPosition().y + 10 >= prg::application.getScreenHeight())
         return DirectionNESW::West;
      break;

    case DirectionNESW::East:
      if(_controllee->GetPosition().x + 10 >= prg::application.getScreenWidth())
        return DirectionNESW::North;
      break;

    case DirectionNESW::South:
      if(_controllee->GetPosition().y - 10 <= 39)
        return DirectionNESW::West;
      break;

    case DirectionNESW::West:
      if(_controllee->GetPosition().x - 10 <= 0)
        return DirectionNESW::North;
      break;
  }

  return currDirection;
}

DirectionNESW AIController::AvoidObstacleCollision(DirectionNESW currDirection) const
{
  // If there are no obstacles then impossible to collide.
  if(!_pObstacles)
    return currDirection;

  // Collision detection between the AI and each obstacle.
  for(unsigned int i = 0; i < _pObstacles->size(); i++)
  {
    switch(currDirection)
    {
      case DirectionNESW::North:
        if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x, _controllee->GetPosition().y + 10}))
          return DirectionNESW::West;
        break;

      case DirectionNESW::South:
        if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x, _controllee->GetPosition().y - 10}))
          return DirectionNESW::West;
        break;

      case DirectionNESW::East:
        if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x + 10, _controllee->GetPosition().y}))
          return DirectionNESW::North;
        break;

      case DirectionNESW::West:
        if((*_pObstacles)[i].WithinBounds({_controllee->GetPosition().x - 10, _controllee->GetPosition().y}))
          return DirectionNESW::North;
        break;
    }
  }

  return currDirection;
}

DirectionNESW AIController::AvoidSelfCollision(DirectionNESW currDirection) const
{
  // Collision detection between AI (self) and tail. (taking into consideration the opponent).
  for(unsigned int i = 0; i < _controllee->GetTailSize();i++)
  {
    switch(currDirection)
    {
      case DirectionNESW::North:
        // top vs. bottom.
        if(WithinBoundaries({_controllee->GetPosition().x, _controllee->GetPosition().y + 10}, _controllee->GetpTail()[i]))
        {
          if(AvoidOpponentCollision(DirectionNESW::West) != DirectionNESW::West)
            return DirectionNESW::East;
          else
            return DirectionNESW::West;
        }
        break;

      case DirectionNESW::East:
        // right vs. left.
        if(WithinBoundaries({_controllee->GetPosition().x + 10, _controllee->GetPosition().y}, _controllee->GetpTail()[i]))
        {
          if(AvoidOpponentCollision(DirectionNESW::North) != DirectionNESW::North)
            return DirectionNESW::South;
          else
            return DirectionNESW::North;
        }
        break;

      case DirectionNESW::South:
        // bottom vs. top.
        if(WithinBoundaries({_controllee->GetPosition().x, _controllee->GetPosition().y - 10}, _controllee->GetpTail()[i]))
        {
          if(AvoidOpponentCollision(DirectionNESW::East) != DirectionNESW::East)
            return DirectionNESW::West;
          else
            return DirectionNESW::East;
        }
        break;

      case DirectionNESW::West:
        // left vs. right.
        if(WithinBoundaries({_controllee->GetPosition().x - 10, _controllee->GetPosition().y}, _controllee->GetpTail()[i]))
        {
          if(AvoidOpponentCollision(DirectionNESW::North) != DirectionNESW::North)
            return DirectionNESW::South;
          else
            return DirectionNESW::North;
        }
        break;
    }
  }

  return currDirection;
}

DirectionNESW AIController::AvoidOpponentCollision(DirectionNESW currDirection) const
{
  // If there is no opponent then impossible to collide.
  if(!_pOpponent)
    return currDirection;

  // Collision avoidance checking between
  // AI head vs. head (opponent) and AI head vs. tail (opponent).
  switch(currDirection)
  {
    case DirectionNESW::North:
      // top vs. bottom.
      if(HeadToOppHeadCollision({_controllee->GetPosition().x, _controllee->GetPosition().y + 10}) ||
         HeadToOppTailCollision({_controllee->GetPosition().x, _controllee->GetPosition().y + 10}))
          return DirectionNESW::East;

      break;

    case DirectionNESW::South:
      // bottom vs.top.
      if(HeadToOppHeadCollision({_controllee->GetPosition().x, _controllee->GetPosition().y - 10}) ||
         HeadToOppTailCollision({_controllee->GetPosition().x, _controllee->GetPosition().y - 10}))
          return  DirectionNESW::West;

      break;

    case DirectionNESW::East:
      // right vs. left.
      if(HeadToOppHeadCollision({_controllee->GetPosition().x + 10, _controllee->GetPosition().y}) ||
         HeadToOppTailCollision({_controllee->GetPosition().x + 10, _controllee->GetPosition().y}))
          return  DirectionNESW::North;

      break;

    case DirectionNESW::West:
      // left vs. right.
      if(HeadToOppHeadCollision({_controllee->GetPosition().x - 10, _controllee->GetPosition().y}) ||
         HeadToOppTailCollision({_controllee->GetPosition().x - 10, _controllee->GetPosition().y}))
          return DirectionNESW::South;

      break;
  }

  return currDirection;
}

bool AIController::WithinBoundaries(Position a, Position b) const
{
  if(// Right vs. Left
     a.x > b.x - 10 &&
     // Left vs. Right
     a.x < b.x + 10 &&
     // Top vs. Bottom
     a.y > b.y - 10 &&
     // Bottom vs. Top
     a.y < b.y + 10)
  {
    return true;
  }

  return false;
}

bool AIController::HeadToOppHeadCollision(Position pos) const
{
  // If AI (self) has a larger tail then go for head vs. head to win!
  if(_controllee->GetTailSize() > _pOpponent->GetpControllee()->GetTailSize())
    return false;

  // If there is a collision then return true.
  if(WithinBoundaries(pos,_pOpponent->GetpControllee()->GetPosition()))
    return true;

  // No collision.
  return false;
}

bool AIController::HeadToOppTailCollision(Position pos) const
{
  // Collision detection between AI (self) head vs. the opponent's tail.
  for(unsigned int i = 0; i < _pOpponent->GetpControllee()->GetTailSize(); i++)
  {
    if(WithinBoundaries(pos, {_pOpponent->GetpControllee()->GetpTail()[i].x + 2,
                              _pOpponent->GetpControllee()->GetpTail()[i].y + 2}))
      return true;
  }

  return false;
}
