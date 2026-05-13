#include "ShootBubbleCommand.h"
#include "TransformComponent.h"
#include "../Components/PlayerFacingComponent.h"
#include "GameObject.h"

ShootBubbleCommand::ShootBubbleCommand(dae::GameObject* pGameObject)
	: m_pGameObject(pGameObject)
{
}

void ShootBubbleCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute)
	{
		//std::cout << "first ShootBubbleCommand executed" << std::endl;
	}
	auto transform = m_pGameObject->GetComponent<dae::TransformComponent>();
	auto facing = m_pGameObject->GetComponent<PlayerFacingComponent>();
	if (transform && facing)
	{
		glm::vec2 spawnOffset{ 0.f, -10.f };
		if (!facing->IsFacingRight())
			spawnOffset.x = -spawnOffset.x;
		glm::vec2 spawnPos = { transform->GetWorldPosition().x + spawnOffset.x, transform->GetWorldPosition().y + spawnOffset.y };
		
		// spawn bubble
	}
}