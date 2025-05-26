#pragma once

struct BulletParams
{
	glm::vec2 pos = { 0,0 };
	glm::vec2 vel = { 0,0 };
	SDL_Color color = { 255, 255, 255 };
};

union BulletState
{
	BulletParams params;
	class Bullet* next = nullptr; //forward declaration of Bullet. Pointer to the next available bullet.
};

class Bullet //simpler class does not inherit from entity
{
public:
	using vec2 = glm::vec2;
	bool Update(Game* game, float deltaTime); //true when destroyed
	void Render(SDL_Renderer* renderer);

	BulletState state;
	bool isActive = false;
	bool hit = false;
	bool hurtsPlayer = false;
	bool removeOnImpact = true;
	inline static SDL_Texture* texture = nullptr;
	class Bullet* next = nullptr;
	inline static const glm::vec2 dim = { 16, 16 };
	SDL_Color color = { 255, 255, 255 };

	static Bullet* None(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* SpreadShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* SingleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* DoubleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* TripleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* QuadrupleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* QuintupleShot(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* ShootAllAround(vec2 pos, float age, Bullet* head, bool hurtsPlayer);
	static Bullet* CreateBullet(vec2 pos, vec2 vel, Bullet* head, bool hurtPlayer);

private:
	float age = 0;
};