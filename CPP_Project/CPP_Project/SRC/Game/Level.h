#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include "../Engine/Logger.h"
#include "Game.h"

class Level
{
	public:
		Level(SDL_Texture* t, const std::string& mapData, size_t w, size_t h, const std::string& mapData2, size_t w2, size_t h2) : tileSetTexture(t), width(w), height(h), width2(w2), height2(h2)
		{
			Logger::Log("Level constructor called");
			ReadTileMapData(mapData, tileMap, w, h);
			ReadTileMapData(mapData2, tileMap2, w2, h2);
		}
		~Level();

		void Update(float deltaTime);
		void Render(SDL_Renderer* renderer);

		float camPosY = 0;
	private:
		void ReadTileMapData(const std::string& filename, std::vector<int>& tileMap, size_t w, size_t h);

		SDL_Texture* tileSetTexture;
		
		static const size_t tileHeight = 128;
		static const size_t tileWidth = 128;

		size_t width;
		size_t height;
		size_t width2;
		size_t height2;
		std::vector<int> tileMap; //assuming we do not have a tileset larger than 256 indices
		std::vector<int> tileMap2; //assuming we do not have a tileset larger than 256 indices

		//camera stuff (could be in its own class)
		const float initCamPosY = height * tileHeight - DISPLAY_HEIGHT;
		float camSpeed = 50.0f;

		//other data such as enemies or pickups initial pos/time or attack patterns could be stored here
};
