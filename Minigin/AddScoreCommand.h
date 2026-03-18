#pragma once
#include "Command.h"
#include "GameObject.h"
#include "ScoreComponent.h"

class AddScoreCommand final : 
    public Command
{
public:
	AddScoreCommand(dae::GameObject* pGameObject, int score = 10);
	virtual void Execute(bool notfirstExecute) override;
private:
	dae::GameObject* m_pGameObject;
	int m_Score;
};