#pragma once
#include "../Engine/Entity.h"
#include <memory>
#include <vector>

struct EnemyType
{
public:
	Mix_Chunk* spawnSound = nullptr;
	std::string name;
	std::string texture;
	glm::vec2 dim;
	Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool);
	glm::vec2* shootPoses;
	float shootingSpeed;
	int health;
	void (*renderer)(SDL_Renderer* renderer, Enemy* enemy, glm::vec2 offset, AssetManager* assetManager);
};

struct EnemySpawnData
{
	std::string type;
	glm::vec2 pos;
	float delay;
	float speed;
	glm::vec2(*path)(Enemy*, float, int, glm::vec2);
	bool mirrorPath;

	bool triggered = false;
};

class EnemySpawner
{
public:
	std::vector<EnemySpawnData> spawners;

	EnemySpawner(std::vector<EnemySpawnData> s) : spawners(s)
	{

	}
	~EnemySpawner() = default;

	float age = 0;
	bool Update(Game* game, float deltaTime);
};

class Enemy
{
public:

	using vec2 = glm::vec2;

	vec2 pos;
	vec2 vel;
	vec2 dim = { 8, 8 }; //width and height
	SDL_Texture* texture;

	vec2(*movementFunction)(Enemy*, float, int, glm::vec2);
	int speed;
	int animSpeed; //frames per second
	int farmeGridX;
	int farmeGridY;
	Mix_Chunk* spawnSound = nullptr;

	int spawnX;
	int spawnY;
	bool mirror = false;

	bool isActive = false;
	float age = 0;


	float shootCooldown = 0;
	float shootingSpeed = 0;
	int health = 999;
	void (*renderer)(SDL_Renderer* renderer, Enemy* enemy, vec2 offset, AssetManager* assetManager);

	vec2* shootPoses;
	int shootPoseIndex;

	class Enemy* next = nullptr;

	bool Update(Game* game, float deltaTime);
	void DamageTick(Game* game, float deltaTime);
	void DamageRender(SDL_Renderer* renderer);
	void DeathRender(SDL_Renderer* renderer);

	void Render(SDL_Renderer* renderer, AssetManager* assetManager);

	static void DefaultEnemyRenderer(SDL_Renderer* renderer, Enemy* enemy, vec2 offset, AssetManager* assetManager);
	static void LaserEnemyRenderer(SDL_Renderer* renderer, Enemy* enemy, vec2 offset, AssetManager* assetManager);
	static void SmallEnemyRenderer(SDL_Renderer* renderer, Enemy* enemy, vec2 offset, AssetManager* assetManager);

	static vec2 downThenLeave(Enemy* enemy, float i, int speed, vec2 pos);
	static vec2 waveThenLeave(Enemy* enemy, float i, int speed, vec2 pos);

	static vec2 stayTop(Enemy* enemy, float i, int speed, vec2 pos);
	static vec2 stayTopThenLeave(Enemy* enemy, float i, int speed, vec2 pos);

	void Shoot(Game* game, vec2 pos);

	Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool) = Bullet::SingleShot;

	SDL_Color color = { 255, 255, 255 };

	bool onHit(Game* game, float deltaTime, bool giveScore);

	void ResetData();
	//SDL_Texture* bulletTexture;

private:

	struct Explosion
	{
		glm::vec2 offset;
		glm::vec2 dim;
		int frame;
	};


	std::vector<Explosion*> explosions;
	float explosionDelay = 0;
	float damageTimer = 0;
	bool isDying = false;

	void Move();
	//Bullet* head;
	//static const uint32_t poolSize = 1000;
	//Bullet bullets[poolSize];
	//bool shooting;
};