#include <string>

#include "redEnemy.hpp"

redEnemy::redEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 10, 50, 40, 200, 100) {
    // TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'redEnemy.hpp', 'redEnemy.cpp' to create a new enemy.
}

void redEnemy::Update(float deltaTime) {
    speedBoostTimer += deltaTime;
    if (speedBoostTimer >= speedBoostInterval && speedBoostDuration <= 0) {
        speed *= speedBoostFactor; // Start speed boost
        speedBoostDuration = 3.0f; // Boost speed for 2 seconds
    }
    if (speedBoostDuration > 0) {
        speedBoostDuration -= deltaTime;
        if (speedBoostDuration <= 0) {
            speed /= speedBoostFactor; // End speed boost
            speedBoostTimer = 0; // Reset timer when speed boost ends
        }
    }
    Enemy::Update(deltaTime);
}
