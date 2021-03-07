#include "my_framework.h"


MyFramework::MyFramework() {

}

void MyFramework::PreInit(int& width, int& height, bool& fullscreen)
{
    width = map_w + border_size * 2;
    height = map_h + border_size * 2;
    fullscreen = false;
}

bool MyFramework::Init() {
    background = createSprite("Project2/data/back.jpg");
    for (int i = 4; i < 8; ++i) {
        for (int j = 10; j < 14; ++j) {
            map[i][j] = std::make_shared<BrickWall> (j, i);
        }	
    }
    player = std::make_unique<Player> (FRKey::UP);
    return true;
}

void MyFramework::Close() {
    destroySprite(background);
}

bool MyFramework::Tick() {
    Move(player.get());
    drawSpriteWithBorder(background, 0, 0);
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            if (map[i][j]) {
                map[i][j]->Draw();
            }
        }	
    }
    player->Draw();
    return false;
}

bool MyFramework::CheckCollision(Movable *object, FRKey k, int expected_x, int expected_y) {
    int sign = k == FRKey::LEFT || k == FRKey::UP ? -1 : 1;
    int row = 0;
    int cell = 0;

    if (k == FRKey::RIGHT || k == FRKey::LEFT) {
        expected_x += object->step_size * sign;
    }
    else {
        expected_y += object->step_size * sign;
    }
    if (expected_x < 0 ||
        expected_x > map_w - object->w) {
        return 1;
    }
    if (expected_y < 0 ||
        expected_y > map_h - object->h) {
        return 1;
    }

    row += expected_y / cell_size;
    cell += expected_x / cell_size;
    if (k == FRKey::RIGHT || k == FRKey::LEFT) {
        if (k == FRKey::RIGHT) {
            cell += object->w / cell_size;
        }
        for (int i = 0; i < object->h / cell_size; i++) {
            if (map[row + i][cell]) {
                return 1;
            }
        }
    }
    else {
        if (k == FRKey::DOWN) {
            row += object->h / cell_size;
        }
        for (int i = 0; i < object->w / cell_size; i++) {
            if (map[row][cell + i]) {
                return 1;
            }
        }
    }
    return 0;
}

void MyFramework::Rotate(Movable* object, FRKey k) {
    int expected_x = object->x;
    int expected_y = object->y;

    object->Rotate(k);
    if (object->current_direction != k) {
        if (k == FRKey::RIGHT || k == FRKey::LEFT) {
            if (object->current_direction == FRKey::DOWN) {
                expected_y += (cell_size * 2)  - object->y % (cell_size * 2);
            }
            else {
                expected_y -= object->y % (cell_size * 2);
            }
        }
        else {
            if (object->current_direction == FRKey::RIGHT) {
                expected_x += (cell_size * 2) - object->x % (cell_size * 2);
            }
            else {
                expected_x -= object->x % (cell_size * 2);
            }
        }
        if (!CheckCollision(object, k, expected_x, expected_y)) {
            object->x = expected_x;
            object->y = expected_y;
        }
        object->current_direction = k;
    }
}

void MyFramework::Move(Movable* object) {
    int frame = object->speed > MAX_SPEED ? GAME_SPEED / MAX_SPEED : GAME_SPEED / object->speed;
    
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

void MyFramework::onMouseMove(int x, int y, int xrelative, int yrelative) {

}

void MyFramework::onMouseButtonClick(FRMouseButton button, bool isReleased) {

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