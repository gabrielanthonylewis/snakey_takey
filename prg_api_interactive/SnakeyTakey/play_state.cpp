#include "play_state.hpp"

bool PlayState::onCreate()
{
  if(debugFunctionSuccess)
    std::cout << "PlayState::onCreate() - Success" << std::endl;
  return true;
}

void PlayState::InitialiseGame(unsigned int difficulty)
{
  if(debugFunctionEnter)
    std::cout << "PlayState::InitialiseGame() - Enter" << std::endl;

  _isPaused = false;

  _obstacles.clear();
  _portals.clear();

  if(difficulty > 0)
  {
    // Set up 2 obstacles on the map.
    _obstacles.push_back(Obstacle({400,400}, 150, 20));
    _obstacles.push_back(Obstacle({350,200}, 400, 20));

    // Set up a portal (coupled so two points are created).
    _portals.push_back(Portal(new Obstacle({50, 400}, 10, 10), new Obstacle( {600,400}, 10, 10)));
  }
  if(difficulty > 1)
  {
    // Set up 2 more obstacles on the map.
    _obstacles.push_back(Obstacle({300,500}, 40, 120));
    _obstacles.push_back(Obstacle({100,475}, 60, 150));
  }

  // Set up the Player and AI loot boxes.
  if(_pLootBox)
    delete _pLootBox;
  _pLootBox = new LootBox({100,100});

  if(_pLootBoxAI)
    delete _pLootBoxAI;
  _pLootBoxAI = new LootBox({650, 100});

  // Set up the fruit manager with 5 fruits.
  if(_pFruitManager)
    _pFruitManager->Reset();
  else
    _pFruitManager = new FruitManager(5, &_obstacles, _pLootBox, _pLootBoxAI, &_portals);

  // Temp variable used to avoid segmentation fault when creating a Player/AI controller instance.
  std::vector<Fruit>* temppFruit = nullptr;
  if(_pFruitManager && _pFruitManager->GetpFruit())
      temppFruit = _pFruitManager->GetpFruit();

  // Set up the player snake.
  if(_pSnake)
    delete _pSnake;
  SnakeAbs* playerAbs = new SnakeAbs();
  playerAbs->SetController(_pSnake = new PlayerController(temppFruit, playerAbs));

  // Set up the AI snake.
  if(_pSnakeAI)
    delete _pSnakeAI;
  SnakeAbs* AISnakeAbs = new SnakeAbs();
  AISnakeAbs->SetController(_pSnakeAI = new AIController(temppFruit, {650, 100}, {650, 100}, &_obstacles, AISnakeAbs, difficulty));

  if(_pSnakeAI)
    _pSnakeAI->SetOpponent(_pSnake);

  // Set up the game timer.
  if(_pGameTimer)
    _pGameTimer->Reset();
  else
    _pGameTimer = new GameTimer(1000);

  // Start all of the timers.
  _timer.start();

  if(_pGameTimer && _pGameTimer->GetTimer())
    _pGameTimer->GetTimer()->start();

  if(_pFruitManager && &_pFruitManager->GetTimer())
    _pFruitManager->GetTimer().start();

  if(debugFunctionSuccess)
    std::cout << "PlayState::InitialiseGame() - Success" << std::endl;
}

bool PlayState::onDestroy()
{
  // Clean up everything.
  if(_pGameTimer)
  {
    delete _pGameTimer;
    _pGameTimer = nullptr;
  }

  if(_pLootBox)
  {
    delete _pLootBox;
    _pLootBox = nullptr;
  }

  if(_pLootBoxAI)
  {
    delete _pLootBoxAI;
    _pLootBoxAI = nullptr;
  }

  if(_pSnake)
  {
    delete _pSnake;
    _pSnake = nullptr;
  }

  if(_pSnakeAI)
  {
    delete _pSnakeAI;
    _pSnakeAI = nullptr;
  }

  if(debugFunctionSuccess)
    std::cout << "PlayState::onDestroy() - Success" << std::endl;
  return true;
}

