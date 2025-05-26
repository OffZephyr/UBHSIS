#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_TTF.h>

struct Music
{
	Mix_Music* music;
	SDL_Texture* texture;
	std::string name;
	std::string author;
};

class AssetManager
{
	public:
		AssetManager(SDL_Renderer* renderer);
		~AssetManager();

		void ClearAssets();
		void AddMusic(std::string name, std::string author, std::string_view fileName, SDL_Texture* texture);
		void AddSound(std::string_view name, std::string_view fileName);
		void AddTexture(std::string_view name, std::string_view fileName);
		void AddSurface(std::string_view name, std::string_view fileName);
		void AddFont(std::string_view name, std::string_view fileName);
		TTF_Font* GetFont(std::string_view name) const;
		Music GetMusic(std::string name) const;
		Mix_Chunk* GetSound(std::string_view name) const;
		SDL_Texture* GetTexture(std::string_view name) const;
		SDL_Surface* GetSurface(std::string_view name) const;

		std::vector<std::string> track_list;
	private:
		SDL_Renderer* renderer;
		SDL_Texture* missingTexture = IMG_LoadTexture(renderer, "./Assets/Images/MissingTexture.tga");

		std::unordered_map<std::string_view, TTF_Font*> fonts;
		std::unordered_map<std::string, Music> musics;
		std::unordered_map<std::string_view, Mix_Chunk*> sounds;
		std::unordered_map<std::string_view, SDL_Texture*> textures;
		std::unordered_map<std::string_view, SDL_Surface*> surfaces;
		//add maps for fonts, audio...

};
