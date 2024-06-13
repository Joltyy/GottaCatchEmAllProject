// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/NameInputScene.hpp"

#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // TODO: done [HACKATHON-1-SCENE] (3/4): Register Scenes here 
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose-scene", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("scoreboard-scene", new ScoreboardScene());
	game.AddNewScene("name-input", new NameInputScene());


	// TODO: done [HACKATHON-1-SCENE] (4/4): Change the start scene
	//game.Start("stage-select", 60, 1600, 832);
	//game.Start("settings", 60, 1600, 832);
	game.Start("start", 60, 1600, 832);
	// std::atexit([]() { std::system("cp Resource/scoreboard.txt Resource/scoreboard.txt"); });
	return 0;
}
