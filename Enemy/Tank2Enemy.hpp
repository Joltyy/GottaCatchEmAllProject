#ifndef TANK2ENEMY_HPP
#define TANK2ENEMY_HPP
#include "Enemy.hpp"
#include "Bullet/Bullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"

class Tank2Enemy : public Enemy {
public:
	Tank2Enemy(int x, int y);
	//void shoot();
	void Update(float deltaTime);
private:
	Turret* enemyTank;
};
#endif // TANK2ENEMY_HPP
