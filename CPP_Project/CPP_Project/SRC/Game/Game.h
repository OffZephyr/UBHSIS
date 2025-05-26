#pragma once
#include <SDL.h>
#include "../Engine/Entity.h"
#include "../Engine/AssetManager.h"
#include "../Engine/Input.h"
#include <vector>
#include <map>
#include <fstream>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"


#define VSYNC //FPS will be determined by the display refresh rate
//#define CAP_FPS //If defined, this will limit to the set frame rate below. Otherwise, only the VSYNC will be the limiting factor.

const uint32_t FPS = 30;//Only used if CAP_FPS
const uint32_t MILLISEC_PER_FRAME = 1000 / FPS;

enum class Action { MoveLeft, MoveRight, MoveUp, MoveDown, Shoot, Confirm };

const uint32_t DISPLAY_WIDTH = 1024;
const uint32_t DISPLAY_HEIGHT = 1024;

class Level;
class Player;

class Game
{
	public:

		Game();
		~Game();
		static inline Game* instance;

		void Start();
		void Run();
		void Quit();

		Player* getPlayer();
		AssetManager* getAssetManager();

		static const uint32_t enemyPoolSize = 300;
		static const uint32_t itemPoolSize = 50;
		static const uint32_t bulletPoolSize = 1000;

		Enemy* enemyHead;
		Enemy enemies[enemyPoolSize];

		DroppedItem* itemHead;
		DroppedItem items[itemPoolSize];

		Bullet* bulletHead;
		Bullet bullets[bulletPoolSize];

		float playtime = 0;

		std::fstream score_file;
		int score = 0;
		int best_score = 0;

		void addScore(int score);

		void CreateBulletUpgrade(ItemData upgradeData, int posX, int posY);
		void CreateEnemy(std::string typeIndex, int posX, int posY, int speed, glm::vec2(*movementFunction)(Enemy*, float, int, glm::vec2), bool mirrorX);

		SDL_Window* window = nullptr;
	private:

		Music currentSong;

		float nowPlayingTimer = 0;
		float dyingTimer = 0;
		int menuPos = 0;
		bool movedUp = false;
		bool movedDown = false;
		bool confirmed = false;
		bool gameStarted = false;
		bool pausedGame = false;

		int timeElapsed = 0;
		int frames = 0;

		int fps = 0;
		int uTime = 0;
		int rTime = 0;
		int tTime = 0;

		int start_tTime = 0;
		int start_uTime = 0;
		int start_rTime = 0;

		SDL_Renderer* renderer = nullptr;
		std::unique_ptr<AssetManager> assetManager;
		std::unique_ptr<Input> input;
		std::unique_ptr<Player> player;

		std::unordered_map<std::string, EnemyType> enemyTypes;
		std::vector<EnemySpawner> enemySpawners;

		float spawnTimer = 2;
		float spawnTimerMax = 10;

		bool isRunning;
		uint64_t millisecondPreviousFrame = 0;

		//Level data
		std::unique_ptr<Level> level; //this could be a vector of levels

		void ProcessInput();
		void Update();
		void UpdateGame(float deltaTime);
		void Render();

		void SetupPools();
		void RegisterEnemyType(std::string, std::string, glm::vec2 textureSize, Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool), glm::vec2 shootPoses[], float shootingSpeed, int health, void (*renderer)(SDL_Renderer* renderer, Enemy* enemy, glm::vec2 offset, AssetManager* assetManager));
		void RegisterEnemyType(std::string, std::string, glm::vec2 textureSize, Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool), glm::vec2 shootPoses[], float shootingSpeed, int health, void (*renderer)(SDL_Renderer* renderer, Enemy* enemy, glm::vec2 offset, AssetManager* assetManager), Mix_Chunk* spawnSound);

		void RenderHUD(SDL_Renderer* renderer);

		void RenderGameHUD(SDL_Renderer* renderer);
		void RenderScore(SDL_Renderer* renderer, glm::vec2 pos, int score);
		void RenderScoreNumber(int num, SDL_Renderer* renderer, glm::vec2 pos);

		void CreatePlayer();
		void ResetGame();
		void LogDebugData();
};