void PlayState::EndGame(int playerNo, std::string gameOverMsg)
{
  _gameState = GameState::GameOver;
  _gameOverMessage = gameOverMsg;

  // playerNo: 1 = Player, 2 = AI, -3 = Draw, -5 = Time has run out

  // if the time has run out...
  if(playerNo == -5)
  {
    // If only one snake is playing then they are the winner.
    if(_pSnake && !_pSnakeAI)
      _winnerNo = 1;
    else if(!_pSnake && _pSnakeAI)
      _winnerNo = 2;

    // If there are two loot boxes, the winner is the one with the most loot.
    if(_pLootBox && _pLootBoxAI)
    {
      if(_pLootBox->GetLootCount() == _pLootBoxAI->GetLootCount())
        _winnerNo = -3; // Draw
      else if(_pLootBox->GetLootCount() > _pLootBoxAI->GetLootCount())
        _winnerNo = 1;
      else
        _winnerNo = 2;
    }
  }
  else
    _winnerNo = playerNo;

  if(debugFunctionSuccess)
    std::cout << "PlayState::SetGameState() - Success" << std::endl;
}

void PlayState::onEntry()
{
  prg::application.addKeyListener(*this);
  _timer.start();

  if(debugFunctionSuccess)
    std::cout << "PlayState::onEntry() - Success" << std::endl;
}

void PlayState::onExit()
{
  prg::application.removeKeyListener(*this);
  _timer.stop();

  if(debugFunctionSuccess)
    std::cout << "PlayState::onExit() - Success" << std::endl;
}

void PlayState::onUpdate()
{
  if(debugFunctionEnter)
    std::cout << "PlayState::onUpdate() - Enter" << std::endl;

  // If the game is paused then don't update.
  if(_isPaused)
    return;

  switch(_gameState)
  {
    case GameState::GameOver:
      // Don't run the update loop if the game is over.
      return;
      break;

    case GameState::Play:
      // If the time is up then end the game.
      if(_pGameTimer && _pGameTimer->TimeLeft() <= 0)
      {
        EndGame(-5, "Out of time!");
        return;
      }
      break;

    case GameState::MainMenu:
      break;
  }

  if(debugFunctionSuccess)
    std::cout << "PlayState::onUpdate() - Success" << std::endl;
}

void PlayState::onRender(prg::Canvas& canvas)
{
  if(debugFunctionEnter)
    std::cout << "PlayState::OnRender() - Enter" << std::endl;

  // Render the appropriate screen depending on the current game state.
  switch(_gameState)
  {
    case GameState::GameOver:
      RenderGameOverScreen(canvas);
      break;

    case GameState::Play:
      RenderGameScreen(canvas);
      break;

    case GameState::MainMenu:
      RenderMainMenuScreen(canvas);
      break;
  }

  if(debugFunctionSuccess)
    std::cout << "PlayState::onRender() - Success" << std::endl;
}

void PlayState::RenderMainMenuScreen(prg::Canvas& canvas) const
{
  prg::uint text_dims[2];

  // Output "Main Menu" message.
  prg::Font::MASSIVE.computePrintDimensions(text_dims, "Main Menu");
  prg::Font::MASSIVE.print(canvas, prg::application.getScreenWidth() / 2 - text_dims[0] / 2,
                          (prg::application.getScreenHeight() - text_dims[1] - 10),
                           prg::Colour(112,128,144), "Main Menu");

  // Output the "Easy mode" message.
  prg::Font::LARGE.computePrintDimensions(text_dims, "1. Easy mode");
  prg::Font::LARGE.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2),
                        (prg::application.getScreenHeight() - text_dims[1] / 2) - 180,
                         prg::Colour(112,128,144), "1. Easy mode");

  // Output the "Medium mode" message.
  prg::Font::LARGE.computePrintDimensions(text_dims, "2. Medium mode");
  prg::Font::LARGE.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2),
                        (prg::application.getScreenHeight() - text_dims[1] / 2) - 260,
                         prg::Colour(112,128,144), "2. Medium mode");

  // Output the "Hard mode" message.
  prg::Font::LARGE.computePrintDimensions(text_dims, "3. Hard mode");
  prg::Font::LARGE.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2),
                        (prg::application.getScreenHeight() - text_dims[1] / 2) - 340,
                         prg::Colour(112,128,144), "3. Hard mode");

  // Output the navigation message.
  prg::Font::SMALL.computePrintDimensions(text_dims, "Press '1', '2' or '3' to proceed or 'esc' to exit.");
  prg::Font::SMALL.print(canvas, prg::application.getScreenWidth() / 2 - text_dims[0] / 2,
                         10, prg::Colour::BLACK, "Press '1', '2' or '3' to proceed or 'esc' to exit.");
}

