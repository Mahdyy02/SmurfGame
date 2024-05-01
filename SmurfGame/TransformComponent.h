#ifndef TransformComponent_H
#define TransformComponent_H

#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component {
public:
	
	Vector2D position;
	Vector2D velocity;

	int speed = 3;

	TransformComponent() {
		this->position.x = 0.0;
		this->position.y = 0.0;
	}

	TransformComponent(float x, float y) {
		this->position.x = x;
		this->position.y = y;
	}

	void init() override {
		velocity.x = 0;
		velocity.y = 0;
	}

	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};

#endif // !TransformComponent_H