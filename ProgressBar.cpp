#include "stdafx.h"
#include "ProgressBar.h"

// Constructors and Destructor:

gui::ProgressBar::ProgressBar(float x, float y, float width, float height, sf::Font* font, unsigned int char_size)
{
	this->widthMax = width;

	this->back.setSize(sf::Vector2f(width, height));
	this->back.setFillColor(sf::Color(50, 50, 50, 200));
	this->back.setPosition(x, y);

	this->inner.setSize(this->back.getSize());
	this->inner.setFillColor(sf::Color(250, 20, 20, 200));
	this->inner.setPosition(this->back.getPosition());

	this->fontPointer = font;

	if (font)
	{
		this->text.setFont(*font);
		this->text.setCharacterSize(char_size);
		this->text.setString("0");
		this->text.setPosition
		(
			this->back.getPosition().x + (this->back.getGlobalBounds().width / 10.f),
			this->back.getPosition().y + (this->back.getGlobalBounds().height / 10.f)
		);
	}
}

gui::ProgressBar::~ProgressBar()
{

}


// Modifiers:

void gui::ProgressBar::setBackColor(const sf::Color& color)
{
	this->back.setFillColor(color);
}

void gui::ProgressBar::setBackTexture(const std::string& file_path)
{
	this->backTexture.loadFromFile(file_path);
	this->back.setTexture(&this->backTexture);
}

void gui::ProgressBar::setInnerColor(const sf::Color& color)
{
	this->inner.setFillColor(color);
}

void gui::ProgressBar::setInnerTexture(const std::string& file_path)
{
	this->innerTexture.loadFromFile(file_path);
	this->inner.setTexture(&this->innerTexture);
}

void gui::ProgressBar::setPosition(const float x, const float y)
{
	this->back.setPosition(x, y);
	this->inner.setPosition(x, y);
}

void gui::ProgressBar::setPosition(const sf::Vector2f& position)
{
	this->back.setPosition(position);
	this->inner.setPosition(position);
}

void gui::ProgressBar::setTextString(const std::string& str)
{
	this->text.setString(str);
}

void gui::ProgressBar::setTextPosition(const float x, const float y)
{
	this->text.setPosition(x, y);
}

void gui::ProgressBar::setTextPosition(const sf::Vector2f& position)
{
	this->text.setPosition(position);
}


// Functions:

void gui::ProgressBar::update(const int current_value, const int max_value, const float& dt, const bool update_color)
{
	float percent = static_cast<float>(current_value / static_cast<float>(max_value));

	this->inner.setSize
	(
		sf::Vector2f(
			static_cast<float>(std::floor(this->widthMax * percent)),
			this->inner.getSize().y
		)
	);

	if (this->fontPointer)
	{
		this->barString = std::to_string(current_value) + " / " + std::to_string(max_value);
		this->text.setString(this->barString);
	}

	if (update_color)
	{
		float color = 254.f;
		this->inner.setFillColor(sf::Color(static_cast<unsigned char>(color - pow(color, percent)), static_cast<unsigned char>(color * (percent)), 20, 200));
	}
}

void gui::ProgressBar::render(sf::RenderTarget& target)
{
	target.draw(this->back);

	target.draw(this->inner);

	if (this->fontPointer)
	{
		target.draw(this->text);
	}
}