void PlayState::RenderGameOverScreen(prg::Canvas& canvas) const
{
  prg::uint text_dims[2];

  // Output "Game Over"
  prg::Font::MASSIVE.computePrintDimensions(text_dims, "Game Over");
  prg::Font::MASSIVE.print(canvas, prg::application.getScreenWidth() / 2 - text_dims[0] / 2,
                          (prg::application.getScreenHeight() / 2 - text_dims[1] / 2) + 100,
                           prg::Colour(112,128,144), "Game Over");

  // Output the winner.
  std::stringstream winnerNoText;

  prg::Colour winnerCol = prg::Colour::RED;
  if(_winnerNo == -3)
  {
    winnerNoText << "DRAW";
    winnerCol = prg::Colour(255,165,0);
  }
  else
  {
    winnerNoText << "Winner: Player ";
    winnerNoText<< _winnerNo;

    if(_winnerNo == 1)
      winnerCol = prg::Colour::BLACK;
  }

  prg::Font::MASSIVE.computePrintDimensions(text_dims, winnerNoText.str());
  prg::Font::MASSIVE.print(canvas, prg::application.getScreenWidth() / 2 - text_dims[0] / 2,
                           prg::application.getScreenHeight() / 2 - text_dims[1] / 2,
                           prg::Colour::RED, winnerNoText.str());

  // Output the Winner's Score
  std::stringstream scoreText;
  scoreText << "Score: ";

  if(_winnerNo == 1)
  {
    // Output the player's score.
    if(_pLootBox)
      scoreText << _pLootBox->GetLootCount();
    else
      scoreText << "nll";
  }
  else if(_winnerNo == 2)
  {
    // Output the AI's score.
    if(_pLootBoxAI)
      scoreText<< _pLootBoxAI->GetLootCount();
    else
      scoreText << "nll";
  }
  else if(_winnerNo == -3)
  {
    // Output both scores.
    if(_pLootBox)
      scoreText << _pLootBox->GetLootCount();
    else
      scoreText << "nll";

    scoreText << " vs. ";

    if(_pLootBoxAI)
      scoreText << _pLootBoxAI->GetLootCount();
    else
      scoreText << "nll";
  }

  prg::Font::LARGE.computePrintDimensions(text_dims, scoreText.str());
  prg::Font::LARGE.print(canvas, prg::application.getScreenWidth() / 2 - text_dims[0] / 2,
                        (prg::application.getScreenHeight() / 2 - text_dims[1] / 2) - 80,
                         winnerCol, scoreText.str());

  // Output the game over message.
  prg::Font::LARGE.computePrintDimensions(text_dims, scoreText.str());
  prg::Font::LARGE.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2) - 150,
                        (prg::application.getScreenHeight() / 2 - text_dims[1] / 2) - 180,
                         prg::Colour(112,128,144), _gameOverMessage);

  // Output the navigation message.
  prg::Font::SMALL.computePrintDimensions(text_dims, "Press 'esc' to quit or 'space' to continue to the main menu.");
  prg::Font::SMALL.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2),
                        (0 + text_dims[1] / 2),
                         prg::Colour(112,128,144), "Press 'esc' to quit or 'space' to continue to the main menu.");

  return;
}

