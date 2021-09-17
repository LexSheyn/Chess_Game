#pragma once
class GraphicsSettings
{
public:

// public: Variables:

	std::string title;
	sf::VideoMode resolution;
	int32_t fullscreen;
	uint32_t style;
	uint32_t frameRateLimit;
	bool verticalSync;
	sf::ContextSettings contextSettings;
	std::vector<sf::VideoMode> videoModes;

// Constructors and Destructor: // Default Destructor

	GraphicsSettings();

// Functions:

	void saveToFile(const std::string file_path);

	void loadFromFile(const std::string file_path);
};

