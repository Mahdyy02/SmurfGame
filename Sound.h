#ifndef MUSICCOMPONENT_H
#define MUSICCOMPONENT_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "Game.h"

class Sound {

public:

	Sound();
	~Sound();

	bool isPlaying() { return this->playing; }

	void play(int times) {
		if (!this->playing) {
			this->playing = true;
			Mix_PlayMusic(this->sound, times);
		}
	}

	void stop() {
		this->playing = false;
	}

	void setMusic(std::string id) {
		this->sound = Game::assets->getSound(id);
	}


private:
	bool playing = false;
	Mix_Music* sound;
};

#endif // !MUSICCOMPONENT_H
