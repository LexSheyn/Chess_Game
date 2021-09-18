#include "stdafx.h"
#include "GameState.h"

// private: Functions:

void GameState::initVariables()
{
	this->buttonPressed = false;
	this->gameOver = false;

	this->selectedPawn = nullptr;
	this->lastSelectedIndex = 0;

	// Allowed positions
	this->allowedPositions.resize(this->positions);
//	this->allowedPositionsTest[Team::Black].resize(this->positions);

	this->direction = sf::Vector2f();
	this->directionNormalized = sf::Vector2f();

	// Turn
	this->turn = Team::White;
	this->turnLast = this->turn;
	this->timer.restart();
	this->timerMax = 1.f;
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
	this->selectedPawn = this->pawn[0][0];
	this->selectedPawn->select();
	this->chosenPosition = this->selectedPawn->getCenter();

	this->lastSelectedIndex = 0;

	// First selected pawn Black
//	this->selectedPawnTest[Team::Black] = this->pawn[Team::Black][8];
//	this->selectedPawnTest[Team::Black]->select();
//	this->chosenPositionTest[Team::Black] = this->selectedPawnTest[Team::Black]->getCenter();
//
//	this->lastSelectedIndexTest[Team::Black] = 0;
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
	
}


// Constructors and Destructor:

GameState::GameState(StateData* state_data)
	: State(state_data), fadeScreen(state_data, sf::Color::Black), selector(state_data)
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

	this->fadeScreen.fadeIn();
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
	this->turn = 2;
	this->timer.restart();
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
	this->updateAllowedMovement(dt);

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
					this->selectedPawn = this->pawn[Team::White][i];
					this->selectedPawn->select();

					this->chosenPosition = this->selectedPawn->getCenter();

					this->lastSelectedIndex = i;

					found = true;
				}
				else
				{
					this->pawn[Team::White][i]->unselect();
				}
			}

			if (found == false)
			{
				this->selectedPawn = this->pawn[Team::White][lastSelectedIndex];
				this->selectedPawn->select();
				this->chosenPosition = this->selectedPawn->getCenter();
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		if (this->buttonPressed == false)
		{
			this->buttonPressed = true;

			for (auto& position : this->allowedPositions)
			{
				if (this->selector.getGlobalBounds().contains(position))
				{
					this->chosenPosition = this->selector.getCenter();

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

		this->selectedPawnTest[Team::Black] = this->pawn[Team::Black][index];
		this->selectedPawnTest[Team::Black]->select();

		this->updateAllowedMovementAI(dt);

		this->chosenPositionTest[Team::Black] = this->selectedPawnTest[Team::Black]->getCenter();

		this->lastSelectedIndexTest[Team::Black] = index; // Maybe pointless

		// Updating tiles availability
		for (int32_t x = 0; x < this->tileMap->getMaxSizeGrid().x; x++)
		{
			for (int32_t y = 0; y < this->tileMap->getMaxSizeGrid().y; y++)
			{
				bool broken = false;

				if (this->tileMap->getTile(x, y)->isAvailable())
				{
					this->allowedPositionsTest[Team::Black].push_back(this->tileMap->getTile(x, y)->getCenter());
				}
			}
		}

		if (!this->allowedPositionsTest[Team::Black].empty())
		{
			this->chosenPositionTest[Team::Black] = this->allowedPositionsTest[Team::Black][this->randomizer.generate(0, this->allowedPositionsTest[Team::Black].size() - 1)];

			this->allowedPositionsTest[Team::Black].clear();

			this->endTurn();

			break;
		}
	}
}

void GameState::updatePauseMenuButtons()
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
		this->fadeScreen.fadeOut();
		this->gameOver = true;
	}
}

void GameState::updateTimer()
{
	if (this->turn == 2)
	{
		if (this->timer.getElapsedTime().asSeconds() >= this->timerMax)
		{
			this->switchTurn();
		}
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

	this->tileMap->updateTiles(this->selectedPawn, dt);

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
			if (counter > 0 || (this->tileMap->getTile(x, y)->getDistance(this->selectedPawn) > this->gridSize))
			{
				this->tileMap->getTile(x, y)->setAvailable(false);
			}
			else
			{
				this->tileMap->getTile(x, y)->setAvailable();
				this->allowedPositions.insert(this->allowedPositions.begin(), this->tileMap->getTile(x, y)->getCenter());
			}
		}
	}

	// Resizing allowed positions vectors to maximum possible size
	this->allowedPositions.resize(this->positions);
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

			if (!unavailable && this->tileMap->getTile(x, y)->getDistance(this->selectedPawnTest[Team::Black]) <= this->gridSize)
			{
				this->tileMap->getTile(x, y)->setAvailable();
			//	this->allowedPositionsTest[Team::Black].insert(this->allowedPositionsTest[Team::Black].begin(), this->tileMap->getTile(x, y)->getCenter());
			}
			else
			{
				this->tileMap->getTile(x, y)->setAvailable(false);
			}
		}
	}

	// Resizing allowed positions vectors to maximum possible size
