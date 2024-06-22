#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Character/Player.hpp"
#include "UI/Component/ImageButton.hpp"
class Turret;
namespace Engine {
	class Group;
	class Image;
	class Label;
	class Sprite;
}  // namespace Engine

class PlayScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
protected:
	int lives;
	int money;
	int SpeedMult;
	int score;
public:
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
	};
	static bool DebugMode;
	static const std::vector<Engine::Point> directions;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
	static const float DangerTime;
	static const Engine::Point SpawnGridPoint;
	std::vector<Engine::Point> SpawnGridPoints;
	static const Engine::Point EndGridPoint;
	static const std::vector<int> code;
	int MapId;
	float ticks;
	float deathCountDown;
	float timeSinceLastSpawn = 0.0f;
	float timeSinceStart = 0.0f;
	float difficultyIncreaseInterval = 30.0f;
	float spawnInterval = 3.0f;
	bool spawning = true;
	float spawnBreak = 5.0f;
	int randomEnemySelector = 1;
	int wave = 0;



	// // camera
	// float centerX = 0;
	// float centerY = 0;

	// Map tiles.
	Group* TileMapGroup;
	Group* GroundEffectGroup;
	Group* DebugIndicatorGroup;
	Group* BulletGroup;
	Group* TowerGroup;
	Group* TurretGroup;
	Group* EnemyGroup;
	Group* EffectGroup;
	Group* UIGroup;
	Group* turretUpgradeButtonGroup;
	Group* miscGroup;
	Engine::Label* UIMoney;
	Engine::Label* UILives;
	Engine::Label* UIScore;

	//player stats
	Player* player;
	Engine::Label* playerEnergy;
	Engine::Label* playerExp;
	Engine::Label* playerLevel;
	Engine::Label* playerSkillPoint;
	Engine::Label* playerStrength;
	Engine::Label* playerIntelligence;
	Engine::Label* playerEndurance;
	Engine::Label* Wave;

	//skill1 stats
	Engine::Label* Skill1LevelLabel;

	//image button
	Engine::ImageButton* skill1UpgradeButton;
	Engine::ImageButton* strengthUpgradeButton;
	Engine::ImageButton* intelligenceUpgradeButton;
	Engine::ImageButton* enduranceUpgradeButton;
	Engine::ImageButton* machinegunUpgradeButton;
	Engine::ImageButton* missileUpgradeButton;
	Engine::ImageButton* laserUpgradeButton;
	Engine::ImageButton* flameUpgradeButton;

	Engine::Image* imgTarget;
	Engine::Sprite* dangerIndicator;
	Turret* preview;
	std::vector<std::vector<TileType>> mapState;
	std::vector<std::vector<int>> mapDistance;
	std::list<std::pair<int, float>> enemyWaveData;
	std::list<int> keyStrokes;
	static Engine::Point GetClientSize();
	explicit PlayScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
	void OnKeyUp(int keyCode) override;
	void Hit();
	int GetMoney() const;
	void EarnMoney(int money);
	void EarnExp(int exp);
	void ReadMap();
	void ReadEnemyWave();
	void ConstructUI();
	void UIBtnClicked(int id);
	bool CheckSpaceValid(int x, int y);
	void WriteScoreToScoreboard(int score);
	std::vector<std::vector<int>> CalculateBFSDistance();
	bool isRemoving = false;
	void GenerateEdgePoints();
	void OnSkill1UpgradeClick();
	void OnStrengthUpgradeClick();
	void OnIntelligenceUpgradeClick();
	void OnEnduranceUpgradeClick();
	void OnMachineGunUpgradeClick();
	void OnMissileUpgradeClick();
	void OnLaserUpgradeClick();
	void OnFlameUpgradeClick();
	//Turret* getTurret(int id);
	//void removeTurret(int id);
	// void ModifyReadMapTiles();

	Turret* currentTurret;
};
#endif // PLAYSCENE_HPP
