#ifndef MENU_H
#define MENU_H

#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Game.h"

class Menu{

public:

	Menu();
	~Menu();
    void menuLoop();

private:
	SDL_Texture* backgroundTexture = NULL;
	TTF_Font* font = NULL;

    bool running = true;
    int selectedButton = 0;
    int hoveredButton = -1;

    SDL_Color selectColor = { 179, 92, 89, 255 };
    SDL_Color hoverColor = { 254, 238, 155, 255 };

    void renderText(const char* text, std::string font_id, int x, int y, int r = 255, int g = 255, int b = 255);
};

#endif

