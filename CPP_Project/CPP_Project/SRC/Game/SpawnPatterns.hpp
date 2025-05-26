#include <glm.hpp>
#include <SDL.h>
#include <cstdint>
#include <random>

class SpawnPatterns
{
	public:
		// SPAWN PATTERNS

		const static inline std::vector<EnemySpawner> PATTERNS = {

			// BOTH SIDES NORMAL
				EnemySpawner(
					{
						EnemySpawnData("DefaultEnemy", glm::vec2(300, 0), 0, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(300, 0), 1, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(300, 0), 2, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(300, 0), 3, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(300, 0), 4, 75, Enemy::downThenLeave, false),
			
						EnemySpawnData("DefaultEnemy", glm::vec2(724, 0), 0, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(724, 0), 1, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(724, 0), 2, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(724, 0), 3, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(724, 0), 4, 75, Enemy::downThenLeave, true)
					}
				),
			// CENTER NORMAL
				EnemySpawner(
					{
						EnemySpawnData("TripleEnemy", glm::vec2(512, 0), 0, 75, Enemy::downThenLeave, false),
						EnemySpawnData("TripleEnemy", glm::vec2(512, 0), 1, 75, Enemy::downThenLeave, true),
						EnemySpawnData("TripleEnemy", glm::vec2(512, 0), 2, 75, Enemy::downThenLeave, false),
						EnemySpawnData("TripleEnemy", glm::vec2(512, 0), 3, 75, Enemy::downThenLeave, true),
						EnemySpawnData("TripleEnemy", glm::vec2(512, 0), 4, 75, Enemy::downThenLeave, false),
						EnemySpawnData("TripleEnemy", glm::vec2(512, 0), 5, 75, Enemy::downThenLeave, true)
					}
				),
			// FAR SIDES NORMAL
				EnemySpawner(
					{
						EnemySpawnData("DoubleEnemy", glm::vec2(100, 0), 0, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DoubleEnemy", glm::vec2(100, 0), 1, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DoubleEnemy", glm::vec2(100, 0), 2, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DoubleEnemy", glm::vec2(100, 0), 3, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DoubleEnemy", glm::vec2(100, 0), 4, 75, Enemy::downThenLeave, false),
		
						EnemySpawnData("DoubleEnemy", glm::vec2(924, 0), 0, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DoubleEnemy", glm::vec2(924, 0), 1, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DoubleEnemy", glm::vec2(924, 0), 2, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DoubleEnemy", glm::vec2(924, 0), 3, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DoubleEnemy", glm::vec2(924, 0), 4, 75, Enemy::downThenLeave, true)
					}
				),
			// FAR SIDES AND CENTER NORMAL
				EnemySpawner(
					{
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 1, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 3, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 5, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 7, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 9, 75, Enemy::downThenLeave, false),
		
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 1, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 3, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 5, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 7, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 9, 75, Enemy::downThenLeave, true),
		
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 0, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 2, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 4, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 6, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 8, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 10, 75, Enemy::downThenLeave, true)
					}
				),
			// TRIPLE DOUBLE NORMAL
				EnemySpawner(
					{
						EnemySpawnData("DefaultEnemy", glm::vec2(350, 0), 1, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(350, 0), 3, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(350, 0), 5, 75, Enemy::downThenLeave, false),
		
						EnemySpawnData("DefaultEnemy", glm::vec2(674, 0), 1, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(674, 0), 3, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(674, 0), 5, 75, Enemy::downThenLeave, true),
		
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 0, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 2, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 4, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(100, 0), 6, 75, Enemy::downThenLeave, false),
		
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 0, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 2, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 4, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(924, 0), 6, 75, Enemy::downThenLeave, true),
		
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 0, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 2, 75, Enemy::downThenLeave, false),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 4, 75, Enemy::downThenLeave, true),
						EnemySpawnData("DefaultEnemy", glm::vec2(512, 0), 6, 75, Enemy::downThenLeave, false)
					}
				),
			// LASERS TOP TWO-THREE
				EnemySpawner(
					{
						EnemySpawnData("LaserEnemy", glm::vec2(350, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(674, 100), 0, 75, Enemy::stayTopThenLeave, true),
			
						EnemySpawnData("LaserEnemy", glm::vec2(200, 100), 2, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(512, 100), 2, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(824, 100), 2, 75, Enemy::stayTopThenLeave, true),
					}
				),
			// LASERS TOP TWO
				EnemySpawner(
					{
						EnemySpawnData("LaserEnemy", glm::vec2(350, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(674, 100), 0, 75, Enemy::stayTopThenLeave, true)
					}
				),
			// LASERS TOP THREE
				EnemySpawner(
					{
						EnemySpawnData("LaserEnemy", glm::vec2(200, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(512, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(824, 100), 0, 75, Enemy::stayTopThenLeave, true)
					}
				),
			// LASERS TOP FOUR
				EnemySpawner(
					{
						EnemySpawnData("LaserEnemy", glm::vec2(150, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(350, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(674, 100), 0, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(874, 100), 0, 75, Enemy::stayTopThenLeave, true)
					}
				),
			// LASERS TOP FIVE
				EnemySpawner(
					{
						EnemySpawnData("LaserEnemy", glm::vec2(50, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(200, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(512, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(824, 100), 0, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(974, 100), 0, 75, Enemy::stayTopThenLeave, true)
					}
				),
			// LASERS TOP LEFT-RIGHT
				EnemySpawner(
					{
						EnemySpawnData("LaserEnemy", glm::vec2(50, 100), 0, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(100, 100), 0.2f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(150, 100), 0.4f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(200, 100), 0.6f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(250, 100), 0.8f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(300, 100), 1, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(350, 100), 1.2f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(400, 100), 1.4f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(450, 100), 1.6f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(500, 100), 1.8f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(550, 100), 2, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(600, 100), 2.2f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(650, 100), 2.4f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(700, 100), 2.6f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(750, 100), 2.8f, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(800, 100), 3, 75, Enemy::stayTopThenLeave, true),
						EnemySpawnData("LaserEnemy", glm::vec2(850, 100), 3.2f, 75, Enemy::stayTopThenLeave, true),
					}
				),
			// LASERS TOP RIGHT-LEFT
				EnemySpawner(
					{
						EnemySpawnData("LaserEnemy", glm::vec2(974, 100), 0, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(924, 100), 0.2f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(874, 100), 0.4f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(824, 100), 0.6f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(774, 100), 0.8f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(724, 100), 1, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(674, 100), 1.2f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(624, 100), 1.4f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(574, 100), 1.6f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(524, 100), 1.8f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(474, 100), 2, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(424, 100), 2.2f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(374, 100), 2.4f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(324, 100), 2.6f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(274, 100), 2.8f, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(224, 100), 3, 75, Enemy::stayTopThenLeave, false),
						EnemySpawnData("LaserEnemy", glm::vec2(174, 100), 3.2f, 75, Enemy::stayTopThenLeave, false),
					}
				),
			// TINY CENTER THEN SIDES
				EnemySpawner(
					{
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 0, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 0.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 1, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 1.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 2, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 2.5f, 150, Enemy::waveThenLeave, true),
		
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 4, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 4.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 5, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 5.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 6, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 6.5f, 150, Enemy::waveThenLeave, false),
		
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 8, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 8.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 9, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 9.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 10, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 10.5f, 150, Enemy::waveThenLeave, true)
					}
				),
			// TINY LEFT THEN RIGHT
				EnemySpawner(
					{
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 0, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 0.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 1, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 1.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 2, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 2.5f, 150, Enemy::waveThenLeave, false),
		
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 4, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 4.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 5, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 5.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 6, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 6.5f, 150, Enemy::waveThenLeave, true)
					}
				),
			// TINY RIGHT THEN LEFT
				EnemySpawner(
					{
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 0, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 0.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 1, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 1.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 2, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 2.5f, 150, Enemy::waveThenLeave, true),
		
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 4, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 4.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 5, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 5.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 6, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 6.5f, 150, Enemy::waveThenLeave, false)
					}
				),
			// TINY BOTH THEN CENTER
				EnemySpawner(
					{
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 4, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 4.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 5, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 5.5f, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 6, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(300, 0), 6.5f, 150, Enemy::waveThenLeave, false),
		
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 0, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 0.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 1, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 0.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 2, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(724, 0), 2.5f, 150, Enemy::waveThenLeave, true),
		
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 8, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 8.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 9, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 9.5f, 150, Enemy::waveThenLeave, true),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 10, 150, Enemy::waveThenLeave, false),
						EnemySpawnData("SmallEnemy", glm::vec2(512, 0), 10.5f, 150, Enemy::waveThenLeave, true)
					}
				)
		};

		static EnemySpawner RndPattern()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dist(0, std::size(PATTERNS));
			EnemySpawner s = PATTERNS[(int)dist(gen)];
			return s;
		}

};
