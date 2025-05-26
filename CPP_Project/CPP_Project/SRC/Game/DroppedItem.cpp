#include "Game.h"
#include "Bullet.h"
#include "DroppedItem.h"
#include "Player.h"
#include "Enemy.h"
#include "../Engine/Utils.hpp"
#include "../Engine/DrawingManager.h"
#include <iostream>
bool DroppedItem::Update(Game* game, float deltaTime)
{
	pos.y += 80 * deltaTime;

	const float bottomBorder = DISPLAY_HEIGHT - dim.y;

	if (pos.y >= bottomBorder + 50)
	{
		return true;
	}

	Player* player = game->getPlayer();

	const SDL_Rect* A = new SDL_Rect(pos.x, pos.y, dim.x, dim.y);
	const SDL_Rect* B = new SDL_Rect(player->pos.x, player->pos.y, player->dim.x, player->dim.y);
	SDL_bool overlap = SDL_HasIntersection(A, B);
	if (overlap) {
		data.pickUpFunction(game, data);
	}
	return overlap;
}

void DroppedItem::Render(SDL_Renderer* renderer)
{
	//source rectangle for the blit
	SDL_Rect src = { data.textureX, data.textureY ,32 ,32 };

	//dest rectangle
	SDL_FRect dst = { pos.x, pos.y, dim.x, dim.y };

	//blit
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	DrawingManager::DrawImage(renderer, texture, &src, &dst);
}

void DroppedItem::BulletUpgradePickup(Game* game, ItemData data)
{
	Mix_PlayChannel(-1, Game::instance->getAssetManager()->GetSound("Equip"), 0);
	game->getPlayer()->shootFunction = data.shootFunction;
	game->getPlayer()->shootCooldownLength = data.data;
}

void DroppedItem::HealthUpgradePickup(Game* game, ItemData data)
{
	Mix_PlayChannel(-1, Game::instance->getAssetManager()->GetSound("Heal"), 0);
	game->getPlayer()->health = std::clamp(game->getPlayer()->health + data.data, 0.0f, 12.0f);
}

void DroppedItem::ShieldUpgradePickup(Game* game, ItemData data)
{
	game->getPlayer()->shield = std::clamp(game->getPlayer()->shield + data.data, 0.0f, 3.0f);
}