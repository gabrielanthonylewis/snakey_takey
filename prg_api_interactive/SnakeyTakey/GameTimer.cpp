#include "GameTimer.h"

GameTimer::GameTimer(prg::uint ms) : _ms(ms)
{
  // Initialise and start the timer.
  prg::Timer temp {4, _ms, *this};
  _gameTimer = temp;
  _gameTimer.start();
}

void GameTimer::Reset()
{
  // Reset back to the original state.
  _timeLeft = 90;
  _gameTimer.reset(_ms);
}

void GameTimer::onTimer(prg::Timer& timer)
{
  if(_gameTimer.running())
  {
    // Reduce the time left by one.
    _timeLeft--;

    // Enforce lower bound of 0.
    if(_timeLeft < 0)
      _timeLeft = 0;
  }
}
