#include "Map.h"
#include "TextureManager.h"

int lvl1[20][25] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

Map::Map() {
	this->grass = TextureManager::loadTexture("grass.png"); //0
	this->water = TextureManager::loadTexture("water.png"); //1
	this->mushroom = TextureManager::loadTexture("mushroom.png"); //2

	this->loadMap(lvl1);

	this->src.x = 0;
	this->src.y = 0;
	this->src.h = 512;
	this->src.w = 512;

	this->dest.x = 0;
	this->dest.y = 0;
	this->dest.h = 32;
	this->dest.w = 32;
}

Map::~Map() {
	SDL_DestroyTexture(this->grass);
	SDL_DestroyTexture(this->grass);
	SDL_DestroyTexture(this->water);
}

void Map::loadMap(int arr[20][25]) {
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 25; ++j) {
			this->map[i][j] = arr[i][j];
		}
	}
}

void Map::drawMap() {

	int type = 0;
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 25; ++j) {
			
			type = this->map[i][j];
			this->dest.x = j * 32;
			this->dest.y = i * 32;

			switch (type){
			case 0:
				TextureManager::draw(this->grass, this->src, this->dest);
				break;
			case 1:
				TextureManager::draw(this->water, this->src, this->dest);
				break;
			case 2:
				TextureManager::draw(this->mushroom, this->src, this->dest);
				break;
			default:
				break;
			}
		}
	}
}