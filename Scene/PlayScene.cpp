#include <allegro5/allegro.h>
#include <allegro5/mouse.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <fstream>
#include <ctime>
#include <string>

#include "Engine/LOG.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/base.hpp"
#include "Turret/MissileTurret.hpp"
#include "UI/Animation/Plane.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "PlayScene.hpp"
#include "Engine/Resources.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Turret/TurretButton.hpp"
#include "StageSelectScene.hpp"
#include "Turret/FlameTurret.hpp"
#include "Enemy/Tank2Enemy.hpp"
#include "Character/Player.hpp"
#include "Enemy/boss1.hpp"
#include "Enemy/snorlax.hpp"
#include "Enemy/zapdos.hpp"
#include "Enemy/growlithe.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 40, PlayScene::MapHeight = 26;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
// Define the spawn points
// const std::vector<Engine::Point> PlayScene::SpawnGridPoints = { 
//     Engine::Point(-1, 0), // kiri atas
//     Engine::Point(0, -1), // kiri atas
//     Engine::Point(PlayScene::MapWidth, 0), // kanan atas
//     Engine::Point(PlayScene::MapWidth, PlayScene::MapHeight) // kanan bawah
// };

const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth / 2, MapHeight / 2);
const std::vector<int> PlayScene::code = { ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
									ALLEGRO_KEY_LEFT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_RIGHT,
									ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER };
Engine::Point PlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

ALLEGRO_BITMAP* PlayerImage;
ALLEGRO_BITMAP* PlayerImageUp;
ALLEGRO_BITMAP* PlayerImageDown;
ALLEGRO_BITMAP* PlayerImageLeft;
ALLEGRO_BITMAP* PlayerImageRight;
ALLEGRO_BITMAP* PlayerImageAttack;
ALLEGRO_BITMAP* PlayerImageAttackUp;
ALLEGRO_BITMAP* PlayerImageAttackDown;
ALLEGRO_BITMAP* PlayerImageAttackLeft;
ALLEGRO_BITMAP* PlayerImageAttackRight;

ALLEGRO_BITMAP* Skill1Image;

