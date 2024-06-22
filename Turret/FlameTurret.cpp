#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "FlameTurret.hpp"
#include "Bullet/FlameBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Enemy/Enemy.hpp"

const int FlameTurret::Price = 300;
int FlameTurret::extraDamage = 0;
FlameTurret::FlameTurret(float x, float y) :
	Turret("play/tower-base.png", "play/turret-7.png", x, y, 500, Price, 2.0, 100, 1 + extraDamage, 35) {
	type = "Flame";
	// Move center downward, since we the turret head is slightly biased upward.
	//Anchor.y += 8.0f / GetBitmapHeight();
}

// void FlameTurret::CreateBullet() {
// 	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
// 	float rotation = atan2(diff.y, diff.x);
// 	Engine::Point normalized = diff.Normalize();
// 	Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
// 	// Change bullet position to the front of the gun barrel.
// 	getPlayScene()->BulletGroup->AddNewObject(new FlameBullet(Position + normalized * 36 - normal * 6, diff, rotation, this));
// 	getPlayScene()->BulletGroup->AddNewObject(new FlameBullet(Position + normalized * 36 + normal * 6, diff, rotation, this));
// 	AudioHelper::PlayAudio("laser.wav");
// }

void FlameTurret::CreateBullet() {
    // Get all enemies in the scene
    auto enemies = getPlayScene()->EnemyGroup->GetObjects();

for (auto& it : enemies) {
	Enemy* enemy = dynamic_cast<Enemy*>(it);
	if (enemy) {
		// Calculate direction to enemy
		Engine::Point diff = enemy->Position - this->Position;
		float rotation = atan2(diff.y, diff.x);
		Engine::Point normalized = diff.Normalize();
		Engine::Point normal = Engine::Point(-normalized.y, normalized.x);

		// Create a bullet for each enemy
		getPlayScene()->BulletGroup->AddNewObject(new FlameBullet(Position + normalized * 36 - normal * 6, diff, rotation, this, damage));
		getPlayScene()->BulletGroup->AddNewObject(new FlameBullet(Position + normalized * 36 + normal * 6, diff, rotation, this, damage));
	}
}

    AudioHelper::PlayAudio("laser.wav");
}
