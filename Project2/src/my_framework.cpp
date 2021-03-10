#include "my_framework.h"


void drawSpriteWithBorder(Sprite* sprite, int x, int y) {
    drawSprite(sprite, x + BORDER_SIZE, y + BORDER_SIZE);
}

MyFramework::MyFramework() : gen(rd())
{

}

MyFramework::MyFramework(int width, int height) : gen(rd()) {
    if (width < MAP_WIDTH || height < MAP_HEIGHT) {
        std::cerr << "Given size is less than minimum required." << std::endl;
    }
    else {
        m_width = width;
        m_height = height;
    }
}

void MyFramework::PreInit(int& width, int& height, bool& fullscreen)
{
    width = m_width + MENU_SIZE + BORDER_SIZE * 2;
    height = m_height + BORDER_SIZE * 2;
    fullscreen = false;
}

bool MyFramework::Init() {
    LoadSprites();
    CreateObjects();
    CreateTanks();
    CreateAnimations();

    //map generation
    GenerateMap(); 

    //player creation
    player = SpawnTank(tank_types["player_base"], 8, 28, FRKey::UP, Role::PLAYER);
    return true;
}

void MyFramework::CreateTanks() {
    bullet_data = std::make_shared<BulletData> ();
    tank_types["player_base"] = std::make_shared<PlayerBaseTank> ();
    tank_types["enemy_base"] = std::make_shared<EnemyBaseTank> ();
    tank_types["red_base"] = std::make_shared<RadBaseTank> ();
}

void MyFramework::CreateObjects() {
    objects["b1"] = std::make_shared<BrickWall> (sprites["b1"]);
    objects["b2"] = std::make_shared<BrickWall> (sprites["b2"]);
    objects["s1"] = std::make_shared<SteelWall> (sprites["s1"]);
    objects["s2"] = std::make_shared<SteelWall> (sprites["s2"]);
    objects["s3"] = std::make_shared<SteelWall> (sprites["s3"]);
    objects["s4"] = std::make_shared<SteelWall> (sprites["s4"]);
}

void MyFramework::CreateAnimations() {
    animations_templates["explosion"] = std::make_shared<Explosion> ();
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
    sprites["flag"] = createSprite("Project2/data/flag.png");
    sprites["p_icon"] = createSprite("Project2/data/player_icon.png");
    sprites["e_icon"] = createSprite("Project2/data/enemy_icon.png");
    sprites["game_over"] = createSprite("Project2/data/game_over.png");
    sprites["tank"] = createSprite("Project2/data/tank.png");
}

void MyFramework::Close() {
    for (auto& [key, value] : sprites) {
        destroySprite(value);
    }
}

void MyFramework::Draw(Essence* essence) {
    drawSpriteWithBorder(essence->sprite, essence->x, essence->y);
}

void MyFramework::DrawMap() {
    //draw background
    drawSpriteWithBorder(sprites["background"], 0, 0);
    //walls
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            if (map[i][j]) {
                drawSpriteWithBorder(map[i][j]->sprite, j * CELL_SIZE, i * CELL_SIZE);
            }
        }	
    }
    //base
    Draw(base.get());
    //icons
    for (int i = 0; i < avaliable_tanks; i++) {
        int x = MAP_WIDTH + BORDER_SIZE * 2;
        int y = BORDER_SIZE;

        y += (i / 2) * 32;
        x += i % 2 == 0 ? 0 : 32;
        drawSpriteWithBorder(sprites["e_icon"], x, y);
    }
    for (int i = 0; i < health - 1; i++) {
        int x = MAP_WIDTH + BORDER_SIZE * 2;
        int y = BORDER_SIZE + 384;

        y += (i / 2) * 32;
        x += i % 2 == 0 ? 0 : 32;
        drawSpriteWithBorder(sprites["p_icon"], x, y);
    }
}

void MyFramework::Respawn() {
    if (--health > 0) {
        player = SpawnTank(tank_types["player_base"], 8, 28, FRKey::UP, Role::PLAYER);
    }
    else {
    }
}

