#include "EnemyWalkState.h"
#include "EnemyAngryState.h"
#include "GameObject.h"
#include "../Components/EnemyStateComponent.h"

void EnemyWalkState::Enter()
{
	if (auto* enemy = m_pOwner->GetComponent<EnemyStateComponent>())
	{
		enemy->UpdateFacingAndAnimation("Walk");
	}
}

std::unique_ptr<BaseState> EnemyWalkState::Update(float deltaTime)
{
	auto* enemy = m_pOwner->GetComponent<EnemyStateComponent>();
	if (enemy == nullptr)
		return nullptr;

	if (enemy->GetElapsedTime() >= enemy->GetAngryTime())
		return std::make_unique<EnemyAngryState>(m_pOwner);

	enemy->UpdateTargetDirection(deltaTime);
	enemy->Move(enemy->GetNormalSpeed());
	enemy->TryUseNavigationPoint();
	enemy->TryJumpAtPlayer();
	enemy->UpdateFacingAndAnimation("Walk");

	return nullptr;
}
