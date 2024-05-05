#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <iostream>
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component {
public:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(std::string id) {
		this->setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated) {
		
		this->animated = isAnimated;

		Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);

		this->animations.emplace("idle", idle);
		this->animations.emplace("walk", walk); 

		this->play("idle");

		this->setTex(id);
	}

	~SpriteComponent() {}

	void setTex(std::string id) {
		texture = Game::assets->getTexture(id);
	}

	void init() override {

		transform = &entity->getComponent<TransformComponent>();

		this->srcRect.x = 0;
		this->srcRect.y = 0;
		this->srcRect.w = 512;
		this->srcRect.h = 512;
	}

	void update() override {

		if (this->animated) {
			this->srcRect.x = this->srcRect.w * static_cast<int>((SDL_GetTicks() / this->speed)%this->frames);
		}

		this->srcRect.y = this->animIndex * 512;

		this->destRect.x = static_cast<int>(this->transform->position.x);
		this->destRect.y = static_cast<int>(this->transform->position.y);

		destRect.w = this->transform->width * this->transform->scale;
		destRect.h = this->transform->height * this->transform->scale;

	}

	void draw() override {
		TextureManager::draw(this->texture, this->srcRect, this->destRect, this->spriteFlip);
	}

	void play(const char* animName) {
		this->frames = this->animations[animName].frames;
		this->animIndex = this->animations[animName].index;
		this->speed = this->animations[animName].speed;
	}

};

#endif // !SPRITECOMPONENT_H