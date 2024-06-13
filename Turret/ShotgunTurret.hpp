#ifndef SHOTGUNTURRET_HPP
#define SHOTGUNTURRET_HPP
#include "Turret.hpp"

class ShotgunTurret: public Turret {
public:
	static const int Price;
    ShotgunTurret(float x, float y);
	void CreateBullet() override;
};
#endif // SHOTGUNTURRET_HPP
