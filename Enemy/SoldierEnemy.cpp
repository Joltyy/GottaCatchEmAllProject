#include <string>

#include "SoldierEnemy.hpp"

SoldierEnemy::SoldierEnemy(int x, int y) : Enemy("play/enemy-1.png", x, y, 10, 50, 5 + (5 * extraHp), 5, 5, 10 + (10 * extraDmg), 10) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
}
