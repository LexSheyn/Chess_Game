#pragma once

#include "HitboxComponent.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"

enum class Figure
{
	PawnWhite = 0,
	PawnBlack
};

class Entity
{
private:

	//private: Functions:

	void initVariables();

protected:

	//sf::Texture* texture;
	sf::Sprite sprite;

	HitboxComponent* hitboxComponent;
	MovementComponent* movementComponent;
	AnimationComponent* animationComponent;

public:

	//Constructors and Destructor:

	Entity();
	virtual ~Entity();

	//Component functions:

	void setTexture(sf::Texture& texture);
	void createHitboxComponent(sf::Sprite& sprite, float offset_x, float offset_y, float width, float height);
	void createMovementComponent(const float max_velocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);

	//Accessors:

	const sf::Vector2f& getPosition() const;
	const sf::Vector2f getCenter() const;
	const sf::Vector2i getGridPosition(const int grid_size_i) const;
	const sf::Vector2i getCenterGridPosition(const int grid_size_i) const;
	const sf::FloatRect getGlobalBounds() const;
	const sf::FloatRect getNextPositionBounds(const float& dt) const;
	const float getDistance(Entity* entity) const;
	const float getDistance(sf::Vector2f position) const;

	//Modifiers:

	void setPosition(const sf::Vector2f position);
	void setPosition(const float x, const float y);
	void setPosition(const float x, const float y, bool from_center);

	//Functions:

	void move(const float dir_x, const float dir_y, const float& dt);
	void stopVelocity();
	void stopVelocityX();
	void stopVelocityY();

	virtual void update(sf::Vector2f& mouse_position_view, const float& dt) = 0;
	virtual void render(sf::RenderTarget* target, sf::Shader* shader = nullptr, const sf::Vector2f shader_position = sf::Vector2f(), const bool show_hitbox = false) = 0;
};

