#pragma once
#include <glm.hpp>
#include <cstdint>
#include <memory>
#include <map>
#include "../Engine/Entity.h"
#include "Bullet.h"
#include "Player.h"

class DroppedItem;

struct ItemData
{
	int textureX = 0;
	int textureY = 0;
	float data = 0; //shootingSpeed | health | shield amount
	void (*pickUpFunction)(Game*, ItemData);
	Bullet* (*shootFunction)(glm::vec2, float, Bullet*, bool) = Bullet::SingleShot;
};

class DroppedItem
{
public:
	virtual bool Update(Game* game, float deltaTime);
	virtual void Render(SDL_Renderer* renderer);

	glm::vec2 pos;

	Bullet* (*shootingFunction)(glm::vec2, float, Bullet*);
	ItemData data;
	SDL_Texture* texture;
	SDL_FRect* dst;
	bool isActive = false;
	class DroppedItem* next = nullptr;

	static void BulletUpgradePickup(Game* game, ItemData item);
	static void HealthUpgradePickup(Game* game, ItemData item);
	static void ShieldUpgradePickup(Game* game, ItemData item);

	static const inline std::vector<ItemData> DROPPABLE_UPGRADES = {
		ItemData(0, 0, 0.2f, BulletUpgradePickup, Bullet::SingleShot),
		ItemData(32, 0, 0.2f, BulletUpgradePickup, Bullet::DoubleShot),
		ItemData(64, 0, 0.2f, BulletUpgradePickup, Bullet::TripleShot),
		ItemData(0, 32, 0.2f, BulletUpgradePickup, Bullet::QuadrupleShot),
		ItemData(32, 32, 0.2f, BulletUpgradePickup, Bullet::QuintupleShot),
		ItemData(64, 32, 0.025f, BulletUpgradePickup, Bullet::SpreadShot),

		ItemData(96, 0, 1, HealthUpgradePickup, Bullet::SingleShot),
		ItemData(128, 0, 2, HealthUpgradePickup, Bullet::SingleShot),
		ItemData(96, 32, 3, HealthUpgradePickup, Bullet::SingleShot),

		ItemData(128, 32, 1, ShieldUpgradePickup, Bullet::SingleShot),
		ItemData(128, 32, 1, ShieldUpgradePickup, Bullet::SingleShot),
		ItemData(128, 32, 1, ShieldUpgradePickup, Bullet::SingleShot)
	};

private:
	inline static const glm::vec2 dim = { 48, 48 };
	inline static const SDL_Color color = { 255, 255, 255 };
};