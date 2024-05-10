#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager* man) : manager(man) {}

AssetManager::~AssetManager(){}

void AssetManager::addTexture(std::string id, const char* path) {
	this->textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::getTexture(std::string id) {
	return this->textures[id];
}

void AssetManager::createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id ,int potionType ) {
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel,potionType );
	projectile.addComponent<ColliderComponent>("projectile");

	switch ( potionType )
	{
	case 0 : 
		projectile.addGroup(Game::groupProjectiles);
		break;
	case 1 :
		projectile.addGroup(Game::groupeRedPotions);
		break;
	case 2:
		projectile.addGroup(Game::groupeBluePotions);
		break;
	default:
		break;
	}
}

void AssetManager::addFont(std::string id, std::string path, int fontSize) {
	this->fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(std::string id) {
	return this->fonts[id];
}

void AssetManager::addSound(std::string id, std::string path) {
	this->sounds.emplace(id, Mix_LoadWAV(path.c_str()));
}

Mix_Chunk* AssetManager::getSound(std::string id) {
	return this->sounds[id];
}