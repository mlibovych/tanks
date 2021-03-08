#pragma once

#include <fstream>
#include <optional>

#include "Framework.h"
#include "objects.h"

using MapRow = std::array<std::shared_ptr<Object>, 32>;
using Map = std::array<MapRow, 32>;

class MyFramework : public Framework {
	std::unordered_map<std::string, Sprite *> sprites;
	std::unordered_map<std::string, std::shared_ptr<TankType>> tank_types;
	std::unordered_map<std::shared_ptr<Tank>, std::shared_ptr<Bullet>> bullets;
	std::shared_ptr<Tank> player;
	std::unique_ptr<Essence> base;

	std::unordered_map<std::string, std::shared_ptr<Object>> objects;
	std::shared_ptr<BulletData> bullet_data;

	Map map;

	int map_w = 512;
	int map_h = 512;
public:
	MyFramework();

	virtual void PreInit(int& width, int& height, bool& fullscreen);

	virtual bool Init();

	virtual void Close();

	void DrawMap();

	virtual bool Tick();

	std::shared_ptr<Tank> SpawnTank(std::shared_ptr<TankType> type, int x, int y, FRKey key);

	std::pair<int, int> GetExpectedCoords(FRKey k, int expected_x, int expected_y);

	bool CheckBorders(Movable *object, FRKey k, int* expected_x, int* expected_y);

	bool CheckEssences(Movable *object, Essence *other, FRKey k, int expected_x, int expected_y);

	bool CheckCollision(Movable *object, FRKey k, int expected_x, int expected_y);

	bool CheckBulletCollision(Bullet *bullet, FRKey key, int expected_x, int expected_y, int power);

	bool HitWall(FRKey k, int row, int cell, int power, bool double_hit);

	void Rotate(Movable* object, FRKey k);

	void Move(Movable* object, int speed);

	void MoveBullet(Bullet* bullet, int speed, int power);

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative);

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);

	virtual void onKeyPressed(FRKey k);

	virtual void onKeyReleased(FRKey k);

	virtual const char* GetTitle() override;

	Map GenerateMap();

	void LoadSprites();

	void CreateObjects();

	void CreateTanks();

	void MoveBullets();

};