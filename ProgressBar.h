#pragma once

namespace gui
{
	class ProgressBar
	{
	private:

		//Variables:

		std::string barString;
		sf::Text text;
		sf::Font* fontPointer;
		float widthMax;
		sf::RectangleShape back;
		sf::RectangleShape inner;
		sf::Texture backTexture;
		sf::Texture innerTexture;

	public:

		// Constructors and Destructor:

		ProgressBar(float x, float y, float width, float height, sf::Font* font = nullptr, unsigned int char_size = 20u);
		virtual ~ProgressBar();

		// Modifiers:

		void setBackColor(const sf::Color& color);
		void setBackTexture(const std::string& file_path);

		void setInnerColor(const sf::Color& color);
		void setInnerTexture(const std::string& file_path);

		void setPosition(const float x, const float y);
		void setPosition(const sf::Vector2f& position);

		void setTextString(const std::string& str);

		void setTextPosition(const float x, const float y);
		void setTextPosition(const sf::Vector2f& position);

		// Functions:

		void update(const int current_value, const int max_value, const float& dt, const bool update_color = false);

		void render(sf::RenderTarget& target);
	};
}

