#pragma once

#include <stdlib.h>
#include <math.h>

#include <array>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "Framework.h"

#define MAX_SPEED 6
#define GAME_SPEED 12
#define FRAMES 3
#define BULLETS_SPEED 6

#define BORDER_SIZE  10
#define CELL_SIZE 16

void drawSpriteWithBorder(Sprite* sprite, int x, int y);

class Object {	
public:
	Object(Sprite *m_sprite);
	virtual ~Object();

	virtual void Draw(int x, int y);

	Sprite *sprite;

	int health = 1;
};

class BrickWall : public Object {
public:
	BrickWall(Sprite *m_sprite);
	virtual ~BrickWall();	
};

class SteelWall : public Object {
public:
	SteelWall(Sprite *m_sprite);
	virtual ~SteelWall();
};

enum class Role {
	PLAYER,
	ENEMY,
	BASE,
	BULLET,
	COUNT
};

class Essence {
public:
	Essence() = default;

	Essence(Sprite *m_sprite) : sprite(m_sprite)
	{

	}
	virtual ~Essence() {
		
	}

	virtual void Draw();

	Sprite *sprite;

	int x = 0;
	int y = 0;
	int w = 64;
	int h = 64;

	int health = 1;

	Role role;
};


class Movable : public Essence {
public:
	Movable(FRKey key);
	virtual ~Movable();

	void Start(FRKey k);

	void Stop(FRKey k);

	void Move(FRKey k);

	void Rotate(FRKey k);

	virtual void ChangeSprite(FRKey k) = 0;

	FRKey getDirection();

	FRKey current_direction;
	std::unordered_map<FRKey, int> directions = {
        {FRKey::RIGHT, 0},
        {FRKey::LEFT, 0},
        {FRKey::UP, 0},
        {FRKey::DOWN, 0}
    };
	
	int step_size = 1;
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

class BulletData {
public:
	BulletData();
	~BulletData();

	std::unordered_map<FRKey, Sprite *> sprites;
};

class PlayerBaseTank : public TankType {
public:
	PlayerBaseTank();
};

class Bullet : public Movable {
public:
	Bullet(std::shared_ptr<BulletData> m_data, FRKey key);

	void SetData(std::shared_ptr<BulletData> m_data);

	std::shared_ptr<BulletData> data;
	virtual void ChangeSprite(FRKey k) override;

	bool active = false;
};

class Tank : public Movable {
public:
	Tank(FRKey key);
	~Tank();
	
	void SetType(std::shared_ptr<TankType> new_type);
	void SetBullet(std::shared_ptr<Bullet> new_bullet);

	virtual void ChangeSprite(FRKey k) override;

	void Shoot();

	std::shared_ptr<TankType> type;
	std::shared_ptr<Bullet> bullet;
};