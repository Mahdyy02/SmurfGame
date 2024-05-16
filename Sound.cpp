#include "Sound.h"
#include "Game.h"

Sound::Sound(std::string id, float volume) {

	this->sound = Game::assets->getSound(id);

	this->channel = getNextChannelID();

	if (this->sound == nullptr) {
		throw std::runtime_error("Sound with ID '" + id + "' not found in AssetManager");
	}

	this->volume = volume;
}

void Sound::play(int times) {
	if (!this->playing) {
		this->playing = true; 

		Mix_PlayChannel(this->channel, this->sound, times);
		Mix_VolumeChunk(this->sound, static_cast<int>(MIX_MAX_VOLUME * this->volume));
	}
}


void Sound::stop() {
	this->playing = false;
	Mix_HaltChannel(this->channel);
}

void Sound::setVolume(float v) {
	this->volume =v;
}

int Sound::getNextChannelID() {
	static int firstChannel = 0;
	return firstChannel++;
}