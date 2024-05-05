#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "AssetManager.h"

class ColliderComponent;
class AssetManager;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return this->isRunning; }

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static SDL_Rect camera;
	static AssetManager *assets;
	static bool isRunning;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		GroupProjectiles
	};

private:

	SDL_Window* window;
	int cnt;
};

#endif