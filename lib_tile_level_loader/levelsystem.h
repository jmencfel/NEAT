#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "maths.h"

#define ls LevelSystem

class LevelSystem {
public:
	enum TILE { EMPTY, START, END, WALL,  WAYPOINT, ENEMY0, ENEMY1, ENEMY2, ENEMY3, WALL2};

	static void loadLevelFile(const std::string&, float tileSize = 100.0f);
	static void loadLevelFile(const std::string&, float tileSize , int x_multiplier, int y_multiplier);
	static void Render(sf::RenderWindow &window);
	static sf::Color getColor(TILE t);
	static void setColor(TILE t, sf::Color c);
	//get tile at coordinate
	static TILE getTile(sf::Vector2ul);
	//get screenspace coords of tile
	static sf::Vector2f getTilePosition(sf::Vector2ul);
	//get tile at screen space pos
	static TILE getTileAt(sf::Vector2f);
	static std::vector<sf::Vector2f> findTiles(TILE t);
	const static size_t getHeight() {return _height;}
	const static size_t getWidth() {return _width;}
	static sf::Vector2f getStart();

protected:
	static std::unique_ptr<TILE[]> _tiles;
	static size_t _width;
	static size_t _height;
	static float _tileSize;
	static sf::Vector2f _offset;
	static std::map<TILE, sf::Color> _colours;
	static std::vector< std::unique_ptr < sf::RectangleShape> > _sprites;

	static void buildSprites();

private:
	LevelSystem() = delete;
	~LevelSystem() = delete;
};