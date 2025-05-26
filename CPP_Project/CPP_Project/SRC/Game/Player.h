#pragma once
#include "../Engine/Entity.h"
#include "Bullet.h"
#include "DroppedItem.h"
#include <memory>
#include <map>

class Game;
class Enemy;
class Bullet;

enum class EntityType { PLAYER, ENEMY };

class Player : public Entity
{
	public:
		float age = 0;

		int health = 12;
		int shield = 0;

		void (*shoot)(glm::vec2, glm::vec2);
		float speed;
		int animSpeed; //frames per second
		std::unique_ptr<Input>& input;

		Player(vec2 p, SDL_Texture* t, SDL_Texture* bt, std::unique_ptr<Input>& i) : Entity(p, vec2(0,0), EntityType::PLAYER, t), bulletTexture(bt), input(i)
		{
			Bullet::texture = bulletTexture;
		}
		~Player() = default;

		bool Update(Game* game, float deltaTime) override;
		void Render(SDL_Renderer* renderer, AssetManager* assetManager) override;

		void Shoot(Game* game, vec2 pos);

		void onHit(Game* game, float deltaTime);
		void onHit(Game* game, float deltaTime, int damage);

		Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool) = Bullet::SingleShot;
		float shootCooldownLength = 0.2f;
		float shootCooldown = 0;

		float invulnerabilityTimer = 0;
		bool isDying = false;
	private:
		Bullet* head;
		SDL_Texture* bulletTexture;
		static const uint32_t poolSize = 1000;
		Bullet bullets[poolSize];


		struct Explosion
		{
			glm::vec2 offset;
			glm::vec2 dim;
			int frame;
		};

		inline static SDL_Color color = { 255, 255, 255 };

		std::vector<Explosion*> explosions;
		float explosionDelay = 0;
		float damageTimer = 0;
};

