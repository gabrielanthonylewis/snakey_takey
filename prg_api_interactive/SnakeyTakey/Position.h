#ifndef POSITION_H
#define POSITION_H

struct Position final
{
  // x and y position on the game screen.
  unsigned int x = 0, y = 0;

  Position(){};
  Position(unsigned int xPos, unsigned int yPos) : x(xPos), y(yPos) {}
};

#endif // POSITION_H