void PlayScene::Initialize() {
	Skill1Image = al_load_bitmap("Resource/images/play/Skill1.png");
	PlayerImage = al_load_bitmap("Resource/images/Animations/Player_Down_Walk.png");
	PlayerImageAttack = al_load_bitmap("Resource/images/Animations/Player_Down_Attack.png");
	this->player = new Player(MapWidth/2 * BlockSize, ((MapHeight/2) + 1) * BlockSize);
	AddNewObject(player);
	// TODO: done [HACKATHON-3-BUG] (1/5): There's a bug in this file, which crashes the game when you lose. Try to find it.
	// TODO: done [HACKATHON-3-BUG] (2/5): Find out the cheat code to test.
    // TODO: done [HACKATHON-3-BUG] (2/5): It should generate a Plane, and add 10000 to the money, but it doesn't work now.
	mapState.clear();
	keyStrokes.clear();
	ticks = 0;
	deathCountDown = -1;
	wave = 0;
	lives = 10;
	money = 300;
	SpeedMult = 1;
	score = 0;
	// Add groups from bottom to top.
	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	AddNewObject(miscGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadMap();
	ReadEnemyWave();
	mapDistance = CalculateBFSDistance();
	ConstructUI();
	imgTarget = new Engine::Image("play/target.png", 0, 0);
	imgTarget->Visible = false;
	preview = nullptr;
	TowerGroup->AddNewObject(imgTarget);
	// Preload Lose Scene
	deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
	Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
	// Start BGM.
	if (MapId == 1)
	{
		bgmId = AudioHelper::PlayBGM("battle1.ogg");
	}
	else if (MapId == 2)
	{
		bgmId = AudioHelper::PlayBGM("battle2.ogg");
	}

    // Add top and bottom edge points
    for (int x = 0; x < MapWidth; x++) {
        SpawnGridPoints.push_back(Engine::Point(x, 0)); // Top edge
        SpawnGridPoints.push_back(Engine::Point(x, MapHeight)); // Bottom edge
    }

    // Add left and right edge points
    for (int y = 0; y < MapHeight; y++) {
        SpawnGridPoints.push_back(Engine::Point(0, y)); // Left edge
        SpawnGridPoints.push_back(Engine::Point(MapWidth, y)); // Right edge
    }
}

void PlayScene::Terminate() {
	AudioHelper::StopBGM(bgmId);
	AudioHelper::StopSample(deathBGMInstance);
	deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	PlayScene::WriteScoreToScoreboard(score);
	IScene::Terminate();
	al_destroy_bitmap(PlayerImage);
	al_destroy_bitmap(PlayerImageUp);
	al_destroy_bitmap(PlayerImageDown);
	al_destroy_bitmap(PlayerImageLeft);
	al_destroy_bitmap(PlayerImageRight);
	al_destroy_bitmap(PlayerImageAttack);
	al_destroy_bitmap(PlayerImageAttackUp);
	al_destroy_bitmap(PlayerImageAttackDown);
	al_destroy_bitmap(PlayerImageAttackLeft);
	al_destroy_bitmap(PlayerImageAttackRight);
	al_destroy_bitmap(Skill1Image);
}

void PlayScene::Update(float deltaTime) {
	// When spawning a new enemy, select a random spawn point
	int spawnIndex = rand() % PlayScene::SpawnGridPoints.size();
	Engine::Point spawnPoint = PlayScene::SpawnGridPoints[spawnIndex];
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	if (SpeedMult == 0)
		deathCountDown = -1;
	else if (deathCountDown != -1)
		SpeedMult = 1;
	// Calculate danger zone.
	std::vector<float> reachEndTimes;
	for (auto& it : EnemyGroup->GetObjects()) {
		reachEndTimes.push_back(dynamic_cast<Enemy*>(it)->reachEndTime);
	}
	// Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
	std::sort(reachEndTimes.begin(), reachEndTimes.end());
	float newDeathCountDown = -1;
	int danger = lives;
	for (auto& it : reachEndTimes) {
		if (it <= DangerTime) {
			danger--;
			if (danger <= 0) {
				// Death Countdown
				float pos = DangerTime - it;
				if (it > deathCountDown) {
					// Restart Death Count Down BGM.
					AudioHelper::StopSample(deathBGMInstance);
					if (SpeedMult != 0)
						deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
				}
				float alpha = pos / DangerTime;
				alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
				dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
				newDeathCountDown = it;
				break;
			}
		}
	}
	deathCountDown = newDeathCountDown;
	if (SpeedMult == 0)
		AudioHelper::StopSample(deathBGMInstance);
	if (deathCountDown == -1 && lives > 0) {
		AudioHelper::StopSample(deathBGMInstance);
		dangerIndicator->Tint.a = 0;
	}
	if (SpeedMult == 0)
        deathCountDown = -1;

	for(int i = 0; i < SpeedMult; i++){
		IScene::Update(deltaTime);
		timeSinceStart += deltaTime;

		if(spawning) timeSinceLastSpawn += deltaTime;

		// Increase difficulty over time.
		if (timeSinceStart >= difficultyIncreaseInterval) {
			spawnInterval = std::max(0.3f, spawnInterval - 0.2f); // Decrease spawn interval to a minimum of 0.5 seconds.
			// Optionally, increase enemy toughness here.
			Enemy::extraDmg *= 1.2;
			Enemy::extraHp *= 1.2;
			timeSinceStart -= difficultyIncreaseInterval;
			spawning = false;
			Wave->Text = "Wave: " + std::to_string(++wave);
		}
		// Spawn enemy if the interval has passed.
		if (timeSinceLastSpawn >= spawnInterval && spawning) {
			timeSinceLastSpawn -= spawnInterval;
			// Randomly select an enemy type to spawn.
			if(timeSinceStart <= 10){
				randomEnemySelector = 2;
			} else if (timeSinceStart <= 20){
				randomEnemySelector = 3;
			} else if (timeSinceStart <= 30){
				randomEnemySelector = 4;
			}
			int enemyType = rand() % randomEnemySelector + 1; // Assuming 4 types of enemies.
			const Engine::Point SpawnCoordinate = Engine::Point(spawnPoint.x * BlockSize + BlockSize / 2, spawnPoint.y * BlockSize + BlockSize / 2);
			Enemy* enemy = nullptr;
			switch (enemyType) {
				case 1:
					enemy = new snorlax(SpawnCoordinate.x, SpawnCoordinate.y);
					break;
				case 2:
					enemy = new zapdos(SpawnCoordinate.x, SpawnCoordinate.y);
					break;
				case 3:
					enemy = new growlithe(SpawnCoordinate.x, SpawnCoordinate.y);
					break;
				case 4:
					enemy = new boss1(SpawnCoordinate.x, SpawnCoordinate.y);
					break;
				// Add cases for more enemy types if needed.
			}
			EnemyGroup->AddNewObject(enemy);
			enemy->UpdatePath(mapDistance);
			enemy->Update(deltaTime);
			std::cout << Enemy::extraHp << std::endl;
		}
	}

	if(!spawning && timeSinceStart >= spawnBreak){
		timeSinceStart -= spawnBreak;
		spawning = true;
	}

	if (preview) {
		float halfScreenWidth = 1280 / 2.0;
		float halfScreenHeight =  832 / 2.0;
		float desiredCenterX = -(player->position.x + 64) + halfScreenWidth;
		float desiredCenterY = -(player->position.y + 64) + halfScreenHeight;
		float centerX = std::max((int)std::min(desiredCenterX, 0.0f), -(PlayScene::MapWidth * PlayScene::BlockSize - 1280));
		float centerY = std::max((int)std::min(desiredCenterY, 0.0f), -(PlayScene::MapHeight * PlayScene::BlockSize - 832));
		preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
		preview->Position.x -= centerX;
		preview->Position.y -= centerY;
		// To keep responding when paused.
		preview->Update(deltaTime);
	}
	player->Update(deltaTime);

	float halfScreenWidth = 1280 / 2.0;
	float halfScreenHeight =  832 / 2.0;
	float desiredCenterX = -(player->position.x + 64) + halfScreenWidth;
	float desiredCenterY = -(player->position.y + 64) + halfScreenHeight;
	float centerX = std::max((int)std::min(desiredCenterX, 0.0f), -(PlayScene::MapWidth * PlayScene::BlockSize - 1280));
	float centerY = std::max((int)std::min(desiredCenterY, 0.0f), -(PlayScene::MapHeight * PlayScene::BlockSize - 832));
}
void PlayScene::Draw() const {
	al_clear_to_color(al_map_rgb(0, 0, 0));
    
	float halfScreenWidth = 1280 / 2.0;
	float halfScreenHeight =  832 / 2.0;
	float desiredCenterX = -(player->position.x + 64) + halfScreenWidth;
	float desiredCenterY = -(player->position.y + 64) + halfScreenHeight;
	float centerX = std::max((int)std::min(desiredCenterX, 0.0f), -(PlayScene::MapWidth * PlayScene::BlockSize - 1280));
	float centerY = std::max((int)std::min(desiredCenterY, 0.0f), -(PlayScene::MapHeight * PlayScene::BlockSize - 832));

    ALLEGRO_TRANSFORM camera;
    al_identity_transform(&camera); // Reset the camera to the identity transformation.
    al_translate_transform(&camera, centerX, centerY); // Move the camera to follow the player.
    al_use_transform(&camera); // Apply the transformed camera to the drawing context.

	//IScene::Draw();
	TileMapGroup->Draw();
	GroundEffectGroup->Draw();
	DebugIndicatorGroup->Draw();
	TowerGroup->Draw();
	EnemyGroup->Draw();
	BulletGroup->Draw();
	EffectGroup->Draw();
	miscGroup->Draw();
	player->Draw();
	if (DebugMode) {
		// Draw reverse BFS distance on all reachable blocks.
		for (int i = 0; i < MapHeight; i++) {
			for (int j = 0; j < MapWidth; j++) {
				if (mapDistance[i][j] != -1) {
					// Not elegant nor efficient, but it's quite enough for debugging.
					Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
					label.Anchor = Engine::Point(0.5, 0.5);
					label.Draw();
				}
			}
		}
		al_draw_filled_circle((Engine::GameEngine::GetInstance().GetMousePosition().x  - centerX), Engine::GameEngine::GetInstance().GetMousePosition().y - centerY, 10, al_map_rgba(255, 0, 0, 100));
	}

	al_identity_transform(&camera); // Reset the camera to the identity transformation.
    al_use_transform(&camera); // Apply the transformed camera to the drawing context.

	UIGroup->Draw();


	//Draw skill 1 icon
	if(player->skill1CooldownTimer > 0)
	{
	    // Draw with slight transparency if on cooldown.
        al_draw_tinted_scaled_bitmap(Skill1Image, al_map_rgba(255, 255, 255, 128), 
                                 0, 0, al_get_bitmap_width(Skill1Image), al_get_bitmap_height(Skill1Image), 
                                 1297, 700, 64, 64, 0);
    } else {
        // Draw normally if not on cooldown.
        al_draw_scaled_bitmap(Skill1Image, 
                          0, 0, al_get_bitmap_width(Skill1Image), al_get_bitmap_height(Skill1Image), 
                          1297, 700, 64, 64, 0);
    }

	//Draw upgrade button
	if(player->SkillPoints <= 0){
		playerSkillPoint->Visible = false;
		skill1UpgradeButton->Visible = false;
		strengthUpgradeButton->Visible = false;
		intelligenceUpgradeButton->Visible = false;
		enduranceUpgradeButton->Visible = false;
	} else {
		if(player->skill1Level < 3) skill1UpgradeButton->Visible = true;
		else skill1UpgradeButton->Visible = false;
		strengthUpgradeButton->Visible = true;
		intelligenceUpgradeButton->Visible = true;
		enduranceUpgradeButton->Visible = true;
		playerSkillPoint->Visible = true;
	}

	//draw turret upgrasde button
	if(money >= 400){
		missileUpgradeButton->Visible = true;
	} else {
		missileUpgradeButton->Visible = false;
	}

	if(money >= 350){
		laserUpgradeButton->Visible = true;
	} else {
		laserUpgradeButton->Visible = false;
	}

	if(money >= 300){
		flameUpgradeButton->Visible = true;
	} else {
		flameUpgradeButton->Visible = false;
	}

	if(money >= 200){
		machinegunUpgradeButton->Visible = true;
	} else {
		machinegunUpgradeButton->Visible = false;
	}
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && preview) {
		// Cancel turret construct.
		TowerGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	float halfScreenWidth = 1280 / 2.0;
	float halfScreenHeight =  832 / 2.0;
	float desiredCenterX = -(player->position.x + 64) + halfScreenWidth;
	float desiredCenterY = -(player->position.y + 64) + halfScreenHeight;
	float centerX = std::max((int)std::min(desiredCenterX, 0.0f), -(PlayScene::MapWidth * PlayScene::BlockSize - 1280));
	float centerY = std::max((int)std::min(desiredCenterY, 0.0f), -(PlayScene::MapHeight * PlayScene::BlockSize - 832));
	int blockx = (centerX) / BlockSize;
	int blocky = (centerY) / BlockSize;
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
		imgTarget->Visible = false;
		return;
	}
	imgTarget->Visible = true;
	imgTarget->Position.x = (x * BlockSize) - (blockx * BlockSize);
	imgTarget->Position.y = (y * BlockSize) - (blocky * BlockSize);
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	float halfScreenWidth = 1280 / 2.0;
	float halfScreenHeight =  832 / 2.0;
	float desiredCenterX = -(player->position.x + 64) + halfScreenWidth;
	float desiredCenterY = -(player->position.y + 64) + halfScreenHeight;
	float centerX = std::max((int)std::min(desiredCenterX, 0.0f), -(PlayScene::MapWidth * PlayScene::BlockSize - 1280));
	float centerY = std::max((int)std::min(desiredCenterY, 0.0f), -(PlayScene::MapHeight * PlayScene::BlockSize - 832));
	if (!imgTarget->Visible)
		return;
	const int x = mx / BlockSize - ((centerX) / BlockSize);
	const int y = my / BlockSize - ((centerY) / BlockSize);
	if (button & 1) {
		if (mapState[y][x] != TILE_OCCUPIED) {
			if (!preview)
				return;
			// Check if valid.
			if (!CheckSpaceValid(x, y)) {
				Engine::Sprite* sprite;
				GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
				sprite->Rotation = 0;
				return;
			}
			// Purchase.
			EarnMoney(-preview->GetPrice());
			// Remove Preview.
			preview->GetObjectIterator()->first = false;
			TowerGroup->RemoveObject(preview->GetObjectIterator());
			// Construct real turret.
			preview->Position.x = x * BlockSize + BlockSize / 2;
			preview->Position.y = y * BlockSize + BlockSize / 2;
			preview->Enabled = true;
			preview->Preview = false;
			preview->Tint = al_map_rgba(255, 255, 255, 255);
			TowerGroup->AddNewObject(preview);
			// To keep responding when paused.
			preview->Update(0);
			// Remove Preview.
			preview = nullptr;

			mapState[y][x] = TILE_OCCUPIED;
			OnMouseMove(mx, my);
		}
	}
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if (keyCode == ALLEGRO_KEY_TAB) {
		DebugMode = !DebugMode;
	}
	else {
		keyStrokes.push_back(keyCode);
		if (keyStrokes.size() > code.size())
			keyStrokes.pop_front();
		if (keyCode == ALLEGRO_KEY_ENTER && keyStrokes.size() == code.size()) {
			auto it = keyStrokes.begin();
			for (int c : code) {
				if (!((*it == c) ||
					(c == ALLEGRO_KEYMOD_SHIFT &&
					(*it == ALLEGRO_KEY_LSHIFT || *it == ALLEGRO_KEY_RSHIFT))))
					return;
				++it;
			}
			EffectGroup->AddNewObject(new Plane());
			money += 10000;
			player->Exp += 10000;
		}
	}
	if (keyCode == ALLEGRO_KEY_Q) {
		// Hotkey for MachineGunTurret.
		UIBtnClicked(0);
	}
	// else if (keyCode == ALLEGRO_KEY_W) {
	// 	// Hotkey for LaserTurret.
	// 	UIBtnClicked(1);
	// }
	// else if (keyCode == ALLEGRO_KEY_E) {
	// 	// Hotkey for MissileTurret.
	// 	UIBtnClicked(2);
	// }
	// else if (keyCode == ALLEGRO_KEY_R) {
	// 	// Key for FlameTurret
	// 	UIBtnClicked(3);
	// }
	// TODO: done [CUSTOM-TURRET]: Make specific key to create the turret.
	else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
		// Hotkey for Speed up.
		SpeedMult = keyCode - ALLEGRO_KEY_0;
	}

	switch (keyCode) {
        case ALLEGRO_KEY_W: player->upPressed = true; break;
        case ALLEGRO_KEY_S: player->downPressed = true; break;
        case ALLEGRO_KEY_A: player->leftPressed = true; break;
        case ALLEGRO_KEY_D: player->rightPressed = true; break;
        case ALLEGRO_KEY_SPACE: player->spacePressed = true; break;
		case ALLEGRO_KEY_1: 
			if(player->skill1CooldownTimer <= 0 && player->energy >= player->Skill1EnergyCost)
			{
				player->isUsingSkill1 = true;
				player->spacePressed = true;
				player->ActivateSkill1();
			}
			break;
		case ALLEGRO_KEY_LSHIFT:
			if(player->energy > 0 && !player->isSprinting){
				player->isSprinting = true;
			}
    }
}

