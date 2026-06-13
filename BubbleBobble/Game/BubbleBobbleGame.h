#pragma once

#include "../Events/GameEvent.h"
#include "../HelperFunctions/LevelLoader.h"
#include "GameMode.h"

#include <array>
#include <filesystem>
#include <memory>

class BubbleBobbleGame final
{
public:
	explicit BubbleBobbleGame(std::filesystem::path dataPath);
	~BubbleBobbleGame();

	void Load();

private:
	void StartGame(GameMode gameMode);
	void HandleGameEvent(const GameEvent& event);
	void ReturnToStartScreen();

	dae::Scene& LoadStartScreen();
	dae::Scene& LoadHighScoreEntryScreen(int score);
	dae::Scene& LoadVersusResultScreen(bool playerOneWon);

	bool m_GameStarted{};
	std::unique_ptr<LevelLoader> m_LevelLoader{};
	std::array<int, 2> m_PlayerScores{};
};
