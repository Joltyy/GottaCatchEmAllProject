#include <string>

#include "Tank2Enemy.hpp"

Tank2Enemy::Tank2Enemy(int x, int y) : Enemy("play/enemy-5.png", x, y, 10, 60, 8 + (8 * extraHp), 5, 50, 10 + (10 * extraDmg), 10) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
}

void Tank2Enemy::Update(float deltaTime) {
    // Increase speed when health is less than a certain value
    if (hp < 3) {
        speed = 180; // Set the speed to your desired value
    }

    // Call the parent update method to continue with the usual update
    Enemy::Update(deltaTime);
}

// void Tank2Enemy::shoot() {
//     // Get the nearest turret
//     Turret* nearestTurret = nullptr;
//     float minDistance = std::numeric_limits<float>::max();
//     for (auto& it : getPlayScene()->EnemyGroup->GetObjects()) {
//         Turret* turret = dynamic_cast<Turret*>(it);
//         if (turret) {
//             float distance = (turret->Position - this->Position).Magnitude();
//             if (distance < minDistance) {
//                 minDistance = distance;
//                 nearestTurret = turret;
//             }
//         }
//     }

//     // If a turret is found, shoot a bullet towards it
//     if (nearestTurret) {
//         Engine::Point direction = (nearestTurret->Position - this->Position).Normalize();
//         // Create a new bullet and add it to the scene
//         Bullet* bullet = new Bullet("play/bullet-3.png", 700, 50, this->Position, direction, 0, nearestTurret);
//         getPlayScene()->BulletGroup->AddNewObject(bullet);
//     }
// }


