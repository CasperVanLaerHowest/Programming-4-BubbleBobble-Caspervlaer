#include "EnemyAngryState.h"
#include "GameObject.h"
#include "../Components/EnemyStateComponent.h"

void EnemyAngryState::Enter()
{
	if (auto* enemy = m_pOwner->GetComponent<EnemyStateComponent>())
	{
		enemy->UpdateFacingAndAnimation("Angry");
	}
}

std::unique_ptr<BaseState> EnemyAngryState::Update(float deltaTime)
{
	auto* enemy = m_pOwner->GetComponent<EnemyStateComponent>();
	if (enemy == nullptr)
		return nullptr;

	enemy->UpdateTargetDirection(deltaTime);
	enemy->Move(enemy->GetAngrySpeed());
	enemy->TryUseNavigationPoint();
	enemy->TryJumpAtPlayer();
	enemy->UpdateFacingAndAnimation("Angry");

	return nullptr;
}
