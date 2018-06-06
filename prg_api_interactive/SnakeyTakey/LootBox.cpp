#include "LootBox.h"

LootBox::LootBox(Position pos) : _position(pos)
{}

void LootBox::Render(prg::Canvas& canvas, prg::Colour colour) const
{
  // Draw LootBox (a circle).
  canvas.drawCircle(_position.x, _position.y, _radius, colour);
}
