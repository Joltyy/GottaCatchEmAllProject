#include "Character/Player.hpp"
#include <allegro5/allegro.h> 
#include <allegro5/allegro_image.h> 
#include <iostream>
#include "Scene/PlayScene.hpp" 
#include "Engine/Point.hpp" 
#include "Engine/AudioHelper.hpp" 
#include "Engine/GameEngine.hpp"
#include "Enemy/Enemy.hpp" 
#include <cmath> 
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/MissileTurret.hpp"
#include "UI/Animation/Plane.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "Engine/Resources.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Turret/TurretButton.hpp"
#include <vector>
#include "Engine/Collider.hpp"
#include "Bullet/Skill1.hpp"

ALLEGRO_BITMAP* prevBitmap = PlayerImage;
ALLEGRO_BITMAP* prevAttackBitmap = PlayerImageAttack;
//ALLEGRO_SAMPLE* swordHit = al_load_sample("Resource/audios/Sword hit.wav");
//ALLEGRO_SAMPLE_ID swordHitID;

Player::Player(float x, float y) : position(x, y), upPressed(false), downPressed(false), leftPressed(false), rightPressed(false), spacePressed(false) {
    PlayerImageUp = al_load_bitmap("Resource/images/Animations/Player_Up_Walk.png");
    PlayerImageDown = al_load_bitmap("Resource/images/Animations/Player_Down_Walk.png");
    PlayerImageLeft = al_load_bitmap("Resource/images/Animations/Player_Left_Walk.png");
    PlayerImageRight = al_load_bitmap("Resource/images/Animations/Player_Right_Walk.png");

    PlayerImageAttackUp = al_load_bitmap("Resource/images/Animations/Player_Up_Attack2.png");
    PlayerImageAttackDown = al_load_bitmap("Resource/images/Animations/Player_Down_Attack2.png");
    PlayerImageAttackLeft = al_load_bitmap("Resource/images/Animations/Player_Left_Attack2.png");
    PlayerImageAttackRight = al_load_bitmap("Resource/images/Animations/Player_Right_Attack2.png");

    prevBitmap = PlayerImageDown;
    this->SkillPoints = 0;
    this->maxEnergy = 100 + ((Endurance - 1) * 10);
}

void Player::Update(float deltaTime) {
    tickcount++;
    UpdateWalkAnimation(deltaTime);

    // Energy regeneration
    energy += energyRegenRate * deltaTime;
    if (energy > maxEnergy) {
        energy = maxEnergy;
    }

    //Player level up
    while(Exp >= maxExp){
        PlayerLevel++;
        SkillPoints += 1;
        Exp -= maxExp;
        maxExp = maxExp * 1.2;

    }

    //PlayerSprinting
    if(isSprinting && energy > 0){
        speed = 100.0f;
        energy -= 0.1;
    } else {
        isSprinting = false;
        speed = 60.0f;
    }

    //Basic attack cooldown
    if(isOnCooldown) {
        cooldownTimer += deltaTime;
        if (cooldownTimer >= attackCooldown) {
            isOnCooldown = false;
            cooldownTimer = 0.0f;
        }
    }


    // Skill 1 Cooldown
    if (skill1CooldownTimer > 0) {
        skill1CooldownTimer -= deltaTime;
    }
    
    // Check if space is pressed to initiate attack.
    if (spacePressed && !isAttacking && !isUsingSkill1) {
        isAttacking = true;
        isOnCooldown = true;
        attackAnimationTickCount = 0; // Reset attack animation tick count.
        spacePressed = false; // Reset spacePressed to prevent continuous attacks.
        AudioHelper::PlayAudio("Sword hit.wav");

        // Update prevBitmap to the correct attack bitmap based on the last direction faced.
        if (prevBitmapDirection.y < 0) { // Up
            prevBitmap = PlayerImageAttackUp;
        } else if (prevBitmapDirection.y > 0) { // Down
            prevBitmap = PlayerImageAttackDown;
        } else if (prevBitmapDirection.x < 0) { // Left
            prevBitmap = PlayerImageAttackLeft;
        } else if (prevBitmapDirection.x > 0) { // Right
            prevBitmap = PlayerImageAttackRight;
        }
    }
    // Update direction immediately when a direction key is pressed.
    if (upPressed) {
        prevBitmap = isAttacking ? PlayerImageAttackUp : PlayerImageUp;
        prevBitmapDirection = Engine::Point(0, -1); // Facing up
    } else if (downPressed) {
        prevBitmap = isAttacking ? PlayerImageAttackDown : PlayerImageDown;
        prevBitmapDirection = Engine::Point(0, 1); // Facing down
    } else if (leftPressed) {
        prevBitmap = isAttacking ? PlayerImageAttackLeft : PlayerImageLeft;
        prevBitmapDirection = Engine::Point(-1, 0); // Facing left
    } else if (rightPressed) {
        prevBitmap = isAttacking ? PlayerImageAttackRight : PlayerImageRight;
        prevBitmapDirection = Engine::Point(1, 0); // Facing right
    } else if (isAttacking) {
        if(prevBitmap == PlayerImageAttackUp) prevBitmap = PlayerImageAttackUp;
        else if(prevBitmap == PlayerImageAttackDown) prevBitmap = PlayerImageAttackDown;
        else if(prevBitmap == PlayerImageAttackLeft) prevBitmap = PlayerImageAttackLeft;
        else if(prevBitmap == PlayerImageAttackRight) prevBitmap = PlayerImageAttackRight;
    }

    if (isAttacking) {
        UpdateAttackAnimation(deltaTime);
        CheckAttackCollision();
    }
    ConfinePlayerWithinMapBounds();

    // Update all player text
    PlayScene* playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
    if (playScene) {
        playScene->playerEnergy->Text = "Energy: " + std::to_string((int)energy);
        playScene->playerExp->Text = "Exp: " + std::to_string(Exp) + "/" + std::to_string(maxExp);
        playScene->playerLevel->Text = "Level: " + std::to_string(PlayerLevel);
        playScene->playerSkillPoint->Text = "Skill Points: " + std::to_string(SkillPoints);
    }
}