void PlayScene::OnKeyUp(int keyCode) {
    switch (keyCode) {
        case ALLEGRO_KEY_W: player->upPressed = false; break;
        case ALLEGRO_KEY_S: player->downPressed = false; break;
        case ALLEGRO_KEY_A: player->leftPressed = false; break;
        case ALLEGRO_KEY_D: player->rightPressed = false; break;
        case ALLEGRO_KEY_SPACE: 
			player->spacePressed = false; 
			player->attackCollisonChecked = false;
			break;
		case ALLEGRO_KEY_1: 
			player->isUsingSkill1 = false; break;
		case ALLEGRO_KEY_LSHIFT:
			player->isSprinting = false;
    }
}

void PlayScene::Hit() {
	lives--;
	UILives->Text = std::string("Life ") + std::to_string(lives);
	if (lives <= 0) {
		Engine::GameEngine::GetInstance().ChangeScene("lose-scene");
	}
}
int PlayScene::GetMoney() const {
	return money;
}
void PlayScene::EarnMoney(int money) {
	int before = this->money;
	this->money += money;
	if (before < this->money) score += money / 5; //if kill then score update

	UIMoney->Text = std::string("$") + std::to_string(this->money);
	UIScore->Text = std::string("Score: ") + std::to_string(score);
}
void PlayScene::ReadMap() {
	std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
	// Read map file.
	char c;
	std::vector<bool> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
		case '0': mapData.push_back(false); break;
		case '1': mapData.push_back(true); break;
		case '\n':
		case '\r':
			if (static_cast<int>(mapData.size()) / MapWidth != 0)
				throw std::ios_base::failure("Map data is corrupted.");
			break;
		default: throw std::ios_base::failure("Map data is corrupted.");
		}
	}
	fin.close();
	// Validate map data.
	if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
		throw std::ios_base::failure("Map data is corrupted.");
	// Store map in 2d array.
	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
			if (num)
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			else
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				if(i == MapHeight / 2 && j == MapWidth / 2){
					TowerGroup->AddNewObject(new base(j * BlockSize + 32, i * BlockSize + 32));
					mapState[i][j] = TILE_OCCUPIED;
				}
		}
	}
}
void PlayScene::ReadEnemyWave() {
    // TODO: done [HACKATHON-3-BUG] (3/5): Trace the code to know how the enemies are created.
    // TODO: done [HACKATHON-3-BUG] (3/5): There is a bug in these files, which let the game only spawn the first enemy, try to fix it.
    std::string filename = std::string("Resource/enemy") + std::to_string(MapId) + ".txt";
	// Read enemy file.
	float type, wait, repeat;
	enemyWaveData.clear();
	std::ifstream fin(filename);
	while (fin >> type && fin >> wait && fin >> repeat) {
		for (int i = 0; i < repeat; i++)
			enemyWaveData.emplace_back(type, wait);
	}
	fin.close();
}
void PlayScene::ConstructUI() {
	// Background
	UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));

	// Text
	UIGroup->AddNewObject(new Engine::Label(std::string("Good Luck"), "pirulen.ttf", 32, 1294, 0));
	UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
	UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));
	UIGroup->AddNewObject(UIScore = new Engine::Label(std::string("Score: ") + std::to_string(score), "pirulen.ttf", 24, 1294, 300));
	TurretButton* btn;

	// Button 1
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1294, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-1.png", 1294, 136 - 8, 0, 0, 0, 0)
		, 1294, 136, MachineGunTurret::Price);
	// Reference: Class Member Function Pointer and std::bind.
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
	UIGroup->AddNewControlObject(btn);

	// Button 2
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1370, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-2.png", 1370, 136 - 8, 0, 0, 0, 0)
		, 1370, 136, LaserTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
	UIGroup->AddNewControlObject(btn);

	// Button 3
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1446, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-3.png", 1446, 136, 0, 0, 0, 0)
		, 1446, 136, MissileTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2));
	UIGroup->AddNewControlObject(btn);

	// TODO: done [CUSTOM-TURRET]: Create a button to support constructing the turret.
	// New turret
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1522, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-7.png", 1522, 136, 0, 0, 0, 0)
		, 1522, 136, FlameTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 3));
	UIGroup->AddNewControlObject(btn);

	// Turret remover button
	// btn = new TurretButton("play/floor.png", "play/dirt.png",
	// 	Engine::Sprite("play/tower-base.png", 1294, 270, 0, 0, 0, 0),
	// 	Engine::Sprite("play/turret-7.png", 1294, 270, 0, 0, 0, 0)
	// 	, 1294, 270, 0); // Assuming turret removal is free
	// btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 4));
	// UIGroup->AddNewControlObject(btn);

	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int shift = 135 + 25;
	dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
	dangerIndicator->Tint.a = 0;
	UIGroup->AddNewObject(dangerIndicator);

	//Skill 1 upgrade button
	skill1UpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1313, 765);
	skill1UpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnSkill1UpgradeClick, this));
	UIGroup->AddNewControlObject(skill1UpgradeButton);
	skill1UpgradeButton->Visible = false;

    //Skill 1 level label
    Skill1LevelLabel = new Engine::Label("lvl " + std::to_string(player->skill1Level), "pirulen.ttf", 12, 1330, 685, 0, 0, 0, 255, 0.5, 0.5);
    UIGroup->AddNewObject(Skill1LevelLabel);

	//Player's Energy level
	playerEnergy = new Engine::Label("Energy: " + std::to_string(player->energy), "pirulen.ttf", 24, 1297, 425, 0, 0, 255, 255, 0, 0);
	UIGroup->AddNewObject(playerEnergy);

	//Players's level and exp
	playerLevel = new Engine::Label("Level: " + std::to_string(player->PlayerLevel), "pirulen.ttf", 24, 1297, 375, 0, 0, 0, 255, 0, 0);
	UIGroup->AddNewObject(playerLevel);
	playerExp = new Engine::Label("Exp: " + std::to_string(player->Exp) + "/" + std::to_string(player->maxExp), "pirulen.ttf", 24, 1297, 400, 0, 200, 0, 255, 0, 0);
	UIGroup->AddNewObject(playerExp);

	//Player's strength
	playerStrength = new Engine::Label("Strength: " + std::to_string(player->Strength), "pirulen.ttf", 18, 1297, 475, 150, 0, 0, 255, 0, 0);
	UIGroup->AddNewObject(playerStrength);
	//Upgrade button
	strengthUpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1550, 472);
	strengthUpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnStrengthUpgradeClick, this));
	UIGroup->AddNewControlObject(strengthUpgradeButton);
	strengthUpgradeButton->Visible = false;

	//Players's Intelligence
	playerIntelligence = new Engine::Label("Intelligence: " + std::to_string(player->Intelligence), "pirulen.ttf", 18, 1297, 525, 0, 0, 150, 255, 0, 0);
	UIGroup->AddNewObject(playerIntelligence);
	//Upgrade button
	intelligenceUpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1550, 522);
	intelligenceUpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnIntelligenceUpgradeClick, this));
	UIGroup->AddNewControlObject(intelligenceUpgradeButton);
	intelligenceUpgradeButton->Visible = false;

	//Players's Endurance
	playerEndurance = new Engine::Label("Endurance: " + std::to_string(player->Endurance), "pirulen.ttf", 18, 1297, 575, 0, 150, 0, 255, 0, 0);
	UIGroup->AddNewObject(playerEndurance);
	//Upgrade button
	enduranceUpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1550, 572);
	enduranceUpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnEnduranceUpgradeClick, this));
	UIGroup->AddNewControlObject(enduranceUpgradeButton);
	enduranceUpgradeButton->Visible = false;

	//Players skill points
	playerSkillPoint = new Engine::Label("Skill Points: " + std::to_string(player->SkillPoints), "pirulen.ttf", 24, 1297, 800, 0, 0, 0, 255, 0, 0);
	UIGroup->AddNewObject(playerSkillPoint);
	playerSkillPoint->Visible = false;

	//MachinegunUpgradebutton
	machinegunUpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1310, 220);
	machinegunUpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnMachineGunUpgradeClick, this));
	UIGroup->AddNewControlObject(machinegunUpgradeButton);
	machinegunUpgradeButton->Visible = false;

	//LaserUpgradebutton
	laserUpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1387, 220);
	laserUpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnLaserUpgradeClick, this));
	UIGroup->AddNewControlObject(laserUpgradeButton);
	laserUpgradeButton->Visible = false;

	//MissileUpgradebutton
	missileUpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1462, 220);
	missileUpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnMissileUpgradeClick, this));
	UIGroup->AddNewControlObject(missileUpgradeButton);
	missileUpgradeButton->Visible = false;

	//FlameUpgradebutton
	flameUpgradeButton = new Engine::ImageButton("play/UpgradeButtonTransparent.png", "play/UpgradeButton.png", 1537, 220);
	flameUpgradeButton->SetOnClickCallback(std::bind(&PlayScene::OnFlameUpgradeClick, this));
	UIGroup->AddNewControlObject(flameUpgradeButton);
	flameUpgradeButton->Visible = false;

	//Wave label
	Wave = new Engine::Label("Wave " + std::to_string(++wave), "pirulen.ttf", 32, 530, 20);
	UIGroup->AddNewObject(Wave);
}

