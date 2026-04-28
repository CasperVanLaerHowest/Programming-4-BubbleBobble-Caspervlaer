#include "AddScoreCommand.h"

AddScoreCommand::AddScoreCommand(dae::GameObject* pGameObject, int score)
	: m_pGameObject{ pGameObject }
	, m_Score{ score }
{
}

void AddScoreCommand::Execute(bool notfirstExecute)
{
	if (!notfirstExecute)
	{
		m_pGameObject->GetComponent<ScoreComponent>()->AddScore(m_Score);
	}
}
