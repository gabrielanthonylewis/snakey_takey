#ifndef LOOTBOX_H
#define LOOTBOX_H

#include <prg_interactive.hpp>
#include "Position.h"

class LootBox final
{
  public:
    LootBox(Position pos);
    ~LootBox() {};

    // Render the Loot Box.
    void Render(prg::Canvas& canvas, prg::Colour colour) const;

    // Add loot to the total amount.
    void AddLoot(unsigned int val) { _lootCount += val; }

    // Getter Functions
    Position GetPosition() const { return _position; }
    unsigned int GetLootCount() const { return _lootCount; }
    unsigned int GetRadius() const { return _radius; }

  private:
    // Position of the Loot Box.
    Position _position { 200, 200};

    // The amount of loot held.
    unsigned int _lootCount = 0;

    // The radius of the object.
    unsigned int _radius = 10;
};

#endif // LOOTBOX_H
