#include "PlayerStateComponent.h"
#include "../States/IdleState.h"
#include "../States/PlayerDeathState.h"
#include "HealthComponent.h"

PlayerStateComponent::PlayerStateComponent(dae::GameObject* owner, const glm::vec2& spawnPosition)
	: Component(owner)
	, m_SpawnPosition{ spawnPosition }
{
	ChangeState(std::make_unique<IdleState>(owner));
}

void PlayerStateComponent::Update(float deltaTime)
{
	if (m_pCurrentState)
	{
		auto newState = m_pCurrentState->Update(deltaTime);
		if (newState)
		{
			ChangeState(std::move(newState));
		}
	}
}

void PlayerStateComponent::ChangeState(std::unique_ptr<BaseState> newState)
{
	if (m_pCurrentState)
		m_pCurrentState->Exit();
	m_pCurrentState = std::move(newState);
	if (m_pCurrentState)
		m_pCurrentState->Enter();
}

void PlayerStateComponent::TakeHit()
{
	if (IsDying())
		return;

	auto* health = GetOwner()->GetComponent<HealthComponent>();
	if (health == nullptr || health->GetHealth() <= 0)
		return;

	health->TakeDamage();
	ChangeState(std::make_unique<PlayerDeathState>(GetOwner(), m_SpawnPosition));
}

bool PlayerStateComponent::IsDying() const
{
	return dynamic_cast<PlayerDeathState*>(m_pCurrentState.get()) != nullptr;
}
