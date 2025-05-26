#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm.hpp>
#include "../Engine/DrawingManager.h"

Level::~Level()
{
	Logger::Log("Level destructor called");
}

void Level::ReadTileMapData(const std::string& filename, std::vector<int>& tileMap, size_t width, size_t height)
{
	std::ifstream file(filename);
	if (!file.is_open()) Logger::Err("Error: Could not open map file!");

	tileMap.reserve(width * height);
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream lineStream(line);
		std::string cell;

		while (std::getline(lineStream, cell, ',')) //get substring (cell) delimited by ','
		{
			int value = static_cast<int>(std::stoi(cell)); //convert to int and cast to byte
			tileMap.emplace_back(value);
		}
	}
	file.close();
}

void Level::Update(float deltaTime)
{
	camPosY -= camSpeed * deltaTime;
	//if (camPosY < tileHeight) camPosY = initCamPosY;
}

void Level::Render(SDL_Renderer* renderer)
{
	size_t tileOffset = (size_t)(camPosY / 2) / tileHeight;

	SDL_SetTextureBlendMode(tileSetTexture, SDL_BlendMode::SDL_BLENDMODE_BLEND); //alpha blend

	// STAR BACKGROUND
	for(size_t y=0; y < 9; y++)
		for (size_t x = 0; x < 8; x++)
		{
			//source rectangle for the blit

			size_t scaledTileOffset = y + tileOffset;
			scaledTileOffset = scaledTileOffset - ((scaledTileOffset / 9)) * 9;

			int tileSetIndex = tileMap[x + scaledTileOffset * width];
			glm::ivec2 srcPos = { (tileSetIndex % 16) * tileWidth , (tileSetIndex / 16) * tileHeight };
			SDL_Rect src = { srcPos.x, srcPos.y, tileWidth, tileHeight };

			//dest rectangle for the blit
			float integral;
			float frac = modf((camPosY / 2), &integral);
			float pixelOffset = frac + (int)integral % tileHeight;
			SDL_FRect dst = { x * tileWidth, y * tileHeight - pixelOffset, tileWidth +.5, tileHeight +.5 };

			//blit
			DrawingManager::DrawImage(renderer, tileSetTexture, &src, &dst);
		}

	size_t tileOffset2 = (size_t)camPosY / tileHeight;

	// PLANET MIDDLEGROUND
	for(size_t y=0; y < 24; y++)
		for (size_t x = 0; x < 8; x++)
		{
			//source rectangle for the blit
	
			size_t scaledTileOffset = y + tileOffset2;
			scaledTileOffset = scaledTileOffset - ((scaledTileOffset / 24)) * 24;
	
			int tileSetIndex = tileMap2[x + scaledTileOffset * width2];
			glm::ivec2 srcPos = { (tileSetIndex % 16) * tileWidth , (tileSetIndex / 16) * tileHeight };
			SDL_Rect src = { srcPos.x, srcPos.y, tileWidth, tileHeight };
	
			//dest rectangle for the blit
			float integral;
			float frac = modf(camPosY, &integral);
			float pixelOffset = frac + (int)integral % tileHeight;
			SDL_FRect dst = { x * tileWidth, y * tileHeight - pixelOffset, tileWidth +.5, tileHeight +.5 };
	
			//blit
			DrawingManager::DrawImage(renderer, tileSetTexture, &src, &dst);
		}
}