void MyFramework::UpdateData() {
    for (auto it = tanks.begin(); it != tanks.end(); it++) {
        if (it->get()->health <= 0) {
            if (it->get()->role == Role::PLAYER) {
                Respawn();
            }
            else {
                score += 1;
                if (it->get()->power_up) {
                    CreatePowerUp(*it->get()->power_up);
                }
            }
            if (!it->get()->bullet->active) {
                bullets.erase(*it);
            }
            tanks.erase(it);
        }
    }
}

void MyFramework::FindWay(Tank *tank) {
    std::uniform_int_distribution<> r_dis(0, 3000);
    int random = r_dis(gen);

    if (!Move(tank, tank->type->speed) || random == 1) {
        std::uniform_int_distribution<> dis(0, 3);
        int direction = dis(gen);

        tank->Stop(tank->current_direction);
        tank->Start(static_cast<FRKey>(direction));
    }
}

void MyFramework::Fire(Tank *tank) {
    if (!tank->bullet->active) {
        int chance = 1500;

        if (tank->current_direction == FRKey::LEFT ||
            tank->current_direction == FRKey::RIGHT) {
            if ((tank->y + tank->h /2 >= player->y && tank->y + tank->h /2 <= player->y + player->h) 
                || (tank->y + tank->h /2 >= base->y && tank->y + tank->h /2 <= base->y + base->h)) {
                chance = 200;
            }
        }

        if (tank->current_direction == FRKey::UP ||
            tank->current_direction == FRKey::DOWN) {
            if ((tank->x + tank->w / 2 >= player->x && tank->x + tank->w / 2 <= player->x + player->w) 
                || (tank->x + tank->w / 2 >= base->x && tank->x + tank->w / 2 <= base->x + base->w)) {
                chance = 200;
            }
        }

        std::uniform_int_distribution<> dis(1, chance);
        int fire = dis(gen);

        if (fire == 1) {
            tank->Shoot();
        }
    }
}

void MyFramework::MoveTanks() {
    for(auto& tank : tanks) {
        if (tank->role == Role::ENEMY) {
            FindWay(tank.get());
            Fire(tank.get());
        }
        else {
            Move(tank.get(), tank->type->speed);
            for (auto it = power_ups.begin(); it != power_ups.end(); it++) {
                if (CheckEssences(tank.get(), it->get(), tank.get()->x, tank.get()->y)) {
                    if (it->get()->type == PowerType::TANK) {
                        health++;
                    }
                    power_ups.erase(it);
                }
            }
        }
        Draw(tank.get());
    }
}

void MyFramework::Spawn() {
    //tanks
    if (avaliable_tanks && tanks.size() < MAX_TANKS_ON_BOARD && getTickCount() % 2000 == 0) {
        std::uniform_int_distribution<> dis(0, 1);
        int x = dis(gen) ? 0 : 28;

        auto tank = SpawnTank(tank_types["enemy_base"], x, 0, FRKey::DOWN, Role::ENEMY);
        if (avaliable_tanks == 17 || avaliable_tanks == 10 || avaliable_tanks == 3) {
            tank->power_up = PowerType::TANK;
            tank->SetType(tank_types["red_base"]);
        } 
        avaliable_tanks--;
    }
    for (auto spawn_it = spawning.begin(); spawn_it != spawning.end(); ++spawn_it) {
        bool can_spawn = 1;

        for (auto& tank : tanks) {
            if (CheckEssences(spawn_it->get(), tank.get(), spawn_it->get()->x, spawn_it->get()->y)) {
                can_spawn = 0;
                break;
            }
        }
        if (can_spawn) {
            tanks.push_back(*spawn_it);
            spawning.erase(spawn_it);
        }  
    }
}

void MyFramework::DrawAnimations() {
    for (auto it = animations.begin(); it != animations.end(); it++) {
        Animation* animation = it->get();

        drawSpriteWithBorder(animation->data->sprites[animation->tick], animation->x, animation->y);
        if (getTickCount() % 30 == 0) {
            if (++(animation->tick) >= static_cast<int> (animation->data->sprites.size())) {
                animations.erase(it);
            }
        }
    }
}

