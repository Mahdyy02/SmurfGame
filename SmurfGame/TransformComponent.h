#ifndef TransformComponent_H
#define TransformComponent_H

#include "Components.h"
#include "Vector2D.h"
#include <math.h>

class TransformComponent : public Component {
public:
	
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 3;

	TransformComponent() {
		this->position.x = 0.0;
		this->position.y = 0.0;
	}

	TransformComponent(float x, float y) {
		this->position.x = x;
		this->position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc) {
		this->position.x = x;
		this->position.y = y;
		this->height = h;
		this->width = w;
		this->scale = sc;
	}

	TransformComponent(int sc) {
		this->position.x = 0.0;
		this->position.y = 0.0;
		this->scale = sc;
	}

	void init() override {
		velocity.x = 0;
		velocity.y = 0;
	}

	void update() override {

		float tmpSpeed = (float)speed;

		if (velocity.x != 0 && velocity.y != 0) {
			tmpSpeed /= sqrt(2);
		}

		position.x += velocity.x * tmpSpeed;
		position.y += velocity.y * tmpSpeed;
	}

};

#endif // !TransformComponent_H