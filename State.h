#pragma once

#include "GraphicsSettings.h"
#include "Button.h"
#include "DropDownList.h"
#include "ProgressBar.h"
#include "SoundEngine.h"
#include "Entity.h"

class State;

class StateData
{
public:

// Variables:

	float gridSize;
	sf::RenderWindow* window;
	GraphicsSettings* gfxSettings;
	std::map<const std::string, int32_t>* supportedKeys;
	std::stack<State*>* states;
};

class State
{
protected:

// Variables:

	StateData* stateData;
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<const std::string, int32_t>* supportedKeys;
	std::map<const std::string, int32_t> keybinds;
	bool quit;
	bool paused;
	float keyTime;
	float keyTimeMax;
	float gridSize;

	sf::Vector2i mousePositionScreen;
	sf::Vector2i mousePositionWindow;
	sf::Vector2f mousePositionView;
	sf::Vector2i mousePositionGrid;

	sf::Font systemFont;

	sf::Text mousePositionTextX;
	sf::Text mousePositionTextY;

	sf::Text fpsCounterText;

// Resources:

	std::map<const Figure, sf::Texture> figureTextures;

// protected: Functions:

	void initKeybinds(const std::string file_path);
	void initFont(sf::Font& font, const std::string file_path);
	void initTexture(const Figure name, const std::string file_path);
	void initMousePositionText();
	void initFpsCounter();

public:

// Constructors and Destructor:

	State(StateData* state_data);
	virtual ~State();

// Accessors:

	const bool& getQuit() const;
	const bool getKeyTime();
	const bool getKeyTime(const float key_time_max);

// Functions:

	void endState();
	void pauseState();
	void unpauseState();

	void updateMousePositions(sf::View* view = nullptr);
	void updateKeyTime(const float& dt);

	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;

	void updateMousePositionText();
	void updateFpsCounter(const float& dt);

	void renderMousePositionText(sf::RenderTarget* target);
	void renderFpsCounter(sf::RenderTarget* target);

	virtual void render(sf::RenderTarget* target) = 0;
};