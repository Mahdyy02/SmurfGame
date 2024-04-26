#include "SDL.h"
#include "Game.h"

#define WIDTH 900
#define HEIGHT 900

Game* game = nullptr;

int main(int argc, char* argv[]) {

	game = new Game();
	game->init("SaneferPawPaw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();

	return 0;
}