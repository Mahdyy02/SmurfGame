#include "Game.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include <sstream>
#include "Sound.h"

int Game::screenWidth = 1500;
int Game::screenHeight = 800;

int minX = 0;
int minY = 0;
int posX = -1;
int posY = -1;

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, 8192, 4096 };

AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());

auto& bug(manager.addEntity());

auto& label(manager.addEntity());
auto& labelHP(manager.addEntity());
auto& labelHouse(manager.addEntity());

Map* map;

Sound* backgroundMusic;
Sound* bluePotionSound;
Sound* redPotionSound;
Sound* hitSound;
Sound* doorSound;

bool Game::isRunning = false;


Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialised!..." << std::endl;
		this->window = SDL_CreateWindow(title, xpos, ypos, screenWidth, screenHeight, flags);

		if (this->window) {
			std::cout << "Window created succesfully..." << std::endl;
		}

		this->renderer = SDL_CreateRenderer(window, -1, 0);
		if (this->renderer) {
			SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
			std::cout << "Renderer created..." << std::endl;
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "Error: SDL_TTF" << std::endl;
	}

	Mix_Init(MIX_INIT_MP3);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	assets->addTexture("terrain", "map.png");
	assets->addTexture("player", "animated_smurf.png");
	assets->addTexture("projectile", "charchabil.png");
	assets->addTexture("collider", "empty.png");

	assets->addTexture("HP10", "hp_10.png");
	assets->addTexture("HP20", "hp_20.png");
	assets->addTexture("HP40", "hp_40.png");
	assets->addTexture("HP60", "hp_60.png");
	assets->addTexture("HP80", "hp_80.png");
	assets->addTexture("HP100", "hp_100.png");

	assets->addTexture("redPotion", "red_potion.png");
	assets->addTexture("bluePotion", "blue_potion.png");

	assets->addFont("arial", "arial.ttf", 16);

	assets->addSound("background", "background_music.wav");
	assets->addSound("walk", "walk.wav");
	assets->addSound("hit", "hit.wav");
	assets->addSound("door", "door.wav");

	assets->addSound("redPotionSound", "red_potion.wav");
	assets->addSound("bluePotionSound", "blue_potion.wav");
	
	map = new Map("terrain", 1, 32, 8192, 4096);

	map->loadMap("map.map", 256, 173);

	player.addComponent<TransformComponent>(470,2327,32,32,3);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addComponent<HealthComponent>(100);
	player.addComponent<SoundComponent>();
	player.getComponent<SoundComponent>().addSound("walk","walk");
	player.addGroup(groupPlayers);

	bug.addComponent<TransformComponent>(570, 2200, 32, 32, 3);
	bug.addComponent<SpriteComponent>("player", true);

	SDL_Color white = { 255,255,255, 255 };
	SDL_Color black = { 0, 0, 0, 255 };
	label.addComponent<UILabel>(10, 10, "", "arial", white, true);
	label.addGroup(Game::groupLabels);

	labelHouse.addComponent<UILabel>(5950, 4880, "Press Y to leave", "arial", black, false, 100);

	labelHP.addComponent<UILabel>(10, 30, "", "arial", white, true);

	assets->createProjectile(Vector2D(640, 2400), Vector2D(1,0), 1, 0, "projectile");


	backgroundMusic = new Sound("background", 0.1);
	backgroundMusic->play(-1);

	redPotionSound = new Sound("redPotionSound", 1);
	bluePotionSound = new Sound("bluePotionSound", 1);
	hitSound = new Sound("hit", 1);
	doorSound = new Sound("door", 1);
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& labels(manager.getGroup(Game::groupLabels));

auto& redPotions(manager.getGroup(Game::groupeRedPotions));
auto& bluePotions(manager.getGroup(Game::groupeBluePotions));

void Game::handleEvents() {
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		this->isRunning = false;
		break;
	default:
		break;
	}

}

