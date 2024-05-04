#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include <SDL.h>
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;
	TransformComponent *transform;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	ColliderComponent(std::string t) {
		this->tag = t;
	}

	ColliderComponent(std::string t, int xPos, int yPos, int size) {
		this->tag = t;
		this->collider.x = xPos;
		this->collider.y = yPos;
		this->collider.w = this->collider.h = size;
	}

	void init() override {
		if (!this->entity->hasComponent<TransformComponent>()) {
			this->entity->addComponent<TransformComponent>();
		}
		this->transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::loadTexture("test.png");
		this->srcR = {0,0,512,512};
		this->destR = { collider.x, collider.y, collider.w, collider.h };
	}

	void update() override {
		if (tag != "terrain") {
			this->collider.x = static_cast<int>(this->transform->position.x);
			this->collider.y = static_cast<int>(this->transform->position.y);
			this->collider.w = this->transform->width * this->transform->scale;
			this->collider.h = this->transform->height * this->transform->scale;
		}
		this->destR.x = this->collider.x - Game::camera.x;
		this->destR.y = this->collider.y - Game::camera.y;
	}

	void draw() {
		TextureManager::draw(tex, srcR, destR, SDL_FLIP_NONE);
	}
};

#endif // !COLLIDERCOMPONENT_H
