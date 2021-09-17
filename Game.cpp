#include "stdafx.h"
#include "Game.h"

// private: Functions:

void Game::initVariables()
{
	this->window = nullptr;

	this->dt = 0.f;

	this->gridSize = 64.f;
}

void Game::initGraphicsSettings(const std::string file_path)
{
	this->gfxSettings.loadFromFile(file_path);

}

void Game::initWindow()
{
	this->window = new sf::RenderWindow
	(
		this->gfxSettings.resolution,
		this->gfxSettings.title,
		this->gfxSettings.style,
		this->gfxSettings.contextSettings
	);
	this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
	this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
	//this->window->setMouseCursorGrabbed(true);
}

void Game::initKeys(const std::string file_path)
{
	std::ifstream in;

	in.open(file_path);

	if (in.fail())
	{
		std::cout << "ERROR::Game::initKeys::Failed to load: " << file_path << std::endl;
	}
	else if (in.is_open())
	{
		std::string key = "";
		int key_value = 0;

		while (in >> key >> key_value)
		{
			this->supportedKeys[key] = key_value;
		}
	}

	in.close();

	//Debug
	std::cout << "Game::initKeys::Supported keys:" << std::endl;
	for (auto i : this->supportedKeys)
	{
		std::cout << i.first << " " << i.second << std::endl;
	}
}

void Game::initStateData()
{
	this->stateData.window = this->window;
	this->stateData.gfxSettings = &this->gfxSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.gridSize = this->gridSize;
}

void Game::initStates()
{
	//Main menu
	this->states.push(new MainMenuState(&this->stateData));
}

Game::Game()
{
	this->initVariables();
	this->initGraphicsSettings("Config/graphics.ini");
	this->initWindow();
	this->initKeys("Config/supported_keys.ini");
	this->initStateData();
	this->initStates();
}

Game::~Game()
{
	//Deleting SFML window
	delete this->window;

	while (this->states.empty() == false)
	{
		delete this->states.top();
		this->states.pop();
	}
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updateSfEvents();

		this->updateDt();

		if (this->window->hasFocus())
		{
			this->update();
		}

		this->render();
	}
}

void Game::endApplication()
{
	std::cout << "Game::endApplication::Ending Application!" << std::endl;
}

void Game::updateDt()
{
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateSfEvents()
{
	while (this->window->pollEvent(this->sfEvent))
	{
		if (this->sfEvent.type == sf::Event::Closed)
		{
			this->window->close();
		}
	}
}

void Game::update()
{
	// Updating states
	if (this->states.empty() == false)
	{
		this->states.top()->update(this->dt);

		if (this->states.top()->getQuit() == true)
		{
			this->states.top()->endState();
			delete this->states.top();
			this->states.pop();
		}
	}
	else
	{
		// Application ending
		this->endApplication();
		this->window->close();
	}
}

void Game::render()
{
	// Clearing frame with black color
	this->window->clear(sf::Color::Black);

	// Rendering states
	if (!this->states.empty())
	{
		this->states.top()->render(this->window);
	}

	// Displaying frame
	this->window->display();
}