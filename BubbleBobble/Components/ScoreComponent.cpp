#include "ScoreComponent.h"

#include "GameObject.h"

ScoreComponent::ScoreComponent(dae::GameObject* owner, int score)
	: Component(owner)
	, m_Score{ score }
{
}

void ScoreComponent::AddScore(int score)
{
	m_Score += score;
	Notify(*GetOwner(), dae::Event::ADD_SCORE);
}
