#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Bullet/MissileBullet.hpp"
#include "MissileTurret.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int MissileTurret::Price = 300;
int MissileTurret::extraDamage = 0;
MissileTurret::MissileTurret(float x, float y) :
	Turret("play/tower-base.png", "play/turret-3.png", x, y, 1000, Price, 4, 100 * extraHp, 4 + extraDamage, 35) {
	type = "Missile";
}
void MissileTurret::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position + normalized * 10 - normal * 6, diff, rotation, this, damage));
	getPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position + normalized * 10 + normal * 6, diff, rotation, this, damage));
	AudioHelper::PlayAudio("missile.wav");
}
