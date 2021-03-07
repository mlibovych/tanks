#pragma once

#include <stdlib.h>
#include <math.h>

#include <array>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "Framework.h"

#define MAX_SPEED 4
#define GAME_SPEED 12
#define FRAMES 3

#define border_size  10
#define cell_size 16

void drawSpriteWithBorder(Sprite* sprite, int x, int y);

class Object {
protected:
	int health = 1;
public:
	Object(Sprite *m_sprite);
	virtual ~Object();

	virtual void Draw(int x, int y);

	Sprite *sprite;
};

class BrickWall : public Object {
public:
	BrickWall(Sprite *m_sprite);
	virtual ~BrickWall();

	int power = 1;	
};

class Movable {
public:
	FRKey current_direction;
	std::unordered_map<FRKey, int> directions = {
        {FRKey::RIGHT, 0},
        {FRKey::LEFT, 0},
        {FRKey::UP, 0},
        {FRKey::DOWN, 0}
    };
	Sprite *sprite;

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int step_size = 1;
public:
	Movable(FRKey key);
	virtual ~Movable();

	virtual void Draw();

	void Start(FRKey k);

	void Stop(FRKey k);

	void Move(FRKey k);

	void Rotate(FRKey k);

	virtual void ChangeSprite(FRKey k) = 0;

	FRKey getDirection();
};

class TankType {
public:
	TankType();
	~TankType();

	std::unordered_map<FRKey, Sprite *> even;
    std::unordered_map<FRKey, Sprite *> odd;

	int max_health;
	int bullet_speed;
	int power;
	int speed;
};

class BaseTank : public TankType {
public:
	BaseTank();
};

class Tank : public Movable{
public:
	Tank(FRKey key);
	~Tank();
	
	void SetType(std::shared_ptr<TankType> new_type);

	virtual void ChangeSprite(FRKey k) override;
	std::shared_ptr<TankType> type;
	int health;
};

class Player : public Tank {
public:
	Player(FRKey key);
	~Player();
};