#include "stdafx.h"
#include "Pawn.h"

// private: Functions:

void Pawn::initVariables()
{
	this->selected = false;
	this->clickable = false;
	this->buttonPressed = false;

	this->selectionFrame.setSize(sf::Vector2f(64.f, 64.f));
	this->selectionFrame.setFillColor(sf::Color::Transparent);
	this->selectionFrame.setOutlineThickness(-4.f);
	this->selectionFrame.setOutlineColor(sf::Color::Green);
}

void Pawn::initAnimations()
{
/*
this->animationComponent->addAnimation
(
	"IDLE_LEFT", //Animation name
	25.f, //Animation time
	0, //X first frame number, starts form 0
	0, //Y first frame number, starts from 0
	3, //X last frame number, starts form 0
	0, //Y last frame number, starts from 0
	58, //Width
	96 //Height
);
*/

	this->animationComponent->addAnimation("IDLE_DOWN", 15.f, 0, 0, 0, 0, 64, 64);
	this->animationComponent->addAnimation("WALK_DOWN", 11.f, 0, 0, 0, 0, 64, 64);
	this->animationComponent->addAnimation("WALK_LEFT", 11.f, 0, 0, 0, 0, 64, 64);
	this->animationComponent->addAnimation("WALK_RIGHT", 11.f, 0, 0, 0, 0, 64, 64);
	this->animationComponent->addAnimation("WALK_UP", 11.f, 0, 0, 0, 0, 64, 64);
	this->animationComponent->addAnimation("ATTACK", 5.f, 0, 0, 0, 0, 64, 64);
}


// Constructors and Destructor:

Pawn::Pawn(float x, float y, sf::Texture& texture_sheet, bool ckickable)
{
	this->initVariables();

	this->clickable = ckickable;

	//Components initialization
	this->createHitboxComponent(this->sprite, 0.f, 0.f, 64.f, 64.f);
	this->createMovementComponent(200.f, 1600.f, 1000.f);
	this->createAnimationComponent(texture_sheet);

	this->setPosition(x, y);
	this->initAnimations();
}

Pawn::~Pawn()
{
}


// Accessors:

const bool& Pawn::isSelected() const
{
	return this->selected;
}

const Pawn* Pawn::getPawn() const
{
	return nullptr;
}


// Modifiers:

void Pawn::select()
{
	this->selected = true;
}

void Pawn::unselect()
{
	this->selected = false;
}


// Functions:

void Pawn::playAnimation(std::string key, const float& dt, bool priority)
{
	this->animationComponent->play(key, dt, priority);
}

void Pawn::updateAnimation(const float& dt)
{
	if (this->movementComponent->getState(MovementStates::State::Idle))
	{
		this->animationComponent->play("IDLE_DOWN", dt);
	}
	else if (this->movementComponent->getState(MovementStates::State::Left))
	{
		this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getVelocityMax());
	}
	else if (this->movementComponent->getState(MovementStates::State::Right))
	{
		this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getVelocityMax());
	}
	else if (this->movementComponent->getState(MovementStates::State::Up))
	{
		this->animationComponent->play("WALK_UP", dt, this->movementComponent->getVelocity().y, this->movementComponent->getVelocityMax());
	}
	else if (this->movementComponent->getState(MovementStates::State::Down))
	{
		this->animationComponent->play("WALK_DOWN", dt, this->movementComponent->getVelocity().y, this->movementComponent->getVelocityMax());
	}
}

bool Pawn::checkSelection(sf::Vector2f& mouse_position_view)
{
	if (this->clickable)
	{
		if (this->hitboxComponent)
		{
			if (this->hitboxComponent->getGlobalBounds().contains(mouse_position_view))
			{
				return true;
			}

			return false;
		}
		else
		{
			if (this->sprite.getGlobalBounds().contains(mouse_position_view))
			{
				return true;
			}

			return false;
		}
	}

	return false;
}

void Pawn::update(sf::Vector2f& mouse_position_view, const float& dt)
{
	this->movementComponent->update(dt);

	this->updateAnimation(dt);

	this->hitboxComponent->update();
}

void Pawn::render(sf::RenderTarget* target, sf::Shader* shader, const sf::Vector2f shader_position, const bool show_hitbox)
{
	if (shader)
	{
		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", shader_position);

		target->draw(this->sprite, shader);
	}
	else
	{
		target->draw(this->sprite);
	}

	if (this->selected)
	{
		if (this->clickable)
		{
		//	this->selectionFrame.setPosition(this->sprite.getPosition());
		//	target->draw(this->selectionFrame);
		}
	}

	if (show_hitbox)
	{
		this->hitboxComponent->render(target);
	}
}
