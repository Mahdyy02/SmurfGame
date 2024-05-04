#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include <iostream>
#include <vector>

class ColliderComponent;

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
	static bool isRunning;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders
	};

private:

	SDL_Window* window;
	int cnt;
};

#endif