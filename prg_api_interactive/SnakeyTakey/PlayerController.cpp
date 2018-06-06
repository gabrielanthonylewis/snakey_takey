#include "PlayerController.h"

PlayerController::PlayerController(std::vector<Fruit>* fruitVector, SnakeAbs* controllee)
{
  _controllee = controllee;

  // Assign reference to the fruit vector if it exists.
  if(fruitVector)
    _pFruit = fruitVector;

  prg::application.addKeyListener(*this);
}

PlayerController::~PlayerController()
{
  // Remove reference to the fruit vector if it exists.
  if(_pFruit)
    _pFruit = nullptr;

  prg::application.removeKeyListener(*this);
}

bool PlayerController::onKey(const prg::IKeyEvent::KeyEvent& key_event)
{
  // Change the direction of the snake.
  if(key_event.key_state == KeyEvent::KB_DOWN)
  {
    switch(toupper(key_event.key))
    {
      case 'W':
        ChangeDirection(DirectionNESW::North);
        break;

      case 'A':
        ChangeDirection(DirectionNESW::West);
        break;

      case 'S':
        ChangeDirection(DirectionNESW::South);
        break;

      case 'D':
        ChangeDirection(DirectionNESW::East);
        break;
      }
  }
  return true;
}

bool PlayerController::Move()
{
  // If there the snake has hit a border, return false.
  if(_controllee->GetPosition().x >= prg::application.getScreenWidth() ||
     _controllee->GetPosition().x <= 0 ||
     _controllee->GetPosition().y >= prg::application.getScreenHeight() ||
     _controllee->GetPosition().y <= 39)
  {
    return false;
  }

  // Update each tail piece's position
  // to the previous tail piece's position.
  for(int i = _controllee->GetTailSize(); i > 0; i--)
    _controllee->SetTailPosAt(i, _controllee->GetpTail()[i - 1]);


  // Update the closest tail piece to the position of the head.
  if(_controllee->GetTailSize() > 0)
  {
    _controllee->GetpTail()[0].x = _controllee->GetPosition().x;
    _controllee->GetpTail()[0].y = _controllee->GetPosition().y;
  }

  // Update the head's position
  // by a step of 10 depending on the direction.
  switch(_direction)
  {
    case DirectionNESW::North:
       _controllee->SetPosition({_controllee->GetPosition().x,_controllee->GetPosition().y + 10});
       break;

    case DirectionNESW::East:
      _controllee->SetPosition({_controllee->GetPosition().x + 10,_controllee->GetPosition().y});
      break;

    case DirectionNESW::South:
      _controllee->SetPosition({_controllee->GetPosition().x,_controllee->GetPosition().y - 10});
      break;

    case DirectionNESW::West:
      _controllee->SetPosition({_controllee->GetPosition().x - 10,_controllee->GetPosition().y});
      break;
  }

  // ---Fruit Collision Check---
  if(_pFruit)
  {
    // If there is a collision with a piece of fruit...
    int _fruitCollisionIdx = FruitCollision();
    if(_fruitCollisionIdx >= 0 &&
       (unsigned)_fruitCollisionIdx < _pFruit->size())
    {
      // Increase the size of the tail by the size of the reward (given by the fruit).
      for(unsigned int i = 0; i < (*_pFruit)[_fruitCollisionIdx].GetReward(); i++)
        _controllee->AddTail();

      //"Eat" the fruit.
      (*_pFruit)[_fruitCollisionIdx].Eat();
    }
  }

  return true;
}