void Player::ConfinePlayerWithinMapBounds() {
    if (position.x < -32) position.x = -32;
    if (position.y < -32) position.y = -32;
    if (position.x > (PlayScene::MapWidth - 1) * PlayScene::BlockSize) position.x = (PlayScene::MapWidth - 1) * PlayScene::BlockSize;
    if (position.y > (PlayScene::MapHeight - 1) * PlayScene::BlockSize) position.y = (PlayScene::MapHeight - 1) * PlayScene::BlockSize;
}

void Player::CheckAttackCollision() {
    if(!attackCollisonChecked){
        PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
        for (auto& it : scene->EnemyGroup->GetObjects()) {
            
            Enemy* enemy = dynamic_cast<Enemy*>(it);
            if (!enemy->Visible) continue;
            Engine::Point directionToEnemy = (enemy->Position - Engine::Point{48, 48}) - position;
            float distanceToEnemy = directionToEnemy.Magnitude();
            // Check if the enemy is a boss1 instance
            Enemy* boss = dynamic_cast<Enemy*>(enemy);
            if (boss != nullptr) {
                // Use boss's hitbox for collision detection
                float collisionDistance = distanceToEnemy - boss->hitbox;
                if (collisionDistance <= attackRange) {
                    // Collision detected, apply damage and knockback
                    boss->Hit(attackDamage); // Assuming a Hit method and attackDamage variable exist
                    Engine::Point knockbackDirection = directionToEnemy.Normalize() * knockbackDistance;
                    boss->Position = boss->Position + knockbackDirection;
                    boss->isKnockback = true; // Assuming a method to set knockback state
                }
            } else {
                // Existing collision detection for other enemies
                Engine::Point directionToEnemyNormalized = directionToEnemy.Normalize();
                Engine::Point prevBitmapDirectionNormalized = prevBitmapDirection.Normalize();
                if (directionToEnemy.Magnitude() > 0 && prevBitmapDirection.Magnitude() > 0) {
                    float dot = directionToEnemyNormalized.Dot(prevBitmapDirectionNormalized);
                    dot = std::max(-1.0f, std::min(1.0f, dot));
                    angle = acos(dot) * (180.0f / M_PI); // Convert radians to degrees
                    const float angleTolerance = 0.5f;
                    if (std::abs(prevAngle - angle) <= angleTolerance) {
                        angle = prevAngle;
                    }
                    prevAngle = angle;
                    if (angle <= attackConeAngle && distanceToEnemy <= attackRange) {
                        enemy->Hit(attackDamage); // Assuming a Hit method and attackDamage variable exist
                        Engine::Point knockbackDirection = directionToEnemyNormalized * knockbackDistance;
                        enemy->Position = enemy->Position + knockbackDirection;
                        enemy->isKnockback = true;
                    }
            } else {
                std::cout << "One or both direction vectors are zero vectors." << std::endl;
            }
        }
        for(auto it : scene->TowerGroup->GetObjects()){
            Turret* object = dynamic_cast<Turret*>(it);
            if(object != nullptr){
                Engine::Point directionToEnemy = (object->Position - Engine::Point{48, 48}) - position;
                float distanceToEnemy = directionToEnemy.Magnitude();
                Engine::Point directionToEnemyNormalized = directionToEnemy.Normalize();
                Engine::Point prevBitmapDirectionNormalized = prevBitmapDirection.Normalize();
                if (directionToEnemy.Magnitude() > 0 && prevBitmapDirection.Magnitude() > 0) {
                    float dot = directionToEnemyNormalized.Dot(prevBitmapDirectionNormalized);
                    dot = std::max(-1.0f, std::min(1.0f, dot));
                    angle = acos(dot) * (180.0f / M_PI); // Convert radians to degrees
                    const float angleTolerance = 0.5f;
                    if (std::abs(prevAngle - angle) <= angleTolerance) {
                        angle = prevAngle;
                    }
                    prevAngle = angle;
                    if (angle <= attackConeAngle && distanceToEnemy <= attackRange) {
                        scene->money += 1;
                        scene->EarnMoney(0);
                    }
                } else {
                    std::cout << "One or both direction vectors are zero vectors." << std::endl;
                }
            }
        }
        attackCollisonChecked = true;
    }
    }
}

