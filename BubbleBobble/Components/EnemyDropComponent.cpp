#include "EnemyDropComponent.h"

#include <utility>

EnemyDropComponent::EnemyDropComponent(dae::GameObject* owner, std::string fruitTexture, int scoreValue)
	: Component(owner)
	, m_FruitTexture{ std::move(fruitTexture) }
	, m_ScoreValue{ scoreValue }
{
}