void PlayScene::UIBtnClicked(int id) {
	if (preview)
		TowerGroup->RemoveObject(preview->GetObjectIterator());
    // TODO: done [CUSTOM-TURRET]: On callback, create the turret.
	if (id == 0 && money >= MachineGunTurret::Price)
		preview = new MachineGunTurret(0, 0);
	else if (id == 1 && money >= LaserTurret::Price)
		preview = new LaserTurret(0, 0);
	else if (id == 2 && money >= MissileTurret::Price)
		preview = new MissileTurret(0, 0);
	else if (id == 3 && money >= FlameTurret::Price)
		preview = new FlameTurret(0, 0);
	// else if (id == 4)
	// 	isRemoving = true;
	if (!preview)
		return;
	preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	preview->Tint = al_map_rgba(255, 255, 255, 200);
	preview->Enabled = false;
	preview->Preview = true;
	TowerGroup->AddNewObject(preview);
	float halfScreenWidth = 1280 / 2.0;
	float halfScreenHeight =  832 / 2.0;
	float desiredCenterX = -(player->position.x + 64) + halfScreenWidth;
	float desiredCenterY = -(player->position.y + 64) + halfScreenHeight;
	float centerX = std::max((int)std::min(desiredCenterX, 0.0f), -(PlayScene::MapWidth * PlayScene::BlockSize - 1280));
	float centerY = std::max((int)std::min(desiredCenterY, 0.0f), -(PlayScene::MapHeight * PlayScene::BlockSize - 832));
	preview->Position.x -= centerX;
	preview->Position.y -= centerY;
	OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x + (centerX - 64), Engine::GameEngine::GetInstance().GetMousePosition().y + (centerY - 64));
}

