#include "stdafx.h"
#include "MainMenuState.h"

//private: Functions:

void MainMenuState::initVariables()
{
	this->shutDown = false;
}

//Should use '../' before the file path to be able to play the game without Visual Studio.
void MainMenuState::initBackground(const std::string file_path)
{
	sf::VideoMode vm = this->stateData->gfxSettings->resolution;

	//Background initialization
	this->background.setSize
	(
		sf::Vector2f
		(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);

	if (!this->backgroundTexture.loadFromFile(file_path))
	{
		std::cout << "ERROR::SettingsState::initBackground::Failed to load: " << file_path << std::endl;
		this->background.setFillColor(sf::Color(30, 60, 60, 200));
	}
	else
	{
		this->background.setTexture(&this->backgroundTexture);
	}

	//Container initialization
//	this->container.setSize(sf::Vector2f(gui::percentIntoX(22.f, vm), gui::percentIntoY(56.f, vm)));
//	this->container.setFillColor(sf::Color(0, 40, 80, 80));
//	this->container.setPosition(sf::Vector2f(gui::percentIntoX(8.f, vm), gui::percentIntoY(12.f, vm)));

	this->container.setPosition(0.f, gui::percentIntoY(70.f, vm));
	this->container.setFillColor(sf::Color(0, 40, 80, 80));
	this->container.setSize
	(
		sf::Vector2f
		(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height) / 9.f
		)
	);
}

void MainMenuState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	// Background
	this->initBackground("Resources/Images/Backgrounds/Chess.png");

	// Buttons
	this->buttons["GAME_STATE"] = new gui::Button
	(
		gui::percentIntoX(12.f, vm), gui::percentIntoY(73.f, vm),
		gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm),
		&this->font, "Start", gui::calculateCharSize(3.f, vm),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
		sf::Color(204, 204, 204), sf::Color(255, 235, 0), sf::Color(255, 100, 0)
	);

	this->buttons["SETTINGS_STATE"] = new gui::Button
	(
		gui::percentIntoX(32.f, vm), gui::percentIntoY(73.f, vm),
		gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm),
		&this->font, "Settings", gui::calculateCharSize(3.f, vm),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
		sf::Color(204, 204, 204), sf::Color(255, 235, 0), sf::Color(255, 100, 0)
	);

	this->buttons["EXIT_STATE"] = new gui::Button
	(
		gui::percentIntoX(72.f, vm), gui::percentIntoY(73.f, vm),
		gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm),
		&this->font, "Exit", gui::calculateCharSize(3.f, vm),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
		sf::Color(204, 204, 204), sf::Color(255, 235, 0), sf::Color(255, 100, 0)
	);
}

//Clears the GUI elements and reinitializes the GUI.
//@return void
void MainMenuState::resetGui()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->buttons.clear();

	this->initGui();
}

//Constructors and Destructor:

MainMenuState::MainMenuState(StateData* state_data)
	: State(state_data), fadeScreen(state_data, sf::Color::Black)
{
	this->initVariables();
	this->initFont(this->font, "Fonts/slkscr.ttf");
	this->initKeybinds("Config/mainmenustate_keybinds.ini");
	this->initGui();

	this->fadeScreen.fadeIn();
}

MainMenuState::~MainMenuState()
{
	//Deleting buttons
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

//Functions:

void MainMenuState::updateInput(const float& dt)
{

}

// Updates all the buttons in the state and handles theis functionality.
void MainMenuState::updateButtons(const float& dt)
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePositionWindow, false, true);
		it.second->updateButtonColor(dt);
	}

	// New game
	if (this->buttons["GAME_STATE"]->isPressed() && this->getKeyTime())
	{
		this->states->push(new GameState(this->stateData));
	}

	// Settings
	if (this->buttons["SETTINGS_STATE"]->isPressed() && this->getKeyTime())
	{
		// TO DO
	}

	// Quit the game
	if (this->buttons["EXIT_STATE"]->isPressed() && this->getKeyTime())
	{
		this->fadeScreen.fadeOut();
		this->shutDown = true;
	}
}

void MainMenuState::update(const float& dt)
{
	this->fadeScreen.update(dt);
	if (!this->fadeScreen.isVisible())
	{
		this->updateMousePositions();
		this->updateKeyTime(dt);
		this->updateInput(dt);
		this->updateButtons(dt);
	}

	if (this->shutDown)
	{
		if (this->fadeScreen.isOpaque())
		{
			this->endState();
		}
	}

	// Debug:
	this->updateFpsCounter(dt);
	this->updateMousePositionText();
}

void MainMenuState::renderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}

	target->draw(this->background);
	target->draw(this->container);

	this->renderButtons(target);

	this->fadeScreen.render(target);

	// Debug:
	this->renderFpsCounter(target);
	this->renderMousePositionText(target);
}