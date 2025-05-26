#include "Game.h"
#include <iostream>
#include "../Engine/Logger.h"
#include <SDL.h>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "../Engine/DrawingManager.h"
#include "../Engine/Utils.hpp"
#include "SpawnPatterns.hpp"
#include <Windows.h>
#include <glm.hpp>
#include "Level.h"

Game::Game()
{
	instance = this;
	Logger::Log("Game constructor called");
	isRunning = false;

}

Game::~Game()
{

	//WRITE HIGH SCORE
	std::string myText;
	std::ofstream File("./Data/best_score.txt");

	File << best_score;
	File.close();

	Logger::Log("Game destructor called");
}

void Game::Start()
{

	//SDL setup
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error initializing SDL!");
		return;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); //bilinear filtering

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	auto Width = DM.w - 200;
	auto Height = DM.h - 200;

	window = SDL_CreateWindow("Untitled Bullet Hell Shooter In Space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		Logger::Err("Error creating SDL Window!");
		return;
	}


#ifdef VSYNC
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
#else
	renderer = SDL_CreateRenderer(window, -1, 0);
#endif // VSYNC

	if (!renderer)
	{
		Logger::Err("Error creating SDL Renderer!");
		return;
	}

	Logger::Log("SDL Initialized !");


	//Engine setup
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	input = std::make_unique<Input>();
	assetManager = std::make_unique<AssetManager>(renderer);

	//Game setup
	//Keyboard bindings
	input->BindKey(Action::MoveLeft, SDL_SCANCODE_LEFT);
	input->BindKey(Action::MoveRight, SDL_SCANCODE_RIGHT);
	input->BindKey(Action::MoveUp, SDL_SCANCODE_UP);
	input->BindKey(Action::MoveDown, SDL_SCANCODE_DOWN);
	input->BindKey(Action::Shoot, SDL_SCANCODE_SPACE);
	input->BindKey(Action::Confirm, SDL_SCANCODE_RETURN);

	//Gamepad mapping (assuming Xbox-style controller)
	input->BindButton(Action::MoveLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	input->BindButton(Action::MoveRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	input->BindButton(Action::MoveUp, SDL_CONTROLLER_BUTTON_DPAD_UP);
	input->BindButton(Action::MoveDown, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	input->BindButton(Action::Shoot, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	input->BindButton(Action::Confirm, SDL_CONTROLLER_BUTTON_A);

	assetManager->AddTexture("Ship", "./Assets/Images/Ship.tga");
	assetManager->AddTexture("EnemyBullet", "./Assets/Images/EnemyBullet.tga");
	assetManager->AddTexture("PlayerBullet", "./Assets/Images/PlayerBullet.tga");
	assetManager->AddTexture("Items", "./Assets/Images/Items.tga");
	assetManager->AddTexture("Tileset_back", "./Assets/Images/Tileset_back.tga");

	assetManager->AddTexture("Explosion", "./Assets/Images/Explosion.tga");

	assetManager->AddTexture("Alien", "./Assets/Images/Alien.tga");
	assetManager->AddTexture("LaserAlien", "./Assets/Images/LaserAlien.tga");
	assetManager->AddTexture("SmallAlien", "./Assets/Images/SmallAlien.tga");

	assetManager->AddTexture("MainMenu", "./Assets/Images/MainMenu.tga");
	assetManager->AddTexture("NumberFont", "./Assets/Images/NumberFont.tga");
	assetManager->AddTexture("Health", "./Assets/Images/Health.tga");
	assetManager->AddTexture("NowPlaying", "./Assets/Images/NowPlaying.tga");

	assetManager->AddTexture("Icon_Cybergrind", "./Assets/Images/MusicIcons/Cybergrind.tga");
	assetManager->AddTexture("Icon_Pandemonium", "./Assets/Images/MusicIcons/Pandemonium.tga");
	assetManager->AddTexture("Icon_TenebreRossoSangue", "./Assets/Images/MusicIcons/TenebreRossoSangue.tga");
	assetManager->AddTexture("Icon_Chaos", "./Assets/Images/MusicIcons/Chaos.tga");
	assetManager->AddSurface("Icon", "./Assets/Images/Icon.tga");

	//ADD FONTS
	assetManager->AddFont("Metropolis", "./Assets/Fonts/Metropolis-Bold.otf");
	assetManager->AddFont("MetropolisThin", "./Assets/Fonts/Metropolis-SemiBold.otf");

	//ADD SOUNDS AND MUSIC
	assetManager->AddMusic("The Cyber Grind", "meganeko | ULTRAKILL OST", "./Assets/Sounds/cybergrind.wav", assetManager->GetTexture("Icon_Cybergrind"));
	assetManager->AddMusic("Pandemonium", "Heaven Pierce Her | ULTRAKILL OST", "./Assets/Sounds/pandemonium.wav", assetManager->GetTexture("Icon_Pandemonium"));
	assetManager->AddMusic("War", "Heaven Pierce Her | ULTRAKILL OST", "./Assets/Sounds/war.wav", assetManager->GetTexture("Icon_Pandemonium"));
	assetManager->AddMusic("Tenebre Rosso Sangue", "Keygen Church | ULTRAKILL OST", "./Assets/Sounds/tenebrerossosangue.wav", assetManager->GetTexture("Icon_TenebreRossoSangue"));
	assetManager->AddMusic("Chaos", "Heaven Pierce Her | ULTRAKILL OST", "./Assets/Sounds/chaos.wav", assetManager->GetTexture("Icon_Chaos"));
	assetManager->AddMusic("Order", "Heaven Pierce Her | ULTRAKILL OST", "./Assets/Sounds/order.wav", assetManager->GetTexture("Icon_Chaos"));

	assetManager->AddSound("HitPlayer", "./Assets/Sounds/player_hit.wav");
	assetManager->AddSound("HitEnemy", "./Assets/Sounds/enemy_hit.wav");

	assetManager->AddSound("Shoot", "./Assets/Sounds/shoot.wav");
	assetManager->AddSound("Laser", "./Assets/Sounds/laser.wav");
	assetManager->AddSound("LaserShoot", "./Assets/Sounds/laser_shoot.wav");
	assetManager->AddSound("Explosion", "./Assets/Sounds/explosion.wav");

	assetManager->AddSound("Heal", "./Assets/Sounds/heal.wav");
	assetManager->AddSound("Equip", "./Assets/Sounds/equip.wav");

	// Set SOUND Volumes
	Mix_VolumeChunk(assetManager->GetSound("Shoot"), 32);
	Mix_VolumeChunk(assetManager->GetSound("Laser"), 16);
	Mix_VolumeChunk(assetManager->GetSound("LaserShoot"), 16);

	Mix_VolumeChunk(assetManager->GetSound("HitEnemy"), 48);
	Mix_VolumeChunk(assetManager->GetSound("HitPlayer"), 32);

	Mix_VolumeChunk(assetManager->GetSound("Heal"), 16);
	Mix_VolumeChunk(assetManager->GetSound("Equip"), 32);


	SDL_SetWindowIcon(window, assetManager->GetSurface("Icon"));

	// Setup POOLS for reused objects
	SetupPools();

	//Create level
	level = std::make_unique<Level>(assetManager->GetTexture("Tileset_back"), "./Assets/Maps/Level_Background.csv", 8, 10, "./Assets/Maps/Level_Middleground.csv", 8, 25);

	Logger::Log("Created LEVEL");

	CreatePlayer();

	Logger::Log("Created PLAYER");


	// Define ENEMY TYPES
	RegisterEnemyType(
		"DefaultEnemy",
		"Alien",
		glm::vec2(64, 64),
		Bullet::SingleShot,
		new glm::vec2[]{ glm::vec2(6, 58), glm::vec2(58, 58) },
		2,
		10,
		Enemy::DefaultEnemyRenderer
	);
	RegisterEnemyType(
		"DoubleEnemy",
		"Alien",
		glm::vec2(64, 64),
		Bullet::DoubleShot,
		new glm::vec2[]{ glm::vec2(6, 58), glm::vec2(58, 58) },
		2,
		10,
		Enemy::DefaultEnemyRenderer
	);
	RegisterEnemyType(
		"TripleEnemy",
		"Alien",
		glm::vec2(64, 64),
		Bullet::TripleShot,
		new glm::vec2[]{ glm::vec2(6, 58), glm::vec2(58, 58) },
		2,
		10,
		Enemy::DefaultEnemyRenderer
	);
	RegisterEnemyType(
		"LaserEnemy",
		"LaserAlien",
		glm::vec2(128, 128),
		Bullet::None,
		new glm::vec2[]{ glm::vec2(6, 58), glm::vec2(58, 58) },
		999999,
		25,
		Enemy::LaserEnemyRenderer,
		assetManager->GetSound("Laser")
	);
	RegisterEnemyType(
		"SmallEnemy",
		"SmallAlien",
		glm::vec2(32, 32),
		Bullet::ShootAllAround,
		new glm::vec2[]{ glm::vec2(24, 24) },
		2,
		3,
		Enemy::SmallEnemyRenderer
	);

	//GET HIGH SCORE
	std::string myText;
	std::ifstream File("./Data/best_score.txt");

	while (std::getline(File, myText)) {
		best_score = std::stoi(myText);
	}
	File.close();

	// START STAT LOGGER
	printf("\n");
	Logger::Log("\x1B[0;30m \x1B[44mThe GOOP is in the bag.");
	printf("\n\n\n\n\n");
	LogDebugData();

	isRunning = true;

	// UPDATE ONCE on start to set them up
	level->Update(0.1f);
	player->Update(this, 0.1f);
}

void Game::ProcessInput()
{
	input->ProcessInput();

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			case SDL_QUIT: isRunning = false; break;
			case SDL_KEYDOWN: if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) // PAUSE or QUIT GAME
				if (gameStarted && !player->isDying) {
					if (!pausedGame) // PAUSE GAME
					{
						Mix_VolumeMusic(16);
						Mix_Pause(-1);
					}
					else // UNPAUSE GAME
					{
						Mix_VolumeMusic(32);
						Mix_Resume(-1);
					}
					pausedGame = !pausedGame;
				}
				else {
					isRunning = false;
				}
				break;
		}
	}
}

