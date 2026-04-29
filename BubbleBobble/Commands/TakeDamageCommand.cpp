#include "TakeDamageCommand.h"
#include "Service/AudioLocator.h"

TakeDamageCommand::TakeDamageCommand(dae::GameObject* pGameObject)
	: m_pGameObject{ pGameObject }
{
}

void TakeDamageCommand::Execute(bool notfirstExecute)
{
	if (!notfirstExecute)
	{
		m_pGameObject->GetComponent<HealthComponent>()->TakeDamage();
		
		// Play the sound via the globally located audio service
		AudioLocator::GetAudio().PlaySound("Data/Sound/Oof.wav");
	}
}