void MyFramework::DrawPowerUps() {
    for (auto& poer_up : power_ups) {
        if (getTickCount() % 100 == 0) {
            power_up_tick = power_up_tick ? false : true;
        }
        if (power_up_tick) {
            Draw(poer_up.get());
        }
    }
}

void MyFramework::CheckState() {
    if (base->health <= 0 || health <= 0 || score >= GOAL) {
        delay++;
    }
    if (delay >= 2000) {
        delay = 0;
        state = State::MENU;
    }
}

bool MyFramework::Tick() {
    if (state == State::GAME) {
        UpdateData();
        //draw objects
        Spawn();
        DrawMap();
        MoveBullets();
        MoveTanks();
        DrawAnimations();
        DrawPowerUps();
        //check game result
        CheckState();
    }
    else if (state == State::MENU) {
        //draw result
        Sprite *sprite;

        if (base->health <= 0 || health <= 0) {
            sprite = sprites["flag"];
        }
        else {
            sprite = sprites["eagle"];
        }
        drawSpriteWithBorder(sprite,
                            (m_width + MENU_SIZE) / 2 - 32, 
                            m_height / 2 - 32);
        if (delay++ >= 5000) {
            return true;
        }
    }
    return false;
}

void MyFramework::MoveBullets() {
    for (auto it = bullets.begin(); it != bullets.end();) {  
        if ((*it).second->active) {
            if (MoveBullet(it->second.get(), (*it).first)) {
                bullets.erase(it++);
            }
            else {
                Draw(it->second.get());
                it++;
            }
        }
        else {
            it++;
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
    if (role == Role::ENEMY) {
        tank->Start(key);
    }
    spawning.push_back(tank);
    return tank;
}

bool MyFramework::CheckBorders(Movable *object, FRKey k, int* expected_x, int* expected_y) {
    int sign = k == FRKey::LEFT || k == FRKey::UP ? -1 : 1;

    if (k == FRKey::LEFT || k == FRKey::RIGHT) {
        *expected_x += object->step_size * sign;
    }
    else if (k == FRKey::UP || k == FRKey::DOWN) {
        *expected_y += object->step_size * sign;
    }
    if (*expected_x < 0 ||
        *expected_x > MAP_WIDTH - object->w) {
        return 1;
    }
    if (*expected_y < 0 ||
        *expected_y > MAP_HEIGHT - object->h) {
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

bool MyFramework::CheckEssences(Movable *object, Essence *other,
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
        if (CheckEssences(object, tank.get(), expected_x, expected_y)) {
            return 1;
        }
    }
    
    if (CheckEssences(object, base.get(), expected_x, expected_y)) {
        return 1;
    }
    
    auto [row, cell] = GetExpectedCoords(k, expected_x, expected_y);
    int x_add = 0;
    int y_add = 0;

    if (expected_y % CELL_SIZE != 0 &&
        (k == FRKey::RIGHT || k == FRKey::LEFT)) {
        y_add = 1;
    }
    if (expected_x % CELL_SIZE != 0 &&
        (k == FRKey::UP || k == FRKey::DOWN)) {
        x_add = 1;
    }

    for (int i = 0; i < object->h / CELL_SIZE + y_add; i++) {
        for (int j = 0; j < object->w / CELL_SIZE + x_add; j++) {
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
    if (!other) {
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
    other->health -= tank->type->power;
    return 1;
}

bool MyFramework::DealDamage(Bullet *bullet, Tank* tank, FRKey k, int expected_x, int expected_y) {
    for (auto it = tanks.begin(); it != tanks.end(); it++) {
        if (tank->role != it->get()->role &&
            CheckBulletEssences(bullet, tank, it->get(), k, expected_x, expected_y))
        {
            return 1;
        }
    }
    if (CheckBulletEssences(bullet, tank, base.get(), k, expected_x, expected_y)) {
        base->sprite = sprites["flag"];
        return 1;
    }
    return 0;
}

bool MyFramework::CheckBulletCollision(Bullet *bullet, Tank* tank, FRKey k, int expected_x, int expected_y) {
    if (CheckBorders(bullet, k, &expected_x, &expected_y)) {
        return 1;
    }
    if (DealDamage(bullet, tank, k, expected_x, expected_y)) {
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

bool MyFramework::Move(Movable* object, int speed) {
    int frame = speed > MAX_SPEED ? GAME_SPEED / MAX_SPEED : GAME_SPEED / speed;
    
    for (const auto& [key, value] : object->directions) {
        if (value == 1) {
            Rotate(object, key);
            if (getTickCount() % frame == 0) {
                if (!CheckCollision(object, key, object->x, object->y)) {
                    object->Move(key);
                    return 1;
                }
                return 0;
            }
            break;
        }
    }
    return 1;
}

bool MyFramework::MoveBullet(Bullet* bullet, std::shared_ptr<Tank> tank) {
    int frame = tank->type->bullet_speed * BULLETS_SPEED > MAX_SPEED ? GAME_SPEED / MAX_SPEED : GAME_SPEED / (tank->type->bullet_speed *  BULLETS_SPEED);

    if (getTickCount() % frame == 0) {
        if (!CheckBulletCollision(bullet, tank.get(), bullet->current_direction, bullet->x, bullet->y)) {
            bullet->Move(bullet->current_direction);
        }
        else  {
            animations.push_back(std::make_unique<Animation> (animations_templates["explosion"], 
                                                              bullet->x + bullet->w / 2 - 32,
                                                              bullet->y + bullet->h / 2 - 32));
            bullet->active = false;
            bullet->directions[bullet->current_direction] = 0;
            if (std::find(tanks.begin(), tanks.end(), tank) == tanks.end()) {
                return 1;
            }
        }
    }
    return 0;
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

void MyFramework::CreateBrick(int row, int cell) {
    if ((row % 2 == 0 && cell % 2 != 0) || (row % 2 != 0 && cell % 2 == 0)) {
        map[row][cell] = objects["b1"];
    }
    else {
        map[row][cell] = objects["b2"];
    }
}

void MyFramework::CreateSteel(int row, int cell) {
    if (row % 2 == 0 && cell % 2 == 0) {
        map[row][cell] = objects["s1"];
    }
    else if (row % 2 == 0 && cell % 2 != 0) {
        map[row][cell] = objects["s2"];
    }
    else if (row % 2 != 0 && cell % 2 == 0) {
        map[row][cell] = objects["s3"];
    }
    else {
        map[row][cell] = objects["s4"];
    }
}

void MyFramework::CreateBase(int row, int cell) {
    base = std::make_shared<Essence> (sprites["eagle"]);
    base->x = cell * CELL_SIZE;
    base->y = row * CELL_SIZE;
    base->role = Role::BASE;
}

void MyFramework::GenerateMap() {
    std::fstream file("Project2/data/MAP.txt");
    std::string line;

    for (int i = 0; i < 32; ++i) {
        std::getline(file, line);
        for (int j = 0; j < 32; ++j) {
            switch (line[j]) {
            case 'b':
                CreateBrick(i, j);
                break;
            case 's':
                CreateSteel(i, j);
                break;
            case 'f':
                CreateBase(i, j);
                break;
            }
        }
    }
}

void MyFramework::CreatePowerUp(PowerType type) {
    std::uniform_int_distribution<> coords_dis(0, MAP_WIDTH - 64);
    int x = coords_dis(gen);
    int y = coords_dis(gen);

    if (type == PowerType::TANK) {
        CreateTankPowerUp(x, y);
    }
}

void MyFramework::CreateTankPowerUp(int x_coord, int y_coord) {
    auto power_up = std::make_shared<PowerUp> ();

    power_up->sprite = sprites["tank"];
    power_up->type = PowerType::TANK;
    power_up->x = x_coord;
    power_up->y = y_coord;
    power_ups.push_back(power_up);
}
