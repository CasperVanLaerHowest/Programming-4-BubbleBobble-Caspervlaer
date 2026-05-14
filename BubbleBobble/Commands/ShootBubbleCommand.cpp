#include "ShootBubbleCommand.h"
#include "TransformComponent.h"
#include "../Components/FacingComponent.h"
#include "GameObject.h"
#include <TextureComponent.h>
#include "../Components/CollisionComponent.h"
#include "../HelperFunctions/GameObjectFactory.h"

ShootBubbleCommand::ShootBubbleCommand(dae::GameObject* pGameObject, dae::Scene& scene)
	: m_pGameObject(pGameObject), m_Scene(scene)
{
}

void ShootBubbleCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute)
		return;

	auto transform = m_pGameObject->GetComponent<dae::TransformComponent>();
	auto facing = m_pGameObject->GetComponent<FacingComponent>();

	if (transform && facing)
	{
		glm::vec2 spawnOffset{ 24.f, -10.f };

		if (!facing->IsFacingRight())
			spawnOffset.x *= -1.f;

		const auto& playerPos = transform->GetWorldPosition();
		glm::vec2 spawnPos{
			playerPos.x + spawnOffset.x,
			playerPos.y + spawnOffset.y
		};

		SpawnBubble(m_Scene, spawnPos, facing->IsFacingRight());
	}
}
