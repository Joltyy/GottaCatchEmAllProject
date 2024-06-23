#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Slider.hpp"
#include "SettingsScene.hpp"
#include "StartScene.h"

void SettingsScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
    //bg
    Engine::Image* background = new Engine::Image("stage-select/bg_blur.png", 0, 0, w, h);
    AddNewObject(background);

    //back
    btn = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW - 200, halfH * 3 / 2 - 50,
                                  400, 100);
    btn->SetOnClickCallback(std::bind(&SettingsScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("BACK", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    //slider
    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(
            std::bind(&SettingsScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(
            new Engine::Label("BGM: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 0, 0, 0, 255, 0.5,
                              0.5));
                              
    sliderSFX = new Slider(40 + halfW - 95, halfH + 50 - 2, 190, 4);
    sliderSFX->SetOnValueChangedCallback(
            std::bind(&SettingsScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderSFX);
    AddNewObject(
            new Engine::Label("SFX: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH + 50, 0, 0, 0, 255, 0.5,
                              0.5));
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("menu.ogg", true, AudioHelper::BGMVolume);

    sliderBGM->SetValue(AudioHelper::BGMVolume);
    sliderSFX->SetValue(AudioHelper::SFXVolume);
}

void SettingsScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void SettingsScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void SettingsScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;

    
    

}

void SettingsScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

// void SettingsScene::BGMSlideOnValueChanged(float value) {
//     StartScene* startScene = static_cast<StartScene*>(Engine::GameEngine::GetInstance().GetScene("start"));
//     if (startScene != nullptr) {
//         startScene->SetBGMVolume(value);
//         AudioHelper::BGMVolume = value;
//     }
// }
