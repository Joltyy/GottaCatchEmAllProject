#ifndef MISSILETURRET_HPP
#define MISSILETURRET_HPP
#include "Turret.hpp"

class MissileTurret: public Turret {
public:
	static const int Price;
    MissileTurret(float x, float y);
    void CreateBullet() override;
    static int extraDamage;
};
#endif // MISSILETURRET_HPP
