#ifndef TransformComponent_H
#define TransformComponent_H

#include "Components.h"
#include "Vector2D.h"
#include <math.h>

class TransformComponent : public Component {
public:
	
	Vector2D position;
	Vector2D velocity;
	Vector2D previousPosition = Vector2D(-1, -1);

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 2;

	int isNearhouse = false;

	TransformComponent() {
		this->position.zero();
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
		this->position.zero();
		this->scale = sc;
	}

	void init() override {
		this->velocity.zero();
	}

	void update() override {
		this->position.x += this->velocity.x * this->speed;
		this->position.y += this->velocity.y * this->speed;

	}

	void chase(Vector2D target) {
		if (sqrt(pow(target.x - this->position.x, 2) + pow(target.y - this->position.y, 2)) <= 750 &&
			sqrt(pow(target.x - this->position.x, 2) + pow(target.y - this->position.y, 2)) >= 32*this->scale) {
			Vector2D direction = target - this->position + Vector2D(-32 * this->scale, 0);
			direction.normalize();
			this->velocity = direction;
		}
		else {
			this->velocity.zero();
		}
	}

};

#endif // !TransformComponent_H