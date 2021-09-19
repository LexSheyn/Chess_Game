#include "stdafx.h"
#include "FadeScreen.h"

// Constructors and Destructor:

void gui::FadeScreen::initVariables()
{
	this->shape.setSize(sf::Vector2f(100.f, 100.f));
	this->shape.setPosition(0.f, 0.f);
	this->shape.setFillColor(sf::Color::Black);

	this->color = color;
	this->alphaMultiplier = 1.f;
	this->speed = 1.f;
	this->fade = false;
	this->covered = false;
}

gui::FadeScreen::FadeScreen()
{
	this->initVariables();
}

gui::FadeScreen::~FadeScreen()
{

}

const bool gui::FadeScreen::isVisible() const
{
	if (this->shape.getFillColor().a != 0)
	{
		return true;
	}

	return false;
}

const bool gui::FadeScreen::isOpaque() const
{
	if (this->shape.getFillColor().a == 255)
	{
		return true;
	}

	return false;
}

void gui::FadeScreen::setSize(const float x, const float y)
{
	this->shape.setSize(sf::Vector2f(x, y));
}

void gui::FadeScreen::setSize(sf::Vector2f size)
{
	this->shape.setSize(size);
}

void gui::FadeScreen::setColor(sf::Color color)
{
	this->shape.setFillColor(color);
}

void gui::FadeScreen::setFadeSpeed(const float speed)
{
	this->speed = speed;
}


// Fucntions:

void gui::FadeScreen::fadeIn()
{
	this->fade = false;
}

void gui::FadeScreen::fadeOut()
{
	this->fade = true;
}

void gui::FadeScreen::update(const float& dt)
{
	if (this->fade)
	{
		this->alphaMultiplier += this->speed * dt;

		if (this->alphaMultiplier > 1)
		{
			this->alphaMultiplier = 1;
		}
	}
	else
	{
		this->alphaMultiplier -= this->speed * dt;

		if (this->alphaMultiplier < 0)
		{
			this->alphaMultiplier = 0;
		}
	}

	this->shape.setFillColor(sf::Color(this->color.r, this->color.g, this->color.b, static_cast<uint8_t>(static_cast<float>(this->color.a) * this->alphaMultiplier)));
}

void gui::FadeScreen::render(sf::RenderTarget* target)
{
	if (this->shape.getFillColor().a != 0)
	{
		target->draw(this->shape);
	}
}
