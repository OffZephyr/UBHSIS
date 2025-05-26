#include <glm.hpp>
#include <SDL.h>
#include <cstdint>
#include <random>

class Utils
{
	public:

		static glm::vec2 RndVec2(float min, float max)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dist(min, max);
			return glm::vec2(dist(gen), dist(gen));
		}

		static float RndFloat(float min, float max)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dist(min, max);
			return dist(gen);
		}

		static SDL_Color RndCol()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, 255);

			SDL_Color color;
			color.r = static_cast<uint8_t>(dist(gen));
			color.g = static_cast<uint8_t>(dist(gen));
			color.b = static_cast<uint8_t>(dist(gen));
			color.a = 255;

			return color;
		}

		static Music RndSong()
		{
			std::vector<std::string> names = Game::instance->getAssetManager()->track_list;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, names.size() - 1);

			std::string track = names[(int)dist(gen)];
			printf(track.c_str());
			return Game::instance->getAssetManager()->GetMusic(track);
		}

		static void RndSpawnUpgrade(Game* game, glm::vec2 pos, int chance)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dist(0, 100);
			if (dist(gen) < chance) {

				std::mt19937 gen2(rd());
				std::uniform_real_distribution<float> dist2(0, 12);

				ItemData upgrade = DroppedItem::DROPPABLE_UPGRADES[(int)dist2(gen2)];

				game->CreateBulletUpgrade(upgrade, pos.x, pos.y);
			}
		}

};
