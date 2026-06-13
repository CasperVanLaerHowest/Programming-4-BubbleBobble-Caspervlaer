#pragma once

#include "../Game/GameMode.h"
#include "Component.h"

class LevelFlowComponent final : public dae::Component
{
public:
	LevelFlowComponent(dae::GameObject* owner, GameMode gameMode, int levelIndex);
	void Update(float deltaTime) override;
	void SkipLevel();

private:
	void StartDelayedWinTransition();
	void QueueTransition(bool levelWon);

	GameMode m_GameMode{ GameMode::SinglePlayer };
	int m_LevelIndex{};
	bool m_TransitionRequested{};
	bool m_TransitionQueued{};
	bool m_PendingLevelWon{};
	float m_TransitionDelay{};
};
