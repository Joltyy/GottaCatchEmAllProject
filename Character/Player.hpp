#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Engine/IObject.hpp"
#include "Engine/IControl.hpp"
#include "Engine/Point.hpp"
#include "Bullet/Bullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Enemy/Enemy.hpp" 

extern ALLEGRO_BITMAP* PlayerImage;
extern ALLEGRO_BITMAP* PlayerImageUp;
extern ALLEGRO_BITMAP* PlayerImageDown;
extern ALLEGRO_BITMAP* PlayerImageLeft;
extern ALLEGRO_BITMAP* PlayerImageRight;
extern ALLEGRO_BITMAP* PlayerImageAttack;
extern ALLEGRO_BITMAP* PlayerImageAttackUp;
extern ALLEGRO_BITMAP* PlayerImageAttackDown;
extern ALLEGRO_BITMAP* PlayerImageAttackLeft;
extern ALLEGRO_BITMAP* PlayerImageAttackRight;


class Player : public Engine::IObject, public Engine::IControl {
public:
    Player(float x, float y);
    void Update(float deltaTime) override;
    void UpdateAttackAnimation(float deltaTime);
    void UpdateWalkAnimation(float deltaTime);
    void Draw();
    void ConfinePlayerWithinMapBounds();
    void CheckAttackCollision();
    Engine::Point position;
    Engine::Point prevBitmapDirection;
    bool upPressed, downPressed, leftPressed, rightPressed, spacePressed;
    long long tickcount = 0;
    int currentAttackFrame = 0;
    int attackAnimationTickCount = 0;
    int attackAnimationTickRate = 12; // For attack animation every x game ticks.
    int walkAnimationTickCount = 0;
    int walkAnimationTickRate = 15; // For walk animation every x game ticks.

    //Player stat variables
    int PlayerLevel = 1;
    float energy = 100;
    float maxEnergy = 100;
    float energyRegenRate = 0.5;
    int SkillPoints = 0;
    int Exp = 0;
    int maxExp = 100;
    bool isMoving = false;
    float speed = 60.0f;

    int Strength = 1;
    int Intligence = 1;
    int Endurance = 1;

    //Basic attack variables
    int attackDamage = 2;
    float angle;
    float prevAngle;
    bool attackCollisonChecked = false;
    float knockbackDistance = 20.0f;
    float attackRange = 50.0f;
    float attackConeAngle = 135.0f;
    bool isAttacking = false;
    bool isOnCooldown = false;
    float attackCooldown = 1.0f;
    float cooldownTimer = 0.0f;

    //Skill 1 variables
    float skill1Cooldown = 8.0f;
    float skill1CooldownTimer = 0.0f;
    int skill1Level = 1;
    int skill1MaxLevel = 3;
    bool isUsingSkill1 = false;
    int Skill1Damage = 4;
    float Skill1Lifetime = 1.0f;
    int skill1ProjectileSpeed = 350;
    int Skill1EnergyCost = 10;

    void ActivateSkill1();
};

#endif // PLAYER_HPP
