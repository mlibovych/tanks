#include "objects.h"


Object::Object(Sprite *m_sprite) :
        sprite(m_sprite)
{

}

Object::~Object() {

}

BrickWall::BrickWall(Sprite *m_sprite) :
        Object(m_sprite)
{

}
BrickWall::~BrickWall()
{
    
}

SteelWall::SteelWall(Sprite *m_sprite) :
        Object(m_sprite)
{
    health = 2;
}
SteelWall::~SteelWall()
{

}

Movable::Movable(FRKey key) : current_direction(key) 
{

}
Movable::~Movable() {

}

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

void Tank::SetBullet(std::shared_ptr<Bullet> new_bullet) {
    bullet = new_bullet;
}

void Tank::Shoot() {
    if (!bullet->active) {
        bullet->active = true;
        bullet->x = x + w / 2 - bullet->w / 2;
        bullet->y = y + h / 2 - bullet->h / 2;
        if (current_direction == FRKey::RIGHT) {
            bullet->x += w / 2 + + bullet->h / 2 - 2;
        }
        if (current_direction == FRKey::LEFT) {
            bullet->x -= w / 2 + + bullet->h / 2 - 2;
        }
        if (current_direction == FRKey::DOWN) {
            bullet->y += h / 2 + + bullet->h / 2 - 2;
        }
        if (current_direction == FRKey::UP) {
            bullet->y -= h / 2 + bullet->h / 2 - 2;
        }
        bullet->directions[current_direction] = 1;
        bullet->current_direction = current_direction;
        bullet->ChangeSprite(current_direction);
    }
}

BulletData::BulletData() {
    sprites = {
        {FRKey::UP, createSprite("Project2/data/u_bull.png")},
        {FRKey::LEFT, createSprite("Project2/data/l_bull.png")},
        {FRKey::DOWN, createSprite("Project2/data/d_bull.png")},
        {FRKey::RIGHT, createSprite("Project2/data/r_bull.png")}
    };
}

BulletData::~BulletData() {
    for (auto &[key, value] : sprites) {
        destroySprite(value);
    };
}

Essence::Essence(Sprite *m_sprite) : sprite(m_sprite)
{

}

Essence::~Essence() {

}

Bullet::Bullet(std::shared_ptr<BulletData> m_data, FRKey key) : Movable(key),
                                                               data(m_data)
{
    w = 16;
    h = 16;
    role = Role::BULLET;
    ChangeSprite(key);
}

void Bullet::SetData(std::shared_ptr<BulletData> m_data) {
    data = m_data;
}

void Bullet::ChangeSprite(FRKey k) {
    sprite = data->sprites[k];
}
