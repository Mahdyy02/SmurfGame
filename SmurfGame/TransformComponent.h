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

	int speed = 1;

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

};

#endif // !TransformComponent_H