void PlayState::RenderGameScreen(prg::Canvas& canvas) const
{
  prg::uint text_dims[2];

  // Render the background and border at the bottom of the screen.
  canvas.drawLine(0, 30, prg::application.getScreenWidth(), 30, prg::Colour(47,79,79));
  canvas.drawLine(0, 29, prg::application.getScreenWidth(), 29, prg::Colour(47,79,79));
  for(int i = 28; i >= 0; i--)
    canvas.drawLine(0, i, prg::application.getScreenWidth(), i, prg::Colour(250,250,250));

  {// Render the Player's score.
    std::stringstream text;
    text << "Score: " ;

    if(_pLootBox)
      text << _pLootBox->GetLootCount();
    else
      text << "nll";

    prg::Font::SMALL.computePrintDimensions(text_dims, text.str());
    prg::Font::SMALL.print(canvas, 30, 10, prg::Colour::BLACK, text.str());
  }

  {// Render the AI's score.
    std::stringstream aiScoreText;
    aiScoreText << "Score: ";

    if(_pLootBoxAI)
      aiScoreText << _pLootBoxAI->GetLootCount();
    else
      aiScoreText << "nll";

    prg::Font::SMALL.computePrintDimensions(text_dims, aiScoreText.str());
    prg::Font::SMALL.print(canvas, prg::application.getScreenWidth() - 100 , 10, prg::Colour::BLUE, aiScoreText.str());
  }

  {// Render the amount of time left.
    std::stringstream timeLeft;
    timeLeft << "Time Left: ";

    if(_pGameTimer)
      timeLeft<< _pGameTimer->TimeLeft();
    else
      timeLeft << "nll";

    prg::Font::SMALL.computePrintDimensions(text_dims, timeLeft.str());
    prg::Font::SMALL.print(canvas, prg::application.getScreenWidth() / 2.0F - 100, 10, prg::Colour(47,79,79), timeLeft.str());
  }

  // Render the Player Snake (inc. tail)
  if(_pSnake && _pSnake->GetpControllee())
    _pSnake->GetpControllee()->Render(canvas, prg::Colour::BLACK);

  // Render the AI Snake (inc. tail)
  if(_pSnakeAI && _pSnakeAI->GetpControllee())
    _pSnakeAI->GetpControllee()->Render(canvas, prg::Colour::BLUE);

  // Render the Player's LootBox
  if(_pLootBox)
    _pLootBox->Render(canvas, prg::Colour::BLACK);

  // Render the AI's LootBox
  if(_pLootBoxAI)
    _pLootBoxAI->Render(canvas, prg::Colour::BLUE);

  // Render all of the visible fruit.
  if(_pFruitManager && _pFruitManager->GetpFruit())
  {
    for(unsigned int i = 0; i < _pFruitManager->GetpFruit()->size(); i++)
    {
      if((*_pFruitManager->GetpFruit())[i].IsActive())
        (*_pFruitManager->GetpFruit())[i].Render(canvas);
    }
  }

  // Render all of the obstacles.
  for(unsigned int i = 0; i < _obstacles.size(); i++)
    _obstacles[i].Render(canvas, prg::Colour::RED, false);

  // Render all of the portals.
  for(unsigned int i = 0; i < _portals.size(); i++)
  {
    if(_portals[i].GetPointA())
      _portals[i].GetPointA()->Render(canvas, prg::Colour(128,0,128), true);
    if(_portals[i].GetPointB())
    _portals[i].GetPointB()->Render(canvas, prg::Colour(128,0,128), true);
  }

  // Outputs "Paused" if the game is paused.
  if(_isPaused)
  {
    // Draw the pause menu background image (a colour).
    for(unsigned int i = prg::application.getScreenWidth() / 2 - 200; i < prg::application.getScreenWidth() / 2 + 200; i++)
      canvas.drawLine(i, prg::application.getScreenHeight() / 2 - 100,
                      i, prg::application.getScreenHeight() / 2 + 200,
                      prg::Colour(200,200,200));

    // Output the title "Paused".
    prg::Font::MASSIVE.computePrintDimensions(text_dims, "Paused");
    prg::Font::MASSIVE.print(canvas, prg::application.getScreenWidth() / 2 - text_dims[0] / 2,
                            (prg::application.getScreenHeight() / 2 - text_dims[1] / 2) + 100,
                             prg::Colour(112,128,144), "Paused");

    // Output the navigation message.
    prg::Font::SMALL.computePrintDimensions(text_dims, "'p' to continue");
    prg::Font::SMALL.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2),
                          (prg::application.getScreenHeight() / 2 - text_dims[1] / 2) + 50,
                           prg::Colour(112,128,144), "'p' to continue");

    // Output the navigation message.
    prg::Font::SMALL.computePrintDimensions(text_dims, "'esc' to quit");
    prg::Font::SMALL.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2),
                          (prg::application.getScreenHeight() / 2 - text_dims[1] / 2 + 25),
                           prg::Colour(112,128,144), "'esc' to quit");

    // Output the navigation message.
    prg::Font::SMALL.computePrintDimensions(text_dims, "'space' to return to the main menu.");
    prg::Font::SMALL.print(canvas, (prg::application.getScreenWidth() / 2 - text_dims[0] / 2),
                          (prg::application.getScreenHeight() / 2 - text_dims[1] / 2),
                           prg::Colour(112,128,144), "'space' to return to the main menu.");
  }
}

