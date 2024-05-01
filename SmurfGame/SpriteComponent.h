#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <iostream>
#include "Components.h"
#include "SDL.h"

class SpriteComponent : public Component {
private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;


public:
	SpriteComponent() = default;

	SpriteComponent(const char* path) {
		this->setTex(path);
	}

	void setTex(const char* path) {
		texture = TextureManager::loadTexture(path);
	}

	void init() override {

		transform = &entity->getComponent<TransformComponent>();

		this->srcRect.x = 0;
		this->srcRect.y = 0;
		this->srcRect.w = 512;
		this->srcRect.h = 512;

		destRect.w = 64;
		destRect.h = 64;
	}

	void update() override {
		this->destRect.x = (int)this->transform->position.x;
		this->destRect.y = (int)this->transform->position.y;
	}

	void draw() override {
		TextureManager::draw(this->texture, this->srcRect, this->destRect);
	}

};

#endif // !SPRITECOMPONENT_H