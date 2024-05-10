#ifndef MAP_H
#define MAP_H

#include <string>

class Map{

public:
	
	Map(std::string tID, int ms, int ts,int w, int h);
	~Map();

	void addTile(int srcX, int srcY, int xpos, int ypos);
	void loadMap(std::string path, int sizeX, int sizeY);
	void drawMap();

	int getWidth() { return this->width; }
	int getHeight() { return this->height; }

private:
	std::string texID;
	int mapScale;
	int tileSize;
	int width;
	int height;
};

#endif


