#include "Globals.h"

int constexpr FPS =  80;
float constexpr FrameDelay = 12.5;

Uint32 frameStart;
int frameTime;

int main(int argc, char* argv[]) {

	game = new Game();
	game->init("SaneferPawPaw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0);

	menu = new Menu();
	menu->menuLoop();

	while (game->running()) {

		frameStart = SDL_GetTicks();

		game->handleEvents();

        if (Game::event.type == SDL_KEYUP) {
            switch (Game::event.key.keysym.sym) {

				case SDLK_ESCAPE:
					menu->menuLoop();
					break;
				default:
					break;
				}
        }

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