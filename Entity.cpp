#include "stdafx.h"
#include "Entity.h"

//private: Functions:

void Entity::initVariables()
{
	this->hitboxComponent = nullptr;
	this->movementComponent = nullptr;
	this->animationComponent = nullptr;
}

//Constructors and Destructor:

Entity::Entity()
{
	this->initVariables();
}

Entity::~Entity()
{
	delete this->hitboxComponent;
	delete this->movementComponent;
	delete this->animationComponent;
}

//Component functions:

void Entity::setTexture(sf::Texture& texture)
{
	this->sprite.setTexture(texture);
}

void Entity::createHitboxComponent(sf::Sprite& sprite, float offset_x, float offset_y, float width, float height)
{
	this->hitboxComponent = new HitboxComponent(sprite, offset_x, offset_y, width, height);
}

void Entity::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->movementComponent = new MovementComponent(this->sprite, max_velocity, acceleration, deceleration);
}

void Entity::createAnimationComponent(sf::Texture& texture_sheet)
{
	this->animationComponent = new AnimationComponent(this->sprite, texture_sheet);
}


//Accessors:

const sf::Vector2f& Entity::getPosition() const
{
	if (this->hitboxComponent)
	{
		return this->hitboxComponent->getPosition();
	}

	return this->sprite.getPosition();
}

const sf::Vector2f Entity::getCenter() const
{
	if (this->hitboxComponent)
	{
		return this->hitboxComponent->getPosition() +
			sf::Vector2f
			(
				this->hitboxComponent->getGlobalBounds().width / 2.f,
				this->hitboxComponent->getGlobalBounds().height / 2.f
			);
	}

	return this->sprite.getPosition() +
		sf::Vector2f
		(
			this->sprite.getGlobalBounds().width / 2.f,
			this->sprite.getGlobalBounds().height / 2.f
		);
}

const sf::Vector2i Entity::getGridPosition(const int grid_size_i) const
{
	if (this->hitboxComponent)
	{
		return sf::Vector2i
		(
			static_cast<int>(this->hitboxComponent->getPosition().x) / grid_size_i,
			static_cast<int>(this->hitboxComponent->getPosition().y) / grid_size_i
		);
	}

	return sf::Vector2i
	(
		static_cast<int>(this->sprite.getPosition().x) / grid_size_i,
		static_cast<int>(this->sprite.getPosition().y) / grid_size_i
	);
}

const sf::Vector2i Entity::getCenterGridPosition(const int grid_size_i) const
{
	if (this->hitboxComponent)
	{
		return sf::Vector2i
		(
			static_cast<int>(this->hitboxComponent->getCenter().x) / grid_size_i,
			static_cast<int>(this->hitboxComponent->getCenter().y) / grid_size_i
		);
	}

	return sf::Vector2i
	(
		static_cast<int>(this->getCenter().x) / grid_size_i,
		static_cast<int>(this->getCenter().y) / grid_size_i
	);
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	if (this->hitboxComponent)
	{
		return this->hitboxComponent->getGlobalBounds();
	}

	return this->sprite.getGlobalBounds();
}

const sf::FloatRect Entity::getNextPositionBounds(const float& dt) const
{
	if (this->hitboxComponent && this->movementComponent)
	{
		return this->hitboxComponent->getNextPosition(this->movementComponent->getVelocity() * dt);
	}

	//Error
	return sf::FloatRect(-1.f, -1.f, -1.f, -1.f);
}

const float Entity::getDistance(Entity* entity) const
{
	return static_cast<float>(std::sqrt(std::pow((this->getCenter().x - entity->getCenter().x), 2) + std::pow((this->getCenter().y - entity->getCenter().y), 2)));
}

const float Entity::getDistance(sf::Vector2f position) const
{
	return static_cast<float>(std::sqrt(std::pow((this->getCenter().x - position.x), 2) + std::pow((this->getCenter().y - position.y), 2)));
}


//Modifiers:

void Entity::setPosition(const sf::Vector2f position)
{
	if (this->hitboxComponent)
	{
		this->hitboxComponent->setPosition(position);
	}
	else
	{
		this->sprite.setPosition(position);
	}
}

void Entity::setPosition(const float x, const float y)
{
	if (this->hitboxComponent)
	{
		this->hitboxComponent->setPosition(x, y);
	}
	else
	{
		this->sprite.setPosition(x, y);
	}
}

void Entity::setPosition(const float x, const float y, bool from_center)
{
	if (this->hitboxComponent)
	{
		this->hitboxComponent->setPosition(x - this->hitboxComponent->getGlobalBounds().width / 2.f, y - this->hitboxComponent->getGlobalBounds().height / 2.f);
	}
	else
	{
		this->sprite.setPosition(x - this->sprite.getGlobalBounds().width / 2.f, y - this->sprite.getGlobalBounds().height / 2.f);
	}
}


//Functions:

void Entity::move(const float dir_x, const float dir_y, const float& dt)
{
	if (this->movementComponent)
	{
		//Sets velocity
		this->movementComponent->move(dir_x, dir_y, dt);
	}
}

void Entity::stopVelocity()
{
	if (this->movementComponent)
	{
		this->movementComponent->stopVelocity();
	}
}

void Entity::stopVelocityX()
{
	if (this->movementComponent)
	{
		this->movementComponent->stopVelocityX();
	}
}

void Entity::stopVelocityY()
{
	if (this->movementComponent)
	{
		this->movementComponent->stopVelocityY();
	}
}
