#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component {
public:
	ProjectileComponent(int rng, int sp, Vector2D vel , int isPo): range(rng), speed(sp), velocity(vel) , isPotion(isPo) {}
	~ProjectileComponent(){}

	void init() override {
		this->transform = &this->entity->getComponent<TransformComponent>();
		this->transform->velocity = this->velocity;
		this->transform->speed = 3;
	}

	void update() override {
	}

private:
	TransformComponent* transform;

	int range;
	int speed;
	int distance;
	bool isPotion;

	Vector2D velocity;
};

#endif // !PROJECTILE_H
