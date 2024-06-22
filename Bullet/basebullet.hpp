#ifndef BASEBULLET_HPP
#define BASEBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class basebullet : public Bullet {
public:
	explicit basebullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent, int damage);
	void OnExplode(Enemy* enemy) override;
};
#endif // LASERBULLET_HPP
