#include "Game.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "../Engine/Utils.hpp"
#include "../Engine/DrawingManager.h"
#include <iostream>

void Player::Shoot(Game* game, vec2 pos)
{
	if (game->bulletHead && shootCooldown == 0)
	{
		Mix_PlayChannel(0, Game::instance->getAssetManager()->GetSound("Shoot"), 0);
		shootCooldown = shootCooldownLength;
		game->bulletHead = shootFunction(pos, age, game->bulletHead, false);
	}
}

// PLAYER

bool Player::Update(Game* game, float deltaTime)
{
	age += deltaTime;

	const float rightBorder = DISPLAY_WIDTH - dim.x;
	const float bottomBorder = DISPLAY_HEIGHT - dim.y;

	if (isDying) {
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

		pos += glm::vec2(0, 2);
	}
	else {
		age += deltaTime;

		if (input->IsActionHeld(Action::MoveLeft))	pos.x -= speed * deltaTime;
		if (input->IsActionHeld(Action::MoveRight))	pos.x += speed * deltaTime;
		if (input->IsActionHeld(Action::MoveUp))	pos.y -= speed * deltaTime;
		if (input->IsActionHeld(Action::MoveDown))	pos.y += speed * deltaTime;
		if (input->IsActionHeld(Action::Shoot))	Shoot(game, vec2(pos + vec2(24, 0)));

		pos.x += input->GetLeftStickX() * speed * deltaTime;
		pos.y += input->GetLeftStickY() * speed * deltaTime;

		//clamp position to borders

		if (pos.x >= rightBorder) pos.x = rightBorder;
		else if (pos.x <= 0) pos.x = 0;
		if (pos.y >= bottomBorder) pos.y = bottomBorder;
		else if (pos.y <= 0) pos.y = 0;

		if (invulnerabilityTimer <= 0)
		{
		const SDL_Rect* A = new SDL_Rect(pos.x, pos.y, dim.x, dim.y);
		for (size_t i = 0; i < game->bulletPoolSize; i++)
		{
			if (game->bullets[i].isActive)
			{
				Bullet* bullet = &game->bullets[i];
				const SDL_Rect* B = new SDL_Rect(bullet->state.params.pos.x, bullet->state.params.pos.y, bullet->dim.x, bullet->dim.y);

				SDL_bool overlap = SDL_HasIntersection(A, B);
				if (overlap && bullet->hurtsPlayer) {
					if (bullet->removeOnImpact) bullet->hit = true;
					onHit(game, deltaTime);
				}
			}
		}
		}

		invulnerabilityTimer = invulnerabilityTimer > 0 ? invulnerabilityTimer - deltaTime : 0;
		damageTimer = damageTimer > 0 ? damageTimer - deltaTime : 0;
		shootCooldown = shootCooldown > 0 ? shootCooldown - deltaTime : 0;

#ifndef NDEBUG
		//std::cout << "Num of bullets: " << bullets.size() << std::endl;
#endif
		return false;
	}
}

void Player::onHit(Game* game, float deltaTime, int damage)
{
	if (shield > 0) {
		shield -= damage;
		if (shield < 0) {
			health += shield;
			shield = 0;
		}
	}
	else {
		Mix_PlayChannel(-1, Game::instance->getAssetManager()->GetSound("HitPlayer"), 0);
		health -= damage;
	}

	health = std::clamp(health, 0, 12);

	isDying = health <= 0;
	damageTimer = 0.25f;
	invulnerabilityTimer = 0.5f;
	if (isDying) {
		Mix_VolumeChunk(Game::instance->getAssetManager()->GetSound("Explosion"), 32);
		Mix_PlayChannel(-1, Game::instance->getAssetManager()->GetSound("Explosion"), 0);
	}
}
void Player::onHit(Game* game, float deltaTime)
{
	onHit(game, deltaTime, 1);
}

void Player::Render(SDL_Renderer* renderer, AssetManager* assetManager)
{
	color.g = 255;
	color.b = 255;
	color.r = 255;

	float xOffset = 0;
	float yOffset = 0;

	if (isDying)
	{
		xOffset = sin(age * 25) * 5;

		float index = (sin(age * 10) + 1) / 2.0f;

		color.g = std::lerp(150, 0, index);
		color.b = std::lerp(150, 0, index);

		animSpeed = 25;
	}
	else if (damageTimer > 0) {
		float index = damageTimer / 0.25f;

		if (shield > 0) {
			color.r = std::lerp(255, 50, index);
			color.g = std::lerp(255, 50, index);
		}
		else {
			color.g = std::lerp(255, 0, index);
			color.b = std::lerp(255, 0, index);
		}
	}

	//compute animation frame
	uint32_t animFrame = (SDL_GetTicks64() * animSpeed / 1000) % 2; //2 frames for this anim

	//source rectangle for the blit
	SDL_Rect src = { animFrame * 64 , 0, 64, 64 };

	//dest rectangle
	SDL_FRect dst = { pos.x + xOffset, pos.y + yOffset, dim.x, dim.y };

	//blit
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND); //alpha blend
	DrawingManager::DrawImage(renderer, texture, &src, &dst);

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
