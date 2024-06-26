#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class AssetManager {
public:
	AssetManager(Manager* man);
	~AssetManager();

	void createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id , int potionType = 0, bool animated = false, int scale = 2);

	void addTexture(std::string id, const char* path);
	SDL_Texture* getTexture(std::string id);
	
	void addFont(std::string id, std::string path, int fontSize);
	TTF_Font* getFont(std::string id);

	void addSound(std::string id, std::string path);
	Mix_Chunk* getSound(std::string id);

private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Chunk*> sounds;
	Manager* manager;
};

#endif // !ASSETMANAGER_H
