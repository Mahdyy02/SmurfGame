#include "Game.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include <sstream>
#include "Sound.h"

int Game::screenWidth = 1800;
int Game::screenHeight = 900;

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera ={0, 0, 8192*1, 4096 *1 };

AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());

auto& label(manager.addEntity());

Map* map;
Sound* backgroundMusic;

bool Game::isRunning = false;


Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialised!..." << std::endl;
		this->window = SDL_CreateWindow(title, xpos, ypos, screenWidth, screenHeight, flags);

		if (this->window) {
			std::cout << "Window created succesfully..." << std::endl;
		}

		this->renderer = SDL_CreateRenderer(window, -1, 0);
		if (this->renderer) {
			SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
			std::cout << "Renderer created..." << std::endl;
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "Error: SDL_TTF" << std::endl;
	}

	Mix_Init(MIX_INIT_MP3);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	assets->addTexture("terrain", "map.png");
	assets->addTexture("player", "animated_smurf.png");
	assets->addTexture("projectile", "charchabil.png");
	assets->addTexture("collider", "empty.png");

	assets->addFont("arial", "arial.ttf", 16);

	assets->addSound("background", "background_music.wav");
	assets->addSound("walk", "walk.wav");
	
	map = new Map("terrain", 1, 32);

	map->loadMap("map.map", 256, 128);

	player.addComponent<TransformComponent>(470,2327,32,32,3);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addComponent<SoundComponent>();
	player.getComponent<SoundComponent>().addSound("walk","walk");
	player.addGroup(groupPlayers);

	SDL_Color white = { 255,255,255, 255 };
	label.addComponent<UILabel>(10, 10, "SaneferPawPaw", "arial", white, true);
	label.addGroup(Game::groupLabels);

	assets->createProjectile(Vector2D(640, 2400), Vector2D(1,0), 1, 0, "projectile");

	backgroundMusic = new Sound("background", 0.1);
	backgroundMusic->play(-1);
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::GroupProjectiles));
auto& labels(manager.getGroup(Game::groupLabels));

void Game::handleEvents() {
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		this->isRunning = false;
		break;
	default:
		break;
	}

}

void Game::update() {

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	std::stringstream ss;
	ss << "Player position: (" << playerPos.x << ", " << playerPos.y << ")";
	label.getComponent<UILabel>().setLabelText(std::move(ss).str(), "arial");

	manager.refresh();
	manager.update();

	for (auto& c : colliders) {

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		Collision::AABB(cCol, playerCol);
		if(Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
			std::cout << "Collision!" << std::endl;
			p->destroy();
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - Game::screenWidth/2;
	camera.y = player.getComponent<TransformComponent>().position.y - Game::screenHeight/2;


	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0; 
	if (camera.x > camera.w - screenWidth) camera.x = camera.w - screenWidth;
	if (camera.y > camera.h - screenHeight) camera.y = camera.h - screenHeight;
	
}

void Game::render() {
	SDL_RenderClear(this->renderer);

	for (auto& t : tiles) {
		t->draw();
	}
	for (auto& p : players) {
		p->draw();
	}

	for (auto& c : colliders) {
		c->draw();
	}

	for (auto& p : projectiles) {
		p->draw();
	}

	for (auto& l : labels) {
		l->draw();
	}

	label.draw();

	SDL_RenderPresent(this->renderer);
}


void Game::clean() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}