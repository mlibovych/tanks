#pragma once

#include <array>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "Framework.h"

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

class PlayerBaseTank : public TankType {
public:
	PlayerBaseTank();
};

class EnemyBaseTank : public TankType {
public:
	EnemyBaseTank();
};

class EnemySpeedTank : public TankType {
public:
	EnemySpeedTank();
};

class EnemyArmorTank : public TankType {
public:
	EnemyArmorTank();
};

class RadBaseTank : public TankType {
public:
	RadBaseTank();
};