void Game::update() {

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	bug.getComponent<TransformComponent>().chase(playerPos);

	std::stringstream ss;
	ss << "Player position: (" << playerPos.x << ", " << playerPos.y << ")";
	label.getComponent<UILabel>().setLabelText(std::move(ss).str(), "arial");

	
	manager.refresh();
	manager.update();

	for (auto& c : colliders) {

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		Collision::AABB(cCol, playerCol);
		if(Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
			player.getComponent<HealthComponent>().decreaseHP(50);
			hitSound->play(0);
			hitSound->playing = false;
			p->destroy();
		}
	}

	for (auto& r : redPotions) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, r->getComponent<ColliderComponent>().collider)) {
			player.getComponent<HealthComponent>().increaseHP(30);
			redPotionSound->play(0);
			redPotionSound->playing = false;
			r->destroy();
		}
	}

	for (auto& b : bluePotions) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, b->getComponent<ColliderComponent>().collider)) {
			player.getComponent<HealthComponent>().increaseHP(60);
			bluePotionSound->play(0);
			bluePotionSound->playing = false;
			b->destroy();
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - Game::screenWidth/2;
	camera.y = player.getComponent<TransformComponent>().position.y - Game::screenHeight/2;


	if (camera.x < minX) camera.x = minX;
	if (camera.y < minY) camera.y = minY; 
	if (camera.x > camera.w - screenWidth) camera.x = camera.w - screenWidth;
	if (camera.y > camera.h - screenHeight) camera.y = camera.h - screenHeight;

	if (player.getComponent<HealthComponent>().getHealth() == 0) isRunning = false;
	

	std::stringstream playerHp; 
	playerHp << "Player HP: " << player.getComponent<HealthComponent>().getHealth();
	labelHP.getComponent<UILabel>().setLabelText(std::move(playerHp).str(), "arial");
	
	for (auto& h : labels) {
		player.getComponent<TransformComponent>().isNearhouse = h->getComponent<UILabel>().inRange(player.getComponent<TransformComponent>().position);
		if (player.getComponent<TransformComponent>().isNearhouse) break;
	}

	if (player.getComponent<TransformComponent>().isNearhouse) {
		if (Game::event.type == SDL_KEYDOWN) {
			if (Game::event.key.keysym.sym == SDLK_e) {

				posX = player.getComponent<TransformComponent>().position.x;
				posY = player.getComponent<TransformComponent>().position.y;

				doorSound->play(0);
				doorSound->playing = false;

				minX = 5632;
				minY = 4096;

				player.getComponent<TransformComponent>().position.x = 6200;
				player.getComponent<TransformComponent>().position.y = 5000;

				camera.x = player.getComponent<TransformComponent>().position.x - Game::screenWidth / 2;
				camera.y = player.getComponent<TransformComponent>().position.y - Game::screenHeight / 2;

				Game::camera.w = 8192; 
				Game::camera.h = 5536; 

				player.getComponent<TransformComponent>().scale = 6;
			}
		}
	}

	if (Game::event.key.keysym.sym == SDLK_y) {

		if (posX != -1 && posY != -1 && labelHouse.getComponent<UILabel>().inRange(player.getComponent<TransformComponent>().position)) {

			minX = 0;
			minY = 0;

			player.getComponent<TransformComponent>().position.x = posX;
			player.getComponent<TransformComponent>().position.y = posY;

			doorSound->play(0);
			doorSound->playing = false;

			Game::camera.x = player.getComponent<TransformComponent>().position.x - (Game::screenWidth / 2);
			Game::camera.y = player.getComponent<TransformComponent>().position.y - (Game::screenHeight / 2);

			Game::camera.w = 8192; 
			Game::camera.h = 4096; 

			posX = -1;
			posY = -1;

			Game::camera = { 0, 0, 8192, 4096 };

			player.getComponent<TransformComponent>().scale = 3;

		}
	}
}

void Game::render() {
	SDL_RenderClear(this->renderer);

	if (camera.x == 0 && camera.y == 0) {
		camera.x = player.getComponent<TransformComponent>().position.x - Game::screenWidth / 2;
		camera.y = player.getComponent<TransformComponent>().position.y - Game::screenHeight / 2;
	}

	for (auto& t : tiles) {
		t->draw();
	}
	for (auto& p : players) {
		p->draw();
	}

	for (auto& c : colliders) {
		c->draw();
	}

	for (auto& p : projectiles) {
		p->draw();
	}

	for (auto& l : labels) {
		l->draw();
	}

	bug.draw();

	label.draw();
	labelHP.draw();
	labelHouse.draw();

	for (auto& r: redPotions)
	{
		r->draw();
	}

	for (auto& b : bluePotions)
	{
		b->draw();
	}

	SDL_RenderPresent(this->renderer);
}


void Game::clean() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}