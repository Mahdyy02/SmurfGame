#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, int x, int y) {
	this->xpos = x;
	this->ypos = y;
	this->objTexture = TextureManager::loadTexture(texturesheet);
}

void GameObject::update() {

	xpos++;
	ypos++;

	srcRect.h = 512;
	srcRect.w = 512;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = 64;
	destRect.h = 64;
}

void GameObject::render() {
	SDL_RenderCopy(Game::renderer, this->objTexture, &this->srcRect, &this->destRect);
}