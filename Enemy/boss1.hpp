#ifndef BOSS1_HPP
#define BOSS1_HPP

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include "Enemy.hpp"
#include "Bullet/Bullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"
#include <vector>
#include <string>

class boss1 : public Enemy {
private:
    int frameCount;
    std::vector<std::string> frames;
    int direction;
public:
    boss1(int x, int y);
    void Update(float deltaTime) override;
    void Draw() const override;
};

#endif