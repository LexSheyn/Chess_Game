#pragma once

namespace gui
{
	class Caption
	{
	private:

	// Variables:

		sf::RectangleShape back;
		sf::Text text;
		bool shown;

	// private: Functions:

		void initVariables();

	public:

	// Constructors and Destructor:

		Caption(sf::Font& font, sf::Color text_color, const std::string str);
		~Caption();

	// Modifiers:

		void setSize(sf::Vector2f size);
		void setSize(const float x, const float y);

		void setPosition(sf::Vector2f position);
		void setPosition(const float x, const float y);

		void setBackColor(sf::Color color);
		void setTextColor(sf::Color color);

		void setString(const std::string str);
		void setTexture(sf::Texture& texture);

	// Functions:

		void show();
		void hide();

		void updatePosition(sf::Vector2f position);
		void updatePosition(sf::Vector2i position);
		void update(const float& dt);
		
		void render(sf::RenderTarget* target);
	};
}