void Game::Update()
{
	start_uTime = SDL_GetTicks();
#ifdef CAP_FPS
	uint64_t timeToWait = MILLISEC_PER_FRAME - (SDL_GetTicks64() - millisecondPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISEC_PER_FRAME)
		SDL_Delay(timeToWait); //yield to other processes
#endif // CAP_FPS

	float deltaTime = (SDL_GetTicks64() - millisecondPreviousFrame) / 1000.0f;
	millisecondPreviousFrame = SDL_GetTicks64();

	// Mute Music when player dies
	if (player->isDying) Mix_VolumeMusic(0);

	// Play DEATH sequence and reset game on death
	if (player->isDying || dyingTimer > 0) {
		dyingTimer += deltaTime;
		if (dyingTimer > 3 && gameStarted) ResetGame();
		if (dyingTimer > 5) dyingTimer = 0;
	}

	// Force-Unpause game if in main menu
	if (!gameStarted) pausedGame = false;

	if (gameStarted && !pausedGame)
	{
		menuPos = 0;
		UpdateGame(deltaTime);
	}
	else // Navigate MENUS
	{
		if (input->IsActionHeld(Action::MoveUp) && !movedUp) {
			menuPos = std::clamp(menuPos - 1, 0, 2);
		}
		else if (input->IsActionHeld(Action::MoveDown) && !movedDown) {
			menuPos = std::clamp(menuPos + 1, 0, 2);
			if (pausedGame) menuPos = std::clamp(menuPos, 0, 1);
		}
		else if ((input->IsActionHeld(Action::Shoot) || input->IsActionHeld(Action::Confirm)) && !confirmed) {
			switch (menuPos) {
				//PLAY|RESUME BUTTON
			case 0: {
				// RESUME
				if (pausedGame)
				{
					Mix_VolumeMusic(32);
					Mix_Resume(1);
					pausedGame = false;
				}
				// PLAY
				else
				{
					currentSong = Utils::RndSong();
					Mix_PlayMusic(currentSong.music, 0);
					Mix_VolumeMusic(32);
					gameStarted = true;
					dyingTimer = 0;
				}
				break;
			}
				  //QUIT BUTTON
			case 1: {
				//Kill Player
				if (pausedGame) {
					getPlayer()->health = 1;
					getPlayer()->shield = 0;
					getPlayer()->onHit(this, deltaTime);
					pausedGame = false;
				}
				//Close Game
				else isRunning = false;
				break;
			}
				//OPEN LINK
			case 2: {
				ShellExecute(0, 0, L"https://linktr.ee/offzephyr", 0, 0, SW_SHOW);
				break;
			}
			};
		}
		movedUp = input->IsActionHeld(Action::MoveUp);
		movedDown = input->IsActionHeld(Action::MoveDown);
		confirmed = input->IsActionHeld(Action::Shoot) || input->IsActionHeld(Action::Confirm);
	}

	uTime = SDL_GetTicks() - start_uTime;
}

