#pragma once

#include "State.h"

namespace gui
{
	class FadeScreen
	{
	private:

	// Variables:

		StateData& stateData;

		sf::RectangleShape shape;
		sf::Color color;
		float alphaMultiplier;
		float speed;
		bool fade;
		bool covered;

	public:

	// Constructors and Destructor:

		FadeScreen(StateData* state_data, sf::Color color, const float speed = 1.f);
		~FadeScreen();

	// Accessors:

		const bool isVisible() const;
		const bool isOpaque() const;

	// Fucntions:

		void fadeIn();
		void fadeOut();

		void update(const float& dt);
		void render(sf::RenderTarget* target);
	};
}

