#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/Group.hpp"
#include "stone.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int stone::Price = 50;
int stone::extraDamage = 0;
stone::stone(float x, float y) :
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'MachineGunTurret.hpp', 'MachineGunTurret.cpp' to create a new turret.
	Turret("play/stone_22.png", "play/transparent.png", x, y, 200, Price, 0.5, 10000000000, 1 + extraDamage, 35) {
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
	type = "MachineGun";
}
void stone::CreateBullet() {
    return;
}
void stone::Hit(float damage) {
	//std::cout << hp << std::endl;
	//std::cout << damage << std::endl;

	hp -= damage;
}