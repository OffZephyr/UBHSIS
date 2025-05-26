#include "DrawingManager.h"

void DrawingManager::GetDrawPos(SDL_Renderer* renderer, int& windowWidth, int& windowHeight, int& windowMax, int& windowMin, float& scale, float& width, float& height, float& x, float& y) {

	SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

	windowMax = std::max(windowWidth, windowHeight);
	windowMin = std::min(windowWidth, windowHeight);

	scale = (1.0f / 1024.0f) * windowMin;

	width *= scale;
	height *= scale;

	x *= scale;
	y *= scale;

	centerPos(windowMax == windowWidth ? x : y, 1024 * scale, windowMax);
}

void DrawingManager::DrawImage(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_FRect* dstrect) {

	//IMAGE SCALED TO WINDOW SIZE

	int windowWidth, windowHeight, windowMax, windowMin;
	float scale = 1;
	float x = dstrect->x;
	float y = dstrect->y;
	float width = dstrect->w;
	float height = dstrect->h;

	GetDrawPos(renderer, windowWidth, windowHeight, windowMax, windowMin, scale, width, height, x, y);

	SDL_FRect dst = {x, y, width, height};

	SDL_RenderCopyF(renderer, texture, srcrect, &dst);
}

void DrawingManager::Fill(SDL_Renderer* renderer, const SDL_Rect* rect) {

	//RECTANGLE SCALED TO WINDOW SIZE

	int windowWidth, windowHeight, windowMax, windowMin;
	float scale = 1;
	float x = rect->x;
	float y = rect->y;
	float width = rect->w;
	float height = rect->h;

	GetDrawPos(renderer, windowWidth, windowHeight, windowMax, windowMin, scale, width, height, x, y);

	SDL_Rect dst = {x, y, width, height};

	SDL_RenderFillRect(renderer, &dst);
}
void DrawingManager::FillOutside(SDL_Renderer* renderer, const SDL_Color color) {

	//HIDE DEAD WINDOW SPACE

	int windowWidth, windowHeight, windowMax, windowMin;
	float scale = 1;
	float x = 0;
	float y = 0;
	float width = 1024;
	float height = 1024;

	GetDrawPos(renderer, windowWidth, windowHeight, windowMax, windowMin, scale, width, height, x, y);

	float size = 1024 * scale;
	width = windowMax == windowWidth ? (windowMax - size) / 2 : size;
	height = windowMax == windowHeight ? (windowMax - size) / 2 : size;

	x = windowMax == windowWidth ? windowMax - width : 0;
	y = windowMax == windowHeight ? windowMax - height : 0;

	SDL_Rect dst1 = {0, 0, width, height};
	SDL_Rect dst2 = {x, y, width, height};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dst1);
	SDL_RenderFillRect(renderer, &dst2);
}
void DrawingManager::WriteText(SDL_Renderer* renderer, TTF_Font* font, std::string text, glm::vec2 pos, int size) {

	//TEXT SCALED TO WINDOW SIZE

	TTF_SetFontSize(font, size);

	SDL_Color White = { 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), White);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);

	int width = 512;
	int height = 512;
	TTF_SizeText(font, text.c_str(), &width, &height);
	SDL_FRect dst = { pos.x, pos.y, width, height };

	DrawingManager::DrawImage(renderer, texture, NULL, &dst);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void DrawingManager::centerPos(float& i, float size, float max) {
	i += (max - size) / 2;
}