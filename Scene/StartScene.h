//
// Created by Hsuan on 2024/4/10.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
class StartScene final : public Engine::IScene {
    private:
        std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
        ALLEGRO_SAMPLE_ID bgmId;
    public:
        explicit StartScene() = default;
        void Initialize() override;
        void Terminate() override;
        void PlayOnClick(int stage);
        void SettingsOnClick(int stage);
        void SetBGMVolume(float volume);
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
