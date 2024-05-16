#include <sstream>
#include "Sound.h"
#include <math.h>
#include <cstdlib>
#include <ctime>

#include "Game.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

Uint32 startTime;

int Game::screenWidth = 1800;
int Game::screenHeight = 900;
int Game::gameLevel = 0;
int Game::levelSpeeds[3] = { 1, 1, 2 };
float Game::redPotionsProbabilities[3] = {0.4, 0.25, 0.3};
float Game::bluePotionsProbabilities[3] = {0.5, 0.15, 0.2};

int minX = 0;
int minY = 0;

bool Game::windowRunning = true;
bool Game::isRunning = false;
bool Game::win = false;
bool Game::gamePlayed;
bool Game::paused = false;

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, 8192, 4096 };

AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());
auto& smurfEngineer(manager.addEntity());
auto& smurfFemale(manager.addEntity());

auto& label(manager.addEntity());
auto& labelHP(manager.addEntity());
auto& labelHouse(manager.addEntity());
auto& labelMessage(manager.addEntity());
auto& labelWalkieTalkie(manager.addEntity());
auto& labelGuide(manager.addEntity());

Map* map;

Sound* backgroundMusic;
Sound* bluePotionSound;
Sound* redPotionSound;
Sound* hitSound;
Sound* doorSound;
Sound* walkieTalkieSound;
Sound* storyOne;
Sound* storyTwo;
Sound* storyThree;
Sound* storyFour;

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
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "Error: SDL_TTF" << std::endl;
		isRunning = false;
	}

	Mix_Init(MIX_INIT_MP3);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		isRunning = false;
	}

	if (Mix_AllocateChannels(16) == -1) {
		printf("Error allocating channels: %s\n", Mix_GetError());
	}

	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		isRunning = false;
	}

	assets->addTexture("terrain", "map.png");
	assets->addTexture("player", "animated_smurf.png");
	assets->addTexture("smurfEngineer", "animated_engineer_smurf.png");
	assets->addTexture("smurfFemale", "animated_female_smurf.png");
	assets->addTexture("cat", "cat_animated.png");
	assets->addTexture("gargamel", "animated_gargamel.png");
	assets->addTexture("walkietalkie", "walkie_talkie.png");
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
	assets->addFont("freeman", "Freeman-Regular.ttf", 40);
	assets->addFont("skincake", "Skincake.ttf", 56);

	assets->addSound("background", "background_music.wav");
	assets->addSound("walk", "walk.wav");
	assets->addSound("hit", "hit.wav");
	assets->addSound("door", "door.wav");
	assets->addSound("walkieTalkie", "walkie_talkie.wav");
	assets->addSound("redPotionSound", "red_potion.wav");
	assets->addSound("bluePotionSound", "blue_potion.wav");
	assets->addSound("story1", "story_1.wav");
	assets->addSound("story2", "story_2.wav");
	assets->addSound("story3", "story_3.wav");
	assets->addSound("story4", "story_4.wav");

	storyOne = new Sound("story1", 0.5);
	storyTwo = new Sound("story2", 0.5);
	storyThree = new Sound("story3", 0.5);
	storyFour = new Sound("story4", 0.5);


	map = new Map("terrain", 1, 32, 8192, 4096);

	map->loadMap("map.map", 256, 173);

	player.addComponent<TransformComponent>(470, 2327, 32, 32, 3);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addComponent<HealthComponent>(100);
	player.addComponent<SoundComponent>();
	player.getComponent<SoundComponent>().addSound("walk", "walk");
	player.addGroup(groupPlayers);

	smurfEngineer.addComponent<TransformComponent>(100000, 100000, 32, 32, 3);
	smurfEngineer.getComponent<TransformComponent>().speed = 4;
	smurfEngineer.addComponent<SpriteComponent>("smurfEngineer", true);
	smurfEngineer.addComponent<ColliderComponent>("smurfEngineer");
	smurfEngineer.addComponent<FindComponent>();
	smurfEngineer.addGroup(groupPlayers);

	smurfFemale.addComponent<TransformComponent>(100000, 100000, 32, 32, 3);
	smurfFemale.getComponent<TransformComponent>().speed = 4;
	smurfFemale.addComponent<SpriteComponent>("smurfFemale", true);
	smurfFemale.addComponent<ColliderComponent>("smurfFemale");
	smurfFemale.addComponent<FindComponent>();
	smurfFemale.addGroup(groupPlayers);

	assets->createProjectile(Vector2D(840, 2400), Vector2D(0, 0), 1, 0, "projectile", 3, true, 3);
	assets->createProjectile(Vector2D(2400, 2400), Vector2D(0, 0), 1, 0, "projectile", 3, true, 3);
	assets->createProjectile(Vector2D(640, 200), Vector2D(0, 0), 1, 0, "projectile", 3, true, 3);
	assets->createProjectile(Vector2D(640, 600), Vector2D(0, 0), 1, 0, "projectile", 3, true, 3);
	assets->createProjectile(Vector2D(4000, 4000), Vector2D(0, 0), 1, 0, "projectile", 3, true, 3);
	assets->createProjectile(Vector2D(4800, 560), Vector2D(0, 0), 1, 0, "projectile", 3, true, 3);
	assets->createProjectile(Vector2D(4800, 800), Vector2D(0, 0), 1, 0, "projectile", 3, true, 3);

	SDL_Color white = { 255,255,255, 255 };
	SDL_Color black = { 0, 0, 0, 255 };
	label.addComponent<UILabel>(10, 10, "", "arial", white, true);
	label.addGroup(Game::groupLabels);
	labelMessage.addComponent<UILabel>(10, 50, "Walkie Talkie Message:", "arial", black, true);
	labelWalkieTalkie.addComponent<UILabel>(10, 70, "", "arial", white, true);
	labelGuide.addComponent<UILabel>(10, 90, "Find the smurf engineer to help you decrypt the message.", "arial", black, true);
	labelHouse.addComponent<UILabel>(5950, 4880, "Press Y to leave", "arial", black, false, 100);
	labelHP.addComponent<UILabel>(10, 30, "", "arial", white, true);

	backgroundMusic = new Sound("background", 0.1);
	backgroundMusic->play(-1);

	redPotionSound = new Sound("redPotionSound", 1);
	bluePotionSound = new Sound("bluePotionSound", 1);
	hitSound = new Sound("hit", 1);
	doorSound = new Sound("door", 1);
	walkieTalkieSound = new Sound("walkieTalkie", 1);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupCats));
