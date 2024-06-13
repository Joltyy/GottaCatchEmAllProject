#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include <vector>
#include <unordered_set>

class Skill1 : public Engine::Sprite {
public:
    Skill1(Engine::Point position, Engine::Point velocity, float rotationm, int damage, float lifetime);

    void Update(float deltaTime) override;
    void OnExplode(Enemy* enemy);
    void Draw() const override;
    Engine::Point velocity;
    float rotation;
    int currentFrame = 0;
    int numFrames = 4; // Total number of frames in the animation
    float frameDuration = 0.2f; // Duration of each frame in seconds
    float frameTimer = 0.0f; // Timer to track when to switch frames
    float lifetime;
    float scale = 4.5f;
    int damage;
    float knockbackDistance = 30.0f;
private:
    float visualOffsetX;
    float visualOffsetY;
    std::unordered_set<Enemy*> hitEnemies;
};