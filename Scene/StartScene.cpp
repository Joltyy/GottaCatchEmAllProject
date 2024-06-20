//
// Created by Hsuan on 2024/4/10.
//

#include "StartScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "Scene/StartScene.h"

void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    // Engine::Image* background = new Engine::Image("stage-select/background.png", 0, 0, w, h);
    // AddNewObject(background);

    Engine::ImageButton* btn;
    AddNewObject(new Engine::Image("stage-select/title.png", halfW, halfH / 3 + 50, w - 100, 150, 0.5, 0.5));
    AddNewObject(new Engine::Label("Gotta Crush Em All", "Pokemon Solid.ttf", 150, halfW, halfH / 3 + 50, 255, 200, 3, 229, 0.5, 0.5));
    AddNewObject(new Engine::Label("Gotta Crush Em All", "Pokemon Hollow.ttf", 150, halfW, halfH / 3 + 50, 33, 56, 110, 229, 0.5, 0.5));
    

    btn = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW - 200, halfH / 2 + 200 , 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pixel.ttf", 70, halfW, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "pixel.ttf", 70, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    //bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);

    // static bool isBGMStarted = false;
    // if (!isBGMStarted) {
    //     bgmInstance = AudioHelper::PlaySample("main_menu.ogg", true, AudioHelper::BGMVolume);
    //     isBGMStarted = true;
    // }
    
    bgmId = AudioHelper::PlayBGM("main_menu.ogg");
}
void StartScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void StartScene::SetBGMVolume(float volume) {
    AudioHelper::ChangeSampleVolume(bgmInstance, volume);
}