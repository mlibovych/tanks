#include "my_framework.h"


MyFramework::MyFramework() {

}

void MyFramework::PreInit(int& width, int& height, bool& fullscreen)
{
    width = map_w + BORDER_SIZE * 2;
    height = map_h + BORDER_SIZE * 2;
    fullscreen = false;
}

bool MyFramework::Init() {
    LoadSprites();
    CreateObjects();
    //tanks generation
    CreateTanks();

    //map generation
    map = GenerateMap(); 

    //player creation
    player = SpawnTank(tank_types["base"], 8, 28, FRKey::UP, Role::PLAYER);

    SpawnTank(tank_types["base"], 0, 0, FRKey::DOWN, Role::ENEMY);

    //check result
    return true;
}

void MyFramework::CreateTanks() {
    bullet_data = std::make_shared<BulletData> ();
    tank_types["base"] = std::make_shared<BaseTank> ();
}

void MyFramework::CreateObjects() {
    objects["b1"] = std::make_shared<BrickWall> (sprites["b1"]);
    objects["b2"] = std::make_shared<BrickWall> (sprites["b2"]);
    objects["s1"] = std::make_shared<SteelWall> (sprites["s1"]);
    objects["s2"] = std::make_shared<SteelWall> (sprites["s2"]);
    objects["s3"] = std::make_shared<SteelWall> (sprites["s3"]);
    objects["s4"] = std::make_shared<SteelWall> (sprites["s4"]);
}

void MyFramework::LoadSprites() {
    sprites["background"] = createSprite("Project2/data/back.jpg");
    sprites["b1"] = createSprite("Project2/data/b1.png");
    sprites["b2"] = createSprite("Project2/data/b2.png");
    sprites["s1"] = createSprite("Project2/data/s1.png");
    sprites["s2"] = createSprite("Project2/data/s2.png");
    sprites["s3"] = createSprite("Project2/data/s3.png");
    sprites["s4"] = createSprite("Project2/data/s4.png");
    sprites["eagle"] = createSprite("Project2/data/eagle.png");
}

void MyFramework::Close() {
    for (auto& [key, value] : sprites) {
        destroySprite(value);
    }
}

void MyFramework::DrawMap() {
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            if (map[i][j]) {
                map[i][j]->Draw(j, i);
            }
        }	
    }
    if (base) {
        base->Draw();
    }
}

bool MyFramework::Tick() {
    //draw background
    drawSpriteWithBorder(sprites["background"], 0, 0);
    //draw map
    DrawMap();
    //draw objects
    MoveBullets();
    
    //draw tanks
    for(auto& tank : tanks) {
        Move(tank.get(), tank->type->speed * 2);
        tank->Draw();
    }
    //check state
    if (!base || score >= GOAL) {
        return true;
    }
    return false;
}

void MyFramework::MoveBullets() {
    for (auto& [tank, bullet] : bullets) {
        if (bullet->active) {
            MoveBullet(bullet.get(), tank.get());
            bullet->Draw();
        }
    }
}

std::shared_ptr<Tank> MyFramework::SpawnTank(std::shared_ptr<TankType> type, int x, int y, FRKey key, Role role) {
    std::shared_ptr<Tank> tank = std::make_shared<Tank> (key);
    bullets[tank] = std::make_shared<Bullet> (bullet_data, key);

    tank->x = x * CELL_SIZE;
    tank->y = y * CELL_SIZE;
    tank->SetType(type);
    tank->SetBullet(bullets[tank]);
    tank->role = role;
    tanks.push_back(tank);
    return tank;
}

bool MyFramework::CheckBorders(Movable *object, FRKey k, int* expected_x, int* expected_y) {
    int sign = k == FRKey::LEFT || k == FRKey::UP ? -1 : 1;

    if (k == FRKey::RIGHT || k == FRKey::LEFT) {
        *expected_x += object->step_size * sign;
    }
    else {
        *expected_y += object->step_size * sign;
    }
    if (*expected_x < 0 ||
        *expected_x > map_w - object->w) {
        return 1;
    }
    if (*expected_y < 0 ||
        *expected_y > map_h - object->h) {
        return 1;
    }
    return 0;
}

