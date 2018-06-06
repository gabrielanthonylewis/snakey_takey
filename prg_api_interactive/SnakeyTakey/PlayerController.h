#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "SnakeController.h"

class PlayerController final : public SnakeController,
                               public prg::IKeyEvent
{
  public:
    PlayerController() = default;
    PlayerController(std::vector<Fruit>* fruitVector, SnakeAbs* controllee);
    virtual ~PlayerController();

  private:
    bool onKey(const prg::IKeyEvent::KeyEvent& key_event) override;

    // Deals with move behaviour of the snake (inc. tail)
    // and also border and fruit collision.
    bool Move() override;
};

#endif // PLAYERCONTROLLER_H
