#ifndef BASE_HPP
#define BASE_HPP
#include "Turret.hpp"

class base: public Turret {
public:
	static const int Price;
    base(float x, float y);
	void CreateBullet() override;
    void OnExplode() override;
	static int extraDamage;
};
#endif // MACHINEGUNTURRET_HPP
