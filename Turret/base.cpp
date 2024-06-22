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
#include "Enemy/boss1.hpp"

const int base::Price = 50;
int base::extraDamage = 0;
base::base(float x, float y) :
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'MachineGunTurret.hpp', 'MachineGunTurret.cpp' to create a new turret.
	Turret("play/Pokémon_Center_RSE.png", "play/Pokémon_Center_RSE.png", x, y, 200, Price, 0.5, 1000, 20, 35) {
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

void base::Update(float deltaTime) {
    Sprite::Update(deltaTime);
	PlayScene* scene = getPlayScene();
	imgBase.Position = Position;
	imgBase.Tint = Tint;
	if (!Enabled)
		return;
	if (Target) {
		Engine::Point diff = Target->Position - Position;
		if (diff.Magnitude() > CollisionRadius || diff.Magnitude() > hitbox) {
			Target->lockedTurrets.erase(lockedTurretIterator);
			Target = nullptr;
			lockedTurretIterator = std::list<Turret*>::iterator();
		}
	}
	if (!Target) {
		// Lock first seen target.
		// Can be improved by Spatial Hash, Quad Tree, ...
		// However simply loop through all enemies is enough for this program.
		for (auto& it : scene->EnemyGroup->GetObjects()) {
			Engine::Point diff = it->Position - Position;
			if (diff.Magnitude() <= CollisionRadius || diff.Magnitude() <= hitbox) {
				Target = dynamic_cast<Enemy*>(it);
				Target->lockedTurrets.push_back(this);
				lockedTurretIterator = std::prev(Target->lockedTurrets.end());
				break;
			}
		}
	}
	if (Target) {
        Engine::Point originRotation = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
		Engine::Point targetRotation = (Target->Position - Position).Normalize();
		float maxRotateRadian = rotateRadian * deltaTime;
		float cosTheta = originRotation.Dot(targetRotation);
		// Might have floating-point precision error.
		if (cosTheta > 1) cosTheta = 1;
		else if (cosTheta < -1) cosTheta = -1;
		float radian = acos(cosTheta);
		Engine::Point rotation;
		if (abs(radian) <= maxRotateRadian)
			rotation = targetRotation;
		else
			rotation = ((abs(radian) - maxRotateRadian) * originRotation + maxRotateRadian * targetRotation) / radian;
		// Add 90 degrees (PI/2 radian), since we assume the image is oriented upward.
		Rotation = atan2(rotation.y, rotation.x) + ALLEGRO_PI / 2;
		reload -= deltaTime;
		if (reload <= 0) {
			// shoot.
			reload = coolDown;
			CreateBullet();
		}
	}
    // Existing update logic...

    // Check for collisions with enemies.
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        Engine::Point diff = it->Position - Position;
        if (diff.Magnitude() <= hitbox) {
            Enemy* enemy = dynamic_cast<Enemy*>(it);

            // Calculate knockback direction.
            Engine::Point knockbackDirection = diff.Normalize();

            // Apply knockback to enemy position.
            enemy->Position = enemy->Position + (knockbackDirection * 20.0f);

            // Set enemy to knockback state if applicable.
            enemy->isKnockback = true;
            enemy->knockbackTimer = 0.3f; // Adjust knockback duration as needed.

			// Check if the enemy is a boss1.
			boss1* boss = dynamic_cast<boss1*>(enemy);
			if (boss != nullptr) {
				// Set the boss to knockbacked state.
				boss->SetKnockbacked(true);
			}

			//decrease turret life when hit by enemy
			Turret::Hit(enemy->damage);
        }
    }
}
