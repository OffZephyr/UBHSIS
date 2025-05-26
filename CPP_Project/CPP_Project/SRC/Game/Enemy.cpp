#include "Game.h"
#include "Enemy.h"
#include "../Engine/Utils.hpp"
#include "../Engine/DrawingManager.h"
#include <iostream>

bool Enemy::Update(Game* game, float deltaTime)
{
	if (spawnSound != nullptr && age == 0) {
		Mix_PlayChannel(-1, spawnSound, 0);
	}
	age += deltaTime;

	bool leftHalf = pos.x < 512 - 32;
	const float rightBorder = DISPLAY_WIDTH - dim.x;
	const float bottomBorder = DISPLAY_HEIGHT - dim.y;

	for (Explosion* e : explosions) {

		if (e->frame / 4 > 16) {
			explosions.erase(std::find(explosions.begin(), explosions.end(), e));
			delete e;
		}
	}


	if (pos.x >= rightBorder + 100)
	{
		return true;
	}
	else if (pos.x <= -100)
	{
		return true;
	}

	if (pos.y >= bottomBorder + 100)
	{
		return true;
	}

	if (isDying)
	{
		float size = Utils::RndFloat(15, 75);

		float x = Utils::RndFloat(0 - (size / 2), dim.x - (size / 2));
		float y = Utils::RndFloat(0 - (size / 2), dim.y - (size / 2));

		explosionDelay -= deltaTime;
		if (explosionDelay < 0) {
			explosionDelay = 0.1f;
			explosions.push_back(
				new Explosion(
					glm::vec2(x, y),
					glm::vec2(size, size),
					0
				)
			);
		}

		pos += glm::vec2(leftHalf ? -2 : 2, 2);
		return false;
	}

	Move();
	const SDL_Rect* A = new SDL_Rect(pos.x, pos.y, dim.x, dim.y);
	for (size_t i = 0; i < game->bulletPoolSize; i++)
	{
		if (game->bullets[i].isActive)
		{
			Bullet* bullet = &game->bullets[i];
			const SDL_Rect* B = new SDL_Rect(bullet->state.params.pos.x, bullet->state.params.pos.y, bullet->dim.x, bullet->dim.y);

			SDL_bool overlap = SDL_HasIntersection(A, B);
			if (overlap && !bullet->hurtsPlayer) {
				if (bullet->removeOnImpact) bullet->hit = true;
				return onHit(game, deltaTime, true);
			}
		}
	}

	const SDL_Rect* B = new SDL_Rect(game->getPlayer()->pos.x, game->getPlayer()->pos.y, game->getPlayer()->dim.x, game->getPlayer()->dim.y);

	SDL_bool overlap = SDL_HasIntersection(A, B);
	if (overlap && !game->getPlayer()->isDying && game->getPlayer()->invulnerabilityTimer <= 0) {
		game->getPlayer()->onHit(game, deltaTime);
		return onHit(game, deltaTime, false);
	}

	if (damageTimer > 0)
	{
		DamageTick(game, deltaTime);
	}

	shootCooldown += deltaTime;
	if (shootCooldown > shootingSpeed) {
		shootCooldown -= shootingSpeed;
		Shoot(game, vec2(pos.x + shootPoses[shootPoseIndex].x, pos.y + shootPoses[shootPoseIndex].y));
		shootPoseIndex = shootPoseIndex + 1 >= shootPoses->length() ? 0 : shootPoseIndex + 1;
	}

	return false;
}

void Enemy::Shoot(Game* game, vec2 pos)
{
	if (game->bulletHead)
	{
		Mix_PlayChannel(0, Game::instance->getAssetManager()->GetSound("Shoot"), 0);
		game->bulletHead = shootFunction(pos, age, game->bulletHead, true);
	}
}

void Enemy::DamageTick(Game* game, float deltaTime)
{
	damageTimer -= deltaTime;
	damageTimer = std::clamp(damageTimer, 0.0f, 1.0f);
}

void Enemy::DamageRender(SDL_Renderer* renderer)
{
	float index = damageTimer / 0.25f;
	color.g = std::lerp(255, 0, index);
	color.b = std::lerp(255, 0, index);
}

void Enemy::DeathRender(SDL_Renderer* renderer)
{
	float index = (sin(age * 10) + 1) / 2.0f;
	color.g = std::lerp(150, 0, index);
	color.b = std::lerp(150, 0, index);
}

void Enemy::Move()
{
	glm::vec2 vecPos = movementFunction(this, age, speed, pos);
	pos.x = vecPos.x;
	int x = vecPos.x;

	if (mirror) x = -x;
	int y = vecPos.y;

	if (age < 2)
	{
		float i = age - 2;
		y = vecPos.y - (i * 200 * i);
	}

	pos.x = spawnX + x;
	pos.y = spawnY + y;
}