bool PlayState::onKey(const prg::IKeyEvent::KeyEvent& key_event)
{
  if(debugFunctionEnter)
    std::cout << "PlayState::onKey() - Enter" << std::endl;

  if(key_event.key_state != KeyEvent::KB_DOWN)
  {
    if(debugFunctionSuccess)
      std::cout << "PlayState::onKey() - Success" << std::endl;

    return true;
  }

  if(_gameState == GameState::Play)
  {
    // Pause/Unpause the game.
    if(toupper(key_event.key) == 'P')
    {
      _isPaused = !_isPaused;
      if(_isPaused)
      {
        _timer.stop();
        if(_pGameTimer && _pGameTimer->GetTimer())
          _pGameTimer->GetTimer()->stop();

        if(_pFruitManager && &_pFruitManager->GetTimer())
          _pFruitManager->GetTimer().stop();
      }
      else
      {
        _timer.start();

        if(_pGameTimer && _pGameTimer->GetTimer())
          _pGameTimer->GetTimer()->start();

        if(_pFruitManager && &_pFruitManager->GetTimer())
          _pFruitManager->GetTimer().start();
      }
    }
  }
  else if(_gameState == GameState::MainMenu)
  {
    switch(toupper(key_event.key))
    {
      // Choose difficulty and start the game.
      case '1':
      case '2':
      case '3':

        // - 48 because ascii code numbers start from 48.
        InitialiseGame((int)key_event.key - 48 - 1);
         _gameState = GameState::Play;
        break;

      // Exit the program.
      case KeyEvent::KB_ESC_KEY:
        prg::application.exit();
        break;
      }
  }

  if(_gameState == GameState::GameOver || _isPaused)
  {
    switch(toupper(key_event.key))
    {
      // Return to the main menu.
      case KeyEvent::KB_SPC_KEY:
        _gameState = GameState::MainMenu;

        if(debugFunctionSuccess)
          std::cout << "PlayState::onKey() - Success" << std::endl;
        return true;
        break;

      // Exit the program.
      case KeyEvent::KB_ESC_KEY:
        prg::application.exit();
        break;
    }
  }

  if(debugFunctionSuccess)
    std::cout << "PlayState::onKey() - Success" << std::endl;

  return true;
}

