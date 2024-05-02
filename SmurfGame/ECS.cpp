#include "ECS.h"

void Entity::addGroup(Group mGroup) {
	this->groupBitSet[mGroup] = true;
	this->manager.addToGroup(this, mGroup);
}