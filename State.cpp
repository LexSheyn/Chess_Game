#include "stdafx.h"
#include "State.h"

// protected: Functions:

void State::initKeybinds(const std::string file_path)
{
	std::ifstream in;

	in.open(file_path);

	if (in.fail())
	{
		std::cout << "ERROR::EditorState::initKeybinds::Failed to load: " << file_path << std::endl;
	}
	else if (in.is_open())
	{
		std::string action = "";
		std::string key = "";

		while (in >> action >> key)
		{
			this->keybinds[action] = this->supportedKeys->at(key);
		}
	}

	in.close();
}

void State::initFont(sf::Font& font, const std::string file_path)
{
	if (!font.loadFromFile(file_path))
	{
		std::string className = typeid(*this).name();
		className.erase(0, 6);
		std::cout << "ERROR:: " << className << "::initFont::Failed to load : " << file_path << std::endl;
	}
}

void State::initTexture(const Figure name, const std::string file_path)
{
	if (!this->figureTextures[name].loadFromFile(file_path))
	{
		std::string className = typeid(*this).name();
		className.erase(0, 6);
		std::cout << "ERROR:: " << className << "::initTexture::Failed to load : " << file_path << std::endl;
	}
}

void State::initMousePositionText()
{
	//X
	this->mousePositionTextX.setFont(this->systemFont);
	this->mousePositionTextX.setCharacterSize(gui::calculateCharSize(3.3f, this->stateData->gfxSettings->resolution));

	//Y
	this->mousePositionTextY.setFont(this->systemFont);
	this->mousePositionTextY.setCharacterSize(gui::calculateCharSize(3.3f, this->stateData->gfxSettings->resolution));
}

void State::initFpsCounter()
{
	// FPS counter
	this->fpsCounterText.setFont(this->systemFont);
	this->fpsCounterText.setCharacterSize(gui::calculateCharSize(2.f, this->stateData->gfxSettings->resolution));
	this->fpsCounterText.setPosition
	(
		gui::percentIntoX(90.f, this->stateData->gfxSettings->resolution),
		gui::percentIntoY(2.f, this->stateData->gfxSettings->resolution)
	);
}


// Constructors and Destructor:

State::State(StateData* state_data, gui::FadeScreen* fade_screen, sfx::SoundEngine* sound_engine)
	: stateData(state_data), fadeScreen(fade_screen), soundEngine(sound_engine)
{
//	this->stateData = state_data;
	this->window = state_data->window;
	this->supportedKeys = state_data->supportedKeys;
	this->states = state_data->states;
	this->quit = false;
	this->paused = false;
	this->keyTime = 0.f;
	this->keyTimeMax = 30.f;
	this->gridSize = state_data->gridSize;

	this->initFont(this->systemFont, "Fonts/slkscr.ttf");

	this->initMousePositionText();
	this->initFpsCounter();
}

State::~State()
{

}


//Accessors:

const bool& State::getQuit() const
{
	return this->quit;
}

const bool State::getKeyTime()
{
	if (this->keyTime >= this->keyTimeMax)
	{
		this->keyTime = 0.f;
		return true;
	}

	return false;
}

const bool State::getKeyTime(const float max_keytime)
{
	if (this->keyTime >= max_keytime)
	{
		this->keyTime = 0.f;
		return true;
	}

	return false;
}


//Functions:

void State::endState()
{
	this->quit = true;
}

void State::pauseState()
{
	this->paused = true;
}

void State::unpauseState()
{
	this->paused = false;
}

void State::updateMousePositions(sf::View* view)
{
	this->mousePositionScreen = sf::Mouse::getPosition();
	this->mousePositionWindow = sf::Mouse::getPosition(*this->window);

	if (view)
	{
		this->window->setView(*view);
	}

	this->mousePositionView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
	this->mousePositionGrid = sf::Vector2i
	(
		static_cast<int>(this->mousePositionView.x / this->gridSize),
		static_cast<int>(this->mousePositionView.y / this->gridSize)
	);

	this->window->setView(this->window->getDefaultView());
}

void State::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 100.f * dt;
	}
}

void State::updateSound(const float& dt)
{
	this->soundEngine->update(dt);
}

void State::updateMousePositionText()
{
	// X
	this->mousePositionTextX.setPosition(this->mousePositionView.x + 20.f, this->mousePositionView.y - 50.f);
	std::stringstream ssX;
	ssX << "x: " << this->mousePositionView.x;
	this->mousePositionTextX.setString(ssX.str());

	// Y
	this->mousePositionTextY.setPosition(this->mousePositionView.x + 20.f, this->mousePositionView.y - 25.f);
	std::stringstream ssY;
	ssY << "y: " << this->mousePositionView.y;
	this->mousePositionTextY.setString(ssY.str());
}

void State::updateFpsCounter(const float& dt)
{
	this->fpsCounterText.setString("FPS: " + std::to_string(static_cast<int>(1.f / dt)));
}

void State::renderMousePositionText(sf::RenderTarget* target)
{
	target->draw(this->mousePositionTextX);
	target->draw(this->mousePositionTextY);
}

void State::renderFpsCounter(sf::RenderTarget* target)
{
	target->draw(this->fpsCounterText);
}