void Game::UpdateGame(float deltaTime)
{
	playtime += deltaTime;

	// Timer for the 'Now Playing' animation when a track starts playing
	nowPlayingTimer += deltaTime;
	
	// Start playing a new track if a music ends
	if (!Mix_PlayingMusic()) {
		nowPlayingTimer = 0;
		currentSong = Utils::RndSong();
		Mix_PlayMusic(currentSong.music, 0);
	}

	level->Update(deltaTime);
	player->Update(this, deltaTime);

	spawnTimer -= deltaTime;

	// SPAWN NEW Enemy Spawners
	if (spawnTimer < 0) {

		EnemySpawner enemy = SpawnPatterns::RndPattern();

		spawnTimer = spawnTimerMax;
		spawnTimerMax *= 0.99f;
		spawnTimerMax = max(spawnTimerMax, 2);

		enemySpawners.push_back(enemy);
	}

	// UPDATE Enemy Spawners
	for (auto& enemySpawner : enemySpawners)
	{
		enemySpawner.Update(this, deltaTime);
	}

	// UPDATE Enemies
	for (size_t i = 0; i < enemyPoolSize; i++)
	{
		if (enemies[i].isActive)
		{
			if (enemies[i].Update(this, deltaTime))
			{
				enemies[i].age = 0;
				enemies[i].isActive = false;
				enemies[i].next = enemyHead;
				enemyHead = &enemies[i];
			}
		}
	}

	// UPDATE Pickupable Items
	for (size_t i = 0; i < itemPoolSize; i++)
	{
		if (items[i].isActive)
		{
			if (items[i].Update(this, deltaTime))
			{
				items[i].isActive = false;
				items[i].next = itemHead;
				itemHead = &items[i];
			}
		}
	}

	// UPDATE Bullets
	for (size_t i = 0; i < bulletPoolSize; i++)
	{
		if (bullets[i].isActive)
		{
			if (bullets[i].Update(this, deltaTime))
			{
				bullets[i].hit = false;
				bullets[i].isActive = false;
				bullets[i].next = bulletHead;
				bulletHead = &bullets[i];
			}
		}
	}
}

