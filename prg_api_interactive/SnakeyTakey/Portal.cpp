#include "Portal.h"

Portal::Portal(Obstacle* pointA, Obstacle* pointB)
: _pointA(pointA), _pointB(pointB)
{}

Portal::~Portal()
{
  _pointA = nullptr;
  _pointB = nullptr;
}
