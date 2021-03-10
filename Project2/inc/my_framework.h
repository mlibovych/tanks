#pragma once

#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <stdexcept>

#include "Framework.h"
#include "objects.h"

#define GOAL 10
#define MAX_TANKS_ON_BOARD 5

#define MAX_SPEED 6
#define GAME_SPEED 12
#define FRAMES 3
#define BULLETS_SPEED 6

#define MAP_WIDTH 512
#define MAP_HEIGHT 512
#define BORDER_SIZE  10
#define MENU_SIZE 200
#define CELL_SIZE 16

using MapRow = std::array<std::shared_ptr<Object>, 32>;
using Map = std::array<MapRow, 32>;

void drawSpriteWithBorder(Sprite* sprite, int x, int y);

class MyFramework : public Framework {
	std::unordered_map<std::string, Sprite *> sprites;
	std::unordered_map<std::string, std::shared_ptr<TankType>> tank_types;
	std::unordered_map<std::shared_ptr<Tank>, std::shared_ptr<Bullet>> bullets;
	std::shared_ptr<Tank> player;
	std::shared_ptr<Essence> base;

	std::unordered_map<std::string, std::shared_ptr<Object>> objects;
	std::shared_ptr<BulletData> bullet_data;
	std::list<std::shared_ptr<Tank>> tanks;
	std::list<std::shared_ptr<Tank>> spawning;

	Map map;

	int m_width = 512;
	int m_height = 512;

	int avaliable_tanks = 10;
	int health = 3;
	int score = 0;

	std::random_device rd;
    std::mt19937 gen;
public:
	MyFramework();

	MyFramework(int width, int heigth);

	virtual void PreInit(int& width, int& height, bool& fullscreen);

	virtual bool Init();

	virtual void Close();

	void DrawMap();

	void Spawn();

	virtual bool Tick();

	std::shared_ptr<Tank> SpawnTank(std::shared_ptr<TankType> type, int x, int y, FRKey key, Role role);

	std::pair<int, int> GetExpectedCoords(FRKey k, int expected_x, int expected_y);

	bool CheckBorders(Movable *object, FRKey k, int* expected_x, int* expected_y);

	bool CheckEssences(Movable *object, Essence *other,
						int expected_x, int expected_y);
	
	bool CheckBulletEssences(Bullet *bullet, Tank* tank, Essence *other, FRKey k,
                                int expected_x, int expected_y);

	bool CheckCollision(Movable *object, FRKey k, int expected_x, int expected_y);

	bool CheckBulletCollision(Bullet *bullet, Tank* tank, FRKey key, int expected_x, int expected_y);

	bool DealDamage(Bullet *bullet, Tank* tank, FRKey k, int expected_x, int expected_y);

	bool HitWall(FRKey k, int row, int cell, int power, bool double_hit);

	void Rotate(Movable* object, FRKey k);

	bool Move(Movable* object, int speed);

	bool MoveBullet(Bullet* bullet, std::shared_ptr<Tank> tank);

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative);

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);

	virtual void onKeyPressed(FRKey k);

	virtual void onKeyReleased(FRKey k);

	virtual const char* GetTitle() override;

	void GenerateMap();

	void CreateBrick(int row, int cell);

	void CreateSteel(int row, int cell);

	void CreateBase(int row, int cell);

	void LoadSprites();

	void CreateObjects();

	void CreateTanks();

	void MoveBullets();

	void FindWay(Tank *tank);

	void Fire(Tank *tank);

	void MoveTanks();

	void Respawn();

	void UpdateData();

	void Draw(Essence* essence);
};