void Game::Render()
{
	start_rTime = SDL_GetTicks();

	//clear draw buffer
	SDL_SetRenderDrawColor(renderer, 80, 60, 40, 255);
	SDL_RenderClear(renderer);

	//RENDER Level
	level->Render(renderer);

	//RENDER Items
	for (size_t i = 0; i < itemPoolSize; i++)
	{
		if (items[i].isActive) items[i].Render(renderer);
	}

	//RENDER Bullets
	for (size_t i = 0; i < bulletPoolSize; i++)
	{
		if (bullets[i].isActive) bullets[i].Render(renderer);
	}

	//RENDER Enemies
	for (size_t i = 0; i < enemyPoolSize; i++)
	{
		if (enemies[i].isActive) enemies[i].Render(renderer, assetManager.get());
	}

	//RENDER Player
	player->Render(renderer, assetManager.get());

	//RENDER HUD Overlay
	RenderHUD(renderer);

	//Fill Empty Space of the window
	DrawingManager::FillOutside(renderer, {0, 0, 0, 255});

	//wait for VSync (or not) and swap buffers -> draw becomes display and vice-versa
	SDL_RenderPresent(renderer);

	rTime = SDL_GetTicks() - start_rTime;
}

void Game::Run()
{
	while (isRunning)
	{
		start_tTime = SDL_GetTicks();

		ProcessInput();
		Update();
		Render();

		LogDebugData();
	}
}

