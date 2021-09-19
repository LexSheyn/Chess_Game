#include "stdafx.h"
#include "GameState.h"

// private: Functions:

void GameState::initVariables()
{
	this->buttonPressed = false;
	this->gameOver = false;
	this->wonTeam = Team::None;

	this->selectedPawn[Team::White] = nullptr;
	this->selectedPawn[Team::Black] = nullptr;
	this->lastSelectedIndex[Team::White] = 0;
	this->lastSelectedIndex[Team::Black] = 0;

	// Allowed positions
	this->allowedPositions[Team::White].resize(this->positions);

	this->direction[Team::White] = sf::Vector2f();
	this->direction[Team::Black] = sf::Vector2f();
	this->directionNormalized[Team::White] = sf::Vector2f();
	this->directionNormalized[Team::Black] = sf::Vector2f();

	// Turn
	this->turn = Team::White;
	this->turnLast = this->turn;
	this->turnTimer.restart();
	this->turnTimerMax = 0.5f; // Better to be 0.5f
	this->exitTimer.restart();
	this->exitTimerMax = 2.f;
}

void GameState::initDefferedRender()
{
	this->renderTexture.create
	(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height
	);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect
	(
		sf::IntRect
		(
			0,
			0,
			this->stateData->gfxSettings->resolution.width,
			this->stateData->gfxSettings->resolution.height
		)
	);
}

void GameState::initView()
{
	this->view.setSize
	(
		sf::Vector2f
		(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height)
		//	1600.f,
		//	900.f
		)
	);

//	this->view.setCenter
//	(
//		static_cast<float>(this->stateData->gfxSettings->resolution.width) / 2.f,
//		static_cast<float>(this->stateData->gfxSettings->resolution.height) / 2.f
//	//	1600.f / 2.f,
//	//	900.f / 2.f
//	);

	this->view.setCenter
	(
		std::floor(this->tileMap->getMaxSizeF().x / 2.f),
		std::floor(this->tileMap->getMaxSizeF().y / 2.f)
	);
}

void GameState::initPauseMenu()
{
	const sf::VideoMode vm = this->stateData->gfxSettings->resolution;

	this->pauseMenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pauseMenu->addButton(gui::ButtonName::Resume, gui::percentIntoY(38.f, vm), gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm), gui::calculateCharSize(3.f, vm), "Resume");
//	this->pauseMenu->addButton(gui::ButtonName::Settings, gui::percentIntoY(46.f, vm), gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm), gui::calculateCharSize(3.f, vm), "Settings");
	this->pauseMenu->addButton(gui::ButtonName::Quit, gui::percentIntoY(54.f, vm), gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm), gui::calculateCharSize(3.f, vm), "Quit");
}

void GameState::initShaders()
{
	if (!this->coreShader.loadFromFile("vertex_shader.vert", "fragment_shader.frag"))
	{
		std::cout << "ERROR::GameState::initShaders::Failed to load shaders" << std::endl;
	}
}

void GameState::initFigures()
{
	// Team White

	for (uint32_t i = 0; i < this->figures; i++)
	{
		this->pawn[Team::White][i] = new Pawn(this->teamTile[Team::White][i]->getPosition().x, this->teamTile[Team::White][i]->getPosition().y, this->figureTextures[Figure::PawnWhite]);
	}

	// Team Black
	for (uint32_t i = 0; i < this->figures; i++)
	{
		this->pawn[Team::Black][i] = new Pawn(this->teamTile[Team::Black][i]->getPosition().x, this->teamTile[Team::Black][i]->getPosition().y, this->figureTextures[Figure::PawnBlack], false);
	}

	// First selected pawn White
	this->selectedPawn[Team::White] = this->pawn[0][0];
	this->selectedPawn[Team::White]->select();
	this->chosenPosition[Team::White] = this->selectedPawn[Team::White]->getCenter();

	this->lastSelectedIndex[Team::White] = 0;
}

