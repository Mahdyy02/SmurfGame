#include "TextureManager.h"
#include <iostream>

SDL_Texture* TextureManager::loadTexture(const char* filename) {
    SDL_Surface* tempSurface = IMG_Load(filename);

    if (tempSurface == nullptr) {
        std::cout << "Failed to load image: " << filename << ". Error: " << IMG_GetError() << std::endl;
        return nullptr; // Return nullptr to indicate failure
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (tex == nullptr) {
        std::cout << "Failed to create texture from surface: " << filename << ". Error: " << SDL_GetError() << std::endl;
    }
    else {
        std::cout << "Loaded texture successfully: " << filename << std::endl;
    }

    return tex;
}


void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    if (SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip) != 0) {
        std::cout << "Failed to render texture. Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}