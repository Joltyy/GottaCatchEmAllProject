#ifndef STONE_HPP
#define STONE_HPP
#include "Turret.hpp"

class stone: public Turret {
public:
	static const int Price;
    stone(float x, float y);
	static int extraDamage;
	void CreateBullet() override;
};
#endif // MACHINEGUNTURRET_HPP
