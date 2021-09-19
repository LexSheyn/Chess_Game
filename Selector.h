#pragma once

namespace gui
{
	class Selector
	{
	private:

	// Variables:

		sf::RectangleShape selectorRectangle;
		float gridSize;

	public:

	// Constructors and Destructor:

		Selector(const float grid_size);
		~Selector();

	// Accessors:

		const sf::Vector2f getPosition() const;
		const sf::Vector2f getCenter() const;
		const sf::FloatRect getGlobalBounds() const;

	// Modifiers:

		void setColor(sf::Color color);

	// Functions:

		void enable();
		void disable();

		void update(sf::Vector2i mouse_position_grid, const float& dt);
		void render(sf::RenderTarget* target);
	};
}

