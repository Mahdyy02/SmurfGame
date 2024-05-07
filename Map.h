#ifndef MAP_H
#define MAP_H

#include <string>

class Map{

public:
	
	Map(std::string tID, int ms, int ts);
	~Map();

	void addTile(int srcX, int srcY, int xpos, int ypos);
	void loadMap(std::string path, int sizeX, int sizeY);
	void drawMap();

private:
	std::string texID;
	int mapScale;
	int tileSize;
};

#endif


