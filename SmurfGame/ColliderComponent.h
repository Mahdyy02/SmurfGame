#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include <SDL.h>
#include "Components.h"

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;
	TransformComponent *transform;

	ColliderComponent(std::string t) {
		this->tag = t;
	}

	void init() override {
		if (!this->entity->hasComponent<TransformComponent>()) {
			this->entity->addComponent<TransformComponent>();
		}
		this->transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		this->collider.x = static_cast<int>(this->transform->position.x);
		this->collider.y = static_cast<int>(this->transform->position.y);
		this->collider.w = this->transform->width * this->transform->scale;
		this->collider.h = this->transform->height * this->transform->scale;
	}
};

#endif // !COLLIDERCOMPONENT_H
