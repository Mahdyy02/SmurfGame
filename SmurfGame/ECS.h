#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Entity;
class Component;

using ComponentID = std::size_t;
inline ComponentID getComponentTypeID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

class Entity {
private:
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;

public:
    void update() {
        for (auto& c : this->components) {
            c->update();
        }
    }

    void draw() {}

    bool isActive() const { return this->active; }

    void destroy() { this->active = false; }

    template <typename T>
    bool hasComponent() const {
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... tArgs>
    T& addComponent(tArgs&&... mArgs) {
        T* c = new T(std::forward<tArgs>(mArgs)...);
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T>
    T& getComponent() const {
        auto ptr = componentArray[getComponentTypeID<T>()];
        return *static_cast<T*>(ptr);
    }
};

class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;

public:
    void update() {
        for (auto& e : this->entities) {
            e->update();
        }
    }

    void draw() {
        for (auto& e : this->entities) {
            e->draw();
        }
    }

    void refresh() {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [](const std::unique_ptr<Entity>& mEntity) {
                return !mEntity->isActive();
            }), entities.end());
    }

    Entity& addEntity() {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};

#endif