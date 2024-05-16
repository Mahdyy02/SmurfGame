#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "TextureManager.h"
#include "AssetManager.h"
#include "Cryptographer.h"

class ColliderComponent;
class AssetManager;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();
	void reset();

	bool running() { return this->isRunning; }

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static SDL_Rect camera;
	static AssetManager *assets;
	static bool isRunning;

	static int screenHeight;
	static int screenWidth;
	static int gameLevel;
	static int levelSpeeds[3];
	static float redPotionsProbabilities[3];
	static float bluePotionsProbabilities[3];

	static bool windowRunning;

	static bool win;
	static bool gamePlayed;
	static bool paused;

	Cryptographer *crpytographer;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles,
		groupLabels,
		groupRedPotions,
		groupBluePotions,
		groupCats,
		groupGargamel,
		groupWalkieTalkie
	};

	int SmurfEngineerHouse;
	int SmurfFemaleHouse;

	int playerNearHouseID = -1;
	bool gargamelSpawned = false;
	bool gargamelAlive = true;
	bool walkieTalkieFound = false;

private:

	SDL_Window* window;
	int cnt;
};

#endif