auto& labels(manager.getGroup(Game::groupLabels));

auto& redPotions(manager.getGroup(Game::groupRedPotions));
auto& bluePotions(manager.getGroup(Game::groupBluePotions));
auto& gargamels(manager.getGroup(Game::groupGargamel));
auto& walkieTalkies(manager.getGroup(Game::groupWalkieTalkie));

void Game::reset() {

	storyOne->play(0);
	startTime = SDL_GetTicks();

	this->SmurfEngineerHouse = rand() % 7;
	this->SmurfFemaleHouse = rand() % 7;
	while (this->SmurfEngineerHouse == this->SmurfFemaleHouse) this->SmurfFemaleHouse = rand() % 7;

	if (this->crpytographer) delete this->crpytographer;
	this->crpytographer = new Cryptographer();
	this->crpytographer->message = "Female Smurf is in house: " + std::to_string(this->SmurfFemaleHouse);


	this->playerNearHouseID = -1;
	this->gargamelSpawned = false;
	this->gargamelAlive = true;
	this->walkieTalkieFound = false;
	paused = false;

	for (auto& r : redPotions) {
		r->destroy();
	}

	for (auto& b : bluePotions) {
		b->destroy();
	}

	for (auto& c : projectiles) {
		c->destroy();
	}

	for (auto& g : gargamels) {
		g->destroy();
	}

	manager.refresh();
	manager.update();

	map->loadMap("map.map", 256, 173, true);

	assets->createProjectile(Vector2D(840, 2400), Vector2D(0, 0), 1, 0, "cat", 3, true, 3);
	assets->createProjectile(Vector2D(2400, 2400), Vector2D(0, 0), 1, 0, "cat", 3, true, 3);
	assets->createProjectile(Vector2D(640, 200), Vector2D(0, 0), 1, 0, "cat", 3, true, 3);
	assets->createProjectile(Vector2D(640, 600), Vector2D(0, 0), 1, 0, "cat", 3, true, 3);
	assets->createProjectile(Vector2D(4000, 4000), Vector2D(0, 0), 1, 0, "cat", 3, true, 3);
	assets->createProjectile(Vector2D(4800, 560), Vector2D(0, 0), 1, 0, "cat", 3, true, 3);
	assets->createProjectile(Vector2D(4800, 800), Vector2D(0, 0), 1, 0, "cat", 3, true, 3);

	assets->createProjectile(Vector2D(3746, 1217), Vector2D(0, 0), 1, 0, "walkietalkie", 5, false, 2);

	player.getComponent<TransformComponent>().reset(470, 2327);
	smurfEngineer.getComponent<TransformComponent>().reset(100000, 100000);
	smurfFemale.getComponent<TransformComponent>().reset(100000, 100000);

	smurfEngineer.getComponent<FindComponent>().reset();
	smurfFemale.getComponent<FindComponent>().reset();

	player.getComponent<HealthComponent>().reset();
	if(smurfEngineer.hasComponent<HealthComponent>()) smurfEngineer.delComponent<HealthComponent>();
	if (smurfFemale.hasComponent<HealthComponent>()) smurfFemale.delComponent<HealthComponent>();

}

