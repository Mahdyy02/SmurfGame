#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component {
public:
	ProjectileComponent(int rng, int sp, Vector2D vel): range(rng), speed(sp), velocity(vel) {}
	~ProjectileComponent(){}

	void init() override {
		this->transform = &this->entity->getComponent<TransformComponent>();
		this->transform->velocity = this->velocity;
	}

	void update() override {

		distance += speed;

		if (this->distance > this->range) {
			this->entity->destroy();
		}
		else if (this->transform->position.x > Game::camera.x + Game::camera.w ||
				this->transform->position.x < Game::camera.x ||
				this->transform->position.y > Game::camera.y + Game::camera.h ||
				this->transform->position.y < Game::camera.y) 
		{
			this->entity->destroy();
		}
	}

private:
	TransformComponent* transform;

	int range;
	int speed;
	int distance;

	Vector2D velocity;
};

#endif // !PROJECTILE_H
