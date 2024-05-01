#include "Game.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;
Map* map;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

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
		this->isRunning = true;
	}
	else {
		this->isRunning = false;
	}
	

	map = new Map();

	player.addComponent<TransformComponent>(2);
	player.addComponent<SpriteComponent>("sanfour.png");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("water.png");
	wall.addComponent<ColliderComponent>("wall");
}

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
	manager.refresh();
	manager.update();

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		wall.getComponent<ColliderComponent>().collider)
		) {
		player.getComponent<TransformComponent>().scale = 1;
		std::cout << "Wall hit!" << std::endl;
	}
}

void Game::render() {
	SDL_RenderClear(this->renderer);
	map->drawMap();
	manager.draw();
	SDL_RenderPresent(this->renderer);
}


void Game::clean() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}