#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include "ECS.h"
#include "Components.h"
#include <iostream>
#include "Game.h"

class HealthComponent : public Component
{
public:

	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	HealthComponent(int hp) {
		this->health = hp;
	}
	~HealthComponent(){}

	void init() override {
		this->transform = &this->entity->getComponent<TransformComponent>();

		this->srcRect.x = 0;
		this->srcRect.y = 0;
		this->srcRect.w = 500;
		this->srcRect.h = 80;
		this->texture = Game::assets->getTexture("HP100");
	}

	void update() override {

		this->timesteps++;
		if (this->timesteps % 500 == 0) {
			decreaseHP(1);
		}

		this->destRect.x = static_cast<int>(this->transform->position.x) - Game::camera.x;
		this->destRect.y = static_cast<int>(this->transform->position.y) - 10 * this->transform->scale - Game::camera.y;

		this->destRect.w = 34 * this->transform->scale;
		this->destRect.h = 6 * this->transform->scale;

		if (this->health > 90) {
			this->texture = Game::assets->getTexture("HP100");
		}
		else if (this->health > 70) {
			this->texture = Game::assets->getTexture("HP80");
		}
		else if (this->health > 50) {
			this->texture = Game::assets->getTexture("HP60");
		}
		else if (this->health > 30) {
			this->texture = Game::assets->getTexture("HP40");
		}
		else if (this->health > 15) {
			this->texture = Game::assets->getTexture("HP20");
		}
		else if (this->health > 5) {
			this->texture = Game::assets->getTexture("HP10");
		}

	}

	void draw() override{
		TextureManager::draw(this->texture, this->srcRect, this->destRect, SDL_FLIP_NONE);
	}

	void decreaseHP(int amount) {
		this->health = this->health - amount < 0 ? 0 : this->health - amount;
	}

	void increaseHP(int amount) {
		this->health = this->health + amount > 100 ? 100 : this->health + amount;
	}

	int getHealth() { return this->health;}

private:
	int health;
	int timesteps = 0 ;
};


#endif // !1


