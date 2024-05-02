#include "Map.h"
#include "Game.h"
#include <fstream>
#include <string>
#include <sstream>

Map::Map() {

}

Map::~Map() {

}

void Map::loadMap(std::string path, int sizeX, int sizeY) {
    std::ifstream mapFile(path);
    if (!mapFile.is_open()) {
        std::cerr << "Error: Failed to open map file " << path << std::endl;
        return;
    }

    std::string line;
    for (int y = 0; y < sizeY; ++y) {
        if (!std::getline(mapFile, line)) {
            std::cerr << "Error: Failed to read line " << y + 1 << " from map file" << std::endl;
            mapFile.close();
            return;
        }

        std::istringstream iss(line);
        std::string tileStr;
        for (int x = 0; x < sizeX; ++x) {
            if (!std::getline(iss, tileStr, ',')) {
                std::cerr << "Error: Failed to read tile at row " << y + 1 << ", column " << x + 1 << std::endl;
                mapFile.close();
                return;
            }

            try {
                int tileIndex = std::stoi(tileStr);
                Game::addTile(tileIndex, x * 32, y * 32);
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid tile index at row " << y + 1 << ", column " << x + 1 << std::endl;
                mapFile.close();
                return;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Error: Tile index out of range at row " << y + 1 << ", column " << x + 1 << std::endl;
                mapFile.close();
                return;
            }
        }
    }
    mapFile.close();
}