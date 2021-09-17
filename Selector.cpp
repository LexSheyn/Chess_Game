#include "stdafx.h"
#include "Selector.h"

// Constructors and Destructor:

gui::Selector::Selector(StateData* state_data)
	: stateData(*state_data)
{
	this->selectorRectangle.setSize(sf::Vector2f(state_data->gridSize, state_data->gridSize));
	this->selectorRectangle.setFillColor(sf::Color::Transparent);
	this->selectorRectangle.setOutlineColor(sf::Color::Transparent);
	this->selectorRectangle.setOutlineThickness(-4.f);
}

gui::Selector::~Selector()
{

}

const sf::Vector2f gui::Selector::getPosition() const
{
	return this->selectorRectangle.getPosition();
}

const sf::Vector2f gui::Selector::getCenter() const
{
	return sf::Vector2f
	(
		this->selectorRectangle.getPosition().x + this->selectorRectangle.getGlobalBounds().width / 2.f,
		this->selectorRectangle.getPosition().y + this->selectorRectangle.getGlobalBounds().height / 2.f
	);
}

const sf::FloatRect gui::Selector::getGlobalBounds() const
{
	return this->selectorRectangle.getGlobalBounds();
}

void gui::Selector::setColor(sf::Color color)
{
	this->selectorRectangle.setOutlineColor(color);
}


// Functions:

void gui::Selector::update(sf::Vector2i mouse_position_grid, const float& dt)
{
	this->selectorRectangle.setPosition
	(
		static_cast<float>(mouse_position_grid.x) * this->stateData.gridSize,
		static_cast<float>(mouse_position_grid.y) * this->stateData.gridSize
	);
}

void gui::Selector::render(sf::RenderTarget* target)
{
	target->draw(this->selectorRectangle);
}
