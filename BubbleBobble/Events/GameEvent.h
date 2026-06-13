#pragma once

#include "../Game/GameMode.h"

enum class GameEventType
{
	LevelCompleted,
	GameOver,
	VersusFinished
};

struct GameEvent
{
	GameEventType type{ GameEventType::LevelCompleted };
	GameMode gameMode{ GameMode::SinglePlayer };
	int levelIndex{};
	bool playerOneWon{};
};
