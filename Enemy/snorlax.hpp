#ifndef SNORLAX_HPP
#define SNORLAX_HPP

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include "Enemy.hpp"
#include "Bullet/Bullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"
#include <vector>
#include <string>

class snorlax : public Enemy {
private:
    int frameCount;
    std::vector<std::string> frames;
    int direction;
    
public:
    snorlax(int x, int y);
    void Update(float deltaTime) override;
    void Draw() const override;
};

#endif