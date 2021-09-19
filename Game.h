#pragma once

#include "MainMenuState.h"
#include "GameState.h"

class Game
{
private:

// Variables:

	GraphicsSettings gfxSettings;
	StateData stateData;
	gui::FadeScreen fadeScreen;
	sfx::SoundEngine soundEngine;
	sf::RenderWindow* window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	std::map<const std::string, int32_t> supportedKeys;

	float gridSize;

// private: Functions:

	void initVariables();
	void initGraphicsSettings(const std::string file_path);
	void initWindow();
	void initKeys(const std::string file_path);
	void initStateData();
	void initFadeScreen();
	void initStates();

public:

// Constructors and Destructor:

	Game();
	~Game();

// Functions:

	void run();

	void endApplication();

	void updateDt();
	void updateSfEvents();
	void update();

	void render();
};

