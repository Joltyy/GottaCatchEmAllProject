#ifndef FLAMEBULLET_HPP
#define FLAMEBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class FlameBullet : public Bullet {
public:
	explicit FlameBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
	void OnExplode(Enemy* enemy) override;
};
#endif // LASERBULLET_HPP
