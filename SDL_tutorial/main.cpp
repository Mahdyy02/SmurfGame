#include "SDL.h"
#include "Game.h"

#define WIDTH 500
#define HEIGHT 500

Game* game = nullptr;

int main(int argc, char* argv[]) {
	
	game = new Game();
	game->init("SaneferPawPaw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 1);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();
	
	return 0;
}