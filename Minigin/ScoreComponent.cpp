#include "ScoreComponent.h"

#include "GameObject.h"

ScoreComponent::ScoreComponent(dae::GameObject* owner, int score)
	: Component(owner)
	, m_Score{ score }
{
}

void ScoreComponent::FixedUpdate(float /*fixedTimeStep*/)
{
	if (m_DirtyFlag)
	{
		Notify(*GetOwner(), dae::Event::ADD_SCORE);
		m_DirtyFlag = false;
	}
}

void ScoreComponent::AddScore(int score)
{
	m_Score += score;
	m_DirtyFlag = true;
}
