#ifndef COMPONENTS_H
#define	COMPONENTS_H

#include "ECS.h"

class PositionComponent: public Component {
private:
	int xpos;
	int ypos;

public:
	int x() { return this->xpos; }
	int y() { return this->ypos; }

	void init() {
		this->xpos = 0;
		this->ypos = 0;
	}

	void update() override {
		xpos++;
		ypos++;
	}

	void setPos(int x, int y) {
		this->xpos = x;
		this->ypos = y;
	}

};

#endif // !COMPONENTS_H