void Game::Quit()
{
	printf("\n");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Logger::Log("SDL closed !");
}

void Game::CreateEnemy(std::string typeIndex, int posX, int posY, int speed, glm::vec2(*movementFunction)(Enemy*, float, int, glm::vec2), bool mirrorPath) {

	EnemyType type = enemyTypes[typeIndex];

	if (type.name.empty()) {
		Logger::Err("Invalid enemy type...");
		return;
	}
	Enemy* enemy = enemyHead;
	Enemy* next = enemy->next;

	enemy->dim = type.dim;
	enemy->pos = glm::vec2(posX - (type.dim.x / 2), posY - (enemy->dim.y / 2));
	enemy->texture = assetManager->GetTexture(type.texture);
	enemy->speed = speed;
	enemy->animSpeed = 5;
	enemy->farmeGridX = 2;
	enemy->farmeGridY = 2;
	enemy->spawnX = posX - (type.dim.x / 2);
	enemy->spawnY = posY - (type.dim.y / 2);
	enemy->mirror = mirrorPath;
	enemy->movementFunction = movementFunction;
	enemy->shootFunction = type.shootFunction;
	enemy->shootPoses = type.shootPoses;
	enemy->shootingSpeed = type.shootingSpeed;
	enemy->health = type.health;
	enemy->renderer = type.renderer;
	enemy->spawnSound = type.spawnSound;
	enemy->isActive = true;

	enemy->ResetData();

	enemyHead = next;
}

void Game::RegisterEnemyType(std::string name, std::string path, glm::vec2 textureSize, Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool), glm::vec2 shootPoses[], float shootingSpeed, int health, void (*renderer)(SDL_Renderer* renderer, Enemy* enemy, glm::vec2 offset, AssetManager* assetManager)) {
	RegisterEnemyType(name, path, textureSize, shootFunction, shootPoses, shootingSpeed, health, renderer, nullptr);
}
void Game::RegisterEnemyType(std::string name, std::string path, glm::vec2 textureSize, Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool), glm::vec2 shootPoses[], float shootingSpeed, int health, void (*renderer)(SDL_Renderer* renderer, Enemy* enemy, glm::vec2 offset, AssetManager* assetManager), Mix_Chunk* spawnSound) {

	EnemyType type;
	type.name = name;
	type.texture = path;
	type.dim = textureSize;
	type.shootFunction = shootFunction;
	type.shootingSpeed = shootingSpeed;
	type.shootPoses = shootPoses;
	type.health = health;
	type.renderer = renderer;
	type.spawnSound = spawnSound;

	enemyTypes.emplace(name, type);

	Logger::Log("Enemy Type " + name + " added");
}
void Game::SetupPools() {

	enemyHead = &enemies[0];
	for (int i = 0; i < enemyPoolSize - 1; i++) enemies[i].next = &enemies[i + 1];
	enemies[enemyPoolSize - 1].next = nullptr;

	itemHead = &items[0];
	for (int i = 0; i < itemPoolSize - 1; i++) items[i].next = &items[i + 1];
	items[itemPoolSize - 1].next = nullptr;

	bulletHead = &bullets[0];
	for (int i = 0; i < bulletPoolSize - 1; i++) bullets[i].next = &bullets[i + 1];
	bullets[bulletPoolSize - 1].next = nullptr;

	Logger::Log("Pools Initialized !");
}
void Game::CreateBulletUpgrade(ItemData upgradeData, int posX, int posY) {

	DroppedItem* upgrade = itemHead;

	DroppedItem* next = upgrade->next;

	upgrade->pos = glm::vec2(posX, posY);
	upgrade->isActive = true;
	upgrade->data = upgradeData;

	upgrade->texture = assetManager->GetTexture("Items");
	itemHead = next;
}

Player* Game::getPlayer() {
	return player.get();
}

