#include <functional>
#include <string>
#include <allegro5/allegro_audio.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "allegro5/allegro.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "SettingsScene.hpp"
#include "StartScene.h"
#include "ScoreboardScene.hpp"
#include "StageSelectScene.hpp"

void ScoreboardScene::Initialize() {
	
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	//bg
    Engine::Image* background = new Engine::Image("stage-select/snorlax.png", 0, 0, w, h);
    AddNewObject(background);

	AddNewObject(new Engine::Label("scoreboard", "pirulen.ttf", 60, halfW, 50, 250, 7, 100, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample("main.ogg", true, AudioHelper::BGMVolume);

	std::ifstream fin("Resource/scoreboard.txt");
	std::string line;
	std::vector<std::tuple<std::string, int, std::string, std::string>> scores; // Create a vector to store the names and scores

	while (std::getline(fin, line)) {
		std::istringstream iss(line);
		std::string name;
		int score;
		std::string date;
		std::string time;
		if (!(iss >> name >> score >> date >> time)) { break; } // error

		scores.push_back(std::make_tuple(name, score, date, time)); // Add the name and score to the vector
	}
	//fin.close();

	// Sort the scores in descending order
	std::sort(scores.begin(), scores.end(), [](const std::tuple<std::string, int, std::string, std::string>& a, 
	const std::tuple<std::string, int, std::string, std::string>& b) {
		return std::get<1>(a) > std::get<1>(b);
	});

	//ScoreboardScene::DisplayScores(scores, halfW, 1);

	// Determine the number of pages
    int scoresPerPage = 10;
    int numPages = (scores.size() + scoresPerPage - 1) / scoresPerPage;

    // Create a variable to store the current page
   
	int ranking = 1;

    // Display the scores for the current page
    DisplayScores(scores, currentPage, halfW);

    // Create buttons to navigate between pages
    Engine::ImageButton* btnNext;
    btnNext = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW + 300, halfH * 7 / 4 - 50, 400, 100);
    // btnNext->SetOnClickCallback(std::bind(&ScoreboardScene::DisplayScores, this, scores, ranking + 9, halfW));
	btnNext->SetOnClickCallback([this, scores, halfW, ranking, halfH, numPages]() {
		if (currentPage < numPages - 1)
		{
			currentPage++;
			DisplayScores(scores, currentPage * 10, halfW);
		}
		// if (currentPage > 0) {
		// 	Engine::ImageButton* btnPrev;
		// 	btnPrev = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW - 700, halfH * 7 / 4 - 50, 400, 100);
		// 	btnPrev->SetOnClickCallback(std::bind(&ScoreboardScene::DisplayScores, this, scores, 0, halfW));
		// 	btnPrev->SetOnClickCallback([this, scores, halfW, btnPrev]() {
		// 		currentPage--;
		// 		DisplayScores(scores, 0, halfW);
		// 		//if (currentPage == 0) RemoveControlObject(btnPrev); 
		// 	});
		// 	AddNewControlObject(btnPrev);
		// 	AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 48, halfW - 500, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
		// }
	});
    AddNewControlObject(btnNext);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 48, halfW + 500, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

	Engine::ImageButton* btnPrev;
	btnPrev = new Engine::ImageButton("stage-select/msg.png", "stage-select/msg2.png", halfW - 700, halfH * 7 / 4 - 50, 400, 100);
	//btnPrev->SetOnClickCallback(std::bind(&ScoreboardScene::DisplayScores, this, scores, 0, halfW));
	btnPrev->SetOnClickCallback([this, scores, halfW]() {
		if (currentPage > 0) currentPage--;
		DisplayScores(scores, currentPage * 10, halfW);
	});
	AddNewControlObject(btnPrev);
	AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 48, halfW - 500, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

	
	
}

void ScoreboardScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	currentPage = 0;
	IScene::Terminate();
}

void ScoreboardScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

std::vector<Engine::Label*> rankingLabels;
void ScoreboardScene::RemoveLabel(Engine::Label* label) {
    auto it = std::find_if(objects.begin(), objects.end(), [&](const std::pair<bool, IObject*>& pair) {
        return pair.second == label;
    });

    if (it != objects.end()) {
        RemoveObject(it);
    }
}

void ScoreboardScene::ClearScores() {
	for (auto& label : rankingLabels) {
		RemoveLabel(label);
	}
	rankingLabels.clear();
}

void ScoreboardScene::DisplayScores(const std::vector<std::tuple<std::string, int, std::string, std::string>>& scores, int i, int halfW)
{
	//clear first 
	ClearScores();

	// Check if i is within the range of the scores vector
    if (i < 0 || i >= scores.size()) {
        return;
    }

	// Calculate the end index
    int end = std::min(i + 9, static_cast<int>(scores.size()) - 1);

	// Display the scores
	int y = 120; // Start position for the scores
	int rank = i + 1;
	int initialXRank = halfW - 270; // Variable to store the x position of the first rank
	int initialXName = halfW; // Variable to store the x position of the first name
	int initialXScore = halfW + 250; // Variable to store the x position of the first score
	for (int index = i; index <= end; index++) {
		const auto& data = scores[index];
		// Create a label for each rank, name, and score
		std::string rankStr = std::to_string(rank) + ".";
		std::string nameStr = std::get<0>(data);
		std::string scoreStr = std::to_string(std::get<1>(data));
		std::string dateStr = std::get<2>(data);
        std::string timeStr = std::get<3>(data);

		Engine::Label* labelDate = new Engine::Label(dateStr, "pirulen.ttf", 48, initialXRank - 270, y, 0, 0, 0, 255, 0.5, 0.5);
		Engine::Label* label1 = new Engine::Label(rankStr, "pirulen.ttf", 48, initialXRank, y, 0, 0, 0, 255, 0.5, 0.5);
		Engine::Label* label2 = new Engine::Label(nameStr, "pirulen.ttf", 48, initialXName, y, 57, 117, 69, 255, 0.5, 0.5);
		Engine::Label* label3 = new Engine::Label(scoreStr, "pirulen.ttf", 48, initialXScore, y, 0, 0, 0, 255, 0.5, 0.5);
		Engine::Label* labelTime = new Engine::Label(timeStr, "pirulen.ttf", 48, initialXRank + 800, y, 0, 0, 0, 255, 0.5, 0.5);

		rankingLabels.push_back(labelDate);
		rankingLabels.push_back(label1);
		rankingLabels.push_back(label2);
		rankingLabels.push_back(label3);
		rankingLabels.push_back(labelTime);

		AddNewObject(labelDate);
		AddNewObject(label1);
		AddNewObject(label2);
		AddNewObject(label3);
		AddNewObject(labelTime);
		
		y += 50; // Move the y position down for the next score
		rank++;
	}
}