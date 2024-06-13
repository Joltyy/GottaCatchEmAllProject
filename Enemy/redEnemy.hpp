#ifndef redENEMY_HPP
#define redENEMY_HPP
#include "Enemy.hpp"

class redEnemy : public Enemy {
public:
    redEnemy(int x, int y);
    void Update(float deltaTime) override;
private:
    float speedBoostTimer = 0;
    float speedBoostDuration = 0;
    const float speedBoostInterval = 10.0f; // Boost speed every 5 seconds
    const float speedBoostFactor = 4.0f; // Speed is doubled during boost
};
#endif // redENEMY_HPP
