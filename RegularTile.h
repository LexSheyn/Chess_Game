#pragma once

#include "Tile.h"

class RegularTile :
    public Tile
{
private:

//Variables:

	sf::RectangleShape capturedFrame;
	sf::RectangleShape availableFrame;
	sf::RectangleShape unavailableFrame;

public:

//Constructors and Destructor:

    RegularTile
	(
		int type,
		int grid_x, int grid_y, float grid_size_f,
		const sf::Texture& texture, const sf::IntRect& texture_rect,
		const bool collision = false, const bool show_captured = false
	);
    virtual ~RegularTile();

//Accessors:
	
	const std::string getAsString() const;

//Functions:

	void update(const float& dt);
	void render(sf::RenderTarget* target, sf::Shader* shader = nullptr, const sf::Vector2f player_position = sf::Vector2f());
};
