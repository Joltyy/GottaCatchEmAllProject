#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/Group.hpp"
#include "ShotgunTurret.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int ShotgunTurret::Price = 100;
ShotgunTurret::ShotgunTurret(float x, float y) :
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'ShotgunTurret.hpp', 'ShotgunTurret.cpp' to create a new turret.
	Turret("play/tower-base.png", "play/turret-5.png", x, y, 150, Price, 1.5, 100, 1, 35) {
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}

void ShotgunTurret::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
	float spread = ALLEGRO_PI / 30;
	// Change bullet position to the front of the gun barrel.
	for (int i = -3; i <= 2; i++) {
        float newRotation = rotation + i * spread;
        Engine::Point newDiff = Engine::Point(cos(newRotation), sin(newRotation));
        getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position + newDiff * 36, newDiff, newRotation, this, damage));
    }

	AudioHelper::PlayAudio("gun.wav");
}