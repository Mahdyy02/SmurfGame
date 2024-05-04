#include "Map.h"
#include "Game.h"
#include <fstream>
#include <string>
#include <sstream>
#include "ECS.h"
#include "Components.h"

extern Manager manager;

Map::Map(const char* mfp, int ms, int ts): mapFilePath(mfp), mapScale(ms), tileSize(ts) {}

Map::~Map() {

}

void Map::loadMap(std::string path,  int sizeX, int sizeY) {

	std::fstream mapFile;
	mapFile.open(path);
	char c;

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			addTile(x * this->tileSize, y * this->tileSize, x * this->tileSize*this->mapScale, y * this->tileSize * this->mapScale);
		}
	}

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			mapFile.get(c);
			if (c == '1') {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * this->tileSize * this->mapScale, y * this->tileSize * this->mapScale, this->tileSize * this->mapScale);
				tcol.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}

void Map::addTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, this->tileSize, this->mapScale, this->mapFilePath);
	tile.addGroup(Game::groupMap);
}