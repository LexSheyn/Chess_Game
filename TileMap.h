#pragma once

#include "RegularTile.h"
#include "Pawn.h"

class TileMap
{
private:

//Variables:

	float gridSizeF;
	int gridSizeI;
	sf::Vector2i maxSizeWorldGrid;
	sf::Vector2f maxSizeWorldF;
	int layers;
	std::vector< std::vector< std::vector< std::vector<Tile*> > > > map;
	std::stack<Tile*> defferedRenderStack;
	std::string textureFilePath;
	sf::Texture tileSheet;
	sf::RectangleShape collisionBox;

	//Culling
	int fromX;
	int toX;
	int fromY;
	int toY;
	int layer;

//private: Functions:

	//Clears the map.
	void clearMap();

public:

//Constructors and Destructor:

	TileMap(float grid_size, int width, int height, int layers, std::string texture_file_path);

	//Should use '\\' instead of '/' in the file path and put this file to the .exe location,
	//to be able to play the game without Visual Studio.
	TileMap(const std::string file_path);

	virtual ~TileMap();

//Accessors:

	const bool tileEmpty (const int x, const int y, const int z) const;
	const sf::Texture* getTileSheet() const;
	const int getLayerSize(const int x, const int y, const int layer) const;
	const int& getLayer();
	const sf::Vector2i& getMaxSizeGrid() const;
	const sf::Vector2f& getMaxSizeF() const;
	const bool checkType(const int x, const int y, const int z, const int type) const;
	Tile* getTile(const int x, const int y, const int z) const;
	Tile* getTile(const int x, const int y) const;

//Functions:

	// Tekes three indices frpm the mouse position in the grid and adds a tile to that position,
	// if the internal tilemap array allows it.
	void addTile(const int x, const int y, const int z, const sf::IntRect& texture_rect, const bool collision, const int type);

	// Tekes three indices frpm the mouse position in the grid and removes a tile on that position,
	// if the internal tilemap array allows it.
	void removeTile(const int x, const int y, const int z, const int type = -1);

	void saveToFile(const std::string file_path);

	void loadFromFile(const std::string file_path);

	void updateWorldBoundsCollision(Entity* entity, const float& dt);
	void updateTileCollision(Entity* entity, const float& dt);
	void updateTiles(Entity* entity, const float& dt);
	void updateCapturing(Entity* entity, const float& dt);
	void update(Entity* entity, const float& dt);

	void render(sf::RenderTarget* target, const sf::Vector2i& grid_position, sf::Shader* shader = nullptr, const sf::Vector2f player_position = sf::Vector2f(), const bool show_collision = false);
	void renderDeffered(sf::RenderTarget* target, sf::Shader* shader = nullptr, const sf::Vector2f player_position = sf::Vector2f());
};

