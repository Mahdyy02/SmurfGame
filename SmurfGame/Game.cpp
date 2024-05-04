#include "Game.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera ={0,0,960,640 };

auto& player(manager.addEntity());

Map* map;

bool Game::isRunning = false;

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialised!..." << std::endl;
		this->window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);


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
	
	map = new Map("map.jpg", 3, 32);

	map->loadMap("map.map", 30, 20);

	player.addComponent<TransformComponent>(300,500,32,32,3);
	player.addComponent<SpriteComponent>("animated_smurf.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

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

	manager.refresh();
	manager.update();
	int i = 0;
	for (auto& c : colliders) {

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		cCol.x -= Game::camera.x;
		cCol.y -= Game::camera.y;

		Collision::AABB(cCol, playerCol);
		if(Collision::AABB(cCol, playerCol)
			|| player.getComponent<TransformComponent>().position.x > 1134
			|| player.getComponent<TransformComponent>().position.y > 836
			|| player.getComponent<TransformComponent>().position.x < 0
			|| player.getComponent<TransformComponent>().position.y < 0) {
			player.getComponent<TransformComponent>().position = playerPos;
		}

	}

	camera.x = player.getComponent<TransformComponent>().position.x - 200;
	camera.y = player.getComponent<TransformComponent>().position.y - 200;

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > camera.w) camera.x = camera.w;
	if (camera.y > camera.h) camera.y = camera.h;
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

	SDL_RenderPresent(this->renderer);
}


void Game::clean() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}