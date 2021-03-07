#pragma once

#include "Framework.h"
#include "objects.h"


class MyFramework : public Framework {
	Sprite *background;
	std::unique_ptr<Tank> player;
	std::array<std::array<std::shared_ptr<Object>, 32>, 32> map;

	int map_w = 512;
	int map_h = 512;
public:
	MyFramework();

	virtual void PreInit(int& width, int& height, bool& fullscreen);

	virtual bool Init();

	virtual void Close();

	virtual bool Tick();

	bool CheckCollision(Movable *object, FRKey k, int expected_x, int expected_y);

	void Rotate(Movable* object, FRKey k);

	void Move(Movable* object);

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative);

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);

	virtual void onKeyPressed(FRKey k);

	virtual void onKeyReleased(FRKey k);

	virtual const char* GetTitle() override;
};