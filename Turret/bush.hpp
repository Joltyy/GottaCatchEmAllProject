#ifndef BUSH_HPP
#define BUSH_HPP
#include "Turret.hpp"

class bush: public Turret {
public:
	static const int Price;
    bush(float x, float y);
	static int extraDamage;
    void CreateBullet() override;
};
#endif // MACHINEGUNTURRET_HPP