void Player::UpdateAttackAnimation(float deltaTime) {
    if (isAttacking && tickcount % attackAnimationTickRate == 0) {
        attackAnimationTickCount++;
        if (attackAnimationTickCount >= 5) { // Assuming 6 frames for the attack animation
            isAttacking = false;
            attackAnimationTickCount = 0;
        }
    }
}

void Player::UpdateWalkAnimation(float deltaTime) {
    isMoving = upPressed || downPressed || leftPressed || rightPressed;

    if (tickcount % walkAnimationTickRate == 0 && isMoving) {
        walkAnimationTickCount++;
        if (walkAnimationTickCount >= 2) {
            walkAnimationTickCount = 0; // Loop back to the first frame
        }
    }

    if (upPressed) position.y -= speed * deltaTime;
    if (downPressed) position.y += speed * deltaTime;
    if (leftPressed) position.x -= speed * deltaTime;
    if (rightPressed) position.x += speed * deltaTime;
}

void Player::Draw() {
    int frameWidth = 64;
    int frameHeight = 64;
    int numFrames = 2; // Default to 2 for walking
    int currentFrame = 0;

    ALLEGRO_BITMAP* currentBitmap = prevBitmap;

    if (isAttacking) {
        numFrames = 5; // Assuming 5 frames for attack
        currentFrame = attackAnimationTickCount % numFrames;
        // Choose the correct attack bitmap based on the last direction pressed
        if (upPressed || prevBitmap == PlayerImageAttackUp) currentBitmap = PlayerImageAttackUp;
        else if (downPressed || prevBitmap == PlayerImageAttackDown) currentBitmap = PlayerImageAttackDown;
        else if (leftPressed || prevBitmap == PlayerImageAttackLeft) currentBitmap = PlayerImageAttackLeft;
        else if (rightPressed || prevBitmap == PlayerImageAttackRight) currentBitmap = PlayerImageAttackRight;
    } else if (isMoving) {
        numFrames = 2; // Assuming 2 frames for walking
        currentFrame = walkAnimationTickCount % numFrames;
        // Choose the correct walking bitmap based on the direction pressed
        if (upPressed) currentBitmap = PlayerImageUp;
        else if (downPressed) currentBitmap = PlayerImageDown;
        else if (leftPressed) currentBitmap = PlayerImageLeft;
        else if (rightPressed) currentBitmap = PlayerImageRight;
    }

    if (!currentBitmap) {
        // Fallback to prevBitmap if currentBitmap is not set.
        currentBitmap = prevBitmap;
    }

    // Draw the current frame of the current bitmap
    if (currentBitmap) {
        al_draw_scaled_bitmap(currentBitmap, 
                              currentFrame * frameWidth, 0, // Source region x, y
                              frameWidth, frameHeight, // Source region width, height
                              position.x, position.y, // Destination x, y
                              frameWidth * 1.5, frameHeight * 1.5, // Destination width, height (scaled)
                              0);
    }
}

void Player::ActivateSkill1() {
    if (energy >= Skill1EnergyCost && skill1CooldownTimer <= 0) {
        isUsingSkill1 = true;
        energy -= 10; // Deduct energy
        skill1CooldownTimer = skill1Cooldown; // Start cooldown timer
        AudioHelper::PlayAudio("Skill1.wav");
        Engine::Point direction = prevBitmapDirection.Normalize();
        float rotation = atan2(direction.y, direction.x) * (180 / M_PI);
        Engine::Point velocity = direction * skill1ProjectileSpeed;
        auto* projectile = new Skill1(position, velocity, rotation, Skill1Damage, Skill1Lifetime);
        PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
        scene->BulletGroup->AddNewObject(projectile);
    }
}

void Player::updateStats(){
    maxEnergy = 100 + ((Endurance - 1) * 10);
    attackDamage = 2 + Strength;
    energyRegenRate = 0.5 + (Intelligence * 0.1);
}