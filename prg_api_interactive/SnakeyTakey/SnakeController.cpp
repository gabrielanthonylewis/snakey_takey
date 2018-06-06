#include "SnakeController.h"

SnakeController::~SnakeController()
{
  _controllee = nullptr;
  _pFruit = nullptr;
}

int SnakeController::FruitCollision() const
{
  // If there are no fruit to collide with then return.
  if(!_pFruit)
    return -2;

  // Collision detection between the snake and each fruit.
  for(unsigned int i = 0; i < _pFruit->size(); i++)
  {
    // If the fruit is not active yet then it can't be collided with so continue..
    if(!((*_pFruit)[i].IsActive()))
      continue;

    if(// Right side of the head vs. Left side of the fruit.
       _controllee->GetPosition().x + _controllee->GetRadius() > (*_pFruit)[i].GetPosition().x - (*_pFruit)[i].GetRadius() - 1.0F &&
       // Left side of the head vs. Right side of the fruit.
       _controllee->GetPosition().x - _controllee->GetRadius() < (*_pFruit)[i].GetPosition().x + (*_pFruit)[i].GetRadius() + 1.0F &&
       // Top of the head vs. Bottom of the fruit.
       _controllee->GetPosition().y + _controllee->GetRadius() > (*_pFruit)[i].GetPosition().y - (*_pFruit)[i].GetRadius() - 1.0F &&
       // Bottom of the head vs. Top of the fruit.
       _controllee->GetPosition().y - _controllee->GetRadius() < (*_pFruit)[i].GetPosition().y + (*_pFruit)[i].GetRadius() + 1.0F)
    {
      return i;
    }
  }

  return -1;
}

bool SnakeController::OtherSnakeCollision(SnakeController* pOtherSnake) const
{
  // If the other snake does not exist then collision is impossible.
  if(!pOtherSnake)
    return false;

  // If the other snake does not have a tail then return.
  if(pOtherSnake->GetpControllee()->GetTailSize() == 0)
    return false;

  // Collision detection between the snake head and another snake's tail.
  for(unsigned int i = 0; i < pOtherSnake->GetpControllee()->GetTailSize(); i++)
  {
    if(// Right vs. Left
       _controllee->GetPosition().x > pOtherSnake->GetpControllee()->GetpTail()[i].x - 10 &&
       // Left vs. Right
       _controllee->GetPosition().x < pOtherSnake->GetpControllee()->GetpTail()[i].x + 10 &&
       // Top vs. Bottom
       _controllee->GetPosition().y > pOtherSnake->GetpControllee()->GetpTail()[i].y - 10 &&
       // Bottom vs. Top
       _controllee->GetPosition().y < pOtherSnake->GetpControllee()->GetpTail()[i].y + 10)
    {
      return true;
    }
  }

  return false;
}

bool SnakeController::HeadOnCollision(SnakeController* pOtherSnake) const
{
  // If the other snake does not exist then collision is impossible.
  if(!pOtherSnake)
    return false;

  // Collision detection between two snake's heads.
  if(// Right vs. Left
     _controllee->GetPosition().x + 3.0F >= pOtherSnake->GetpControllee()->GetPosition().x - 3.0F &&
     // Left vs. Right
     _controllee->GetPosition().x - 3.0F <= pOtherSnake->GetpControllee()->GetPosition().x + 3.0F &&
     // Top vs. Bottom
     _controllee->GetPosition().y + 3.0F >= pOtherSnake->GetpControllee()->GetPosition().y - 3.0F &&
     // Bottom vs. Top
     _controllee->GetPosition().y - 3.0F <= pOtherSnake->GetpControllee()->GetPosition().y + 3.0F)
  {
    return true;
  }

  return false;
}

bool SnakeController::SelfTailCollision() const
{
  // If the snake has no tail then tail collision is impossible.
  if(_controllee->GetTailSize() == 0)
    return false;

  // Collision detection between snake's head and snake's tail.
  for(unsigned int i = 0; i < _controllee->GetTailSize(); i++)
  {
    if(// Right vs. Left
       _controllee->GetPosition().x > _controllee->GetpTail()[i].x - 10 &&
       // Left vs. Right
       _controllee->GetPosition().x < _controllee->GetpTail()[i].x + 10 &&
       // Top vs. Bottom
       _controllee->GetPosition().y > _controllee->GetpTail()[i].y - 10 &&
       // Bottom vs. Top
       _controllee->GetPosition().y < _controllee->GetpTail()[i].y + 10)
    {
      return true;
    }
  }

  return false;
}
