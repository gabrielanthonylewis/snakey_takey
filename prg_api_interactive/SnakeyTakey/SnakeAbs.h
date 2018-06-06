#ifndef SNAKEABS_H
#define SNAKEABS_H

#include <vector>

#include <prg_interactive.hpp>
#include "Position.h"

class SnakeController;

class SnakeAbs final
{
  public:
    SnakeAbs();
    virtual ~SnakeAbs();

    // Render the entire snake.
    void Render(prg::Canvas& canvas, prg::Colour colour) const;

    // Add one tail piece to the snake.
    void AddTail();

    // Reset the size of the snake (resulting in no tail).
    void ResetSize();

    // Checks whether there is a collision between 2 positions each with their respective radius.
    bool WithinBounds(Position left, Position right, float lRadius, float rRadius) const;

    // Setter Functions
    void SetController(SnakeController* controller) { _controller = controller; }
    void SetPosition(Position pos) { _position = pos; }
    void SetTailPosAt(int idx, Position pos) { _tail[idx] = pos; }

    // Getter Functions
    std::vector<Position>& GetpTail() { return _tail; }
    unsigned int GetTailSize() const { return _tail.size(); }
    const Position& GetPosition() const { return _position; }
    float GetRadius() const { return _radius; }
    SnakeController* GetController() const { return _controller; }

  protected:
    // A pointer to the controller of the snake.
    SnakeController* _controller = nullptr;

    // The radius of the snake head.
    float _radius = 6.0F;

    // The position of the snake head.
    Position _position {100, 100};

    // The snake's tail, represented by positions.
    std::vector<Position> _tail;
};

#endif // SNAKEABS_H
