#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include "ECS.h"
#include "Components.h"
#include "Sound.h"
#include <iostream>

class SoundComponent : public Component {
public:

    std::map < std::string, Sound*> sounds;

	SoundComponent() = default;
    ~SoundComponent() {
        this->sounds.clear();
    }

    void update() override {
        if (this->entity->hasComponent<TransformComponent>()) {
            auto& transform = this->entity->getComponent<TransformComponent>();
            if (transform.velocity.x != 0 || transform.velocity.y != 0) {
                this->sounds["walk"]->play(-1);
            }
            else {
                this->sounds["walk"]->stop();
            }
        }
    }

	void addSound(std::string id, std::string sound_id) {
		Sound* s = new Sound(sound_id, 0.5);
		this->sounds[id] = s;
	}

};

#endif // !SOUNDCOMPONENT_H
