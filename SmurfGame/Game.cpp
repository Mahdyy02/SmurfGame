#include "Game.h"

SDL_Texture* playerTex;
SDL_Rect srcR, destR;

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
		if (this->renderer) {
			SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
			std::cout << "Renderer created..." << std::endl;
		}
		this->isRunning = true;
	}
	else {
		this->isRunning = false;
	}

	SDL_Surface* tmpSurafce = IMG_Load("sanfour.png");
	playerTex = SDL_CreateTextureFromSurface(this->renderer, tmpSurafce);
	SDL_FreeSurface(tmpSurafce);
}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		this->isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			destR.y -= 10;
			break;
		case SDLK_DOWN:
			destR.y += 10;
			break;
		case SDLK_LEFT:
			destR.x -= 10;
			break;
		case SDLK_RIGHT:
			destR.x += 10;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

}

void Game::update() {
	cnt++;
	destR.h = 64;
	destR.w = 64;
	std::cout << cnt << std::endl;
}

void Game::render() {
	// Clear the renderer with black color
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // Black
	SDL_RenderClear(this->renderer);

	// Render the player texture (assuming playerTex is a valid texture)
	SDL_RenderCopy(renderer, playerTex, NULL, &destR);

	// Draw the first red rectangle
	SDL_Rect rect1 = { 10, 20, 500, 50 }; // x, y, width, height
	SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255); 
	SDL_RenderFillRect(this->renderer, &rect1);

	// Draw the second green rectangle
	SDL_Rect rect2 = { 60, 20, 50, 50 }; // x, y, width, height (adjust as needed)
	SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255); 
	SDL_RenderFillRect(this->renderer, &rect2);

	// Draw the second green rectangle
	SDL_Rect rect3 = { 60, 20, 50, 50 }; // x, y, width, height (adjust as needed)
	SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(this->renderer, &rect2);

	// Present the renderer to display the rendered content
	SDL_RenderPresent(this->renderer);
}


void Game::clean() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}