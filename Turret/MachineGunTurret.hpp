#ifndef MACHINEGUNTURRET_HPP
#define MACHINEGUNTURRET_HPP
#include "Turret.hpp"

class MachineGunTurret: public Turret {
public:
	static const int Price;
    MachineGunTurret(float x, float y);
	void CreateBullet() override;
	static int extraDamage;
};
#endif // MACHINEGUNTURRET_HPP
