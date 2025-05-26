#include "Game.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "../Engine/Utils.hpp"
#include "../Engine/DrawingManager.h"
#include <iostream>

bool Bullet::Update(Game* game, float deltaTime)
{
	this->age += deltaTime;
	if (hit) return true;
	const float rightBorder = DISPLAY_WIDTH - dim.x;
	const float bottomBorder = DISPLAY_HEIGHT - dim.y;

	state.params.pos += state.params.vel * deltaTime;

	if (state.params.pos.x >= rightBorder)
	{
		if (hurtsPlayer) return true;
		state.params.pos.x = rightBorder - (state.params.pos.x - rightBorder);
		state.params.vel.x = -state.params.vel.x;
		//return true;
	}
	else if (state.params.pos.x <= 0)
	{
		if (hurtsPlayer) return true;
		state.params.pos.x = -state.params.pos.x;
		state.params.vel.x = -state.params.vel.x;
		//return true;
	}

	if (state.params.pos.y >= bottomBorder)
	{
		//state.params.pos.y = bottomBorder - (state.params.pos.y - bottomBorder);
		//state.params.vel.y = -state.params.vel.y;
		return true;
	}
	else if (state.params.pos.y <= 0)
	{
		//state.params.pos.y = -state.params.pos.y;
		//state.params.vel.y = -state.params.vel.y;
		return true;
	}
	return false;
}

void Bullet::Render(SDL_Renderer* renderer)
{

	uint32_t animFrame = ((int)(this->age * 16)) % 2;

	//source rectangle for the blit
	SDL_Rect src = { animFrame * 16,0,16,16 };

	//dest rectangle
	SDL_FRect dst = { state.params.pos.x, state.params.pos.y, dim.x, dim.y };

	color = SDL_Color(255, 255, 255);

	//blit
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	DrawingManager::DrawImage(renderer, texture, &src, &dst);
}

// SHOOTING TYPES

