#include "objects.h"

void drawSpriteWithBorder(Sprite* sprite, int x, int y) {
    drawSprite(sprite, x + border_size, y + border_size);
}

Object::Object(Sprite *m_sprite) :
        sprite(m_sprite)
{

}

Object::~Object() {

}

void Object::Draw(int x, int y) {
    drawSpriteWithBorder(sprite, x * cell_size, y * cell_size);
}

BrickWall::BrickWall(Sprite *m_sprite) :
        Object(m_sprite)
{

}
BrickWall::~BrickWall()
{
    destroySprite(sprite);
}	

Movable::Movable(FRKey key) : current_direction(key) 
{

}
Movable::~Movable() {

}

void Movable::Draw() {
    drawSpriteWithBorder(sprite, x, y);
};

void Movable::Start(FRKey k) {
    for (auto& [key, value] : directions) {
        if (value) {
            value++;
        }
    }
    directions[k] = 1;
}

void Movable::Stop(FRKey k) {
    for (auto& [key, value] : directions) {
        if (value && value > directions[k]) {
            value--;
        }
    }
    directions[k] = 0;
}

void Movable::Move(FRKey k) {
    int sign = 1;

    if (k == FRKey::LEFT || k == FRKey::UP) {
        sign *= -1;
    }
    if (k == FRKey::RIGHT || k == FRKey::LEFT) {
        x += step_size * sign;
    }
    else {
        y += step_size * sign;
    }
}

void Movable::Rotate(FRKey k) {
    ChangeSprite(k);
}

FRKey Movable::getDirection() {
    FRKey direction = FRKey::COUNT;

    for (const auto& [key, value] : directions) {
        if (value == 1) {
            direction = key;
            break;
        }
    }
    return direction;
}


Tank::Tank(FRKey key) : Movable(key) 
{	
    w = 64;
    h = 64;
}
Tank::~Tank()
{
    
}

void Tank::ChangeSprite(FRKey k) {
    sprite = getTickCount() % 2 == 0 ?
            type->even[k] :
            type->odd[k];
} 

void Tank::SetType(std::shared_ptr<TankType> new_type) {
    type = new_type;
    sprite = type->even[current_direction];
    health = type->max_health;
}


Player::Player(FRKey key) : Tank(key) 
{	

}
Player::~Player() {

}

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


BaseTank::BaseTank() : TankType() {
    even = {
        {FRKey::UP, createSprite("Project2/data/u_even.png")},
        {FRKey::LEFT, createSprite("Project2/data/l_even.png")},
        {FRKey::DOWN, createSprite("Project2/data/d_even.png")},
        {FRKey::RIGHT, createSprite("Project2/data/r_even.png")}
    };
    odd = {
        {FRKey::UP, createSprite("Project2/data/u_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/l_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/d_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/r_odd.png")}
    };
    speed = 1;
    max_health = 1;
    bullet_speed = 1;
    power = 1;
}