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
    base = std::make_unique<Essence> (sprites["eagle"]);
    base->x = 14 * CELL_SIZE;
    base->y = 28 * CELL_SIZE; 

    //player creation
    player = SpawnTank(tank_types["base"], 8, 28, FRKey::UP);

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
    base->Draw();
}

bool MyFramework::Tick() {
    //draw background
    drawSpriteWithBorder(sprites["background"], 0, 0);
    //draw map
    DrawMap();
    //move objects
    MoveBullets();
    Move(player.get(), player->type->speed * 2);
    //draw tanks
    player->Draw();
    //draw 
    return false;
}

void MyFramework::MoveBullets() {
    for (auto& [tank, bullet] : bullets) {
        if (bullet->active) {
            MoveBullet(bullet.get(), tank->type->bullet_speed * BULLETS_SPEED, tank->type->power);
            bullet->Draw();
        }
    }
}

std::shared_ptr<Tank> MyFramework::SpawnTank(std::shared_ptr<TankType> type, int x, int y, FRKey key) {
    std::shared_ptr<Tank> tank = std::make_shared<Tank> (key);
    bullets[tank] = std::make_shared<Bullet> (bullet_data, key);

    tank->x = x * CELL_SIZE;
    tank->y = y * CELL_SIZE;
    tank->SetType(type);
    tank->SetBullet(bullets[tank]);
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

bool MyFramework::CheckCollision(Movable *object, FRKey k, int expected_x, int expected_y) {
    if (CheckBorders(object, k, &expected_x, &expected_y)) {
        return 1;
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

bool MyFramework::HitWall(int row, int cell, int power) {
    if (row < 0 || row >= 32 || cell < 0 || cell >= 32) {
        return 1;
    }
    if (map[row ][cell]) {
        if (map[row ][cell]->health <= power) {
            map[row ][cell] = nullptr;
        }
        return 1;
    }
    return 0;
}

// int x_add = 1;
// int y_add = 1;
// if (expected_x % CELL_SIZE != 0) {
//     x_add += 1;
// }
// if (expected_y % CELL_SIZE != 0) {
//     y_add += 1;
// }

bool MyFramework::CheckBulletCollision(Bullet *bullet, FRKey k, int expected_x, int expected_y, int power) {
    if (CheckBorders(bullet, k, &expected_x, &expected_y)) {
        return 1;
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
            res = HitWall(row, cell + i, power) ? 1 : res;
        }
        if (k == FRKey::LEFT || k == FRKey::RIGHT) {
            res = HitWall(row + i, cell, power) ? 1 : res;
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
            else {
                expected_y -= object->y % (CELL_SIZE * 2);
            }
        }
        else {
            if (object->current_direction == FRKey::RIGHT) {
                expected_x += (CELL_SIZE * 2) - object->x % (CELL_SIZE * 2);
            }
            else {
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

void MyFramework::MoveBullet(Bullet* bullet, int speed, int power) {
    int frame = speed > MAX_SPEED ? GAME_SPEED / MAX_SPEED : GAME_SPEED / speed;
    
    for (const auto& [key, value] : bullet->directions) {
        if (value == 1) {
            if (getTickCount() % frame == 0) {
                if (!CheckBulletCollision(bullet, key, bullet->x, bullet->y, power)) {
                    bullet->Move(key);
                }
                else  {
                    bullet->active = false;
                    bullet->directions[bullet->current_direction] = 0;
                }
            }
            break;
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
    for (int i = 4; i < 8; ++i) {
        for (int j = 4; j < 8; ++j) {
            if ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0)) {
                map[i][j] = objects["b1"];
            }
            else {
                map[i][j] = objects["b2"];
            }
        }	
    }

    for (int i = 4; i < 8; ++i) {
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

    return map;
}