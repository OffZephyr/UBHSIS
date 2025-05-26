#pragma once
#include <SDL.h>
#include "../Game/Game.h"


class DrawingManager
{
public:
	static void GetDrawPos(SDL_Renderer* renderer, int& windowWidth, int& windowHeight, int& windowMax, int& windowMin, float& scale, float& width, float& height, float& x, float& y);
	static void DrawImage(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_FRect* dstrect);
	static void Fill(SDL_Renderer* renderer, const SDL_Rect* rect);
	static void FillOutside(SDL_Renderer* renderer, const SDL_Color color);
	static void WriteText(SDL_Renderer* renderer, TTF_Font* font, std::string text, glm::vec2 pos, int size);
	static void centerPos(float& i, float size, float max);
};