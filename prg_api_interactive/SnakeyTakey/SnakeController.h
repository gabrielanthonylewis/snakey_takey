#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include <prg_interactive.hpp>
#include "Fruit.h"
#include "SnakeAbs.h"

// Possible directions, North being 1, East being 2, etc.
enum class DirectionNESW { North = 1, East, South, West};

class SnakeAbs;

class SnakeController
{
  public:
    virtual ~SnakeController() = 0;
    virtual bool Move() = 0;
    // NOTE: Will only be implemented by AI but compiler needs to be aware of this function.
    virtual void SetOpponent(SnakeController* opp) {};

    // Snake Collision detection
    bool SelfTailCollision() const; //Self vs. own tail.
    bool OtherSnakeCollision(SnakeController* pOtherSnake) const; //Self vs. other snake's tail.
    bool HeadOnCollision(SnakeController* pOtherSnake) const; //Self vs. other snake's head.

    // Setter Functions
    void SetControllee(SnakeAbs* controllee) {_controllee = controllee; }
    void ChangeDirection(DirectionNESW new_direction) { _direction = new_direction; }

    // Getter Functions
    SnakeAbs* GetpControllee() const { return _controllee; }

  protected:
    // Collision detection between self and the fruit.
    int FruitCollision() const;

  protected:
    // A pointer to the base snake itself.
    SnakeAbs* _controllee = nullptr;

    // The current direction the snake is travelling in.
    DirectionNESW _direction {DirectionNESW::North};

    // Reference to the fruit in the scene.
    std::vector<Fruit>* _pFruit = nullptr;
};

#endif // SNAKECONTROLLER_H
