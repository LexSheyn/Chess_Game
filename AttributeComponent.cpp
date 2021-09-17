#include "stdafx.h"
#include "AttributeComponent.h"

//Construcrors and Destructor:

AttributeComponent::AttributeComponent(const int hp, const int damage)
{
	this->hpMax = hp;
	this->hp = this->hpMax;

	this->damage = damage;
}

AttributeComponent::~AttributeComponent()
{

}


//Accessors:

const bool AttributeComponent::isDead() const
{
	return this->hp <= 0;
}


//Functions:

void AttributeComponent::loseHp(const int hp)
{
	this->hp -= hp;

	if (this->hp < 0)
	{
		this->hp = 0;
	}
}

void AttributeComponent::gainHp(const int hp)
{
	this->hp += hp;

	if (this->hp > this->hpMax)
	{
		this->hp = this->hpMax;
	}
}

void AttributeComponent::update()
{

}
