#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "Components.h"

class PositionComponent : public Component {
private:
	int xpos;
	int ypos;

public:

	PositionComponent() {
		this->xpos = 0;
		this->ypos = 0;
	}

	PositionComponent(int x, int y) {
		this->xpos = x;
		this->ypos = y;
	}

	void update() override {
		this->xpos++;
		this->ypos++;
	}

	int x() { return this->xpos; }
	void x(int x) { this->xpos = x; }
	int y() { return this->ypos; }
	void y(int y) { this->ypos = y; }

	void setPos(int x, int y) {
		this->xpos = x;
		this->ypos = y;
	}

};

#endif // !POSITIONCOMPONENT_H