//	this->allowedPositionsTest[Team::Black].resize(this->positions);
}

void GameState::updateGui(const float& dt)
{
	this->selector.update(this->mousePositionGrid, dt);

//	this->allowedPositions.resize(this->positions);

	for (auto& position : this->allowedPositions)
	{
		if (this->selector.getGlobalBounds().contains(position))
		{
			this->selector.setColor(sf::Color::Green);
			break;
		}
		else
		{
			this->selector.setColor(sf::Color::Red);
		}
	}
}

void GameState::updateMovement(const float& dt)
{
	if (this->selectedPawn->getDistance(this->chosenPosition) < this->gridSize / 20.f)
	{
		this->selectedPawn->setPosition(this->chosenPosition.x, this->chosenPosition.y, true);
		this->chosenPosition = this->selectedPawn->getCenter();;
	}
	else
	{
		this->direction = this->chosenPosition - this->selectedPawn->getCenter();
		this->directionNormalized = this->direction / static_cast<float>(std::sqrt(std::pow(this->direction.x, 2) + std::pow(this->direction.y, 2)));

		this->selectedPawn->move(this->directionNormalized.x, this->directionNormalized.y, dt);
	}
}

void GameState::updateMovementAI(const float& dt)
{
	if (this->selectedPawnTest[Team::Black])
	{
		if (this->selectedPawnTest[Team::Black]->getDistance(this->chosenPositionTest[Team::Black]) < this->gridSize / 20.f)
		{
			this->selectedPawnTest[Team::Black]->setPosition(this->chosenPositionTest[Team::Black].x, this->chosenPositionTest[Team::Black].y, true);
			this->chosenPositionTest[Team::Black] = this->selectedPawnTest[Team::Black]->getCenter();;

			// TEST
		//	this->turn = Team::White;
		}
		else
		{
			this->directionTest[Team::Black] = this->chosenPositionTest[Team::Black] - this->selectedPawnTest[Team::Black]->getCenter();
			this->directionNormalizedTest[Team::Black] = this->directionTest[Team::Black] / static_cast<float>(std::sqrt(std::pow(this->directionTest[Team::Black].x, 2) + std::pow(this->directionTest[Team::Black].y, 2)));

			this->selectedPawnTest[Team::Black]->move(this->directionNormalizedTest[Team::Black].x, this->directionNormalizedTest[Team::Black].y, dt);
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

void GameState::update(const float& dt)
{
	this->fadeScreen.update(dt);
	if (!this->fadeScreen.isVisible())
	{
		this->updateMousePositions(&this->view);
		this->updateKeyTime(dt);
		this->updateInput(dt);

		// Unpaused update
		if (!this->paused)
		{
			this->updateView(dt);
			this->updateGui(dt);

			if (this->turn == Team::White) // If White's turn
			{
				this->updatePlayerInput(dt);
			}
			else if (this->turn == Team::Black)
			{
				this->updateInputAI(dt);
			}

			this->updateTimer();
			this->uodateTileMap(dt);

			// Pawns
			this->updateFigures(dt);
			this->updateMovement(dt);
			this->updateMovementAI(dt);
		}
		else // Paused update
		{
			this->pauseMenu->update(this->mousePositionWindow, dt);
			this->updatePauseMenuButtons();
		}
	}

	if (this->gameOver)
	{
		if (this->fadeScreen.isOpaque())
		{
			this->endState();
		}
	}

	// Debug:
	this->updateFpsCounter(dt);
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

	this->tileMap->render(&this->renderTexture, this->viewGridPosition, &this->coreShader, this->selectedPawn->getCenter());

	if (this->selectedPawn->isSelected())
	{		
		this->selector.render(&this->renderTexture);
	}

	this->renderFigures();

	// Deffered rendering
	this->tileMap->renderDeffered(&this->renderTexture, &this->coreShader, this->selectedPawn->getCenter());

	// Player GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());

	// Pause menu render
	if (this->paused) 
	{
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pauseMenu->render(&this->renderTexture);
	}

	// Fade screen
	this->fadeScreen.render(&this->renderTexture);

	// Debug:
	this->renderFpsCounter(&this->renderTexture);

	// Final render
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}