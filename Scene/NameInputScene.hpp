#ifndef NAMEINPUTSCENE_HPP
#define NAMEINPUTSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class NameInputScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
	Engine::Label* playerNameInput;
	Engine::Label* playerNameLabel;
public:
	explicit NameInputScene() = default;
	void Initialize() override;
	void Terminate() override;
	void ConfirmOnClick(int stage);
	std::string GetPlayerName();
	void OnKeyDown(int keyCode) override;
};

#endif // NAMEINPUTSCENE_HPP
