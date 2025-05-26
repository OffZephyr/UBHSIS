#pragma once
#include <glm.hpp>
#include <cstdint>
#include "../Engine/AssetManager.h"

class Game;

enum class EntityType;

class Entity
{
	public:
		using vec2 = glm::vec2;

		vec2 pos;
		vec2 vel;
		vec2 dim = {8, 8}; //width and height

		EntityType type;
		SDL_Texture* texture;
		bool toBeDestroyed = false;

		Entity(vec2 p, vec2 v, EntityType t, SDL_Texture* tx) : pos(p), vel(v), type(t), texture(tx) {}
		virtual ~Entity() {}

		virtual bool Update(Game* game, float deltaTime) = 0;
		virtual void Render(SDL_Renderer* renderer, AssetManager* assetManager) = 0;

	private:
};

