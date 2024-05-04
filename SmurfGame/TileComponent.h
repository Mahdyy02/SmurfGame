#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL.h>
#include "ECS.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include <iostream>
#include <string>
#include <cstring>

class TileComponent : public Component {
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	TileComponent() = default;

	~TileComponent() {
		SDL_DestroyTexture(this->texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, const char *path) {
		this->texture = TextureManager::loadTexture(path);

		this->position.x = xpos;
		this->position.y = ypos;
		
		this->srcRect.x = srcX;
		this->srcRect.y = srcY;
		this->srcRect.w = this->srcRect.h = tsize;

		this->destRect.x = xpos;
		this->destRect.y = ypos;
		this->destRect.w = this->destRect.h = tsize*tscale;
	}

	void draw() {
		TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	void update() override { 
		this->destRect.x = this->position.x - Game::camera.x;
		this->destRect.y = this->position.y - Game::camera.y;
	}

};

#endif // !TILECOMPONENT_H
