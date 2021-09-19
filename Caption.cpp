#include "stdafx.h"
#include "Caption.h"

// private: Functions:

void gui::Caption::initVariables()
{
	this->shown = false;
	this->back.setFillColor(sf::Color::Transparent);
}


// Constructors and Destructor:

gui::Caption::Caption(sf::Font& font, sf::Color text_color, const std::string str)
{
	this->initVariables();
	this->text.setFont(font);
	this->text.setFillColor(text_color);
	this->text.setString(str);
}

gui::Caption::~Caption()
{

}


// Modifiers:

void gui::Caption::setSize(sf::Vector2f size)
{
	this->back.setSize(size);
	this->text.setCharacterSize(static_cast<uint32_t>(size.y / 2.f));
}

void gui::Caption::setSize(const float x, const float y)
{
	this->back.setSize(sf::Vector2f(x, y));
	this->text.setCharacterSize(static_cast<uint32_t>(y / 2.f));
}

void gui::Caption::setPosition(sf::Vector2f position)
{
	this->back.setPosition(position);
	this->text.setPosition
	(
		(this->back.getPosition().x + (this->back.getSize().x / 2.f)) - (this->text.getGlobalBounds().width / 2.f ),
		position.y
	);
}

void gui::Caption::setPosition(const float x, const float y)
{
	this->back.setPosition(x, y);
	this->text.setPosition
	(
		(this->back.getPosition().x + (this->back.getSize().x / 2.f)) - (this->text.getGlobalBounds().width / 2.f),
		y
	);
}

void gui::Caption::setBackColor(sf::Color color)
{
	this->back.setFillColor(color);
}

void gui::Caption::setTextColor(sf::Color color)
{
	this->text.setFillColor(color);
}

void gui::Caption::setString(const std::string str)
{
	this->text.setString(str);
}

void gui::Caption::setTexture(sf::Texture& texture)
{
	this->back.setTexture(&texture);
}


// Functions:

void gui::Caption::show()
{
	this->shown = true;
}

void gui::Caption::hide()
{
	this->shown = false;
}

void gui::Caption::updatePosition(sf::Vector2f position)
{
	this->back.setPosition(position);
	this->text.setPosition
	(
		(this->back.getPosition().x + (this->back.getSize().x / 2.f)) - (this->text.getGlobalBounds().width / 2.f),
		position.y
	);
}

void gui::Caption::updatePosition(sf::Vector2i position)
{
	this->back.setPosition(sf::Vector2f(position));
	this->text.setPosition
	(
		(this->back.getPosition().x + (this->back.getSize().x / 2.f)) - (this->text.getGlobalBounds().width / 2.f),
		static_cast<float>(position.y)
	);
}

void gui::Caption::update(const float& dt)
{

}

void gui::Caption::render(sf::RenderTarget* target)
{
	if (this->shown)
	{
		target->draw(this->back);
		target->draw(this->text);
	}
}
