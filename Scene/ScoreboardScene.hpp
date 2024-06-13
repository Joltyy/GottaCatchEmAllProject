#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class ScoreboardScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
	int currentPage = 0;
public:
	explicit ScoreboardScene() = default;
	void Initialize() override;
	void Terminate() override;
	void BackOnClick(int stage);
	void DisplayScores(const std::vector<std::tuple<std::string, int, std::string, std::string>>& scores, int halfW, int page);
	void ClearScores();
	void RemoveLabel(Engine::Label* label);
};

#endif // SCOREBOARSCENE_HPP
