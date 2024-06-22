#ifndef GROWLITHE_HPP
#define GROWLITHE_HPP

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include "Enemy.hpp"
#include "Bullet/Bullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"
#include <vector>
#include <string>

class growlithe : public Enemy {
private:
    int frameCount;
    std::vector<std::string> frames;
    int direction;
    
public:
    growlithe(int x, int y);
    //~growlithe();
    void Update(float deltaTime) override;
    void Draw() const override;
    // bool boss_Knockback; // Add this state variable
    // void SetKnockbacked(bool knockbacked) { boss_Knockback = knockbacked; } // Add this setter
    
};

#endif