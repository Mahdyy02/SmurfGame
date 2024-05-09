#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include "ECS.h"
#include "Components.h"
#include <iostream>

class HealthComponent : public Component
{
public:
	HealthComponent(int hp) {
		this->health = hp;
	}
	~HealthComponent(){}

	void decreaseHP(int amount) {
		this->health = this->health - amount < 0 ? 0 : this->health - amount ;
	}

	void increaseHP(int amount) {
		this->health = this->health + amount > 100 ? 100 : this->health + amount ;
		// Optionally, you can set a maximum HP limit here if needed
	}

	void update() override {
		cnt++;
		if (cnt % 100 == 0) decreaseHP(1);
	}

	int getHealth() { return this->health;}

private:
	int health;
	int cnt = 0 ;
};


#endif // !1


