#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <vector>
#include <string>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;
class Turret;

class Turret: public Engine::Sprite {
protected:
    int price;
    float coolDown;
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;
    float hp;   // new
    Sprite imgBase;
    std::list<Turret*>::iterator lockedTurretIterator;
    PlayScene* getPlayScene();
    // Reference: Design Patterns - Factory Method.
    virtual void CreateBullet() = 0;
    virtual void OnExplode();   // new

public:
    int damage;
    bool Enabled = true;
    bool Preview = false;
    Enemy* Target = nullptr;
    float knockbackDistance = 10.0f;
    Turret(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price, float coolDown, float hp, int damage, float hitbox);
    void Update(float deltaTime) override;
    void Draw() const override;
	int GetPrice() const;
    void Hit(float damage);
};
#endif // TURRET_HPP
