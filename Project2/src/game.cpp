#include "Framework.h"
#include <stdlib.h>
#include <math.h>

#include <array>
#include <memory>
#include <iostream>
#include <unordered_map>

#define MAX_SPEED 4
#define GAME_SPEED 12
#define FRAMES 3

int border_size = 10;
int cell_size = 16;

void drawSpriteWithBorder(Sprite* sprite, int x, int y) {
	drawSprite(sprite, x + border_size, y + border_size);
}

class Object {
protected:
	int health = 1;
public:
	Object(const char *path, int n_x, int n_y) :
			sprite(createSprite(path)),
			x(n_x),
			y(n_y)
	{

	}
	virtual ~Object() {
		destroySprite(sprite);
	}

	virtual void Draw() {
		drawSpriteWithBorder(sprite, x * cell_size, y * cell_size);
	}

	Sprite *sprite;
	int x;
	int y;
};

class BrickWall : public Object {
public:
	BrickWall(int n_x, int n_y) :
			Object("Project2/data/brick.png", n_x, n_y)
	{

	}
	virtual ~BrickWall()
	{
		destroySprite(sprite);
	}	
};

class Movable {
public:
	FRKey current_direction;
	std::unordered_map<FRKey, int> directions = {
        {FRKey::RIGHT, 0},
        {FRKey::LEFT, 0},
        {FRKey::UP, 0},
        {FRKey::DOWN, 0}
    };
	Sprite *sprite;

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int speed = 1;
	int step_size = 1;
public:
	Movable(FRKey key) : current_direction(key) 
	{

	}
	virtual ~Movable() {

	}

	virtual void Draw() {
		drawSpriteWithBorder(sprite, x, y);
	};

	void Start(FRKey k) {
		for (auto& [key, value] : directions) {
			if (value) {
				value++;
			}
		}
		directions[k] = 1;
	}

	void Stop(FRKey k) {
		for (auto& [key, value] : directions) {
			if (value && value > directions[k]) {
				value--;
			}
		}
		directions[k] = 0;
	}

	// virtual void Move() = 0;

	void Move(FRKey k) {
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

	void Rotate(FRKey k) {
		if (current_direction != k) {
			current_direction = k;
		}
		ChangeSprite();
	}

	virtual void ChangeSprite() = 0;

	FRKey getDirection() {
		FRKey direction = FRKey::COUNT;

		for (const auto& [key, value] : directions) {
			if (value == 1) {
				direction = key;
				break;
			}
		}
		return direction;
	}
};

class Tank : public Movable{
	std::unordered_map<FRKey, Sprite *> even = {
        {FRKey::UP, createSprite("Project2/data/u_even.png")},
        {FRKey::LEFT, createSprite("Project2/data/l_even.png")},
        {FRKey::DOWN, createSprite("Project2/data/d_even.png")},
        {FRKey::RIGHT, createSprite("Project2/data/r_even.png")}
    };
    std::unordered_map<FRKey, Sprite *> odd = {
        {FRKey::UP, createSprite("Project2/data/u_odd.png")},
        {FRKey::LEFT, createSprite("Project2/data/l_odd.png")},
        {FRKey::DOWN, createSprite("Project2/data/d_odd.png")},
        {FRKey::RIGHT, createSprite("Project2/data/r_odd.png")}
    };
public:
	Tank(FRKey key) : Movable(key) 
	{	
		sprite = even[current_direction];
		w = 64;
		h = 64;
	}
	~Tank()
	{
		for (auto &[key, value] : even) {
			destroySprite(value);
		};
		for (auto &[key, value] : odd) {
			destroySprite(value);
		};
	}

	virtual void ChangeSprite() override {
		sprite = getTickCount() % 2 == 0 ?
					even[current_direction] :
					odd[current_direction];
	} 
};

class Player : public Tank {
public:
	Player(FRKey key) : Tank(key) 
	{	
		speed = 2;
	}
	~Player() {

	}
};

class MyFramework : public Framework {
	Sprite *background;
	std::unique_ptr<Tank> player;
	std::array<std::array<std::shared_ptr<Object>, 32>, 32> map;

	int map_w = 512;
	int map_h = 512;
public:
	MyFramework() {

	}

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = map_w + border_size * 2;
		height = map_h + border_size * 2;
		fullscreen = false;
	}

	virtual bool Init() {
		background = createSprite("Project2/data/back.jpg");
		for (int i = 4; i < 8; ++i) {
			for (int j = 0; j < 4; ++j) {
				map[i][j] = std::make_shared<BrickWall> (j, i);
			}	
		}
		player = std::make_unique<Player> (FRKey::UP);
		return true;
	}

	virtual void Close() {
		destroySprite(background);
	}

	virtual bool Tick() {
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

	bool CheckCollision(Movable *object, FRKey k) {
		int sign = k == FRKey::LEFT || k == FRKey::UP ? -1 : 1;
		int expected_x = object->x;
		int expected_y = object->y;
		// int row = 0;
		// int cell = 0;

		if (k == FRKey::RIGHT || k == FRKey::LEFT) {
			expected_x += object->step_size * sign;
			if (expected_x < 0 ||
				expected_x > map_w - object->w) {
				return 1;
			}
		}
		else {
			expected_y += object->step_size * sign;
			if (expected_y < 0 ||
				expected_y > map_h - object->h) {
				return 1;
			}
		}
		// row += expected_y / cell_width /stretch;
		// cell += expected_x / cell_width /stretch;
		// if (k == FRKey::RIGHT || k == FRKey::LEFT) {
		// 	if (k == FRKey::RIGHT) {
		// 		cell += object->drawing.w / cell_width;
		// 	}
		// 	for (int i = 0; i <= object->drawing.h / cell_width; i++) {
		// 		if (map[row + i][cell]) {
		// 			return 1;
		// 		}
		// 	}
		// }
		// else {
		// 	if (k == FRKey::DOWN) {
		// 		row += object->drawing.h / cell_width;
		// 	}
		// 	for (int i = 0; i <= object->drawing.w / cell_width; i++) {
		// 		if (map[row][cell + i]) {
		// 			return 1;
		// 		}
		// 	}
		// }
		return 0;
	}

	void Move(Movable* object) {
		int frame = object->speed > MAX_SPEED ? GAME_SPEED / MAX_SPEED : GAME_SPEED / object->speed;
		
		for (const auto& [key, value] : object->directions) {
			if (value == 1) {
				object->Rotate(key);
				if (getTickCount() % frame == 0) {
					if (!CheckCollision(object, key)) {
						object->Move(key);
					}
				}
				break;
			}
		}
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

	}

	virtual void onKeyPressed(FRKey k) {
		player->Start(k);
	}

	virtual void onKeyReleased(FRKey k) {
		player->Stop(k);
	}

	virtual const char* GetTitle() override
	{
		return "Tanks";
	}

};

int main(int argc, char *argv[])
{
	return run(new MyFramework);
}
