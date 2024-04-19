#include "Game.h"

Game::Game() {

}

Game::~Game() {

}

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
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created..." << std::endl;
		}
		this->isRunning = true;
	}
	else {
		isRunning = false;
	}
}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update(){
	cnt++;
	std::cout << cnt << std::endl;
}

void Game::render() {
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);
}

void Game::clean() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
} 