bool PlayScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
		return false;
	auto map00 = mapState[y][x];
	mapState[y][x] = TILE_OCCUPIED;
	std::vector<std::vector<int>> map = CalculateBFSDistance();
	mapState[y][x] = map00;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0) pnt.x = 0;
		if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
		if (pnt.y < 0) pnt.y = 0;
		if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
		// if (map[pnt.y][pnt.x] == -1)
		// 	return false;
	}
	// if (map[0][0] == -1)
	// 	return false;
	// All enemy have path to exit.
	mapState[y][x] = TILE_OCCUPIED;
	mapDistance = map;
	for (auto& it : EnemyGroup->GetObjects())
		dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
	return true;
}

std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
    // Reverse BFS to find path.
    std::vector<std::vector<int>> map(MapHeight, std::vector<int>(MapWidth, -1));
    std::queue<Engine::Point> que;
    // Push center point.
    int centerX = MapWidth / 2;
    int centerY = MapHeight / 2;
    // BFS from center point.
    if (mapState[centerY][centerX] != TILE_DIRT)
        return map;
    que.push(Engine::Point(centerX, centerY));
    map[centerY][centerX] = 0;
    while (!que.empty()) {
        Engine::Point p = que.front();
        que.pop();
        // Check all four directions around the current point
        for (const auto& dir : directions) {
            int new_x = p.x + dir.x;
            int new_y = p.y + dir.y;
            // Check if the new position is within the map and is a dirt tile
            if (new_x >= 0 && new_x < MapWidth && new_y >= 0 && new_y < MapHeight && mapState[new_y][new_x] == TILE_DIRT) {
                // Check if the new position has not been visited yet
                if (map[new_y][new_x] == -1) {
                    que.push(Engine::Point(new_x, new_y));
                    // The distance to the new position is the distance to the current position plus one
                    map[new_y][new_x] = map[p.y][p.x] + 1;
                }
            }
        }
    }
    return map;
}
void PlayScene::WriteScoreToScoreboard(int score) {
	std::time_t now = std::time(0);
	std::tm* localTime = std::localtime(&now);
	char dateTime[80];
	std::strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", localTime);

    std::ofstream outFile("Resource/scoreboard.txt", std::ios::app); // Open the file in append mode
    if (!outFile) {
        std::cerr << "Unable to open file scoreboard.txt";
        return;
    }
    outFile << score << " " << dateTime << std::endl; // Write the score to the file
    if (!outFile) {
        std::cerr << "Error writing to file scoreboard.txt";
        return;
    }
    outFile.close(); // Close the file
	// char cwd[1024];
	// if (getcwd(cwd, sizeof(cwd)) != NULL) {
	// 	std::cout << "Current working dir: " << cwd << std::endl;
	// } else {
	// 	std::cerr << "getcwd() error" << std::endl;
	// }
}

