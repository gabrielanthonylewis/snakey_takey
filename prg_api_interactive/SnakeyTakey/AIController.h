#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <cmath> // sqrt, pow

#include "SnakeController.h"
#include "Obstacle.h"

class AIController final : public SnakeController
{
  public:
     AIController() = default;
     AIController(std::vector<Fruit>* fruitVector, Position spawnPos, Position lootBoxPos,
                  std::vector<Obstacle>* obstacleVector, SnakeAbs* controllee, unsigned int difficulty);
    virtual ~AIController();

    // Deals with move behaviour of the snake (inc. tail)
    // and also border and fruit collision.
    // If the snake hits a border false will be returned.
    bool Move() override;

    // Setter Functions
    void SetOpponent(SnakeController* opp) override { _pOpponent = opp; }

  private:
    // Returns the direction to travel in to reach the target
    // (depending on whether there is a fruit target or if the snake should move towards the loot box)
    DirectionNESW CalculateDirection();

  private:
    // Chooses the next fruit target.
    Fruit* CalculateFruitTarget() const;

    // Returns true if position a is within another position b.
    bool WithinBoundaries(Position a, Position b) const;

    // Returns true if the head (self) collides with the opponent's head.
    bool HeadToOppHeadCollision(Position pos) const;
    // Returns true if the head (self) collides with the opponent's tail.
    bool HeadToOppTailCollision(Position pos) const;

    // Returns true if the direction is safe to travel in.
    bool DirectionSafe(DirectionNESW dir) const;

    // Returns a random free direction.
    DirectionNESW RandomDirection(bool northFree, bool eastFree, bool southFree, bool westFree) const;

    // Returns a direction that will ...
    // ...avoid any collision.
    DirectionNESW AvoidCollision(DirectionNESW currDirection) const;
    // ...avoid colliding with a border.
    DirectionNESW AvoidBorder(DirectionNESW currDirection) const;
    // ...avoid colliding with the opponent.
    DirectionNESW AvoidOpponentCollision(DirectionNESW currDirection) const;
    // ...avoid self collision.
    DirectionNESW AvoidSelfCollision(DirectionNESW currDirection) const;
    // ...avoid colliding with an obstacle.
    DirectionNESW AvoidObstacleCollision(DirectionNESW currDirection) const;

    // The current fruit target that the snake will head towards.
    Fruit* _pFruitTarget = nullptr;

    // The position of the Loot box to return back to.
    Position _lootBoxPos = {0,0};

    // A pointer to the opponent (to avoid in most cases).
    SnakeController* _pOpponent = nullptr;

    // A pointer to the obstacles on the map (to avoid).
    std::vector<Obstacle>* _pObstacles = nullptr;

    // Difficulty level, the higher the difficulty (max is 2),
    // the more efficient and smarter the snake is.
    unsigned int _difficulty = 0;
};

#endif // AICONTROLLER_H
