# 2024 I2P Mini Project 2 - Tower Defense

## Grading

| **Hackathon Tasks**              | **Score** | **Check** |
| :------------------------------- | :-------: | :-------: |
| Add `StartScene`                 |    5%     |     -     |
| Add `StageSelectScene`           |    5%     |     -     |
| Move Controls to `SettingsScene` |    5%     |     -     |
| Fix Bugs in Scene                |    15%    |     -     |

| **Project Tasks**           | **Score** | **Check** |
| :-------------------------- | :-------: | :-------: |
| Add `ScoreboardScene`       |    40%    |     -     |
| Enemy Pathfinding           |    10%    |     -     |
| 2 New Turrets/Enemies/Tools |    20%    |     -     |

<!-- Please describe the new turret/enemy/tool you have implemented in the above table. -->

| **Bonus Functions**   | **Score** | **Check** |
| :-------------------- | :-------: | :-------: |
| Scoreboard Date Time  |    3%     |     -     |
| Record User Name      |    7%     |     -     |
| Other Bonus Functions |   ≤10%    |     -     |

<!-- Bonus Feature for Final Project -->
-    Added a Player with following features:
      -    Has level that levels up with exp, energy for sprinting and casting skills that regenerates over time
      -    Upgradable stats that increases various variables such as attack damage, skill damage, etc.
      -    An Active skill that can be used with its cooldown and energy cost
      -    Sprinting mechanic that uses energy
      -    Can launch a basic attack
      -    Everytime player levels up they will earn a skill point that can be spent to upgrade one of three stats or skill
           Stats :
           - Strengh : Increases damage
           - Intelligence : Increases energy regeneration speed
           - Endurance : Increases maximum energy
-    Enemy Mechanic changes
      -    Enemy now spawn endlessly from all directions
      -    Enemy will go to the center(Base) and try to destroy it
      -    If the base is covered by turrets, the enemy will destroy the turrets first
      -    Each enemy has it's own animation, and the boss has an attack animation
      -    A wave system that increases enemy HP and damage the longer the game goes on
-    Game Mechanic changes
      -    Goal of the game changed to survive for the longest time
      -    Enemies will keep spawning with higher and higher hp and damage to turrets
      -    if the base is destroyed, the game ends and the score will be written to memory
      -    Rocks and bushes can be hit to gain additional money
-    Turret Changes
      -    Turrets are now destroyable when enemy gets close
      -    Turrets can now be upgraded using players Money
      -    Turret can now be placed to cover all paths to the base to slow down the enemies


---

<style>
table th{
    width: 100%;
}
</style>
