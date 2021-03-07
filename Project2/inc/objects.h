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
	Object(const char *path, int n_x, int n_y);
	virtual ~Object();

	virtual void Draw();

	Sprite *sprite;
	int x;
	int y;
};

class BrickWall : public Object {
public:
	BrickWall(int n_x, int n_y);
	virtual ~BrickWall();	
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
	int speed = 1;
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

class Tank : public Movable{
	std::unordered_map<FRKey, Sprite *> even = {
        {FRKey::UP, createSprite("Project2/data/u_even.png")},
        {FRKey::LEFT, createSprite("Project2/data/l_even.png")},
        {FRKey::DOWN, createSprite("Project2/data/d_even.png")},
        {FRKey::RIGHT, createSprite("Project2/data/r_even.png")}
    };
    std::unordered_map<FRKey, Sprite *> odd = {
        {FRKey::UP, createSprite("Project2/data/u_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/l_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/d_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/r_odd.png")}
    };
public:
	Tank(FRKey key);
	~Tank();

	virtual void ChangeSprite(FRKey k) override; 
};

class Player : public Tank {
public:
	Player(FRKey key);
	~Player();
};