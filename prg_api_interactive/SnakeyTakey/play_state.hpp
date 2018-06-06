#if !defined PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include <iostream>
#include <cstdlib>
#include <vector>

#include <prg_interactive.hpp>
#include "SnakeAbs.h"
#include "AIController.h"
#include "PlayerController.h"
#include "Fruit.h"
#include "GameTimer.h"
#include "LootBox.h"
#include "FruitManager.h"
#include "Obstacle.h"
#include "Portal.h"

class PlayState final : public prg::IAppState,
                        public prg::IKeyEvent,
                        public prg::ITimerEvent
{
  private:
    bool onCreate() override;
    bool onDestroy() override;
    void onEntry() override;
    void onExit() override;
    void onUpdate() override;
    void onRender(prg::Canvas& canvas) override;
    bool onKey(const prg::IKeyEvent::KeyEvent& key_event) override;
    void onTimer(prg::Timer& timer) override;

    // Initialises the game depending on the difficulty set,
    // 0 = easy, 1 = medium, 2 = hard.
    void InitialiseGame(unsigned int difficulty);

    // Render Screen functions.
    void RenderGameOverScreen(prg::Canvas& canvas) const;
    void RenderGameScreen(prg::Canvas& canvas) const;
    void RenderMainMenuScreen(prg::Canvas& canvas) const;

    // Ends the playable game.
    // playerNo: 1 = Player, 2 = AI, -3 = Draw, -5 = Time has run out
    void EndGame(int playerNo, std::string gameOverMsg);

  private:
    // Possible game states.
    enum class GameState {MainMenu = 0, Play = 1, GameOver = 2};
    // The current game state, by default the game will start on the main menu.
    GameState _gameState = GameState::MainMenu;

    // If true then the game is paused until the player decides to continue.
    bool _isPaused = false;

    // Game timer itself.
    prg::Timer _timer{0, 60, *this};

    // The game timer is used to end the game after x amount of seconds.
    GameTimer* _pGameTimer = nullptr;

    // The player's loot box.
    LootBox* _pLootBox = nullptr;
    // The AI's loot box.
    LootBox* _pLootBoxAI = nullptr;

    // The player snake.
    SnakeController* _pSnake = nullptr;
    // The AI snake.
    SnakeController* _pSnakeAI = nullptr;

    // The fruit manager, that spawns all of the fruit and manages them.
    FruitManager* _pFruitManager = nullptr;

    // The obstacles on the map.
    std::vector<Obstacle> _obstacles;

    // The portals (coupled) on the map.
    std::vector<Portal> _portals;

    // The number depends on which player has won or if it is a draw.
    int _winnerNo = 0;

    // The message to be outputted on the game over screen, e.g. ("Wall Collision).
    std::string _gameOverMessage = "nll";

    // Debug flags, when true specific messages will be outputted into the console.
    bool debugFunctionEnter = true;
    bool debugFunctionSuccess = true;
};

#endif // PLAY_STATE_HPP
