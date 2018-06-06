#include "Obstacle.h"

Obstacle::Obstacle(Position spawnpoint, int width, int height) : _position(spawnpoint), _width(width), _height(height)
{}

bool Obstacle::WithinBounds(Position position) const
{
  //NOTE: 2.5f is simply used so that the player can see the Snake slightly intersect the object.
  // (3 is radius of snake head)

  // If the position is within the bounds of the obstacle...
  if(position.x > _position.x - (_width / 2.0F) - 2.5f &&
     position.x < _position.x + (_width / 2.0F) + 2.5f &&
     position.y > _position.y - (_height / 2.0F) - 2.5f &&
     position.y < _position.y + (_height / 2.0F) + 2.5f)
  {
    return true;
  }
  return false;
}

bool Obstacle::WithinBounds(Position position, float radius) const
{
  // If the position + the radius (i.e. the edge of the other object)
  // is within the bounds of the obstacle...
  if(position.x + radius > _position.x - (_width / 2.0F) &&
     position.x - radius < _position.x + (_width / 2.0F) &&
     position.y + radius > _position.y - (_height / 2.0F) &&
     position.y - radius < _position.y + (_height / 2.0F))
  {
    return true;
  }
  return false;
}

void Obstacle::Render(prg::Canvas& canvas, prg::Colour colour, bool innerCircle) const
{
  //Draw an inner circle if requested
  if(innerCircle)
    canvas.drawCircle(_position.x, _position.y, 2.0, colour);

  // Top line of the rectangle.
  canvas.drawLine(_position.x - (_width / 2.0F),
                  _position.y + (_height / 2.0F),
                  _position.x + (_width / 2.0F),
                  _position.y + (_height / 2.0F),
                  colour);

  // Bottom line of the rectangle.
  canvas.drawLine(_position.x - (_width / 2.0F),
                  _position.y - (_height / 2.0F),
                  _position.x + (_width / 2.0F),
                  _position.y - (_height / 2.0F),
                  colour);

  // Left line of the rectangle.
  canvas.drawLine(_position.x - (_width / 2.0F),
                  _position.y + (_height / 2.0F),
                  _position.x - (_width / 2.0F),
                  _position.y - (_height / 2.0F),
                  colour);

  // Right line of the rectangle.
  canvas.drawLine(_position.x + (_width / 2.0F),
                  _position.y + (_height / 2.0F),
                  _position.x + (_width / 2.0F),
                  _position.y - (_height / 2.0F),
                  colour);
}
