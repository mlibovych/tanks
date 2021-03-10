#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Framework.h"

class AnimationData {
public:
    std::vector<Sprite *> sprites;

    virtual ~AnimationData();
};

class Explosion : public AnimationData {
public:
    Explosion();
};

class Animation {
public:
    Animation(std::shared_ptr<AnimationData> animation_data, int x_coord, int y_coord);
    
    std::shared_ptr<AnimationData> data;
    
    int x = 0;
    int y = 0;
    int tick = 0;
};
