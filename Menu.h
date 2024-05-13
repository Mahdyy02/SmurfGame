#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include "Game.h"

class Menu {
public:
    Menu();
    ~Menu();

    void menuLoop();

private:
    SDL_Texture* backgroundTexture;
    TTF_Font* font;
    int selectedButton;
    int hoveredButton;
    bool running;
    const SDL_Color selectColor = { 255, 0, 0 };
    const SDL_Color hoverColor = { 0, 255, 0 };

    void renderText(const char* text, std::string font_id, int x, int y);
    void renderMainMenu();
    void renderLevelSelection();
    void handleMainMenuInput(SDL_Event& event);
    void handleLevelSelectionInput(SDL_Event& event);
};

#endif // !MENU_H
