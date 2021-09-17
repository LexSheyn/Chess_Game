#include "stdafx.h"
#include "FadeScreen.h"

// Constructors and Destructor:

gui::FadeScreen::FadeScreen(StateData* state_data, sf::Color color, const float speed)
	: stateData(*state_data)
{
	this->shape.setSize
	(
		sf::Vector2f
		(
			static_cast<float>(this->stateData.gfxSettings->resolution.width), 
			static_cast<float>(this->stateData.gfxSettings->resolution.height)
		)
	);

	this->shape.setPosition(0.f, 0.f);
	this->shape.setFillColor(color);

	this->color = color;
	this->alphaMultiplier = 1.f;
	this->speed = speed;
	this->fade = false;
	this->covered = false;
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
