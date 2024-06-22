#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
#include "Bullet/basebullet.hpp"
#include "Engine/Group.hpp"
#include "base.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int base::Price = 50;
int base::extraDamage = 0;
base::base(float x, float y) :
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'MachineGunTurret.hpp', 'MachineGunTurret.cpp' to create a new turret.
	Turret("play/tower-base.png", "play/turret-1.png", x, y, 200, Price, 0.5, 1000, 20, 35) {
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
	type = "baseturret";
}
void base::CreateBullet() {
Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new basebullet(Position + normalized * 36, diff, rotation, this, damage));
	AudioHelper::PlayAudio("gun.wav");
}

void base::OnExplode(){
    Engine::GameEngine::GetInstance().ChangeScene("lose-scene");
}
