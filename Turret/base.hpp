#ifndef BASE_HPP
#define BASE_HPP
#include "Turret.hpp"

class base: public Turret {
public:
	static const int Price;
    base(float x, float y);
	void CreateBullet() override;
    void OnExplode() override;
    void Update(float deltaTime) override;
	static int extraDamage;
    static int Hp;
};
#endif // MACHINEGUNTURRET_HPP
