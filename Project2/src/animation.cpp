#include "animation.h"

AnimationData::~AnimationData() {
    //
}

Explosion::Explosion() {
    for (int i = 1; i <= 3; i++) {
        std::string path = "Project2/data/ex" + std::to_string(i) + ".png";
        
        sprites.push_back(createSprite(path.c_str()));
    }
}

Animation::Animation(std::shared_ptr<AnimationData> animation_data,
                    int x_coord,
                    int y_coord) :
                    data(animation_data),
                    x(x_coord),
                    y(y_coord)
{

}
