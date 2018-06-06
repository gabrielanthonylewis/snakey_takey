#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <prg_interactive.hpp>

class GameTimer final : public prg::ITimerEvent
{
  public:
    GameTimer(prg::uint ms);
    ~GameTimer() {};

    // Reset the Timer.
    void Reset();

    // Getter functions
    unsigned int TimeLeft() const { return _timeLeft; }
    prg::Timer* GetTimer()  { return &_gameTimer; }

  private:
    // Used to reduce time left.
    void onTimer(prg::Timer& timer) override;

  private:
    // The timer.
    prg::Timer _gameTimer;

    // The amount of time left (before game over).
    unsigned int _timeLeft = 90;

    // The milliseconds used within the timer.
    unsigned int _ms = 0;
};

#endif // GAMETIMER_H
