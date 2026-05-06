#pragma once
#include "Component.h"
#include "Subject.h"

class ScoreComponent final : 
    public dae::Component, 
    public dae::Subject
{
public:
    ScoreComponent(dae::GameObject* owner, int score = 0);
    virtual ~ScoreComponent() = default;

	void Update(float /*deltaTime*/) override {};

	void AddScore(int score);
    int GetScore() const { return m_Score; }

private:
	int m_Score;
};
