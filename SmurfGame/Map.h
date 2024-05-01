#ifndef MAP_H
#define MAP_H

#include "Game.h"

class Map{

public:
	
	Map();
	~Map();

	void loadMap(int arr[20][25]);
	void drawMap();

private:

	SDL_Rect src, dest;

	SDL_Texture *mushroom;
	SDL_Texture *grass;
	SDL_Texture *water;

	int map[20][25];
};

#endif


