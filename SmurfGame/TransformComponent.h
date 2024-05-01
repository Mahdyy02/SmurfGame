#ifndef TransformComponent_H
#define TransformComponent_H

#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component {
public:
	
	Vector2D position;

	TransformComponent() {
		this->position.x = 0.0;
		this->position.y = 0.0;
	}

	TransformComponent(float x, float y) {
		this->position.x = x;
		this->position.y = y;
	}

	void update() override {
		
	}

};

#endif // !TransformComponent_H