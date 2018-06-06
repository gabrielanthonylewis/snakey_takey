#include "SnakeAbs.h"

SnakeAbs::SnakeAbs()
{
  // FIXES A CRASH WHICH OCCURRED AFTER PICKING UP A 3rd ADDITIONAL FRUIT
  _tail.reserve(100);
}

SnakeAbs::~SnakeAbs()
{
  _controller = nullptr;
}

void SnakeAbs::AddTail()
{
  // Initialise the tail piece off screen.
  Position tempPos {999, 999};

  // Add the tail piece to the tail.
  _tail.push_back(tempPos);
}

void SnakeAbs::ResetSize()
{
  _tail.clear();
}

void SnakeAbs::Render(prg::Canvas& canvas, prg::Colour colour) const
{
  // Draw the head
  canvas.drawCircle(GetPosition().x, GetPosition().y, _radius, colour);

  // Draw each tail piece (size if one less than the head).
  for(Position pos : _tail)
    canvas.drawCircle(pos.x, pos.y, (_radius - 1), colour);
}

bool SnakeAbs::WithinBounds(Position left, Position right, float lRadius, float rRadius) const
{
  // Half the radius for the correct calculation.
  lRadius /= 2.0F;
  rRadius /= 2.0F;

  if(// Right side vs. Left Side
     left.x + lRadius > right.x - rRadius &&
     // Left Side vs. Right Side
     left.x - lRadius < right.x + rRadius &&
     // Top vs. Bottom
     left.y + lRadius > right.y - rRadius &&
     // Bottom vs. Top
     left.y - lRadius < right.y + rRadius)
  {
    return true;
  }

  return false;
}