AssetManager* Game::getAssetManager() {
	return assetManager.get();
}

void Game::addScore(int score) {
	if (this->score + score >= 999999999)
	{
		this->score = 999999999;
		return;
	}
	this->score += score;
}

void Game::RenderHUD(SDL_Renderer* renderer) {

	if (gameStarted) {
		RenderGameHUD(renderer);
		RenderScore(renderer, { DISPLAY_WIDTH - 254, 2 }, score);

		if (pausedGame) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
			DrawingManager::Fill(renderer, new SDL_Rect(0, 0, 1024, 1024));

			//RENDER GAME TITLE
			SDL_Texture* texture = assetManager->GetTexture("MainMenu");
			SDL_Rect title_src1 = { 0, 0, 512, 92 };
			SDL_Rect title_src2 = { 0, 92, 298, 92 };
			SDL_FRect title_dst1 = { 107, 256, 512, 92 };
			SDL_FRect title_dst2 = { 619, 256, 298, 92 };

			SDL_SetTextureColorMod(texture, 255, 255, 255);
			SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
			DrawingManager::DrawImage(renderer, texture, &title_src1, &title_dst1);
			DrawingManager::DrawImage(renderer, texture, &title_src2, &title_dst2);

			//RENDER BUTTONS
			SDL_Rect resume_src = { 0, 246, 404, 60 };
			SDL_FRect resume_dst = { 512 - 202, 512, 404, 60 };

			SDL_Rect quit_src = { 0, 306, 342, 60 };
			SDL_FRect quit_dst = { 512 - 171, 650, 342, 60 };

			DrawingManager::DrawImage(renderer, texture, &resume_src, &resume_dst);
			DrawingManager::DrawImage(renderer, texture, &quit_src, &quit_dst);

			//RENDER SELECTION ARROWS
			float xOffset = menuPos == 2 ? 512 : 0;
			float yOffset = menuPos == 2 ? 381 : menuPos * 138;

			SDL_Rect arrow_src = { 0, 366, 36, 70 };
			SDL_FRect arrow_dst = { xOffset + 256 - 18, yOffset + 507, 36, 70 };
			SDL_Rect arrow_src2 = { 0, 436, 36, 70 };
			SDL_FRect arrow_dst2 = { xOffset + 768 - 18, yOffset + 507, 36, 70 };

			DrawingManager::DrawImage(renderer, texture, &arrow_src, &arrow_dst);
			DrawingManager::DrawImage(renderer, texture, &arrow_src2, &arrow_dst2);
		}
	}
	else {
		//RENDER GAME TITLE
		SDL_Texture* texture = assetManager->GetTexture("MainMenu");
		SDL_Rect title_src1 = { 0, 0, 512, 92 };
		SDL_Rect title_src2 = { 0, 92, 298, 92 };
		SDL_FRect title_dst1 = { 107, 256, 512, 92 };
		SDL_FRect title_dst2 = { 619, 256, 298, 92 };

		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
		DrawingManager::DrawImage(renderer, texture, &title_src1, &title_dst1);
		DrawingManager::DrawImage(renderer, texture, &title_src2, &title_dst2);

		//RENDER BUTTONS
		SDL_Rect start_src = { 0, 184, 404, 60 };
		SDL_FRect start_dst = { 512 - 202, 512, 404, 60 };

		SDL_Rect quit_src  = { 0, 306, 342, 60 };
		SDL_FRect quit_dst = { 512 - 171, 650, 342, 60 };

		DrawingManager::DrawImage(renderer, texture, &start_src, &start_dst);
		DrawingManager::DrawImage(renderer, texture, &quit_src, &quit_dst);

		SDL_Rect logo_src = { 334, 334, 178, 178 };
		SDL_FRect logo_dst = { 840, 840, 178, 178 };

		DrawingManager::DrawImage(renderer, texture, &logo_src, &logo_dst);

		//RENDER SELECTION ARROWS
		float xOffset = menuPos == 2 ? 512 : 0;
		float yOffset = menuPos == 2 ? 381 : menuPos * 138;

		SDL_Rect arrow_src = { 0, 366, 36, 70 };
		SDL_FRect arrow_dst = { xOffset + 256 - 18, yOffset + 507, 36, 70 };
		SDL_Rect arrow_src2 = { 0, 436, 36, 70 };
		SDL_FRect arrow_dst2 = { xOffset + 768 - 18, yOffset + 507, 36, 70 };

		DrawingManager::DrawImage(renderer, texture, &arrow_src, &arrow_dst);
		DrawingManager::DrawImage(renderer, texture, &arrow_src2, &arrow_dst2);

		DrawingManager::WriteText(renderer, assetManager->GetFont("Metropolis"), "BEST SCORE: ", {2, DISPLAY_HEIGHT - 54 - 28 }, 26);
		RenderScore(renderer, {0, DISPLAY_HEIGHT - 54}, best_score);
	}

	if (dyingTimer > 1) {
		float index = dyingTimer > 3 ? ((dyingTimer - 7) * -1) / 2 : (dyingTimer - 1) / 2;
		float alpha = std::lerp(0, 255, index);
		SDL_Color color = { 0, 0, 0, alpha };

		SDL_Rect src = { 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT };

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
		DrawingManager::Fill(renderer, &src);
	}
}

