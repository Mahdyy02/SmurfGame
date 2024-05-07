#ifndef ANIMATION_H
#define ANIMATION_H

struct Animation {
	int index;
	int frames;
	int speed;

	Animation(){}

	Animation(int i, int f, int s) {
		index = i;
		frames = f;
		speed = s;
	}
};

#endif 
