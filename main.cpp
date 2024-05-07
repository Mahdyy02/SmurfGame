#include "SDL.h"
#include "Game.h"

#define FPS 80
#define FrameDelay 12.5 

Game* game = nullptr;

Uint32 frameStart;
int frameTime;

int main(int argc, char* argv[]) {

	game = new Game();
	game->init("SaneferPawPaw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0);

	while (game->running()) {

		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (FrameDelay > frameTime) {
			SDL_Delay(FrameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}