bool Enemy::onHit(Game* game, float deltaTime, bool giveScore)
{
	Mix_PlayChannel(-1, Game::instance->getAssetManager()->GetSound("HitEnemy"), 0);
	health--;
	isDying = health <= 0;
	if(giveScore) game->addScore(isDying ? 50 : 10);
	damageTimer = 0.25f;

	if (isDying) {
		Mix_VolumeChunk(Game::instance->getAssetManager()->GetSound("Explosion"), 16);
		Mix_PlayChannel(-1, Game::instance->getAssetManager()->GetSound("Explosion"), 0);
	}

	Utils::RndSpawnUpgrade(game, vec2(pos.x + (dim.x / 2) - 16, pos.y + (dim.y / 2) - 16), isDying ? 25 : 1);

	return false;
}

void Enemy::Render(SDL_Renderer* renderer, AssetManager* assetManager)
{
	color.g = 255;
	color.b = 255;
	color.r = 255;

	float xOffset = 0;
	float yOffset = 0;

	if (isDying)
	{
		yOffset = sin(age * 15) * 16;
		DeathRender(renderer);
		animSpeed = 25;
	}
	else if (damageTimer > 0)
	{
		DamageRender(renderer);
	}

	// Get renderer per enemy
	this->renderer(renderer, this, vec2(xOffset, yOffset), assetManager);

	for (Explosion* e : explosions) {
		//compute animation frame
		uint32_t e_animFrame = e->frame / 4;

		//source rectangle for the blit
		SDL_Rect e_src = { e_animFrame * 71 - ((e_animFrame / 6) * 426) , (e_animFrame / 6) * 100, 71, 100 };

		//dest rectangle
		SDL_FRect e_dst = { pos.x + e->offset.x, pos.y + e->offset.y, e->dim.x, e->dim.y };

		e->frame++;

		SDL_Texture* explosionTexture = assetManager->GetTexture("Explosion");

		SDL_SetTextureColorMod(explosionTexture, 255, 255, 255);
		SDL_SetTextureBlendMode(explosionTexture, SDL_BlendMode::SDL_BLENDMODE_BLEND); //alpha blend
		DrawingManager::DrawImage(renderer, explosionTexture, &e_src, &e_dst);
	}
}
void Enemy::DefaultEnemyRenderer(SDL_Renderer* renderer, Enemy* enemy, vec2 offset, AssetManager* assetManager)
{
	//compute animation frame
	uint32_t animFrame = (SDL_GetTicks64() * enemy->animSpeed / 1000) % 4;

	//source rectangle for the blit
	SDL_Rect src = { animFrame * enemy->dim.x - ((animFrame / 2) * (enemy->dim.y * 2)) , (animFrame / 2) * enemy->dim.y, enemy->dim.x, enemy->dim.y };

	//dest rectangle
	SDL_FRect dst = { enemy->pos.x + offset.x, enemy->pos.y + offset.y, enemy->dim.x, enemy->dim.y };

	//blit
	SDL_SetTextureColorMod(enemy->texture, enemy->color.r, enemy->color.g, enemy->color.b);
	SDL_SetTextureBlendMode(enemy->texture, SDL_BlendMode::SDL_BLENDMODE_BLEND); //alpha blend
	DrawingManager::DrawImage(renderer, enemy->texture, &src, &dst);
}
void Enemy::LaserEnemyRenderer(SDL_Renderer* renderer, Enemy* enemy, vec2 offset, AssetManager* assetManager)
{
	//compute animation frame
	uint32_t animFrame = (int)((enemy->age) * (2 * (enemy->age))) % 3;

	if (enemy->isDying) animFrame = 0;

	//source rectangle for the blit
	SDL_Rect defaultFrame = { animFrame * enemy->dim.x, 0, enemy->dim.x, enemy->dim.y };

	SDL_Rect laser1 = { 0, enemy->dim.y, enemy->dim.x, enemy->dim.y };
	SDL_Rect laser2 = { enemy->dim.x, enemy->dim.y, enemy->dim.x, enemy->dim.y };
	SDL_Rect laser3 = { enemy->dim.x * 2, enemy->dim.y, enemy->dim.x, enemy->dim.y };

	SDL_Rect src;

	float width = std::clamp(std::lerp(0, 44, enemy->age / 4.0f), 0.0, 44.0);
	int beam_x = enemy->pos.x + (enemy->dim.y / 2) - (width / 2);
	int beam_y = enemy->pos.y + (enemy->dim.y / 2);
	SDL_Rect* beam_src = new SDL_Rect(beam_x, beam_y, width, 2048);

	if (!enemy->isDying) {
		if (enemy->age < 4) {
			src = defaultFrame;

			int alpha = std::clamp(std::lerp(0, 180, enemy->age / 4.0f), 0.0, 180.0);
			SDL_Color color = { 216, 63, 111, alpha };

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
			DrawingManager::Fill(renderer, beam_src);

		}
		else {
			src = laser1;
			if (enemy->age < 4.015f) Mix_PlayChannel(-1, assetManager->GetSound("LaserShoot"), 0);
			if (enemy->age > 4.1f) src = laser2;
			if (enemy->age > 4.2f) src = laser3;
			if (enemy->age > 4.3f) src = { 0, 0, (int)enemy->dim.x, (int)enemy->dim.y };

			SDL_Rect* beam_texture_src = new SDL_Rect(src.x + 42, src.y + src.h - 1, width, 1);
			SDL_FRect beam_dst = { beam_x, beam_y, 44, 2048 };

			SDL_SetTextureColorMod(enemy->texture, enemy->color.r, enemy->color.g, enemy->color.b);
			SDL_SetTextureBlendMode(enemy->texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
			DrawingManager::DrawImage(renderer, enemy->texture, beam_texture_src, &beam_dst);


			if (enemy->age <= 4.3f) {
				Game* game = Game::instance;

				const SDL_Rect* A = new SDL_Rect(beam_x, beam_y, 44, 2048);
				const SDL_Rect* B = new SDL_Rect(game->getPlayer()->pos.x, game->getPlayer()->pos.y, game->getPlayer()->dim.x, game->getPlayer()->dim.y);

				SDL_bool overlap = SDL_HasIntersection(A, B);
				if (overlap && !game->getPlayer()->isDying && game->getPlayer()->invulnerabilityTimer <= 0) {
					game->getPlayer()->onHit(game, 0, 3);
				}
			}
		}

	}
	else {
		src = defaultFrame;
	}
	//dest rectangle
	SDL_FRect dst = { enemy->pos.x, enemy->pos.y + offset.y, enemy->dim.x, enemy->dim.y };

	//blit
	SDL_SetTextureColorMod(enemy->texture, enemy->color.r, enemy->color.g, enemy->color.b);
	SDL_SetTextureBlendMode(enemy->texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	DrawingManager::DrawImage(renderer, enemy->texture, &src, &dst);
}
void Enemy::SmallEnemyRenderer(SDL_Renderer* renderer, Enemy* enemy, vec2 offset, AssetManager* assetManager)
{
	//compute animation frame
	uint32_t animFrame = (SDL_GetTicks64() * enemy->animSpeed / 1000) % 3;

	//source rectangle for the blit
	SDL_Rect src = { animFrame * enemy->dim.x ,0, enemy->dim.x, enemy->dim.y };

	//dest rectangle
	SDL_FRect dst = { enemy->pos.x + offset.x - 8, enemy->pos.y + offset.y - 8, 48, 48 };

	//blit
	SDL_SetTextureColorMod(enemy->texture, enemy->color.r, enemy->color.g, enemy->color.b);
	SDL_SetTextureBlendMode(enemy->texture, SDL_BlendMode::SDL_BLENDMODE_BLEND); //alpha blend
	DrawingManager::DrawImage(renderer, enemy->texture, &src, &dst);
}

glm::vec2 Enemy::downThenLeave(Enemy* enemy, float i, int speed, vec2 pos) {
	int x = 0;
	int y = i * speed;

	if (pos.y > 400)
	{
		float j = (y - 400.0f) / 500.0f;
		j = glm::clamp(j, 0.0f, 10.0f);
		x = x - (j * 1000 * j);
	}

	return vec2(x, y);
}
glm::vec2 Enemy::waveThenLeave(Enemy* enemy, float i, int speed, vec2 pos) {

	int x = 0;
	int y = i * speed;

	x = sin(((y / M_PI) / (10))) * 100;

	if (pos.y > 400)
	{
		float j = (y - 400.0f) / 500.0f;
		j = glm::clamp(j, 0.0f, 10.0f);
		x = x - (j * 1000 * j);
	}
	return vec2(x, y);
}
glm::vec2 Enemy::stayTop(Enemy* enemy, float i, int speed, vec2 pos) {

	int x = 0;
	int y = 0;
	return vec2(x, y);
}
glm::vec2 Enemy::stayTopThenLeave(Enemy* enemy, float i, int speed, vec2 pos) {

	int x = 0;
	int y = 0;

	if (i > 6)
	{
		float i2 = (i - 6);
		y = i2 * (i2 * speed);
		float j = y / 300.0f;
		j = glm::clamp(j, 0.0f, 10.0f);
		x = x - (j * 1000 * j);
	}

	return vec2(x, y);
}

bool EnemySpawner::Update(Game* game, float deltaTime) {

	// SPAWN ENEMIES THEN UNLOAD
	bool done = true;
	for (auto& data : spawners) {
		if (!data.triggered) {
			done = false;
			if (data.delay <= age) {
				data.triggered = true;
				game->CreateEnemy(data.type, data.pos.x, data.pos.y, data.speed, data.path, data.mirrorPath);
			}
		}
	}
	age += deltaTime;
	return done;
}

void Enemy::ResetData() {
	age = 0;
	isDying = false;
	shootCooldown = Utils::RndFloat(0, shootingSpeed);
	explosionDelay = 0;
	damageTimer = 0;
}