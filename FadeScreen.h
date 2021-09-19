#pragma once

namespace gui
{
	class FadeScreen
	{
	private:

	// Variables:

		sf::RectangleShape shape;
		sf::Color color;
		float alphaMultiplier;
		float speed;
		bool fade;
		bool covered;

	// Functions:

		void initVariables();

	public:

	// Constructors and Destructor:

		FadeScreen();
		~FadeScreen();

	// Accessors:

		const bool isVisible() const;
		const bool isOpaque() const;

	// Modifiers:

		void setSize(const float x, const float y);
		void setSize(sf::Vector2f size);
		void setColor(sf::Color color);
		void setFadeSpeed(const float speed);

	// Fucntions:

		void fadeIn();
		void fadeOut();

		void update(const float& dt);
		void render(sf::RenderTarget* target);
	};
}

