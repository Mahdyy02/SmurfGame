#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL.h>
#include "ECS.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include <iostream>
#include <string>
#include <cstring>

class TileComponent : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	SDL_Rect tileRect;
	int tileID;
	const char* path;

	TileComponent() = default;

	~TileComponent() {
		delete[] this->path;
	}

	TileComponent(int x, int y, int w, int h, int id) {
		this->tileRect.x = x;
		this->tileRect.y = y;
		this->tileRect.w = w;
		this->tileRect.h = h;
		this->tileID = id;

		std::string pathStr = "map/" + std::to_string(id) + ".png";
		path = new char[pathStr.length() + 1];
		strcpy_s(const_cast<char*>(path), pathStr.length() + 1, pathStr.c_str());
	}

	void init() override {
		this->entity->addComponent<TransformComponent>((float)this->tileRect.x, (float)this->tileRect.y, this->tileRect.w, this->tileRect.h, 1);
		this->transform = &this->entity->getComponent<TransformComponent>();
		this->entity->addComponent<SpriteComponent>(this->path);
		this->sprite = &this->entity->getComponent<SpriteComponent>();
	}
};

#endif // !TILECOMPONENT_H