std::pair<int, int> MyFramework::GetExpectedCoords(FRKey k, int expected_x, int expected_y) {
    int row = expected_y / CELL_SIZE;
    int cell = expected_x / CELL_SIZE;

    if (k == FRKey::RIGHT) {
        cell += expected_x / CELL_SIZE == 0 && expected_x > CELL_SIZE ? 0 : 1;
    }
    if (k == FRKey::DOWN) {
        row += expected_y / CELL_SIZE == 0 && expected_y > CELL_SIZE ? 0 : 1;
    }

    return {row, cell};
}

bool MyFramework::CheckEssences(Movable *object, Essence *other, FRKey k,
                                int expected_x, int expected_y) {
    if (!other || other == object) {
        return 0;
    }
    int a_x0 = expected_x;
    int a_y0 = expected_y;
    int a_x1 = expected_x + object->w;
    int a_y1 = expected_y + object->h;

    int b_x0 = other->x;
    int b_y0 = other->y;
    int b_x1 = other->x + other->w;
    int b_y1 = other->y + other->h;

    if (a_x0 >= b_x1 || a_x1 <= b_x0 || a_y0 >= b_y1 || a_y1 <= b_y0) {
        return 0;
    }
    return 1;
}

bool MyFramework::CheckCollision(Movable *object, FRKey k, int expected_x, int expected_y) {
    if (CheckBorders(object, k, &expected_x, &expected_y)) {
        return 1;
    }
    for (auto& tank : tanks) {
        if (CheckEssences(object, tank.get(), k, expected_x, expected_y)) {
            return 1;
        }
    }

    auto [row, cell] = GetExpectedCoords(k, expected_x, expected_y);

    for (int i = 0; i < object->h / CELL_SIZE; i++) {
        for (int j = 0; j < object->w / CELL_SIZE; j++) {
            if (map[row + i][cell + j]) {
                return 1;
            }
        }
    }
    return 0;
}

bool MyFramework::HitWall(FRKey k, int row, int cell, int power, bool double_hit) {
    if (row < 0 || row >= 32 || cell < 0 || cell >= 32) {
        return 1;
    }
    if (map[row ][cell]) {
        if (map[row ][cell]->health <= power) {
            map[row ][cell] = nullptr;
            if (power == 2 && double_hit) {
                if (k == FRKey::DOWN || k == FRKey::UP) {
                    row += k == FRKey::DOWN ? 1 : -1;
                }
                if (k == FRKey::RIGHT || k == FRKey::LEFT) {
                    cell += k == FRKey::RIGHT ? 1 : -1;
                }
                HitWall(k, row, cell, power, 0);
            }
        }
        return 1;
    }
    return 0;
}

bool MyFramework::CheckBulletEssences(Bullet *bullet, Tank* tank, Essence *other, FRKey k,
                                int expected_x, int expected_y) {
    if (!other || tank->role == other->role) {
        return 0;
    }
    int a_x0 = expected_x;
    int a_y0 = expected_y;
    int a_x1 = expected_x + bullet->w;
    int a_y1 = expected_y + bullet->h;

    int b_x0 = other->x;
    int b_y0 = other->y;
    int b_x1 = other->x + other->w;
    int b_y1 = other->y + other->h;

    if (a_x0 >= b_x1 || a_x1 <= b_x0 || a_y0 >= b_y1 || a_y1 <= b_y0) {
        return 0;
    }
    return 1;
}

bool MyFramework::CheckBulletCollision(Bullet *bullet, Tank* tank, FRKey k, int expected_x, int expected_y) {
    if (CheckBorders(bullet, k, &expected_x, &expected_y)) {
        return 1;
    }
    for (auto it = tanks.begin(); it != tanks.end(); it++) {
        if (CheckBulletEssences(bullet, tank, it->get(), k, expected_x, expected_y)) {
            (*it)->health -= tank->type->power;
            if ((*it)->health <= 0) {
                if (*it == player) {
                    //Respawn;
                }
                else {
                    score += 1;
                }
                tanks.erase(it);
            }
            return 1;
        }
    }
    
    bool res = 0;
    auto [row, cell] = GetExpectedCoords(k, expected_x, expected_y);

    if (k == FRKey::UP || k == FRKey::DOWN) {
        cell -= 1;
    }
    if (k == FRKey::LEFT || k == FRKey::RIGHT) {
        row -= 1;
    }

    for (int i = 0; i < 4; i++) {
        if (k == FRKey::UP || k == FRKey::DOWN) {
            res = HitWall(k, row, cell + i, tank->type->power, 1) ? 1 : res;
        }
        if (k == FRKey::LEFT || k == FRKey::RIGHT) {
            res = HitWall(k, row + i, cell, tank->type->power, 1) ? 1 : res;
        }
    }
    return res;
}