void Game::handleEvents() {
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		this->isRunning = false;
		windowRunning = false;
		break;
	default:
		break;
	}

}

void Game::update() {

	Uint32 currentTime = SDL_GetTicks();
	float elapsedTime = (currentTime - startTime) / 1000.0f; 

	if (elapsedTime >= 40.0f) {
		storyOne->playing = false;
		storyTwo->play(0);
	}

	if (elapsedTime >= 80.0f) {
		storyTwo->playing = false;
		storyThree->play(0);
	}

	if (elapsedTime >= 120.0f) {
		storyThree->playing = false;
	}

	if (smurfFemale.getComponent<FindComponent>().isFound() && !storyFour->playing) {
		storyFour->play(0);
	}

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	SDL_Rect smurfEngineerCol = smurfEngineer.getComponent<ColliderComponent>().collider;
	Vector2D smurfEngineerPos = smurfEngineer.getComponent<TransformComponent>().position;

	SDL_Rect smurfFemaleCol = smurfFemale.getComponent<ColliderComponent>().collider;
	Vector2D smurfFemalePos = smurfFemale.getComponent<TransformComponent>().position;

	SDL_Rect garmagelCol;
	Vector2D gargamelPos;

	if (gargamelSpawned) {
		garmagelCol = gargamels[0]->getComponent<ColliderComponent>().collider;
		gargamelPos = gargamels[0]->getComponent<TransformComponent>().position;
	}


	smurfEngineer.getComponent<TransformComponent>().chase(playerPos, false);
	if(smurfFemale.getComponent<FindComponent>().isSpawnedInMainMap()) smurfFemale.getComponent<TransformComponent>().chase(smurfEngineerPos, false);
	else smurfFemale.getComponent<TransformComponent>().chase(playerPos, false);

	std::stringstream ss;
	ss << "Player position: (" << playerPos.x << ", " << playerPos.y << ")";
	label.getComponent<UILabel>().setLabelText(std::move(ss).str(), "arial");
	
	if (!smurfEngineer.getComponent<FindComponent>().isFound()) {
		if (!this->crpytographer->isEncrypted()) {
			this->crpytographer->encrypt();
		} 
	}
	else {
		if (this->crpytographer->isEncrypted()) this->crpytographer->decrypt();
	}

	labelWalkieTalkie.getComponent<UILabel>().setLabelText(this->crpytographer->message, "arial");

	manager.refresh();
	manager.update();

	for (auto& c : colliders) {

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		if(Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
			player.getComponent<SpriteComponent>().play("idle");
			player.getComponent<SoundComponent>().sounds["walk"]->stop();
		}
	}

	for (auto& c : colliders) {

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(cCol, smurfEngineerCol)) {
			smurfEngineer.getComponent<TransformComponent>().position = smurfEngineerPos;
			smurfEngineer.getComponent<SpriteComponent>().play("idle");
		}
	}

	for (auto& c : colliders) {

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(cCol, smurfFemaleCol)) {
			smurfFemale.getComponent<TransformComponent>().position = smurfFemalePos;
			smurfFemale.getComponent<SpriteComponent>().play("idle");
		}
	}

	if (gargamelSpawned) {
		for (auto& c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, garmagelCol)) {
				gargamels[0]->getComponent<TransformComponent>().position = gargamelPos;
				gargamels[0]->getComponent<SpriteComponent>().play("idle");
			}
		}
	}

	for (auto& p : projectiles) {
		p->getComponent<TransformComponent>().chase(playerPos);
		for (auto& pl : players)
		{
			if (Collision::AABB(pl->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
				pl->getComponent<HealthComponent>().decreaseHP(30);
				hitSound->play(0);
				hitSound->playing = false;
				p->destroy();
			}
		}
	}

	for (auto& w : walkieTalkies) {
		for (auto& pl : players)
		{
			if (Collision::AABB(pl->getComponent<ColliderComponent>().collider, w->getComponent<ColliderComponent>().collider)) {
				this->walkieTalkieFound = true;
				walkieTalkieSound->play(0);
				walkieTalkieSound->playing = false;
				w->destroy();
			}
		}
	}

	if (gargamelSpawned) {
		gargamels[0]->getComponent<TransformComponent>().chase(playerPos);
		for (auto& pl : players) {
			if (Collision::AABB(pl->getComponent<ColliderComponent>().collider, gargamels[0]->getComponent<ColliderComponent>().collider)) {
				pl->getComponent<HealthComponent>().decreaseHP(90);
				hitSound->play(0);
				hitSound->playing = false;
				gargamels[0]->destroy();
				gargamelSpawned = false;
				gargamelAlive = false;
			}
		}
	}
	
	for (auto& r : redPotions) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, r->getComponent<ColliderComponent>().collider)) {

			for (auto& pl : players) {
				if(pl->hasComponent<HealthComponent>()) pl->getComponent<HealthComponent>().increaseHP(20);
			}
			redPotionSound->play(0);
			redPotionSound->playing = false;
			r->destroy();
		}
	}

	for (auto& b : bluePotions) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, b->getComponent<ColliderComponent>().collider)) {
			player.getComponent<HealthComponent>().increaseHP(30);
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

	if (player.getComponent<HealthComponent>().getHealth() == 0) {
		player.getComponent<SoundComponent>().sounds["walk"]->stop();
		isRunning = false;
	} 

	if (smurfEngineer.hasComponent<HealthComponent>() && smurfEngineer.getComponent<HealthComponent>().getHealth() == 0) {
		player.getComponent<SoundComponent>().sounds["walk"]->stop();
		isRunning = false;
	}
	
	if (smurfFemale.hasComponent<HealthComponent>() && smurfFemale.getComponent<HealthComponent>().getHealth() == 0) {
		player.getComponent<SoundComponent>().sounds["walk"]->stop();
		isRunning = false;
	}

	std::stringstream playerHp; 
	playerHp << "Player HP: " << player.getComponent<HealthComponent>().getHealth();
	labelHP.getComponent<UILabel>().setLabelText(std::move(playerHp).str(), "arial");

	for (auto& l : labels) {
		player.getComponent<TransformComponent>().isNearhouse = l->getComponent<UILabel>().inRange(player.getComponent<TransformComponent>().position);
		if (player.getComponent<TransformComponent>().isNearhouse) {
			playerNearHouseID = l->getComponent<UILabel>().labelID/2;
			break;
		} 
	}

	if (player.getComponent<TransformComponent>().isNearhouse) {
		if (Game::event.type == SDL_KEYDOWN) {
			if (Game::event.key.keysym.sym == SDLK_e) {

				player.getComponent<TransformComponent>().previousPosition.x = player.getComponent<TransformComponent>().position.x;
				player.getComponent<TransformComponent>().previousPosition.y = player.getComponent<TransformComponent>().position.y;

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
				smurfEngineer.getComponent<TransformComponent>().scale = 6;
				smurfFemale.getComponent<TransformComponent>().scale = 6;

				if (playerNearHouseID == this->SmurfEngineerHouse && !smurfEngineer.getComponent<FindComponent>().isFound()) {
					smurfEngineer.getComponent<FindComponent>().find();
					smurfEngineer.getComponent<FindComponent>().spawnInsideHouse();
					smurfEngineer.addComponent<HealthComponent>(100);
				}

				if (playerNearHouseID == this->SmurfFemaleHouse && 
					smurfEngineer.getComponent<FindComponent>().isFound() &&
					!smurfFemale.getComponent<FindComponent>().isFound()) 
				{
					smurfFemale.getComponent<FindComponent>().find();
					smurfFemale.getComponent<FindComponent>().spawnInsideHouse();
					smurfFemale.addComponent<HealthComponent>(100);
				}
			}
		}
	}

	if (Game::event.key.keysym.sym == SDLK_y) {

		if (player.getComponent<TransformComponent>().previousPosition.x != -1 &&
			player.getComponent<TransformComponent>().previousPosition.y != -1 &&
			labelHouse.getComponent<UILabel>().inRange(player.getComponent<TransformComponent>().position)) {

			minX = 0;
			minY = 0;

			player.getComponent<TransformComponent>().position = player.getComponent<TransformComponent>().previousPosition;

			doorSound->play(0);
			doorSound->playing = false;

			Game::camera.x = player.getComponent<TransformComponent>().position.x - (Game::screenWidth / 2);
			Game::camera.y = player.getComponent<TransformComponent>().position.y - (Game::screenHeight / 2);

			Game::camera.w = 8192; 
			Game::camera.h = 4096; 

			player.getComponent<TransformComponent>().previousPosition.x = -1;
			player.getComponent<TransformComponent>().previousPosition.y = -1;

			Game::camera = { 0, 0, 8192, 4096 };

			player.getComponent<TransformComponent>().scale = 3;
			smurfEngineer.getComponent<TransformComponent>().scale = 3;
			smurfFemale.getComponent<TransformComponent>().scale = 3;

			if (smurfEngineer.getComponent<FindComponent>().isFound() &&
				!smurfEngineer.getComponent<FindComponent>().isSpawnedInMainMap()) 
			{
				if (playerNearHouseID == 6) {
					smurfEngineer.getComponent<TransformComponent>().position = Vector2D(4614, 2985);
					smurfEngineer.getComponent<FindComponent>().setSpawnedInMap();
				}
		
				else if (playerNearHouseID == 5) {
					smurfEngineer.getComponent<TransformComponent>().position = Vector2D(6700, 2600);
					smurfEngineer.getComponent<FindComponent>().setSpawnedInMap();
				}
				else 
					smurfEngineer.getComponent<FindComponent>().spawnInMainMap(player.getComponent<TransformComponent>().position);
			}

			if (smurfFemale.getComponent<FindComponent>().isFound() &&
				smurfEngineer.getComponent<FindComponent>().isSpawnedInMainMap() &&
				!smurfFemale.getComponent<FindComponent>().isSpawnedInMainMap()) 
			{

				if (!storyThree->playing) {
					storyFour->play(0);
				}

				if (playerNearHouseID == 6) {
					smurfFemale.getComponent<TransformComponent>().position = Vector2D(4614, 2985);
					smurfFemale.getComponent<FindComponent>().setSpawnedInMap();
				}
				else if (playerNearHouseID == 5) {
					smurfFemale.getComponent<TransformComponent>().position = Vector2D(6700, 2600);
					smurfFemale.getComponent<FindComponent>().setSpawnedInMap();
				}
				else
					smurfFemale.getComponent<FindComponent>().spawnInMainMap(player.getComponent<TransformComponent>().position);
			}
		}
	}

	win = true;
	for (auto& pl : players) {
		win *= sqrt(pow(pl->getComponent<TransformComponent>().position.x - 8050, 2) + pow(pl->getComponent<TransformComponent>().position.y - 70, 2)) <= 200;
	}
	if (win) {
		player.getComponent<SoundComponent>().sounds["walk"]->stop();
		this->isRunning = false;
	} 

	bool nearGamrgamelHouse = true;
	for (auto& pl : players) {
		nearGamrgamelHouse *= sqrt(pow(pl->getComponent<TransformComponent>().position.x - 7150, 2) + pow(pl->getComponent<TransformComponent>().position.y - 855, 2)) <= 700;
	}
	if (nearGamrgamelHouse) {
		if (!gargamelSpawned && gargamelAlive) {
			assets->createProjectile(Vector2D(7150, 855), Vector2D(0, 0), 1, 0, "gargamel", 4, true, 5);
			gargamelSpawned = true;
		}
	}

	if (paused || !isRunning) {
		player.getComponent<TransformComponent>().velocity.zero();
		player.getComponent<SoundComponent>().sounds["walk"]->stop();
		player.getComponent<SpriteComponent>().play("idle");

		storyOne->stop();
		storyTwo->stop();
		storyThree->stop();
		storyFour->stop();

	}

}

void Game::render() {
	SDL_RenderClear(this->renderer);

	if (camera.x == 0 && camera.y == 0) {
		camera.x = std::max((int)player.getComponent<TransformComponent>().position.x - Game::screenWidth / 2, minX);
		camera.y = std::max((int)player.getComponent<TransformComponent>().position.y - Game::screenHeight / 2, minY);
	}

	for (auto& t : tiles) {
		t->draw();
	}
	for (auto& p : players) {
		if(!p->hasComponent<FindComponent>())
			p->draw();
		else {
			if(p->getComponent<FindComponent>().isFound())
				p->draw();
		}
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
	labelHouse.draw();

	for (auto& r: redPotions)
	{
		r->draw();
	}

	for (auto& b : bluePotions)
	{
		b->draw();
	}

	if(gargamelSpawned) gargamels[0]->draw();

	for (auto& w : walkieTalkies) {
		w->draw();
	}
	
	if (this->walkieTalkieFound) labelMessage.draw();
	if (this->walkieTalkieFound) labelWalkieTalkie.draw();
	if (this->walkieTalkieFound && !smurfEngineer.getComponent<FindComponent>().isFound()) labelGuide.draw();

	SDL_RenderPresent(this->renderer);
}


void Game::clean() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}