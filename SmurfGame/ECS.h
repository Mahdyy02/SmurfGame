#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;
inline ComponentID getNewComponentTypeID() {
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
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
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

public:
    Entity(Manager& mManager) : manager(mManager){}
    void update() {
        for (auto& c : this->components) {
            c->update();
        }
    }

    void draw() {
        for (auto& c : this->components) {
            c->draw();
        }
    }

    bool isActive() const { return this->active; }

    void destroy() { this->active = false; }

    bool hasGroup(Group mGroup) { return this->groupBitSet[mGroup]; }

    void addGroup(Group mGroup);

    void delGroup(Group mGroup) {
        this->groupBitSet[mGroup] = false;
    }

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
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

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

        for (auto i(0u); i < maxGroups; i++) {
            auto& v(groupedEntities[i]);
            v.erase(
                std::remove_if(
                    std::begin(v),
                    std::end(v),
                    [i](Entity* mEntity) {
                        return !mEntity->isActive() || !mEntity->hasGroup(i); 
                    }
                ), std::end(v)
            );
        }


        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [](const std::unique_ptr<Entity> &mEntity) {
                return !mEntity->isActive();
            }), entities.end());
    }

    void addToGroup(Entity* mEntity, Group mGroup) {
        this->groupedEntities[mGroup].emplace_back(mEntity);
    }

    std::vector<Entity*>& getGroup(Group mGroup) {
        return groupedEntities[mGroup];
    }

    Entity& addEntity() {
        Entity* e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};

#endif