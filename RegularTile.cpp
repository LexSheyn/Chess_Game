#include "stdafx.h"
#include "RegularTile.h"

//Constructor and Destructor:

RegularTile::RegularTile
(
	int type, int grid_x, int grid_y, float grid_size_f, 
	const sf::Texture& texture, const sf::IntRect& texture_rect, 
	const bool collision, const bool show_captured
)
	: Tile(type, grid_x, grid_y, grid_size_f, texture, texture_rect, collision)
{
	// Captured frame
	this->capturedFrame.setSize(sf::Vector2f(64.f, 64.f));
	this->capturedFrame.setFillColor(sf::Color(155, 155, 0, 150));
	this->capturedFrame.setOutlineColor(sf::Color(255, 255, 0, 150));
	this->capturedFrame.setOutlineThickness(-2.f);

	// Availible frame
	this->availableFrame.setSize(sf::Vector2f(64.f, 64.f));
	this->availableFrame.setFillColor(sf::Color(0, 155, 0, 100));
	this->availableFrame.setOutlineColor(sf::Color(0, 255, 0, 100));
	this->availableFrame.setOutlineThickness(-2.f);

	// Unavailible frame
	this->unavailableFrame.setSize(sf::Vector2f(64.f, 64.f));
	this->unavailableFrame.setFillColor(sf::Color(155, 0, 0, 100));
	this->unavailableFrame.setOutlineColor(sf::Color(255, 0, 0, 100));
	this->unavailableFrame.setOutlineThickness(-2.f);
}

RegularTile::~RegularTile()
{

}


//Accessors:

const std::string RegularTile::getAsString() const
{
	std::stringstream ss;

	ss << this->type << " " << this->shape.getTextureRect().left << " " << this->shape.getTextureRect().top
		<< " " << this->collision;

	return ss.str();
}


//Functions:

void RegularTile::update(const float& dt)
{
	this->capturedFrame.setPosition(this->getPosition());
	this->availableFrame.setPosition(this->getPosition());
	this->unavailableFrame.setPosition(this->getPosition());
}

void RegularTile::render(sf::RenderTarget* target, sf::Shader* shader, const sf::Vector2f player_position)
{
	if (shader)
	{
		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", player_position);

		target->draw(this->shape, shader);
	}
	else
	{
		target->draw(this->shape);
	}

	if (this->showCaptured)
	{
		if (this->captured)
		{
			target->draw(this->capturedFrame);
		}
	}

	if (this->showAvailable)
	{
		if (this->available)
		{
			target->draw(this->availableFrame);
		}
		else
		{
		//	target->draw(this->unavailableFrame);
		}
	}
}