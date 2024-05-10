#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "Game.h"
#include <iostream>
#include "AssetManager.h"

class Sound {

public:

	bool playing = false;

	Sound(std::string id, float volume);
	~Sound();

	bool isPlaying() { return this->playing; }
	void setSound(std::string id);
	void play(int times);
	void stop();
	void setVolume(float v);

private:
	Mix_Chunk* sound = nullptr;
	int channel;
	float volume;
	static int getNextChannelID();

};

#endif // !SOUND_H
