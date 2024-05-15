#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;


	void init() override {
		this->transform = &entity->getComponent<TransformComponent>();
		this->sprite = &entity->getComponent<SpriteComponent>();
	}

    void update() override {
        static SDL_Keycode currentKey = SDLK_UNKNOWN;

        if (Game::paused || !Game::isRunning) currentKey = SDLK_UNKNOWN;

        if (Game::event.type == SDL_KEYDOWN) {
            if (currentKey == SDLK_UNKNOWN) {
                switch (Game::event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_z:
                    currentKey = SDLK_UP;
                    transform->velocity.x = 2;
                    transform->velocity.y = -1;
                    sprite->play("walk");
                    break;
                case SDLK_LEFT:
                case SDLK_q:
                    currentKey = SDLK_LEFT;
                    transform->velocity.x = -2;
                    transform->velocity.y = -1;
                    sprite->play("walk");
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    currentKey = SDLK_DOWN;
                    transform->velocity.x = -2;
                    transform->velocity.y = 1;
                    sprite->play("walk");
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    currentKey = SDLK_RIGHT;
                    transform->velocity.x = 2;
                    transform->velocity.y = 1;
                    sprite->play("walk");
                    break;
                default:
                    break;
                }
            }
        }

        if (Game::event.type == SDL_KEYUP) {
            switch (Game::event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_z:
                if (currentKey == SDLK_UP) {
                    currentKey = SDLK_UNKNOWN;
                    transform->velocity.x = 0;
                    transform->velocity.y = 0;
                    sprite->play("idle");
                }
                break;
            case SDLK_LEFT:
            case SDLK_q:
                if (currentKey == SDLK_LEFT) {
                    currentKey = SDLK_UNKNOWN;
                    transform->velocity.x = 0;
                    transform->velocity.y = 0;
                    sprite->play("idle");
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                if (currentKey == SDLK_DOWN) {
                    currentKey = SDLK_UNKNOWN;
                    transform->velocity.x = 0;
                    transform->velocity.y = 0;
                    sprite->play("idle");
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                if (currentKey == SDLK_RIGHT) {
                    currentKey = SDLK_UNKNOWN;
                    transform->velocity.x = 0;
                    transform->velocity.y = 0;
                    sprite->play("idle");
                }
                break;
            default:
                break;
            }
        }
    }
};

#endif // KEYBOARDCONTROLLER_H