void Game::RenderGameHUD(SDL_Renderer* renderer) {

	// NOW PLAYING

	// nowPlayingTimer
	float xOffset = pausedGame ? 0 : nowPlayingTimer > 10 ? -512 : nowPlayingTimer > 1 && nowPlayingTimer < 9 ? 0 : nowPlayingTimer < 1 ? -512 + (sin(nowPlayingTimer * (M_PI/2)) * 512) : -512 + (cos((nowPlayingTimer - 9) * (M_PI / 2)) * 512);

	SDL_Texture* track_texture = assetManager->GetTexture("NowPlaying");
	SDL_SetTextureBlendMode(track_texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_Rect track_src = { 0, 0, 512, 147 };
	SDL_Rect track_src2 = { 0, 147, 512, 147 };
	SDL_FRect track_dst = { xOffset  + -512, DISPLAY_HEIGHT - 96, 512, 96 };
	SDL_FRect track_dst2 = { xOffset + 0, DISPLAY_HEIGHT - 96, 512, 96 };

	DrawingManager::DrawImage(renderer, track_texture, &track_src, &track_dst);
	DrawingManager::DrawImage(renderer, track_texture, &track_src2, &track_dst2);

	SDL_Rect logo_src = { 0, 0, 64, 64 };
	SDL_FRect logo_dst = { xOffset + 20, DISPLAY_HEIGHT - 116, 96, 96 };

	DrawingManager::DrawImage(renderer, currentSong.texture, &logo_src, &logo_dst);

	//DrawingManager::WriteText(renderer, assetManager->GetFont("Metropolis"), "Currently Playing:", { xOffset + 128, DISPLAY_HEIGHT - 106 }, 32);

	DrawingManager::WriteText(renderer, assetManager->GetFont("MetropolisThin"), currentSong.name, { xOffset + 118, DISPLAY_HEIGHT - 65 }, 24);

	DrawingManager::WriteText(renderer, assetManager->GetFont("MetropolisThin"), "by: " + currentSong.author, {xOffset + 138, DISPLAY_HEIGHT - 40}, 18);

	// TOP STATS

	SDL_Texture* texture = assetManager->GetTexture("Health");
	SDL_SetTextureColorMod(texture, 255, 255, 255);
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);

	// HEALTH

	SDL_Rect back_health_src = { 0, 33, 84, 10 };
	SDL_Rect health_src = { 0, 43, getPlayer()->health * 7, 10 };

	SDL_FRect back_health_dst = { 61, 14, 168, 20 };
	SDL_FRect health_dst = { 61, 14, getPlayer()->health * 14, 20};

	DrawingManager::DrawImage(renderer, texture, &back_health_src, &back_health_dst);
	DrawingManager::DrawImage(renderer, texture, &health_src, &health_dst);

	// SHIELD

	SDL_Rect back_shield_src = { 0, 53, 75, 5 };
	SDL_Rect shield_src = { 0, 58, getPlayer()->shield * 25, 5 };

	SDL_FRect back_shield_dst = { 61, 40, 150, 10 };
	SDL_FRect shield_dst = { 61, 40, getPlayer()->shield * 50, 10 };

	DrawingManager::DrawImage(renderer, texture, &back_shield_src, &back_shield_dst);

	DrawingManager::DrawImage(renderer, texture, &shield_src, &shield_dst);

	// TOP

	SDL_Rect top_src = { 0, 0, 128, 32 };
	SDL_FRect top_dst = { 2, 2, 256, 64 };

	DrawingManager::DrawImage(renderer, texture, &top_src, &top_dst);
}

