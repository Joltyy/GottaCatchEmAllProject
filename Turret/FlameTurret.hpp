#ifndef FLAMETURRET_HPP
#define FLAMETURRET_HPP
#include "Turret.hpp"

class FlameTurret: public Turret {
public:
	static const int Price;
    FlameTurret(float x, float y);
    void CreateBullet() override;
    static int extraDamage;
};
#endif // FLAMETURRET_HPP