void GameState::initTileMap()
{
	this->tileMap = new TileMap("Maps/test.tilemap");

	// White
	this->teamTile[Team::White][0] = this->tileMap->getTile(5, 5);
	this->teamTile[Team::White][1] = this->tileMap->getTile(6, 5);
	this->teamTile[Team::White][2] = this->tileMap->getTile(7, 5);

	this->teamTile[Team::White][3] = this->tileMap->getTile(5, 6);
	this->teamTile[Team::White][4] = this->tileMap->getTile(6, 6);
	this->teamTile[Team::White][5] = this->tileMap->getTile(7, 6);

	this->teamTile[Team::White][6] = this->tileMap->getTile(5, 7);
	this->teamTile[Team::White][7] = this->tileMap->getTile(6, 7);
	this->teamTile[Team::White][8] = this->tileMap->getTile(7, 7);

	// Setting captured visualisation
	for (uint32_t i = 0; i < this->tiles; i++)
	{
		this->teamTile[Team::White][i]->setShowCaptured();
	}

	// Black
	this->teamTile[Team::Black][0] = this->tileMap->getTile(0, 0);
	this->teamTile[Team::Black][1] = this->tileMap->getTile(1, 0);
	this->teamTile[Team::Black][2] = this->tileMap->getTile(2, 0);

	this->teamTile[Team::Black][3] = this->tileMap->getTile(0, 1);
	this->teamTile[Team::Black][4] = this->tileMap->getTile(1, 1);
	this->teamTile[Team::Black][5] = this->tileMap->getTile(2, 1);

	this->teamTile[Team::Black][6] = this->tileMap->getTile(0, 2);
	this->teamTile[Team::Black][7] = this->tileMap->getTile(1, 2);
	this->teamTile[Team::Black][8] = this->tileMap->getTile(2, 2);

	// Setting captured visualisation
	for (uint32_t i = 0; i < this->tiles; i++)
	{
		this->teamTile[Team::Black][i]->setShowCaptured();
	}
}

void GameState::initGui()
{
	sf::VideoMode vm = this->stateData->gfxSettings->resolution;

	// Win
	this->textWin.setPosition
	(
		static_cast<float>(vm.width / 2),
		static_cast<float>(vm.height / 2)
	);

	// Lose
	this->textLose.setPosition
	(
		static_cast<float>(vm.width / 2),
		static_cast<float>(vm.height / 2)
	);
}

void GameState::initSound()
{
	this->soundEngine->stop();
	this->soundEngine->initMusic(sfx::Music::Tension, "Resources/Music/Tension.mp3");
	this->soundEngine->playMusic(sfx::Music::Tension);
}


// Constructors and Destructor:

GameState::GameState(StateData* state_data, gui::FadeScreen* fade_screen, sfx::SoundEngine* sound_engine)
	: State(state_data, fade_screen, sound_engine), selector(state_data->gridSize), 
	textWin(this->font, sf::Color::Yellow, "YOU WIN!"), textLose(this->font, sf::Color::Red, "YOU LOSE!")
{
	this->initVariables();

	this->initDefferedRender();	
	this->initKeybinds("Config/gamestate_keybinds.ini");
	this->initFont(this->font, "Fonts/slkscr.ttf");
	this->initTexture(Figure::PawnWhite, "Resources/Images/Sprites/Figures/Pawn_cyan.png");
	this->initTexture(Figure::PawnBlack, "Resources/Images/Sprites/Figures/Pawn_red.png");
	this->initPauseMenu();
	this->initShaders();

	this->initTileMap();
	this->initView();
	this->initGui();
	this->initFigures();

	// VFX
	this->fadeScreen->setColor(sf::Color::Black);
	this->fadeScreen->fadeIn();

	// Music
	this->initSound();
}

GameState::~GameState()
{
	// Deleting pause menu
	delete this->pauseMenu;

	// Deleting tile map
	delete this->tileMap;

	// Deleting figures
	for (uint32_t i = 0; i < this->teams; i++)
	{
		for (uint32_t j = 0; j < this->figures; j++)
		{
			delete this->pawn[i][j];
		}
	}
}


//Functions:

void GameState::switchTurn()
{
	if (this->turnLast == Team::White)
	{
		this->turn = Team::Black;
	}
	else if (this->turnLast == Team::Black)
	{
		this->turn = Team::White;
	}
}

void GameState::endTurn()
{
	this->turnLast = this->turn;
	this->turn = Team::None;
	this->turnTimer.restart();
}

