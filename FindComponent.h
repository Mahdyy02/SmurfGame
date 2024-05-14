#ifndef FINDCOMPONENT_H
#define FINDCOMPONENT_H

#include "ECS.h"
#include "Components.h"

class FindComponent : public Component {

public:

	void find() {
		this->found = true;
	}

	void setSpawnedInMap() {
		this->spawnedInMainMap = true;
	}

	void spawnInsideHouse() const {
		if (this->found) {
			entity->getComponent<TransformComponent>().position = Vector2D(7412, 5142);
		}
	}

	void spawnInMainMap(Vector2D vec) {
		if (this->found && !this->spawnedInMainMap) {
			this->spawnedInMainMap = true;
			entity->getComponent<TransformComponent>().position = vec + Vector2D(-32 * entity->getComponent<TransformComponent>().scale, 32 * entity->getComponent<TransformComponent>().scale);
		}
	}

	void reset() {
		this->found = false;
		this->spawnedInMainMap = false;
	}

	bool isFound() const { return this->found; }
	bool isSpawnedInMainMap() const { return this->spawnedInMainMap; }

private:
	bool found = false;
	bool spawnedInMainMap = false;

};

#endif // !FINDCOMPONENT_H
