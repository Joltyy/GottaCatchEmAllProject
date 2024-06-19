#include "Skill1.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Collider.hpp"
#include "Enemy/Enemy.hpp" 
#include <cmath>
#include <allegro5/allegro_primitives.h>
#include <random>
#include "UI/Animation/DirtyEffect.hpp"
#include "math.h"
#include <iostream>

Skill1::Skill1(Engine::Point position, Engine::Point velocity, float rotation, int damage, float lifetime) : Sprite("Animations/Skill1.png", position.x + 64, position.y + 64), velocity(velocity), rotation(rotation), damage(damage), lifetime(lifetime) {
    if (rotation == 0) { // Right
        visualOffsetY = -5.0f; // Example adjustment
    } else if (rotation == 180) { // Left
        visualOffsetY = 5.0f; // Example adjustment
    } else if (rotation == 90) { // Down
        visualOffsetX = 5.0f; // Example adjustment
    } else if (rotation == -90) { // Up
        visualOffsetX = -5.0f; // Example adjustment
    }
    this->Rotation = rotation;
    currentFrame = 0;
}

void Skill1::Update(float deltaTime) {
    Position.x += velocity.x * deltaTime;
    Position.y += velocity.y * deltaTime;

    frameTimer += deltaTime;
    if (frameTimer >= frameDuration) {
        currentFrame = (currentFrame + 1) % numFrames; // Loop back to the first frame after the last frame
        frameTimer = 0.0f; // Reset the timer for the next frame
    }

    lifetime -= deltaTime; // Decrease the lifetime by deltaTime
    if (lifetime <= 0) {
        Visible = false; // Make the skill disappear after its lifetime expires
        return; // Skip the collision detection
    }
    
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(it);
        if (!enemy->Visible || !Engine::Collider::IsCircleOverlap(Position, 20, enemy->Position, 20)) continue;
        OnExplode(enemy);

        break;
    }
}


void Skill1::Draw() const {
    int frameWidth = 16;
    int frameHeight = 16;
    // Calculate the x position of the current frame on the sprite sheet
    int frameX = currentFrame * (frameWidth + 1); // Assuming 1 pixel spacing between frames

    ALLEGRO_BITMAP* frameBitmap = al_create_sub_bitmap(Sprite::bmp.get(), frameX, 0, frameWidth, frameHeight);
    if (frameBitmap) {
        al_draw_scaled_rotated_bitmap(frameBitmap, frameWidth / 2, frameHeight / 2, Position.x + visualOffsetX, Position.y + visualOffsetY, scale, scale, rotation * M_PI / 180, 0);
        al_destroy_bitmap(frameBitmap);
    }
}

void Skill1::OnExplode(Enemy* enemy) {
    if (hitEnemies.find(enemy) == hitEnemies.end()) {
        // Calculate knockback direction
        Engine::Point directionToEnemy = enemy->Position - Position;
        Engine::Point knockbackDirection = directionToEnemy.Normalize();
        // Apply knockback to enemy position
        enemy->Position = enemy->Position + (knockbackDirection * knockbackDistance);
        // Set enemy to knockback state if applicable
        enemy->isKnockback = true;
        enemy->knockbackTimer = 0.3f; // Adjust knockback duration as needed
        enemy->Hit(damage); // Assuming a Hit method exists and deals 2 damage.
        hitEnemies.insert(enemy); // Mark this enemy as hit
    }
}