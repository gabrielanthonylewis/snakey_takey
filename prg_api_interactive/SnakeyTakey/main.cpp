#include <prg_interactive.hpp>
#include <stdexcept>
#include "play_state.hpp"

int main()
{
  try
  {
    PlayState play_state;
    prg::application.addState("play_state", play_state);
    prg::application.run("Gabriel Lewis", "Q5094111", 800, 600);
  }
  catch(std::exception& e)
  {
    prg::logger.error(e.what());
  }

  return 0;
}
