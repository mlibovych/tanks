#include "objects.h"

void drawSpriteWithBorder(Sprite* sprite, int x, int y) {
    drawSprite(sprite, x + border_size, y + border_size);
}

Object::Object(const char *path, int n_x, int n_y) :
        sprite(createSprite(path)),
        x(n_x),
        y(n_y)
{

}

Object::~Object() {
    destroySprite(sprite);
}

void Object::Draw() {
    drawSpriteWithBorder(sprite, x * cell_size, y * cell_size);
}

BrickWall::BrickWall(int n_x, int n_y) :
        Object("Project2/data/brick.png", n_x, n_y)
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
    sprite = even[current_direction];
    w = 64;
    h = 64;
}
Tank::~Tank()
{
    for (auto &[key, value] : even) {
        destroySprite(value);
    };
    for (auto &[key, value] : odd) {
        destroySprite(value);
    };
}

void Tank::ChangeSprite(FRKey k) {
    sprite = getTickCount() % 2 == 0 ?
            even[k] :
            odd[k];
} 


Player::Player(FRKey key) : Tank(key) 
{	
    speed = 2;
}
Player::~Player() {

}