void PlayState::onTimer(prg::Timer& timer)
{
  if(debugFunctionEnter)
    std::cout << "PlayState::onTimer() - Enter" << std::endl;

  // Return if the timer is not running.
  if(!_timer.running())
    return;

  // If the game is over then return.
  if(_gameState == GameState::GameOver || _isPaused)
    return;

  if(_gameState == GameState::Play)
  {
    // If both snakes hit wall at the same time then draw!
    if(_pSnake && _pSnakeAI)
    {
      if((_pSnake->GetpControllee()->GetPosition().x >= prg::application.getScreenWidth() ||
          _pSnake->GetpControllee()->GetPosition().x <= 0 ||
          _pSnake->GetpControllee()->GetPosition().y >= prg::application.getScreenHeight() ||
          _pSnake->GetpControllee()->GetPosition().y <= 38)
         &&
         (_pSnakeAI->GetpControllee()->GetPosition().x >= prg::application.getScreenWidth() ||
          _pSnakeAI->GetpControllee()->GetPosition().x <= 0 ||
          _pSnakeAI->GetpControllee()->GetPosition().y >= prg::application.getScreenHeight() ||
          _pSnakeAI->GetpControllee()->GetPosition().y <= 38))
        {
          EndGame(-3, "Both Wall Collision");
          return;
        }
    }

    // Player Snake collision detection and movement.
    if(_pSnake)
    {
      // Movement + Border collision detection.
      if(_pSnake->Move() == false)
      {
        EndGame(2, "Border Collision");
        return;
      }
      // Snake collision detection (self and opponent).
      if(_pSnake->SelfTailCollision())
      {
        EndGame(2, "Self Collision");
        return;
      }
      if(_pSnake->OtherSnakeCollision(_pSnakeAI))
      {
        EndGame(2, "Opponent Collision");
        return;
      }
      // Obstacle collision detection.
      for(unsigned int i = 0; i < _obstacles.size(); i++)
      {
        if(_obstacles[i].WithinBounds(_pSnake->GetpControllee()->GetPosition()))
        {
          EndGame(2, "Obstacle Collision");
          return;
        }
      }
    }

    // Head on collision logic.
    if(_pSnakeAI && _pSnakeAI)
    {
      if(_pSnakeAI->HeadOnCollision(_pSnake))
      {
        if(_pSnakeAI->GetpControllee()->GetTailSize() == _pSnake->GetpControllee()->GetTailSize())
          EndGame(-3, "Head on Head collision");
        else if(_pSnake->GetpControllee()->GetTailSize() > _pSnakeAI->GetpControllee()->GetTailSize())
          EndGame(1, "Head on Head collision");
        else
          EndGame(2, "Head on Head collision");

         return;
      }
    }

    // AI Snake collision detection and movement.
    if(_pSnakeAI)
    {
      // Movement + Border collision detection.
      if(_pSnakeAI->Move() == false)
      {
        EndGame(1, "Border Collision");
        return;
      }
      // Snake collision detection (self and opponent).
      if(_pSnakeAI->SelfTailCollision())
      {
        EndGame(1, "Self Collision");
        return;
      }
      if(_pSnakeAI->OtherSnakeCollision(_pSnake))
      {
        EndGame(1, "Opponent Collision");
        return;
      }
      // Obstacle collision detection.
      for(unsigned int i = 0; i < _obstacles.size(); i++)
      {
        if(_obstacles[i].WithinBounds(_pSnakeAI->GetpControllee()->GetPosition()))
        {
          EndGame(1, "Obstacle Collision");
          return;
        }
      }
    }

    // Portal Collision Detection
    for(unsigned int i = 0; i < _portals.size(); i++)
    {
      if(_pSnake)
      {
        if(_portals[i].GetPointA()->WithinBounds(_pSnake->GetpControllee()->GetPosition(), 2.0F))
          _pSnake->GetpControllee()->SetPosition(_portals[i].GetPointB()->GetPosition());
        else if(_portals[i].GetPointB()->WithinBounds(_pSnake->GetpControllee()->GetPosition(), 2.0F))
          _pSnake->GetpControllee()->SetPosition(_portals[i].GetPointA()->GetPosition());
      }
      if(_pSnakeAI)
      {
        if(_portals[i].GetPointA()->WithinBounds(_pSnakeAI->GetpControllee()->GetPosition(), 2.0F))
          _pSnakeAI->GetpControllee()->SetPosition(_portals[i].GetPointB()->GetPosition());
        else if(_portals[i].GetPointB()->WithinBounds(_pSnakeAI->GetpControllee()->GetPosition(), 2.0F))
          _pSnakeAI->GetpControllee()->SetPosition(_portals[i].GetPointA()->GetPosition());
      }
    }

    // Player LootBox Collision Detection
    if(_pLootBox && _pSnake)
    {
      if(_pSnake->GetpControllee()->GetTailSize() > 0)
      {
        if(_pSnake->GetpControllee()->GetPosition().x > _pLootBox->GetPosition().x - 20 &&
           _pSnake->GetpControllee()->GetPosition().x < _pLootBox->GetPosition().x + 20 &&
           _pSnake->GetpControllee()->GetPosition().y > _pLootBox->GetPosition().y - 20 &&
           _pSnake->GetpControllee()->GetPosition().y < _pLootBox->GetPosition().y + 20)
        {
          _pLootBox->AddLoot(_pSnake->GetpControllee()->GetTailSize());
          _pSnake->GetpControllee()->ResetSize();
        }
      }
    }

    // AI LootBox Collision Detection
    if(_pLootBoxAI && _pSnakeAI)
    {
      if(_pSnakeAI->GetpControllee()->GetTailSize() > 0)
      {
        if(_pSnakeAI->GetpControllee()->GetPosition().x > _pLootBoxAI->GetPosition().x - 20 &&
           _pSnakeAI->GetpControllee()->GetPosition().x < _pLootBoxAI->GetPosition().x + 20 &&
           _pSnakeAI->GetpControllee()->GetPosition().y > _pLootBoxAI->GetPosition().y - 20 &&
            _pSnakeAI->GetpControllee()->GetPosition().y < _pLootBoxAI->GetPosition().y + 20)
        {
          _pLootBoxAI->AddLoot(_pSnakeAI->GetpControllee()->GetTailSize());
          _pSnakeAI->GetpControllee()->ResetSize();
        }
      }
    }
  }

  if(debugFunctionSuccess)
    std::cout << "PlayState::onTimer() - Success" << std::endl;
}
