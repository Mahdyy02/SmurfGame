#include "Map.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include "ECS.h"
#include "Components.h"

extern Manager manager;

Map::Map(std::string tID, int ms, int ts, int w, int h): texID(tID), mapScale(ms), tileSize(ts), width(w), height(h) {}

Map::~Map() {

}

void Map::loadMap(std::string path,  int sizeX, int sizeY, bool reset) {

	std::fstream mapFile;
	mapFile.open(path);
	char c;

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			if(!reset) addTile(x * this->tileSize, y * this->tileSize, x * this->tileSize*this->mapScale, y * this->tileSize * this->mapScale);
		}
	}

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			mapFile.get(c);
			if (c == '1') {
				if (!reset) {
					auto& tcol(manager.addEntity());
					tcol.addComponent<ColliderComponent>("terrain", x * this->tileSize * this->mapScale, y * this->tileSize * this->mapScale, this->tileSize * this->mapScale);
					tcol.addGroup(Game::groupColliders);
				}
			}
			mapFile.ignore();
		}
	}
	
	mapFile.ignore();

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			mapFile.get(c);
			if (c == '1') {
				if (!reset) {
					auto& label(manager.addEntity());
					auto& labelIndex(manager.addEntity());
					SDL_Color white = { 255,255,255, 255 };
					label.addComponent<UILabel>(x * this->tileSize * this->mapScale, y * this->tileSize * this->mapScale, "Press E to enter", "arial", white, false, 100);
					labelIndex.addComponent<UILabel>(x * this->tileSize * this->mapScale + 20, y * this->tileSize * this->mapScale + 20, "House: " + std::to_string(label.getComponent<UILabel>().labelID / 2), "arial", white, false, 100);
					label.addGroup(Game::groupLabels);
					labelIndex.addGroup(Game::groupLabels);
				}
			}
			mapFile.ignore();
		}
	}

	mapFile.ignore();

	srand((unsigned)time(NULL));

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			mapFile.get(c);
			if (c == '1') {
				if((float)rand() / RAND_MAX < Game::redPotionsProbabilities[Game::gameLevel])
					Game::assets->createProjectile(Vector2D(x * this->tileSize * this->mapScale, y * this->tileSize * this->mapScale), Vector2D(0, 0), 1, 0, "redPotion", 1);
			}
			mapFile.ignore();
		}
	}

	mapFile.ignore();

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			mapFile.get(c);
			if (c == '1') {
				if ((float)rand() / RAND_MAX < Game::bluePotionsProbabilities[Game::gameLevel])
					Game::assets->createProjectile(Vector2D(x * this->tileSize * this->mapScale, y * this->tileSize * this->mapScale), Vector2D(0, 0), 1, 0, "bluePotion", 2);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}

void Map::addTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, this->tileSize, this->mapScale, this->texID);
	tile.addGroup(Game::groupMap);
}
