#include "AssetManager.h"
#include "Logger.h"
#include <SDL_image.h>
#include <SDL_mixer.h>

AssetManager::AssetManager(SDL_Renderer* renderer)
{
	this->renderer = renderer;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_AllocateChannels(128);
	TTF_Init();
	Logger::Log("AssetManager constructor called");
}

AssetManager::~AssetManager()
{
	ClearAssets();
	Logger::Log("AssetManager destructor called");
}

void AssetManager::ClearAssets()
{
	for (auto [name, texture] : textures) //key, value
	{
		SDL_DestroyTexture(texture);
	}
	for (auto [name, surface] : surfaces)
	{
		SDL_FreeSurface(surface);
	}
	textures.clear();
	surfaces.clear();
	sounds.clear();
	track_list.clear();
	musics.clear();
	fonts.clear();
}

void AssetManager::AddMusic(std::string name, std::string author, std::string_view fileName, SDL_Texture* texture)
{
	Mix_Music* music = Mix_LoadMUS(fileName.data());

	if (music)
	{
		Music musicStruct = {
			music,
			texture,
			name,
			author
		};

		track_list.push_back(name);
		musics.emplace(name, musicStruct);
		Logger::Log("New music added to the assetManager: " + static_cast<std::string>(name));
	}
	else Logger::Err("Cannot find file: " + static_cast<std::string>(fileName));
}
void AssetManager::AddFont(std::string_view name, std::string_view fileName)
{
	TTF_Font* font = TTF_OpenFont(fileName.data(), 24);
	if (font)
	{
		fonts.emplace(name, font);
		Logger::Log("New font added to the assetManager: " + static_cast<std::string>(name));
	}
	else {
		Logger::Err(TTF_GetError());
		Logger::Err("Cannot find file: " + static_cast<std::string>(fileName));
	}
}
void AssetManager::AddSound(std::string_view name, std::string_view fileName)
{
	Mix_Chunk* sound = Mix_LoadWAV(fileName.data());
	if (sound)
	{
		sounds.emplace(name, sound);
		Logger::Log("New sound added to the assetManager: " + static_cast<std::string>(name));
	}
	else Logger::Err("Cannot find file: " + static_cast<std::string>(fileName));
}
void AssetManager::AddTexture(std::string_view name, std::string_view fileName)
{
	SDL_Texture* texture = IMG_LoadTexture(renderer, fileName.data()); //get the raw const char*
	if (texture)
	{
		textures.emplace(name, texture);
		Logger::Log("New texture added to the assetManager: " + static_cast<std::string>(name));
	}
	else Logger::Err("Cannot find file: " + static_cast<std::string>(fileName));
}

void AssetManager::AddSurface(std::string_view name, std::string_view fileName)
{
	SDL_Surface* texture = IMG_Load(fileName.data()); //get the raw const char*
	if (texture)
	{
		surfaces.emplace(name, texture);
		Logger::Log("New surface added to the assetManager: " + static_cast<std::string>(name));
	}
	else Logger::Err("Cannot find file: " + static_cast<std::string>(fileName));
}

Music AssetManager::GetMusic(std::string name) const
{
	return musics.at(name);
}
Mix_Chunk* AssetManager::GetSound(std::string_view name) const
{
	return sounds.at(name);
}
TTF_Font* AssetManager::GetFont(std::string_view name) const
{
	return fonts.at(name);
}
SDL_Texture* AssetManager::GetTexture(std::string_view name) const
{
	if (textures.contains(name)) {
		return textures.at(name);
	}
	else {
		return missingTexture;
	}
}
SDL_Surface* AssetManager::GetSurface(std::string_view name) const
{
	return surfaces.at(name);
}

