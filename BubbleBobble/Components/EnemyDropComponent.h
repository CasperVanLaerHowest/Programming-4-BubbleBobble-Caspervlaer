#pragma once

#include "Component.h"
#include <string>

class EnemyDropComponent final : public dae::Component
{
public:
	EnemyDropComponent(dae::GameObject* owner, std::string fruitTexture, int scoreValue);

	const std::string& GetFruitTexture() const { return m_FruitTexture; }
	int GetScoreValue() const { return m_ScoreValue; }

private:
	std::string m_FruitTexture{};
	int m_ScoreValue{};
};
