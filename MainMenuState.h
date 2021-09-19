#pragma once

#include "GameState.h"

class MainMenuState :
	public State
{
private:

// private: Variables:

	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::RectangleShape container;
	sf::Font font;
	bool shutDown;

	std::map<std::string, gui::Button*> buttons;

	gui::FadeScreen fadeScreen;

// private: Functions:

	void initVariables();
	void initBackground(const std::string file_path);
	void initGui();
	void initSound();

	void resetGui();

public:

// Constructors and Destructor:

	MainMenuState(StateData* state_data);
	~MainMenuState();

// Functions:

	void updateInput(const float& dt);
	void updateButtons(const float& dt);
	void updateSound(const float& dt);
	void update(const float& dt);

	void renderButtons(sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
};