void MyFramework::Rotate(Movable* object, FRKey k) {
    int expected_x = object->x;
    int expected_y = object->y;

    object->Rotate(k);
    if (object->current_direction != k) {
        if (k == FRKey::RIGHT || k == FRKey::LEFT) {
            if (object->current_direction == FRKey::DOWN) {
                expected_y += (CELL_SIZE * 2)  - object->y % (CELL_SIZE * 2);
            }
            else if (object->current_direction == FRKey::UP) {
                
                expected_y -= object->y % (CELL_SIZE * 2);
            }
        }
        else {
            if (object->current_direction == FRKey::RIGHT) {
                expected_x += (CELL_SIZE * 2) - object->x % (CELL_SIZE * 2);
            }
            else if (object->current_direction == FRKey::LEFT) {
                expected_x -= object->x % (CELL_SIZE * 2);
            }
        }
        if (!CheckCollision(object, k, expected_x, expected_y)) {
            object->x = expected_x;
            object->y = expected_y;
        }
        object->current_direction = k;
    }
}

void MyFramework::Move(Movable* object, int speed) {
    int frame = speed > MAX_SPEED ? GAME_SPEED / MAX_SPEED : GAME_SPEED / speed;
    
    for (const auto& [key, value] : object->directions) {
        if (value == 1) {
            Rotate(object, key);
            if (getTickCount() % frame == 0) {
                if (!CheckCollision(object, key, object->x, object->y)) {
                    object->Move(key);
                }
            }
            break;
        }
    }
}

void MyFramework::MoveBullet(Bullet* bullet, Tank* tank) {
    int frame = tank->type->bullet_speed * BULLETS_SPEED > MAX_SPEED ? GAME_SPEED / MAX_SPEED : GAME_SPEED / (tank->type->bullet_speed *  BULLETS_SPEED);
    
    if (getTickCount() % frame == 0) {
        if (!CheckBulletCollision(bullet, tank, bullet->current_direction, bullet->x, bullet->y)) {
            bullet->Move(bullet->current_direction);
        }
        else  {
            bullet->active = false;
            bullet->directions[bullet->current_direction] = 0;
        }
    }
}

void MyFramework::onMouseMove(int x, int y, int xrelative, int yrelative) {

}

void MyFramework::onMouseButtonClick(FRMouseButton button, bool isReleased) {
    if (button == FRMouseButton::LEFT && !isReleased) {
        player->Shoot();
    }
}

void MyFramework::onKeyPressed(FRKey k) {
    player->Start(k);
}

void MyFramework::onKeyReleased(FRKey k) {
    player->Stop(k);
}

const char* MyFramework::GetTitle()
{
    return "Tanks";
}

Map MyFramework::GenerateMap() {
    Map map;
    std::fstream file("Project2/data/MAP.txt");
    std::string line;

    while(std::getline(file, line)) {

    }
    for (int i = 28; i < 32; ++i) {
        for (int j = 4; j < 8; ++j) {
            if ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0)) {
                map[i][j] = objects["b1"];
            }
            else {
                map[i][j] = objects["b2"];
            }
        }	
    }

    for (int i = 12; i < 16; ++i) {
        for (int j = 8; j < 12; ++j) {
            if (i % 2 == 0 && j % 2 == 0) {
                map[i][j] = objects["s1"];
            }
            else if (i % 2 == 0 && j % 2 != 0) {
                map[i][j] = objects["s2"];
            }
            else if (i % 2 != 0 && j % 2 == 0) {
                map[i][j] = objects["s3"];
            }
            else {
                map[i][j] = objects["s4"];
            }
        }	
    }

    base = std::make_shared<Essence> (sprites["eagle"]);
    base->x = 14 * CELL_SIZE;
    base->y = 28 * CELL_SIZE;
    base->role = Role::BASE;

    return map;
}