#include "ShootMaitaBallCommand.h"

#include "../Components/FacingComponent.h"
#include "../Components/MaitaPlayerComponent.h"
#include "../Components/PlayerSlotComponent.h"
#include "../Components/PlayerStateComponent.h"
#include "../HelperFunctions/GameObjectFactory.h"
#include "Service/AudioLocator.h"
#include "TransformComponent.h"

ShootMaitaBallCommand::ShootMaitaBallCommand(dae::GameObject* pGameObject, dae::Scene& scene)
	: m_pGameObject{ pGameObject }
	, m_Scene{ scene }
{
}

void ShootMaitaBallCommand::Execute(bool notfirstExecute)
{
	if (notfirstExecute)
		return;

	if (auto* playerState = m_pGameObject->GetComponent<PlayerStateComponent>(); playerState && playerState->IsDying())
		return;

	if (auto* maitaPlayer = m_pGameObject->GetComponent<MaitaPlayerComponent>(); maitaPlayer && maitaPlayer->IsTrapped())
		return;

	auto* transform = m_pGameObject->GetComponent<dae::TransformComponent>();
	auto* facing = m_pGameObject->GetComponent<FacingComponent>();
	if (transform == nullptr || facing == nullptr)
		return;

	const float direction = facing->IsFacingRight() ? 1.f : -1.f;
	const auto& playerPos = transform->GetWorldPosition();
	const int ownerPlayerIndex = m_pGameObject->GetComponent<PlayerSlotComponent>() != nullptr
		? m_pGameObject->GetComponent<PlayerSlotComponent>()->GetPlayerIndex()
		: -1;

	SpawnMaitaBall(m_Scene, { playerPos.x + direction * 24.f, playerPos.y - 10.f }, direction, ownerPlayerIndex);
	AudioLocator::GetAudio().PlaySound("Data/Sound/shoot.wav");
}
