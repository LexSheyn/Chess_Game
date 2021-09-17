#pragma once

#include "Entity.h"

enum class TileTypes
{
	Default = 0,
	Damaging,
	Doodad,
	EnemySpawner,
	TILE_TYPES_MAX = EnemySpawner
	//And so on...
};

class Tile
{
private:



protected:

	sf::Sprite shape;
	int type;
	bool collision;
	bool captured;
	bool showCaptured;
	bool available;
	bool showAvailable;

public:

// Constructors and Destructor:

	Tile();
	Tile
	(
		int type,
		int grid_x, int grid_y, float grid_size_f,
		const sf::Texture& texture, const sf::IntRect& texture_rect,
		const bool collision
	);
	virtual ~Tile();

// Accessors:

	const bool& isCaptured() const;
	const bool& isAvailable() const;	
	const int& getType() const;
	const bool& getCollision() const;
	const sf::Vector2f getPosition() const;
	const sf::Vector2f getCenter() const;
	const sf::FloatRect getGlobalBounds() const;
	const float getDistance(const Entity* entity) const;

// Modifiers:

	void setCaptured(const bool captured = true);
	void setShowCaptured(const bool show_captured = true);
	void setAvailable(const bool available = true);
	void setShowAvailable(const bool show_available = true);	

// Fuctions:

	const bool intersects(const sf::FloatRect bounds) const;
	
	virtual const std::string getAsString() const = 0;

	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target, sf::Shader* shader = nullptr, const sf::Vector2f player_position = sf::Vector2f()) = 0;
};