void PlayScene::OnSkill1UpgradeClick() {
	if(skill1UpgradeButton->Visible){
		if (player->SkillPoints > 0) {
			if(player->skill1Level == 1){
				Skill1LevelLabel->Text = "lvl 2";
				player->skill1Level = 2;
				player->SkillPoints--;
				player->skill1Cooldown -= 3.0;
				player->Skill1Damage += 2;
				player->skill1CooldownTimer = 0;
			} else if(player->skill1Level == 2){
				Skill1LevelLabel->Text = "lvl MAX";
				player->skill1Level = 3;
				player->SkillPoints--;
				player->skill1Cooldown -= 3.0;
				player->Skill1Damage += 3;
				player->Skill1Lifetime += 1.0;
				player->skill1ProjectileSpeed += 200;
				player->skill1CooldownTimer = 0;
			}
		}
	}
}

void PlayScene::OnStrengthUpgradeClick() {
	if(strengthUpgradeButton->Visible){
		if(player->SkillPoints > 0){
			player->SkillPoints--;
			player->Strength++;
			playerStrength->Text = "Strength: " + std::to_string(player->Strength);
			player->updateStats();
		}
	}
}

void PlayScene::OnIntelligenceUpgradeClick() {
	if(intelligenceUpgradeButton->Visible){
		if(player->SkillPoints > 0){
			player->SkillPoints--;
			player->Intelligence++;
			playerIntelligence->Text = "Intelligence: " + std::to_string(player->Intelligence);
			player->updateStats();
		}
	}
}