Bullet* Bullet::None(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	return head;
}
Bullet* Bullet::SpreadShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	int x = sin(age * 10) * 500;

	vec2 vel = vec2(x, -500);

	return CreateBullet(pos, vel, head, hurtsPlayer);
}
Bullet* Bullet::SingleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	vec2 vel = vec2(0, -500);

	if (hurtsPlayer) {
		vec2 player_pos = { Game::instance->getPlayer()->pos.x + 32, Game::instance->getPlayer()->pos.y + 32 };
		vec2 aimvec = { player_pos.x - pos.x, pos.y - player_pos.y };
		aimvec = glm::normalize(aimvec);

		vel = { aimvec.x * 200, aimvec.y * 200};
	}

	//vec2 vel = vec2((anglevec.x / (2 * M_PI)) * -500, (anglevec.y / (2 * M_PI)) * -500);

	return CreateBullet(pos, vel, head, hurtsPlayer);
}
Bullet* Bullet::DoubleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	if (hurtsPlayer) {
		vec2 player_pos = { Game::instance->getPlayer()->pos.x + 32, Game::instance->getPlayer()->pos.y + 32 };
		vec2 aimvec = { player_pos.x - pos.x, pos.y - player_pos.y };
		aimvec = glm::normalize(aimvec);

		//float dot = glm::dot(aimvec.x, aimvec.y);
		float rotation = atan2(aimvec.y, aimvec.x);

		constexpr float angle = glm::radians(10.0f);

		vec2 vel1 = { cos(rotation + angle) * 200, sin(rotation + angle) * 200 };
		vec2 vel2 = { cos(rotation - angle) * 200, sin(rotation - angle) * 200 };

		Bullet* next = CreateBullet(pos, vel1, head, hurtsPlayer);
		return CreateBullet(pos, vel2, next, hurtsPlayer);
	}
	else {
		vec2 vel = vec2(75, -500);
		Bullet* next = CreateBullet(pos, vel, head, hurtsPlayer);
		vel = vec2(-75, -500);
		return CreateBullet(pos, vel, next, hurtsPlayer);
	}
}
Bullet* Bullet::TripleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	if (hurtsPlayer) {
		vec2 player_pos = { Game::instance->getPlayer()->pos.x + 32, Game::instance->getPlayer()->pos.y + 32 };
		vec2 aimvec = { player_pos.x - pos.x, pos.y - player_pos.y };
		aimvec = glm::normalize(aimvec);

		//float dot = glm::dot(aimvec.x, aimvec.y);
		float rotation = atan2(aimvec.y, aimvec.x);

		constexpr float angle = glm::radians(25.0f);

		vec2 vel1 = { cos(rotation + angle) * 200, sin(rotation + angle) * 200 };
		vec2 vel2 = { cos(rotation) * 200, sin(rotation) * 200 };
		vec2 vel3 = { cos(rotation - angle) * 200, sin(rotation - angle) * 200 };

		Bullet* next = CreateBullet(pos, vel1, head, hurtsPlayer);
		next = CreateBullet(pos, vel2, next, hurtsPlayer);
		return CreateBullet(pos, vel3, next, hurtsPlayer);
	}
	else {
	vec2 vel = vec2(125, -500);
	Bullet* next = CreateBullet(pos, vel, head, hurtsPlayer);
	vel = vec2(0, -500);
	next = CreateBullet(pos, vel, next, hurtsPlayer);
	vel = vec2(-125, -500);
	return CreateBullet(pos, vel, next, hurtsPlayer);
	}
}
Bullet* Bullet::QuadrupleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	if (hurtsPlayer) {
		vec2 player_pos = { Game::instance->getPlayer()->pos.x + 32, Game::instance->getPlayer()->pos.y + 32 };
		vec2 aimvec = { player_pos.x - pos.x, pos.y - player_pos.y };
		aimvec = glm::normalize(aimvec);

		//float dot = glm::dot(aimvec.x, aimvec.y);
		float rotation = atan2(aimvec.y, aimvec.x);

		constexpr float angle = glm::radians(7.0f);
		constexpr float angle2 = glm::radians(21.0f);

		vec2 vel1 = { cos(rotation + angle) * 200, sin(rotation + angle) * 200 };
		vec2 vel2 = { cos(rotation - angle) * 200, sin(rotation - angle) * 200 };
		vec2 vel3 = { cos(rotation + angle2) * 200, sin(rotation + angle2) * 200 };
		vec2 vel4 = { cos(rotation - angle2) * 200, sin(rotation - angle2) * 200 };

		Bullet* next = CreateBullet(pos, vel1, head, hurtsPlayer);
		next = CreateBullet(pos, vel2, next, hurtsPlayer);
		next = CreateBullet(pos, vel3, next, hurtsPlayer);
		return CreateBullet(pos, vel4, next, hurtsPlayer);
	}
	else {
	vec2 vel = vec2(150, -500);
	Bullet* next = CreateBullet(pos, vel, head, hurtsPlayer);
	vel = vec2(50, -500);
	next = CreateBullet(pos, vel, next, hurtsPlayer);
	vel = vec2(-50, -500);
	next = CreateBullet(pos, vel, next, hurtsPlayer);
	vel = vec2(-150, -500);
	return CreateBullet(pos, vel, next, hurtsPlayer);
	}
}
Bullet* Bullet::QuintupleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	if (hurtsPlayer) {
		vec2 player_pos = { Game::instance->getPlayer()->pos.x + 32, Game::instance->getPlayer()->pos.y + 32 };
		vec2 aimvec = { player_pos.x - pos.x, pos.y - player_pos.y };
		aimvec = glm::normalize(aimvec);

		//float dot = glm::dot(aimvec.x, aimvec.y);
		float rotation = atan2(aimvec.y, aimvec.x);

		constexpr float angle = glm::radians(20.0f);
		constexpr float angle2 = glm::radians(40.0f);

		vec2 vel1 = { cos(rotation + angle) * 200, sin(rotation + angle) * 200 };
		vec2 vel2 = { cos(rotation - angle) * 200, sin(rotation - angle) * 200 };
		vec2 vel3 = { cos(rotation) * 200, sin(rotation) * 200 };
		vec2 vel4 = { cos(rotation + angle2) * 200, sin(rotation + angle2) * 200 };
		vec2 vel5 = { cos(rotation - angle2) * 200, sin(rotation - angle2) * 200 };

		Bullet* next = CreateBullet(pos, vel1, head, hurtsPlayer);
		next = CreateBullet(pos, vel2, next, hurtsPlayer);
		next = CreateBullet(pos, vel3, next, hurtsPlayer);
		next = CreateBullet(pos, vel4, next, hurtsPlayer);
		return CreateBullet(pos, vel5, next, hurtsPlayer);
	}
	else {
	vec2 vel = vec2(175, -500);
	Bullet* next = CreateBullet(pos, vel, head, hurtsPlayer);
	vel = vec2(87.5f, -500);
	next = CreateBullet(pos, vel, next, hurtsPlayer);
	vel = vec2(0, -500);
	next = CreateBullet(pos, vel, next, hurtsPlayer);
	vel = vec2(-87.5f, -500);
	next = CreateBullet(pos, vel, next, hurtsPlayer);
	vel = vec2(-175, -500);
	return CreateBullet(pos, vel, next, hurtsPlayer);
	}
}

Bullet* Bullet::ShootAllAround(vec2 pos, float age, Bullet* head, bool hurtsPlayer)
{
	if (hurtsPlayer) {
		vec2 vel = vec2(0, -500);
		Bullet* next = CreateBullet(pos, vel, head, hurtsPlayer);
		vel = vec2(-250, -250);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(-500, 0);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(-250, 250);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(0, 500);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(250, 250);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(500, 0);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(250, -250);
		return CreateBullet(pos, vel, next, hurtsPlayer);
	}
	else {
		vec2 vel = vec2(0, -200);
		Bullet* next = CreateBullet(pos, vel, head, hurtsPlayer);
		vel = vec2(-100, -100);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(-200, 0);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(-100, 100);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(0, 200);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(100, 100);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(200, 0);
		next = CreateBullet(pos, vel, next, hurtsPlayer);
		vel = vec2(100, -100);
		return CreateBullet(pos, vel, next, hurtsPlayer);
	}
}

// Create Bullet

Bullet* Bullet::CreateBullet(vec2 pos, vec2 vel, Bullet* head, bool hurtPlayer)
{

	if (hurtPlayer) {
		vel = glm::vec2(vel.x, -vel.y);
	}

	Bullet* bullet = head;
	Bullet* next = bullet->next;

	bullet->texture = hurtPlayer ? Game::instance->getAssetManager()->GetTexture("EnemyBullet") : Game::instance->getAssetManager()->GetTexture("PlayerBullet");

	bullet->state.params.pos = pos;
	bullet->state.params.vel = vel;
	bullet->hurtsPlayer = hurtPlayer;
	bullet->isActive = true;
	return next;
}