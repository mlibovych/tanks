#pragma once

#include "Framework.h"
#include "objects.h"

using MapRow = std::array<std::shared_ptr<Object>, 32>;
using Map = std::array<MapRow, 32>;

class MyFramework : public Framework {
	std::unordered_map<std::string, Sprite *> sprites;
	std::unordered_map<std::string, std::shared_ptr<TankType>> tank_types;
	std::unique_ptr<Tank> player;
	Map map;

	std::shared_ptr<BrickWall> b_wall;
	std::shared_ptr<TankType> base_tank;

	int map_w = 512;
	int map_h = 512;
public:
	MyFramework();

	virtual void PreInit(int& width, int& height, bool& fullscreen);

	virtual bool Init();

	virtual void Close();

	virtual bool Tick();

	void SpawnTank(std::shared_ptr<TankType> type, int x, int y, FRKey key);

	void CreatePlayer();

	bool CheckCollision(Movable *object, FRKey k, int expected_x, int expected_y);

	void Rotate(Movable* object, FRKey k);

	void Move(Movable* object, int speed);

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative);

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);

	virtual void onKeyPressed(FRKey k);

	virtual void onKeyReleased(FRKey k);

	virtual const char* GetTitle() override;

	Map GenerateMap();

	void LoadSprites();
};