void PlayScene::OnEnduranceUpgradeClick() {
	if(enduranceUpgradeButton->Visible){
		if(player->SkillPoints > 0){
			player->SkillPoints--;
			player->Endurance++;
			playerEndurance->Text = "Endurance: " + std::to_string(player->Endurance);
			player->updateStats();
		}
	}
}

void PlayScene::EarnExp(int exp){
	player->Exp += exp;
}


void PlayScene::OnMachineGunUpgradeClick() {
	if(machinegunUpgradeButton->Visible){
		if(money >= 200){
			MachineGunTurret::extraDamage += 1;
			EarnMoney(-200);
			for(auto& it : TowerGroup->GetObjects()){
				Turret* myturret = dynamic_cast<Turret*>(it);
				if(myturret && myturret->type == "MachineGun"){
					myturret->damage += 1;
				}
			}
		}
	}
}

void PlayScene::OnLaserUpgradeClick() {
	if(laserUpgradeButton->Visible){
		if(money >= 350){
			LaserTurret::extraDamage += 2;
			EarnMoney(-350);
			for(auto& it : TowerGroup->GetObjects()){
				Turret* myturret = dynamic_cast<Turret*>(it);
				if(myturret && myturret->type == "Laser"){
					myturret->damage += 2;
				}
			}
		}
	}
}

void PlayScene::OnMissileUpgradeClick() {
	if(missileUpgradeButton->Visible){
		if(money >= 400){
			MissileTurret::extraDamage += 4;
			EarnMoney(-400);
			for(auto& it : TowerGroup->GetObjects()){
				Turret* myturret = dynamic_cast<Turret*>(it);
				if(myturret && myturret->type == "Missile"){
					myturret->damage += 4;
				}
			}
		}
	}
}

void PlayScene::OnFlameUpgradeClick() {
	if(flameUpgradeButton->Visible){
		if(money >= 300){
			FlameTurret::extraDamage += 1;
			EarnMoney(-300);
			for(auto& it : TowerGroup->GetObjects()){
				Turret* myturret = dynamic_cast<Turret*>(it);
				if(myturret && myturret->type == "Flame"){
					myturret->damage += 1;
				}
			}
		}
	}
}