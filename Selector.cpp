#include "stdafx.h"
#include "Selector.h"

// Constructors and Destructor:

gui::Selector::Selector(const float grid_size)
	: gridSize(grid_size)
{
	this->selectorRectangle.setSize(sf::Vector2f(grid_size, grid_size));
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

void gui::Selector::enable()
{
	this->selectorRectangle.setOutlineColor(sf::Color::Green);
}

void gui::Selector::disable()
{
	this->selectorRectangle.setOutlineColor(sf::Color::Red);
}

void gui::Selector::update(sf::Vector2i mouse_position_grid, const float& dt)
{
	this->selectorRectangle.setPosition
	(
		static_cast<float>(mouse_position_grid.x) * this->gridSize,
		static_cast<float>(mouse_position_grid.y) * this->gridSize
	);
}

void gui::Selector::render(sf::RenderTarget* target)
{
	target->draw(this->selectorRectangle);
}