void Game::RenderScore(SDL_Renderer* renderer, glm::vec2 pos, int score) {

	SDL_Color color = { 0, 0, 0, 150 };
	SDL_Rect* src = new SDL_Rect(pos.x, pos.y, 252, 52);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	DrawingManager::Fill(renderer, src);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	std::string str = std::to_string(score);
	for (int i = 0; i < 9; i++) {

		int spacing = 4;
		int x = (pos.x - 2) + (10 * 24) - 12 - (i * 24) - (i * spacing);

		if (i < str.size()) {
			char a = str[str.size() - i - 1];
			int ia = a - '0';


			RenderScoreNumber(ia, renderer, glm::vec2(x, pos.y + 2));
		}
		else {
			RenderScoreNumber(0, renderer, glm::vec2(x, pos.y + 2));
		}
	}

}
void Game::RenderScoreNumber(int value, SDL_Renderer* renderer, glm::vec2 pos) {

	SDL_Rect src = { 16 * value, 0, 16, 32 };
	SDL_FRect dst = { pos.x, pos.y, 24, 48 };

	SDL_Texture* texture = assetManager->GetTexture("NumberFont");

	SDL_SetTextureColorMod(texture, 255, 255, 255);
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND); //alpha blend
	DrawingManager::DrawImage(renderer, texture, &src, &dst);
}

void Game::CreatePlayer() {
	player = std::make_unique<Player>(glm::vec2(512 - 32, 900),
		assetManager->GetTexture("Ship"),
		assetManager->GetTexture("Bullet"),
		input);
	player->dim = glm::vec2(64, 64);
	player->speed = 400;
	player->animSpeed = 20;
}

void Game::ResetGame() {

	Mix_HaltChannel(-1);

	player.reset();
	CreatePlayer();

	enemySpawners.clear();

	for (size_t i = 0; i < enemyPoolSize; i++)
	{
		if (enemies[i].isActive)
		{
			enemies[i].age = 0;
			enemies[i].isActive = false;
			enemies[i].next = enemyHead;
			enemyHead = &enemies[i];
		}
	}
	for (size_t i = 0; i < itemPoolSize; i++)
	{
		if (items[i].isActive)
		{
			items[i].isActive = false;
			items[i].next = itemHead;
			itemHead = &items[i];
		}
	}
	for (size_t i = 0; i < bulletPoolSize; i++)
	{
		if (bullets[i].isActive)
		{
			bullets[i].hit = false;
			bullets[i].isActive = false;
			bullets[i].next = bulletHead;
			bulletHead = &bullets[i];
		}
	}

	if (score > best_score) best_score = score;
	score = 0;
	spawnTimer = 2;
	spawnTimerMax = 10;
	pausedGame = false;
	gameStarted = false;
	isRunning = true;
	spawnTimer = 0;
	playtime = 0;
	nowPlayingTimer = 0;

	level->camPosY = 0;
	level->Update(0.1f);
}

void Game::LogDebugData()
{
	tTime = SDL_GetTicks() - start_tTime;

	frames++;
	timeElapsed += (SDL_GetTicks64() - millisecondPreviousFrame);
	if (timeElapsed >= 1000)
	{
		fps = frames;
		timeElapsed = 0;
		frames = 0;


		printf("\033[A\33[2KT\r\033[A\33[2KT\r\033[A\33[2KT\r\033[A\33[2KT\r");
		printf("FPS: %i\n", fps);
		printf("Update Time: %ims\n", uTime);
		printf("Render Time: %ims\n", rTime);
		printf("Total Time: %ims\n", tTime);
	}
}

