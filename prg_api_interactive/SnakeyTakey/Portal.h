#ifndef PORTAL_H
#define PORTAL_H

#include "Obstacle.h"

class Portal final
{
  public:
    Portal(Obstacle* pointA, Obstacle* pointB);
    ~Portal();

    // Getter Functions
    Obstacle* GetPointA() const { return _pointA; }
    Obstacle* GetPointB() const { return _pointB; }

  private:
    // Both points as the portals are coupled.
    Obstacle* _pointA;
    Obstacle* _pointB;
};

#endif // PORTAL_H
