#include "stdafx.h"
#include "MainMenuState.h"

//private: Functions:

void MainMenuState::initVariables()
{
	this->shutDown = false;

	this->start = false;
	this->fadeTimerMax = 1.f;
}

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

	this->container.setFillColor(sf::Color(0, 120, 160, 80));
	this->container.setSize
	(
		sf::Vector2f
		(
			static_cast<float>(vm.width) / 5.f,
			static_cast<float>(vm.height) / 5.f
		)
	);
	this->container.setPosition
	(
		(static_cast<float>(vm.width) / 2.f) - this->container.getSize().x / 2.f,
		gui::percentIntoY(66.2f, vm)
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
		gui::percentIntoX(43.8f, vm), gui::percentIntoY(70.f, vm),
		gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm),
		&this->font, "Start", gui::calculateCharSize(3.f, vm),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
		sf::Color(204, 204, 204), sf::Color(255, 235, 0), sf::Color(255, 100, 0)
	);

//	this->buttons["SETTINGS_STATE"] = new gui::Button
//	(
//		gui::percentIntoX(32.f, vm), gui::percentIntoY(73.f, vm),
//		gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm),
//		&this->font, "Settings", gui::calculateCharSize(3.f, vm),
//		sf::Color::Red, sf::Color::Transparent, sf::Color::Transparent,
//		sf::Color(204, 204, 204), sf::Color(255, 235, 0), sf::Color(255, 100, 0)
//	);

	this->buttons["EXIT_STATE"] = new gui::Button
	(
		gui::percentIntoX(43.8f, vm), gui::percentIntoY(78.f, vm),
		gui::percentIntoX(12.f, vm), gui::percentIntoY(4.f, vm),
		&this->font, "Exit", gui::calculateCharSize(3.f, vm),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
		sf::Color(204, 204, 204), sf::Color(255, 235, 0), sf::Color(255, 100, 0)
	);
}

void MainMenuState::initSound()
{
	// SFX
	this->soundEngine->initSound(sfx::Sound::Button_Positive, "Resources/Sounds/Button_Positive.mp3");
	this->soundEngine->initSound(sfx::Sound::Button_Negative, "Resources/Sounds/Button_Negative.mp3");
	this->soundEngine->initSound(sfx::Sound::Button_Hover, "Resources/Sounds/Button_Hover.mp3");

	// Music
	this->soundEngine->initMusic(sfx::Music::Menu, "Resources/Music/Menu.mp3");
	this->soundEngine->playMusic(sfx::Music::Menu);
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

MainMenuState::MainMenuState(StateData* state_data, gui::FadeScreen* fade_screen, sfx::SoundEngine* sound_engine)
	: State(state_data, fade_screen, sound_engine)
{
	this->initVariables();
	this->initFont(this->font, "Fonts/slkscr.ttf");
	this->initKeybinds("Config/mainmenustate_keybinds.ini");
	this->initGui();

	// VFX
	this->fadeScreen->setColor(sf::Color::Black);
	this->fadeScreen->fadeIn();

	// Music
	this->initSound();
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
		this->soundEngine->playSound(sfx::Sound::Button_Positive);
		this->start = true;
		this->fadeTimer.restart();
	}

	// Settings
//	if (this->buttons["SETTINGS_STATE"]->isPressed() && this->getKeyTime())
//	{
//		// TO DO
//	}

	// Quit the game
	if (this->buttons["EXIT_STATE"]->isPressed() && this->getKeyTime())
	{
		this->soundEngine->playSound(sfx::Sound::Button_Negative);
		this->fadeScreen->fadeOut();
		this->shutDown = true;
	}
}

void MainMenuState::update(const float& dt)
{
	this->fadeScreen->update(dt);
	if (!this->fadeScreen->isVisible())
	{
		this->updateMousePositions();
		this->updateKeyTime(dt);
		this->updateInput(dt);
		this->updateButtons(dt);
	}

	if (this->start)
	{
		this->fadeScreen->fadeOut();
		this->soundEngine->fadeOut(dt);

		if (this->fadeTimer.getElapsedTime().asSeconds() >= this->fadeTimerMax)
		{
			this->start = false;
			this->states->push(new GameState(this->stateData, this->fadeScreen, this->soundEngine));
		}
	}

	if (this->shutDown)
	{
		this->soundEngine->fadeOut(dt);

		if (this->fadeScreen->isOpaque())
		{
			this->endState();
		}
	}
	
	if (!this->start && !this->shutDown)
	{
		this->soundEngine->fadeIn(dt);
	}

	// Debug:
	this->updateFpsCounter(dt);

	// Music and SFX
	this->updateSound(dt);
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

	this->fadeScreen->render(target);

	// Debug:
	this->renderFpsCounter(target);
}