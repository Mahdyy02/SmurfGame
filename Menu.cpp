#include "Menu.h"

Menu::Menu(){
    this->backgroundTexture = IMG_LoadTexture(Game::renderer, "background.jpg");
    if (this->backgroundTexture == NULL) {
        printf("Failed to load background image! SDL_Error: %s\n", SDL_GetError());
    }

    this->font = Game::assets->getFont("freeman");
}

Menu::~Menu() {
	SDL_DestroyTexture(this->backgroundTexture);
	TTF_CloseFont(this->font);
}

void Menu::renderText(const char* text, std::string font_id, int x, int y, int r, int g, int b) {
    SDL_Color textColor = { r, g, b };
    SDL_Surface* textSurface = TTF_RenderText_Blended(Game::assets->getFont(font_id), text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    SDL_Rect dstRect = { x - textWidth / 2, y - textHeight / 2, textWidth, textHeight };
    SDL_RenderCopy(Game::renderer, textTexture, NULL, &dstRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Menu::menuLoop() {

    this->running = true;
    bool inLevelSelection = false;

    while (this->running) {

        this->hoveredButton = -1;
        this->selectedButton = 0;

        while (this->running && !inLevelSelection) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    this->running = false;
                    Game::isRunning = false;
                    Game::windowRunning = false;
                }
                else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        this->selectedButton = (this->selectedButton - 1 + 3) % 3;
                        break;
                    case SDLK_DOWN:
                        this->selectedButton = (this->selectedButton + 1) % 3;
                        break;
                    case SDLK_RETURN:
                        switch (this->selectedButton) {
                        case 0:
                            Game::isRunning = true;
                            this->running = false;
                            break;
                        case 1:
                            inLevelSelection = true;
                            break;
                        case 2:
                            this->running = false;
                            Game::isRunning = false;
                            Game::windowRunning = false;
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;

                    this->hoveredButton = -1;

                    if (mouseX >= (Game::screenWidth / 2 - 100) && mouseX <= (Game::screenWidth / 2 + 100)) {
                        if (mouseY >= (Game::screenHeight / 2 - 50) && mouseY <= (Game::screenHeight / 2 + 50)) {
                            this->hoveredButton = 0;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 50) && mouseY <= (Game::screenHeight / 2 + 150)) {
                            this->hoveredButton = 1;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 150) && mouseY <= (Game::screenHeight / 2 + 250)) {
                            this->hoveredButton = 2;
                        }
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX >= (Game::screenWidth / 2 - 100) && mouseX <= (Game::screenWidth / 2 + 100)) {
                        if (mouseY >= (Game::screenHeight / 2 - 50) && mouseY <= (Game::screenHeight / 2 + 50)) {
                            Game::isRunning = true;
                            this->running = false;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 50) && mouseY <= (Game::screenHeight / 2 + 150)) {
                            inLevelSelection = true;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 150) && mouseY <= (Game::screenHeight / 2 + 250)) {
                            this->running = false;
                            Game::isRunning = false;
                            Game::windowRunning = false;
                        }
                    }
                }
            }

            SDL_RenderClear(Game::renderer);

            SDL_RenderCopy(Game::renderer, this->backgroundTexture, NULL, NULL);

            const char* firstButtonText = Game::isRunning ? "Resume" : "Play";

            renderText("SANFOURA", "skincake", Game::screenWidth / 2, 75);

            if (Game::gamePlayed) {
                if (Game::win) renderText("Winner Winner! Chicken dinner...!", "freeman", Game::screenWidth / 2, 125, 254, 238, 155);
                else renderText("You lost...", "freeman", Game::screenWidth / 2, 125, 179, 92, 89);
            }

            renderText(firstButtonText, "freeman", Game::screenWidth / 2, Game::screenHeight / 2 - 25);
            renderText("Levels", "freeman", Game::screenWidth / 2, Game::screenHeight / 2 + 75);
            renderText("Quit", "freeman", Game::screenWidth / 2, Game::screenHeight / 2 + 175);

            for (int i = 0; i < 3; ++i) {
                SDL_Color borderColor = (i == this->selectedButton) ? this->selectColor : SDL_Color{ 255, 255, 255, 255 };
                if (i == this->hoveredButton) {
                    borderColor = this->hoverColor;
                }

                SDL_SetRenderDrawColor(Game::renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
                SDL_Rect highlightRect = {
                    Game::screenWidth / 2 - 110,
                    Game::screenHeight / 2 - 50 + i * 100,
                    220,
                    50
                };
                SDL_RenderDrawRect(Game::renderer, &highlightRect);
            }

            SDL_RenderPresent(Game::renderer);

        }

        this->hoveredButton = -1;
        this->selectedButton = 0;

        while (inLevelSelection) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    this->running = false;
                    Game::isRunning = false;
                    inLevelSelection = false;
                    Game::windowRunning = false;
                }
                else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            this->selectedButton = (this->selectedButton - 1 + 3) % 3;
                            Game::gameLevel = 0;
                            break;
                        case SDLK_DOWN:
                            this->selectedButton = (this->selectedButton + 1) % 3;
                            Game::gameLevel = 1;
                            break;
                        case SDLK_RETURN:
                            inLevelSelection = false;
                            Game::gameLevel = 2;
                            break;
                    }
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;

                    this->hoveredButton = -1;

                    if (mouseX >= (Game::screenWidth / 2 - 100) && mouseX <= (Game::screenWidth / 2 + 100)) {
                        if (mouseY >= (Game::screenHeight / 2 - 50) && mouseY <= (Game::screenHeight / 2 + 50)) {
                            this->hoveredButton = 0;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 50) && mouseY <= (Game::screenHeight / 2 + 150)) {
                            this->hoveredButton = 1;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 150) && mouseY <= (Game::screenHeight / 2 + 250)) {
                            this->hoveredButton = 2;
                        }
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX >= (Game::screenWidth / 2 - 100) && mouseX <= (Game::screenWidth / 2 + 100)) {
                        if (mouseY >= (Game::screenHeight / 2 - 50) && mouseY <= (Game::screenHeight / 2 + 50)) {
                            Game::gameLevel = 0;
                            inLevelSelection = false;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 50) && mouseY <= (Game::screenHeight / 2 + 150)) {
                            Game::gameLevel = 1;
                            inLevelSelection = false;
                        }
                        else if (mouseY >= (Game::screenHeight / 2 + 150) && mouseY <= (Game::screenHeight / 2 + 250)) {
                            Game::gameLevel = 2;
                            inLevelSelection = false;
                        }
                    }
                }
            }

            SDL_RenderClear(Game::renderer);

            SDL_RenderCopy(Game::renderer, this->backgroundTexture, NULL, NULL);

            renderText("SANFOURA", "skincake", Game::screenWidth / 2, 75);
            renderText("Easy", "freeman", Game::screenWidth / 2, Game::screenHeight / 2 - 25);
            renderText("Medium", "freeman", Game::screenWidth / 2, Game::screenHeight / 2 + 75);
            renderText("Hard", "freeman", Game::screenWidth / 2, Game::screenHeight / 2 + 175);

            for (int i = 0; i < 3; ++i) {
                SDL_Color borderColor = (i == this->selectedButton) ? this->selectColor : SDL_Color{ 255, 255, 255, 255 };
                if (i == this->hoveredButton) {
                    borderColor = this->hoverColor;
                }

                SDL_SetRenderDrawColor(Game::renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
                SDL_Rect highlightRect = {
                    Game::screenWidth / 2 - 110,
                    Game::screenHeight / 2 - 50 + i * 100,
                    220,
                    50
                };
                SDL_RenderDrawRect(Game::renderer, &highlightRect);
            }

            SDL_RenderPresent(Game::renderer);

        }
    }

}