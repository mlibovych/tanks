#include "tanks.h"

TankType::TankType() {

}

TankType::~TankType() {
    for (auto &[key, value] : even) {
        destroySprite(value);
    };
    for (auto &[key, value] : odd) {
        destroySprite(value);
    };
}

PlayerBaseTank::PlayerBaseTank() : TankType() {
    even = {
        {FRKey::UP, createSprite("Project2/data/player_base_up_even.png")},
        {FRKey::LEFT, createSprite("Project2/data/player_base_left_even.png")},
        {FRKey::DOWN, createSprite("Project2/data/player_base_down_even.png")},
        {FRKey::RIGHT, createSprite("Project2/data/player_base_right_even.png")}
    };
    odd = {
        {FRKey::UP, createSprite("Project2/data/player_base_up_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/player_base_left_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/player_base_down_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/player_base_right_odd.png")}
    };
    speed = 2;
    max_health = 1;
    bullet_speed = 1;
    power = 1;
}

EnemyBaseTank::EnemyBaseTank() : TankType() {
    even = {
        {FRKey::UP, createSprite("Project2/data/enemy_base_up_even.png")},
        {FRKey::LEFT, createSprite("Project2/data/enemy_base_left_even.png")},
        {FRKey::DOWN, createSprite("Project2/data/enemy_base_down_even.png")},
        {FRKey::RIGHT, createSprite("Project2/data/enemy_base_right_even.png")}
    };
    odd = {
        {FRKey::UP, createSprite("Project2/data/enemy_base_up_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/enemy_base_left_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/enemy_base_down_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/enemy_base_right_odd.png")}
    };
    speed = 1;
    max_health = 1;
    bullet_speed = 1;
    power = 1;
}

RadBaseTank::RadBaseTank() : TankType() {
    even = {
        {FRKey::UP, createSprite("Project2/data/base_red_up.png")},
        {FRKey::LEFT, createSprite("Project2/data/base_red_left.png")},
        {FRKey::DOWN, createSprite("Project2/data/base_red_down.png")},
        {FRKey::RIGHT, createSprite("Project2/data/base_red_right.png")}
    };
    odd = {
        {FRKey::UP, createSprite("Project2/data/enemy_base_up_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/enemy_base_left_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/enemy_base_down_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/enemy_base_right_odd.png")}
    };
    speed = 2;
    max_health = 1;
    bullet_speed = 1;
    power = 2;
}

EnemySpeedTank::EnemySpeedTank() : TankType() {
    even = {
        {FRKey::UP, createSprite("Project2/data/enemy_speed_up_even.png")},
        {FRKey::LEFT, createSprite("Project2/data/enemy_speed_left_even.png")},
        {FRKey::DOWN, createSprite("Project2/data/enemy_speed_down_even.png")},
        {FRKey::RIGHT, createSprite("Project2/data/enemy_speed_right_even.png")}
    };
    odd = {
        {FRKey::UP, createSprite("Project2/data/enemy_speed_up_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/enemy_speed_left_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/enemy_speed_down_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/enemy_speed_right_odd.png")}
    };
    speed = 3;
    max_health = 1;
    bullet_speed = 12;
    power = 1;
}

EnemyArmorTank::EnemyArmorTank() : TankType() {
    even = {
        {FRKey::UP, createSprite("Project2/data/enemy_armor_up_even.png")},
        {FRKey::LEFT, createSprite("Project2/data/enemy_armor_left_even.png")},
        {FRKey::DOWN, createSprite("Project2/data/enemy_armor_down_even.png")},
        {FRKey::RIGHT, createSprite("Project2/data/enemy_armor_right_even.png")}
    };
    odd = {
        {FRKey::UP, createSprite("Project2/data/enemy_armor_up_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/enemy_armor_left_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/enemy_armor_down_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/enemy_armor_right_odd.png")}
    };
    speed = 1;
    max_health = 4;
    bullet_speed = 1;
    power = 2;
}