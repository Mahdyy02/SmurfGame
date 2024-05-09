#include "Game.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include <sstream>
#include "Sound.h"

int Game::screenWidth = 1500;
int Game::screenHeight = 800;

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera ={0, 0, 8192*1, 4096 *1 };

AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());
auto& health(manager.addEntity());

auto& label(manager.addEntity());
auto& labelHP(manager.addEntity());

Map* map;
Sound* backgroundMusic;
Sound* bluePotionSound;
Sound* redPotionSound;

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

	assets->addSound("redPotionSound", "red_potion.wav");
	assets->addSound("bluePotionSound", "blue_potion.wav");
	
	map = new Map("terrain", 1, 32);

	map->loadMap("map.map", 256, 128);

	player.addComponent<TransformComponent>(470,2327,32,32,3);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addComponent<SoundComponent>();
	player.getComponent<SoundComponent>().addSound("walk","walk");
	player.addGroup(groupPlayers);

	health.addComponent<HealthComponent>(100);
	health.addComponent<TransformComponent>(470 , 2307 ,16,100,1);
	health.addComponent<SpriteComponent>("HP100", false);
	

	SDL_Color white = { 255,255,255, 255 };
	label.addComponent<UILabel>(10, 10, "SaneferPawPaw", "arial", white, true);
	label.addGroup(Game::groupLabels);

	labelHP.addComponent<UILabel>(10, 30, "SaneferPawPaw", "arial", white, true);

	assets->createProjectile(Vector2D(640, 2400), Vector2D(1,0), 1, 0, "projectile");

	assets->createProjectile(Vector2D(400,2550),Vector2D(0,0),1,0,"redPotion",1);
	assets->createProjectile(Vector2D(550, 2550), Vector2D(0, 0), 1, 0, "bluePotion", 2);

	backgroundMusic = new Sound("background", 0.1);
	backgroundMusic->play(-1);

	redPotionSound = new Sound("redPotionSound", 1);
	bluePotionSound = new Sound("bluePotionSound", 1);
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
			std::cout << "Collision!" << std::endl;
			p->destroy();
		}
	}

	for (auto& r : redPotions) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, r->getComponent<ColliderComponent>().collider)) {
			health.getComponent<HealthComponent>().increaseHP(30);
			redPotionSound->play(0);
			r->destroy();
		}
	}

	for (auto& b : bluePotions) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, b->getComponent<ColliderComponent>().collider)) {
			health.getComponent<HealthComponent>().increaseHP(60);
			bluePotionSound->play(0);
			b->destroy();
		}
	}



	camera.x = player.getComponent<TransformComponent>().position.x - Game::screenWidth/2;
	camera.y = player.getComponent<TransformComponent>().position.y - Game::screenHeight/2;


	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0; 
	if (camera.x > camera.w - screenWidth) camera.x = camera.w - screenWidth;
	if (camera.y > camera.h - screenHeight) camera.y = camera.h - screenHeight;

	if (health.getComponent<HealthComponent>().getHealth() == 0) isRunning = false;
	

	std::stringstream playerHp; 
	playerHp << "Player HP: " << health.getComponent<HealthComponent>().getHealth();
	labelHP.getComponent<UILabel>().setLabelText(std::move(playerHp).str(), "arial");

	health.getComponent<TransformComponent>().position.x = playerPos.x;
	health.getComponent<TransformComponent>().position.y = playerPos.y - 30;

	int hp = health.getComponent<HealthComponent>().getHealth();

	if (hp > 90) {
		health.getComponent<SpriteComponent>().setTex("HP100");
	}
	else if (hp >70 ) {
		health.getComponent<SpriteComponent>().setTex("HP80");
	}
	else if ( hp > 50 ) {
		health.getComponent<SpriteComponent>().setTex("HP60");
	}
	else if ( hp > 30 ) {
		health.getComponent<SpriteComponent>().setTex("HP40");
	}
	else if ( hp > 15 ) {
		health.getComponent<SpriteComponent>().setTex("HP20");
	}
	else if ( hp > 5) {
		health.getComponent<SpriteComponent>().setTex("HP10");
	}
}

void Game::render() {
	SDL_RenderClear(this->renderer);

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

	label.draw();
	labelHP.draw();

	health.draw();

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