void GameState::updateView(const float& dt)
{
	this->view.setCenter
	(
		std::floor(this->tileMap->getMaxSizeF().x / 2.f),
		std::floor(this->tileMap->getMaxSizeF().y / 2.f)
	);

	//Check for X
	if (this->view.getSize().x <= this->tileMap->getMaxSizeF().x)
	{
		if (this->view.getCenter().x - (this->view.getSize().x / 2.f) < 0.f)
		{
			this->view.setCenter(0.f + (this->view.getSize().x / 2.f), this->view.getCenter().y);
		}
		else if (this->view.getCenter().x + (this->view.getSize().x / 2.f) > this->tileMap->getMaxSizeF().x)
		{
			this->view.setCenter(this->tileMap->getMaxSizeF().x - (this->view.getSize().x / 2.f), this->view.getCenter().y);
		}
	}

	//Check for Y
	if (this->view.getSize().y <= this->tileMap->getMaxSizeF().y)
	{
		if (this->view.getCenter().y - (this->view.getSize().y / 2.f) < 0.f)
		{
			this->view.setCenter(this->view.getCenter().x, 0.f + (this->view.getSize().y / 2.f));
		}
		else if (this->view.getCenter().y + (this->view.getSize().y / 2.f) > this->tileMap->getMaxSizeF().y)
		{
			this->view.setCenter(this->view.getCenter().x, this->tileMap->getMaxSizeF().y - (this->view.getSize().y / 2.f));
		}
	}

	this->viewGridPosition.x = static_cast<int>(this->view.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPosition.y = static_cast<int>(this->view.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime())
	{
		if (!this->paused)
		{
			this->pauseState();
		}
		else
		{
			this->unpauseState();
		}
	}
}

void GameState::updatePlayerInput(const float& dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (this->buttonPressed == false)
		{
			this->buttonPressed = true;
			
			bool found = false;

			for (uint32_t i = 0; i < this->figures; i++)
			{
				if (pawn[Team::White][i]->checkSelection(this->mousePositionView))
				{
					this->selectedPawn[Team::White] = this->pawn[Team::White][i];
					this->selectedPawn[Team::White]->select();

					this->chosenPosition[Team::White] = this->selectedPawn[Team::White]->getCenter();

					this->lastSelectedIndex[Team::White] = i;

					found = true;
				}
				else
				{
					this->pawn[Team::White][i]->unselect();
				}
			}

			if (found == false)
			{
				this->selectedPawn[Team::White] = this->pawn[Team::White][this->lastSelectedIndex[Team::White]];
				this->selectedPawn[Team::White]->select();
				this->chosenPosition[Team::White] = this->selectedPawn[Team::White]->getCenter();
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		if (this->buttonPressed == false)
		{
			this->buttonPressed = true;

			for (auto& position : this->allowedPositions[Team::White])
			{
				if (this->selector.getGlobalBounds().contains(position))
				{
					this->chosenPosition[Team::White] = this->selector.getCenter();

					this->endTurn();

					break;
				}
			}
		}
	}
	else
	{
		this->buttonPressed = false;
	}
}

void GameState::updateInputAI(const float& dt)
{
	while(true)
	{
		uint32_t index = this->randomizer.generate(0, this->figures - 1);

		this->selectedPawn[Team::Black] = this->pawn[Team::Black][index];
		this->selectedPawn[Team::Black]->select();

		this->updateAllowedMovementAI(dt);

		this->chosenPosition[Team::Black] = this->selectedPawn[Team::Black]->getCenter();

		this->lastSelectedIndex[Team::Black] = index; // Maybe pointless

		// Updating tiles availability
		for (int32_t x = 0; x < this->tileMap->getMaxSizeGrid().x; x++)
		{
			for (int32_t y = 0; y < this->tileMap->getMaxSizeGrid().y; y++)
			{
				bool broken = false;

				if (this->tileMap->getTile(x, y)->isAvailable())
				{
					this->allowedPositions[Team::Black].push_back(this->tileMap->getTile(x, y)->getCenter());
				}
			}
		}

		// Chosing position
		if (!this->allowedPositions[Team::Black].empty())
		{
			uint32_t index_x = 0;
			uint32_t index_y = 0;

			for (size_t i = 0; i < this->allowedPositions[Team::Black].size(); i++)
			{
				if (this->allowedPositions[Team::Black][i].x > this->allowedPositions[Team::Black][0].x)
				{
					index_x = i;
				}

				if (this->allowedPositions[Team::Black][i].y > this->allowedPositions[Team::Black][0].y)
				{
					index_y = i;
				}
			}

			int32_t chance = this->randomizer.generate(0, 100);

			if (chance >= 0 && chance < 45)
			{
				this->chosenPosition[Team::Black] = this->allowedPositions[Team::Black][index_x];
			}
			else if (chance >= 45 && chance < 90)
			{
				this->chosenPosition[Team::Black] = this->allowedPositions[Team::Black][index_y];
			}
			else
			{
				this->chosenPosition[Team::Black] = this->allowedPositions[Team::Black][this->randomizer.generate(0, this->allowedPositions[Team::Black].size() - 1)];
			}

			this->allowedPositions[Team::Black].clear();

			this->endTurn();

			break;
		}
	}
}

void GameState::updateTimerTurn()
{
	if (this->turn == Team::None)
	{
		if (this->turnTimer.getElapsedTime().asSeconds() >= this->turnTimerMax)
		{
			this->switchTurn();
		}
	}
}

void GameState::updatePauseMenuButtons(const float& dt)
{
	if (this->pauseMenu->isButtonPressed(gui::ButtonName::Resume))
	{
		this->paused = false;
	}
//	else if (this->pauseMenu->isButtonPressed(gui::ButtonName::Settings))
//	{
//		//
//	}
	else if (this->pauseMenu->isButtonPressed(gui::ButtonName::Quit))
	{
		this->gameOver = true;
	}
}

void GameState::uodateTileMap(const float& dt)
{
	for (uint32_t i = 0; i < this->teams; i++)
	{
		for (uint32_t j = 0; j < this->figures; j++)
		{
			this->tileMap->updateWorldBoundsCollision(this->pawn[i][j], dt);
		}
	}

	this->tileMap->updateTiles(this->selectedPawn[Team::White], dt);

	// Capturing White
	for (uint32_t i = 0; i < this->tiles; i++)
	{
		uint16_t counter = 0;

		for (uint32_t j = 0; j < this->figures; j++)
		{
			if (this->teamTile[Team::White][i]->getGlobalBounds().contains(this->pawn[Team::Black][j]->getCenter()))
			{
				++counter;
			}
		}

		if (counter > 0)
		{
			this->teamTile[Team::White][i]->setCaptured();
		}
		else
		{
			this->teamTile[Team::White][i]->setCaptured(false);
		}
	}

	// Capturing Black
	for (uint32_t i = 0; i < this->tiles; i++)
	{
		uint16_t counter = 0;

		for (uint32_t j = 0; j < this->figures; j++)
		{
			if (this->teamTile[Team::Black][i]->getGlobalBounds().contains(this->pawn[Team::White][j]->getCenter()))
			{
				++counter;
			}
		}

		if (counter > 0)
		{
			this->teamTile[Team::Black][i]->setCaptured();
		}
		else
		{
			this->teamTile[Team::Black][i]->setCaptured(false);
		}
	}
}

void GameState::updateAllowedMovement(const float& dt)
{
	// Updating tiles availability
	for (int32_t x = 0; x < this->tileMap->getMaxSizeGrid().x; x++)
	{
		for (int32_t y = 0; y < this->tileMap->getMaxSizeGrid().y; y++)
		{
			uint16_t counter = 0;

			for (uint32_t i = 0; i < this->teams; i++)
			{
				for (uint32_t j = 0; j < this->figures; j++)
				{
					if (this->tileMap->getTile(x, y)->getGlobalBounds().contains(this->pawn[i][j]->getCenter()))
					{
						++counter;
					}
				}
			}

			this->tileMap->getTile(x, y)->setShowAvailable();

			// White
			if (counter > 0 || (this->tileMap->getTile(x, y)->getDistance(this->selectedPawn[Team::White]) > this->gridSize))
			{
				this->tileMap->getTile(x, y)->setAvailable(false);
			}
			else
			{
				this->tileMap->getTile(x, y)->setAvailable();
				this->allowedPositions[Team::White].insert(this->allowedPositions[Team::White].begin(), this->tileMap->getTile(x, y)->getCenter());
			}
		}
	}

	// Resizing allowed positions vectors to maximum possible size
	this->allowedPositions[Team::White].resize(this->positions);
}

void GameState::updateAllowedMovementAI(const float& dt)
{
	// Updating tiles availability
	for (int32_t x = 0; x < this->tileMap->getMaxSizeGrid().x; x++)
	{
		for (int32_t y = 0; y < this->tileMap->getMaxSizeGrid().y; y++)
		{
			bool unavailable = false;
		
			for (uint32_t i = 0; i < this->teams; i++)
			{
				bool broken = false;

				for (uint32_t j = 0; j < this->figures; j++)
				{
					if (this->tileMap->getTile(x, y)->getGlobalBounds().contains(this->pawn[i][j]->getCenter()))
					{
						this->tileMap->getTile(x, y)->setAvailable(false);
						unavailable = true;
						broken = true;
						break;
					}
				}

				if (broken)
				{
					break;
				}
			}

			this->tileMap->getTile(x, y)->setShowAvailable();

			if (!unavailable && this->tileMap->getTile(x, y)->getDistance(this->selectedPawn[Team::Black]) <= this->gridSize)
			{
				this->tileMap->getTile(x, y)->setAvailable();
			}
			else
			{
				this->tileMap->getTile(x, y)->setAvailable(false);
			}
		}
	}
}

void GameState::updateGui(const float& dt)
{
	this->selector.update(this->mousePositionGrid, dt);

	for (auto& position : this->allowedPositions[Team::White])
	{
		if (this->selector.getGlobalBounds().contains(position))
		{
			this->selector.enable();
			break;
		}
		else
		{
			this->selector.disable();
		}
	}
}

void GameState::updateMovement(const float& dt)
{
	if (this->selectedPawn[Team::White]->getDistance(this->chosenPosition[Team::White]) < this->gridSize / 20.f)
	{
		this->selectedPawn[Team::White]->setPosition(this->chosenPosition[Team::White].x, this->chosenPosition[Team::White].y, true);
		this->chosenPosition[Team::White] = this->selectedPawn[Team::White]->getCenter();;
	}
	else
	{
		this->direction[Team::White] = this->chosenPosition[Team::White] - this->selectedPawn[Team::White]->getCenter();
		this->directionNormalized[Team::White] = this->direction[Team::White] / static_cast<float>(std::sqrt(std::pow(this->direction[Team::White].x, 2) + std::pow(this->direction[Team::White].y, 2)));

		this->selectedPawn[Team::White]->move(this->directionNormalized[Team::White].x, this->directionNormalized[Team::White].y, dt);
	}
}

void GameState::updateMovementAI(const float& dt)
{
	if (this->selectedPawn[Team::Black])
	{
		if (this->selectedPawn[Team::Black]->getDistance(this->chosenPosition[Team::Black]) < this->gridSize / 20.f)
		{
			this->selectedPawn[Team::Black]->setPosition(this->chosenPosition[Team::Black].x, this->chosenPosition[Team::Black].y, true);
			this->chosenPosition[Team::Black] = this->selectedPawn[Team::Black]->getCenter();;
		}
		else
		{
			this->direction[Team::Black] = this->chosenPosition[Team::Black] - this->selectedPawn[Team::Black]->getCenter();
			this->directionNormalized[Team::Black] = this->direction[Team::Black] / static_cast<float>(std::sqrt(std::pow(this->direction[Team::Black].x, 2) + std::pow(this->direction[Team::Black].y, 2)));

			this->selectedPawn[Team::Black]->move(this->directionNormalized[Team::Black].x, this->directionNormalized[Team::Black].y, dt);
		}
	}
}

void GameState::updateFigures(const float& dt)
{
	for (uint32_t i = 0; i < this->teams; i++)
	{
		for (uint32_t j = 0; j < this->figures; j++)
		{
			pawn[i][j]->update(this->mousePositionView, dt);
		}
	}
}

void GameState::updateGameOver(const float& dt)
{	
	uint32_t counter[this->teams];
	counter[Team::White] = 0;
	counter[Team::Black] = 0;

	// White
	for (uint32_t i = 0; i < this->tiles; i++)
	{
		if (this->teamTile[Team::White][i]->isCaptured())
		{
			counter[Team::White]++;
		}		
	}

	// Black
	for (uint32_t i = 0; i < this->tiles; i++)
	{
		if (this->teamTile[Team::Black][i]->isCaptured())
		{
			counter[Team::Black]++;
		}		
	}

	// Check Black team won
	if (counter[Team::White] >= this->figures)
	{
		this->wonTeam = Team::Black;
		this->textLose.show();
		this->gameOver = true;
	}

	// Check White team won
	if (counter[Team::Black] >= this->figures)
	{
		this->wonTeam = Team::White;
		this->textWin.show();
		this->gameOver = true;
	}
}

void GameState::update(const float& dt)
{
	this->fadeScreen->update(dt);
	if (!this->fadeScreen->isVisible())
	{
		this->updateMousePositions(&this->view);
		this->updateKeyTime(dt);
		this->updateInput(dt);

		// Unpaused update
		if (!this->paused)
		{
			this->updateView(dt);
			this->updateGui(dt);

			// Player
			this->updateAllowedMovement(dt);

			if (this->turn == Team::White)
			{
				// Player
				this->updatePlayerInput(dt);
			}
			else if (this->turn == Team::Black)
			{
				// AI
				this->updateInputAI(dt);
			}

			this->updateTimerTurn();
			this->uodateTileMap(dt);

			// Pawns
			this->updateFigures(dt);
			this->updateMovement(dt);
			this->updateMovementAI(dt);
		}
		else // Paused update
		{
			this->pauseMenu->update(this->mousePositionWindow, dt);
			this->updatePauseMenuButtons(dt);
		}
	}

	this->updateGameOver(dt);

	if (this->gameOver)
	{
		this->fadeScreen->fadeOut();
		this->soundEngine->fadeOut(dt);

		if (this->fadeScreen->isOpaque())
		{
			this->fadeScreen->fadeIn();
			this->soundEngine->stop();
			this->soundEngine->playMusic(sfx::Music::Menu);

			if (this->exitTimer.getElapsedTime().asSeconds() >= this->exitTimerMax)
			{
				this->endState();
			}			
		}
	}
	else
	{
		this->exitTimer.restart();
		this->soundEngine->fadeIn(dt);
	}

	// Debug:
	this->updateFpsCounter(dt);
	
	// Music and SFX
	this->updateSound(dt);
}

void GameState::renderFigures()
{
	for (uint32_t i = 0; i < this->teams; i++)
	{
		for (uint32_t j = 0; j < this->figures; j++)
		{
			pawn[i][j]->render(&this->renderTexture, &this->coreShader, pawn[i][j]->getCenter());
		}
	}
}

void GameState::renderGui(sf::RenderTarget* target)
{
	if (this->wonTeam == Team::White)
	{
		this->textWin.render(target);
	}
	else if (this->wonTeam == Team::Black)
	{
		this->textLose.render(target);
	}	
}

void GameState::render(sf::RenderTarget* target)
{
	// All to the window
	if (!target)
	{
		target = this->window;
	}

	this->renderTexture.clear();

	// Map
	this->renderTexture.setView(this->view);

	this->tileMap->render(&this->renderTexture, this->viewGridPosition, &this->coreShader, this->selectedPawn[Team::White]->getCenter());

	if (this->selectedPawn[Team::White]->isSelected())
	{		
		this->selector.render(&this->renderTexture);
	}

	this->renderFigures();

	// Deffered rendering
	this->tileMap->renderDeffered(&this->renderTexture, &this->coreShader, this->selectedPawn[Team::White]->getCenter());

	// Player GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());

	// Pause menu render
	if (this->paused) 
	{
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pauseMenu->render(&this->renderTexture);
	}

	// Fade screen
	this->fadeScreen->render(&this->renderTexture);

	// Game over
	this->renderGui(&this->renderTexture);

	// Debug:
	this->renderFpsCounter(&this->renderTexture);

	// Final render
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}