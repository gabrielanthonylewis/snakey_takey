#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <prg_interactive.hpp>
#include "Position.h"

class Obstacle final
{
  public:
    Obstacle(Position spawnpoint, int width, int height);

    // Render the Obstacle.
    void Render(prg::Canvas& canvas, prg::Colour colour, bool innerCircle) const;

    // Checks to see whether a position is inside the obstacle.
    bool WithinBounds(Position position) const;
    // Checks to see whether a position (+ radius) is inside the obstacle (e.g. top of the snake head).
    bool WithinBounds(Position position, float radius) const;

    // Getter Functions
    Position GetPosition() const { return _position; }
    unsigned int GetWidth() const { return _width; }
    unsigned int GetHeight() const { return _height; }

  private:
    // The position of the obstacle (centred).
    Position _position;

    // The width and height of the obstacle.
    unsigned int _width = 0, _height = 0;
};

#endif // OBSTACLE_H
