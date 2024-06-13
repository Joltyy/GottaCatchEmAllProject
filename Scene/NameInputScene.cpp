#include <functional>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "NameInputScene.hpp"

void NameInputScene::Initialize() {
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
    
    Engine::Image* background = new Engine::Image("stage-select/bg_blur.png", 0, 0, w, h);
    AddNewObject(background);

    Engine::ImageButton* btn;

    //player input
    btn = new Engine::ImageButton("stage-select/banner2.png", "stage-select/banner2.png", halfW - 360, halfH - 50, 710, 100); 
    btn->SetOnClickCallback(std::bind(&NameInputScene::OnKeyDown, this, 1));
    AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("Insert your name", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5));
    
    //text bg height -50 from text label
    btn = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW - 200, halfH + 70, 400, 100); 
    btn->SetOnClickCallback(std::bind(&NameInputScene::ConfirmOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("CONFIRM", "pirulen.ttf", 48, halfW, halfH + 120, 0, 0, 0, 255, 0.5, 0.5));

    // Create a new label to display the player's name.
    playerNameInput = new Engine::Label("", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5);
    playerNameLabel = new Engine::Label("Enter Your Name", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5);
    AddNewObject(playerNameLabel);
}
void NameInputScene::Terminate() {
	IScene::Terminate();
}

void NameInputScene::ConfirmOnClick(int stage) {

    //std::cout << playerNameInput->Text << "\n";
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("play");

    // Write the player's name to the scoreboard.txt file.
    std::ofstream outFile("Resource/scoreboard.txt", std::ios::app); // Open the file in append mode
    if (!outFile) {
        std::cout << "Unable to open file scoreboard.txt";
        return;
    }
    
    outFile << playerNameInput->Text << " "; // Write the player's name to the file
    if (!outFile) {
        std::cout << "Error writing to file scoreboard.txt";
        return;
    }
   
    outFile.close(); // Close the file
}

void NameInputScene::OnKeyDown(int keyCode) {
    if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) 
    {
        // Append the new character to the player's name.
        std::string playerName = playerNameInput->Text;
        playerName += (char)(keyCode + 'A' - ALLEGRO_KEY_A);
        playerNameInput->Text = playerName;
    } 
    else if (keyCode == ALLEGRO_KEY_BACKSPACE && !playerNameInput->Text.empty()) 
    {
        // Remove the last character from the player's name.
        playerNameInput->Text.pop_back();
    }
    else if (keyCode == ALLEGRO_KEY_ENTER)
    {
        NameInputScene::ConfirmOnClick(1);
    }

    playerNameLabel->Text = playerNameInput->Text;
}


