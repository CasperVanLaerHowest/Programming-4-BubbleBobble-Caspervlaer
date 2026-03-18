#include "TakeDamageCommand.h"


TakeDamageCommand::TakeDamageCommand(dae::GameObject* pGameObject)
	: m_pGameObject{ pGameObject }
{
}

void TakeDamageCommand::Execute(bool notfirstExecute)
{
	if (!notfirstExecute)
	{
		m_pGameObject->GetComponent<HealthComponent>()->TakeDamage();
	}
}

