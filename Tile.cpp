#include "stdafx.h"
#include "Tile.h"

//Constructors and Destructor:

Tile::Tile()
{
	this->type = 0;
	this->collision = false;
	this->captured = false;
	this->showCaptured = false;
	this->available = false;
	this->showAvailable = false;
}

Tile::Tile
(
	int type,
	int grid_x, int grid_y, float grid_size_f,
	const sf::Texture& texture, const sf::IntRect& texture_rect,
	const bool collision
)
{
	this->type = type;

	this->collision = collision;

	this->captured = false;
	this->showCaptured = false;
	this->available = false;
	this->showAvailable = false;

	this->shape.setPosition(static_cast<float>(grid_x) * grid_size_f, static_cast<float>(grid_y) * grid_size_f);

	this->shape.setTexture(texture);

	this->shape.setTextureRect(texture_rect);	
}

Tile::~Tile()
{

}

// Accessors:

const bool& Tile::isAvailable() const
{
	return this->available;
}

const bool& Tile::isCaptured() const
{
	return this->captured;
}

const int& Tile::getType() const
{
	return this->type;
}

const bool& Tile::getCollision() const
{
	return this->collision;
}

const sf::Vector2f Tile::getPosition() const
{
	return this->shape.getPosition();
}

const sf::Vector2f Tile::getCenter() const
{
	return sf::Vector2f
	(
		this->shape.getPosition().x + this->shape.getGlobalBounds().width / 2.f, 
		this->shape.getPosition().y + this->shape.getGlobalBounds().height / 2.f
	);
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}

const float Tile::getDistance(const Entity* entity) const
{
	return static_cast<float>(std::sqrt(std::pow((this->getCenter().x - entity->getCenter().x), 2) + std::pow((this->getCenter().y - entity->getCenter().y), 2)));
}


// Modifires:

void Tile::setCaptured(const bool captured)
{
	this->captured = captured;
}

void Tile::setShowCaptured(const bool show_captured)
{
	this->showCaptured = show_captured;
}

void Tile::setAvailable(const bool available)
{
	this->available = available;
}

void Tile::setShowAvailable(const bool show_availability)
{
	this->showAvailable = show_availability;
}


// Functions:

const bool Tile::intersects(const sf::FloatRect bounds) const
{
	return this->shape.getGlobalBounds().intersects(bounds);
}
